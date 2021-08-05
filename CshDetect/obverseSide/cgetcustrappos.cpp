#include "cgetcustrappos.h"
#include "XmlRead.h"
#include "QDebug"

namespace CSHDetect {

class CGetCuStrapPosPrivate
{
public:
    CGetCuStrapPosPrivate() {}

    int m_width;
    int m_height;

    int minThre = 20;
    int CuPos = 3;
    int strapDirection = 1;

    int numMinThre = 0;
    int numMaxThre = 150;
    double numAreaRatoi = 0.6;
    QSize regionSize = QSize(1, 1);
    QList<RectInfo> rectList;
    RegionS m_region;
};

CGetCuStrapPos::CGetCuStrapPos(const int wid, const int height):
    d(new CGetCuStrapPosPrivate())
{
    d->m_width = wid;
    d->m_height = height;
}

CGetCuStrapPos::CGetCuStrapPos(const QString name):
    d(new CGetCuStrapPosPrivate())
{
    taskName = name;
}

CGetCuStrapPos::CGetCuStrapPos(const RegionS& region):
    d(new CGetCuStrapPosPrivate())
{

}

CGetCuStrapPos::~CGetCuStrapPos()
{
    delete d;
}

void CGetCuStrapPos::setImgSize(const int width, const int height)
{
    d->m_width = width;
    d->m_height = height;
}

void CGetCuStrapPos::setRegions(const RegionS& regions)
{
    d->m_region = regions;
}

CError CGetCuStrapPos::getCuPos(const RegionS& regions, HObject& resCuRegion, int& CuPos, cint minThre, cint connectDirect)
{
    try {

        CHECKEMPIMG(regions.MidRect, "CGetCuStrapPos::getCuPos MidRect is empty");
        CHECKEMPIMG(regions.TapeRegion, "CGetCuStrapPos::getCuPos TapeRegion is empty");
        CHECKEMPIMG(regions.DarkRegion, "CGetCuStrapPos::getCuPos DarkRegion is empty");
        CHECKEMPIMG(regions.MidBlueTape, "CGetCuStrapPos::getCuPos MidBlueTape is empty");

        CHECKEMPIMG(regions.m_RImg, "CGetCuStrapPos::getCuPos m_RImg is empty");
        CHECKEMPIMG(regions.m_GImg, "CGetCuStrapPos::getCuPos m_GImg is empty");

        CHECKTHREVALUE(minThre, "CGetCuStrapPos::getCuPos minthre is out of range");

        HObject hTestRegion, hSubImg, hReduce, hResRegion;
        HTuple area, col, row;

        hTestRegion = regions.MidRect;
        Difference(hTestRegion, regions.TapeRegion, &hTestRegion);
        Difference(hTestRegion, regions.DarkRegion, &hTestRegion);
        Difference(hTestRegion, regions.MidBlueTape, &hTestRegion);

        SubImage(regions.m_RImg, regions.m_GImg, &hSubImg, 1, 0);

        ReduceDomain(hSubImg, hTestRegion, &hReduce);
        Threshold(hReduce, &hResRegion, minThre, 255);
        CHECKEMPIMG(hResRegion, "hResRegion m_GImg is empty");

        Connection(hResRegion, &hResRegion);
        SelectShapeStd(hResRegion, &resCuRegion, "max_area", 70);
        AreaCenter(resCuRegion, &area, &row, &col);

        int STD, value;
        if (connectDirect == UPDOWN) {
            STD = regions.height / 2;
            value = row.D();
            CuPos = value > STD ? 1 : 0;
        } else {
            STD = regions.width / 2;
            value = col.D();
            CuPos = value > STD ? 3 : 2;
        }
        return 0;
    }  catch (...) {
        return CError(UNEXCEPTION, "CGetCuStrapPos::getCuPos crashed");
    }
}

CError CGetCuStrapPos::checkTapeNum(const RegionS& region, const HObject& cuRegion, QList<RectInfo> testInfo, QList<QRect>& gdInfo, QList<QRect>& ngInfo,
                                    cint minThre, cint maxThre, const qreal areaRatoi, const QSize size)
{
    try {
        HObject Rimg = region.m_RImg;
        HObject hShow;

        CHECKEMPIMG(Rimg, "CGetCuStrapPos::checkTapeNum rimg empty");
        CHECKEMPIMG(cuRegion, "CGetCuStrapPos::checkTapeNum cuRegion empty");

        HTuple area, row, col;
        AreaCenter(cuRegion, &area, &row, &col);

        QPoint basePt(col.D(), row.D());
        QList<RectInfo> gInfo, nInfo;
        CError err = Algorithm::detectRegionExit(Rimg, hShow, testInfo, gInfo, nInfo, basePt, minThre, maxThre, areaRatoi, size);
        CHECKERR(err);

        for (auto rect : gInfo) {
            QRect rectres = QRect(QPoint(rect.col - rect.len1, rect.row - rect.len2),
                                  QPoint(rect.col + rect.len1, rect.row + rect.len2));
            gdInfo.push_back(rectres);
        }

        for (auto rect : nInfo) {
            QRect rectres = QRect(QPoint(rect.col - rect.len1, rect.row - rect.len2),
                                  QPoint(rect.col + rect.len1, rect.row + rect.len2));
            ngInfo.push_back(rectres);
        }

        return 0;
    }  catch (...) {
        return CError(UNEXCEPTION, "CGetCuStrapPos::checkTapeNum crashed");
    }
}

CError CGetCuStrapPos::detect(const RegionS& regions, QList<QRect>& gdRect, QList<QRect>& ngRect)
{
    try {
        CError err;
        HObject midRegion, cuRegion;
        int CuPos;

        //检测铜片位置
        err = getCuPos(regions, cuRegion, CuPos, d->minThre, d->strapDirection);
        CHECKERR(err);

        HTuple row1, col1, row2, col2;
        SmallestRectangle1(cuRegion, &row1, &col1, &row2, &col2);
        QRect resRect(QPoint(col1, row1), QPoint(col2, row2));

        if (d->CuPos != CuPos) {
            ngRect.push_back(resRect);
            return CError(CUPOS, "Cu Position err");
        } else {
            gdRect.push_back(resRect);
        }

        err = checkTapeNum(regions, cuRegion, d->rectList, gdRect, ngRect, d->numMinThre, d->numMaxThre, d->numAreaRatoi, d->regionSize);
        CHECKERR(err);

        if (ngRect.size())
            return CError(REGIONNUM, "tapeNum err");
        return 0;
    }  catch (...) {
        return CError(UNEXCEPTION, "CGetCuStrapPos::detect crashed!");
    }
}

CError CGetCuStrapPos::pamRead(const char* xmlfilePath)
{
    XmlRead xmlRead;

    try {
        std::map<std::string, xmlInfo> res = xmlRead.parseXML(xmlfilePath, taskName.toLocal8Bit().data());

        READPAM(d->CuPos, "CuPos", res);
        READPAM(d->minThre, "minThre", res);
        READPAM(d->strapDirection, "strapDirection", res);

        d->rectList.clear();
        READPAM(d->numMinThre, "numMinThre", res);
        READPAM(d->numMaxThre, "numMaxThre", res);
        READPAM(d->numAreaRatoi, "numAreaRatoi", res);
        qDebug() << d->numMinThre << d->numMaxThre << d->numAreaRatoi;

        int hight = 9999, width = 9999;
        READPAM(hight, "regionHight", res);
        READPAM(width, "regionWidth", res);
        d->regionSize.setHeight(hight);
        d->regionSize.setWidth(width);

        RectInfo info;
        READPAM(info.XBia, "XBia1", res);
        READPAM(info.YBia, "YBia1", res);
        READPAM(info.len1, "len11", res);
        READPAM(info.len2, "len21", res);
        d->rectList.push_back(info);

        READPAM(info.XBia, "XBia2", res);
        READPAM(info.YBia, "YBia2", res);
        READPAM(info.len1, "len12", res);
        READPAM(info.len2, "len22", res);
        d->rectList.push_back(info);

        READPAM(info.XBia, "XBia3", res);
        READPAM(info.YBia, "YBia3", res);
        READPAM(info.len1, "len13", res);
        READPAM(info.len2, "len23", res);
        d->rectList.push_back(info);

        READPAM(info.XBia, "XBia4", res);
        READPAM(info.YBia, "YBia4", res);
        READPAM(info.len1, "len14", res);
        READPAM(info.len2, "len24", res);
        d->rectList.push_back(info);

//        for (int i = 0; i < d->rectList.size(); i++) {

//            qDebug() << "biaPam:"
//                     << d->rectList[i].XBia
//                     << d->rectList[i].YBia
//                     << d->rectList[i].len1
//                     << d->rectList[i].len2;
//        }

        return 0;
    } catch (...) {
        return CError(PAMREAD, "task: CGetCuStrapPos, pamRead failed!");
    }
}

CError CGetCuStrapPos::pamRead(QMap<QString, QMap<QString, QVariant> > &pamMap)
{
    try {
        auto res = pamMap.value(taskName);

        READPAMINT(d->CuPos, "CuPos", res);
        READPAMINT(d->minThre, "minThre", res);
        READPAMINT(d->strapDirection, "strapDirection", res);

        d->rectList.clear();
        READPAMINT(d->numMinThre, "numMinThre", res);
        READPAMINT(d->numMaxThre, "numMaxThre", res);
        READPAMFLOAT(d->numAreaRatoi, "numAreaRatoi", res);
        qDebug() << d->numMinThre << d->numMaxThre << d->numAreaRatoi;

        int hight = 9999, width = 9999;
        READPAMINT(hight, "regionHight", res);
        READPAMINT(width, "regionWidth", res);
        d->regionSize.setHeight(hight);
        d->regionSize.setWidth(width);

        RectInfo info;
        READPAMINT(info.XBia, "XBia1", res);
        READPAMINT(info.YBia, "YBia1", res);
        READPAMINT(info.len1, "len11", res);
        READPAMINT(info.len2, "len21", res);
        d->rectList.push_back(info);

        READPAMINT(info.XBia, "XBia2", res);
        READPAMINT(info.YBia, "YBia2", res);
        READPAMINT(info.len1, "len12", res);
        READPAMINT(info.len2, "len22", res);
        d->rectList.push_back(info);

        READPAMINT(info.XBia, "XBia3", res);
        READPAMINT(info.YBia, "YBia3", res);
        READPAMINT(info.len1, "len13", res);
        READPAMINT(info.len2, "len23", res);
        d->rectList.push_back(info);

        READPAMINT(info.XBia, "XBia4", res);
        READPAMINT(info.YBia, "YBia4", res);
        READPAMINT(info.len1, "len14", res);
        READPAMINT(info.len2, "len24", res);
        d->rectList.push_back(info);

        return 0;
    } catch (...) {
        return CError(PAMREAD, "task: CGetCuStrapPos, pamRead failed!");
    }
}

}
