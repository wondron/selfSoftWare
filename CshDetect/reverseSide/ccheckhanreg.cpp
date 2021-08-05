#include "ccheckhanreg.h"
#include "qdebug.h"
#include "../commonUsage/commonDef.h"
#include "XmlRead.h"

namespace CSHDetect {

class CCheckHanRegPrivate
{
public:
    CCheckHanRegPrivate() {}

    int controlNum = 4;

    int dynThreKernel = 30;
    int dynThreValu = 5;

    int hanHoleSize = 15;
    int maxHanArea = 180;

    double lenRatio = 1;//焊印的宽除长的比值

    int minHanRegionSize = 15;
};

CCheckHanReg::CCheckHanReg():
    d(new CCheckHanRegPrivate())
{

}

CCheckHanReg::~CCheckHanReg()
{
    delete d;
}

CError CCheckHanReg::detect(CONSTIMG obj, ReverRegionS& res, QList<QRect>& rects)
{
    HObject hanRegion;
    CError err = getHanRegion(obj, res.midRegion, res.hanregion,
                              d->hanHoleSize, d->dynThreValu, d->dynThreKernel,
                              d->maxHanArea, d->minHanRegionSize, d->lenRatio, rects);
    CHECKERR(err);

    if (rects.size() < d->controlNum)
        return CError(HANSIZE, QString("CCheckHanReg size is not %1, res is %2").arg(d->controlNum).arg(rects.size()));

    return 0;
}

CError CCheckHanReg::pamRead(const char* xmlfilePath)
{
    XmlRead xmlRead;

    try {
        std::map<std::string, xmlInfo> res = xmlRead.parseXML(xmlfilePath, taskName.toLocal8Bit().data());

        READPAM(d->controlNum, "controlNum", res);

        READPAM(d->dynThreValu, "dynThreValu", res);
        READPAM(d->dynThreKernel, "dynThreKernel", res);

        READPAM(d->hanHoleSize, "hanHoleSize", res);
        READPAM(d->maxHanArea, "maxHanArea", res);

        READPAM(d->lenRatio, "lenRatio", res);
        READPAM(d->minHanRegionSize, "minHanRegionSize", res);

        return 0;

    } catch (...) {
        return CError(PAMREAD, "task: CCheckHanReg, pamRead failed!");
    }
}

CError CCheckHanReg::pamRead(QMap<QString, QMap<QString, QVariant> >& pamMap)
{
    try {
        auto res = pamMap.value(taskName);

        READPAMINT(d->controlNum, "controlNum", res);

        READPAMINT(d->dynThreValu, "dynThreValu", res);
        READPAMINT(d->dynThreKernel, "dynThreKernel", res);

        READPAMINT(d->hanHoleSize, "hanHoleSize", res);
        READPAMINT(d->maxHanArea, "maxHanArea", res);

        READPAMFLOAT(d->lenRatio, "lenRatio", res);

        READPAMINT(d->minHanRegionSize, "minHanRegionSize", res);

        return 0;

    } catch (...) {
        return CError(PAMREAD, "task: CCheckHanReg, pamRead failed!");
    }
}

CError CCheckHanReg::getHanRegion(CONSTIMG dst, CONSTIMG midRegion, RHOBJ hanRegion,
                                  int hanHoleSize, int dynThreValu, int dynKernel, int maxHanArea, int minHanRegionSize, double lenRatio, QList<QRect>& rects)
{
    try {
        CHECKEMPIMG(dst, "getHanRegion::Image is empty");
        CHECKEMPIMG(midRegion, "getHanRegion::midRegion is empty");
        CHECKTHREVALUE(dynThreValu, "getHanRegion::dynThreValu out of range");

        HObject  ImageReduced, ImageMean, RegionDynThresh;
        HObject  ConnectedRegions1, hanRegions, RegionUnion;
        HObject  ConnectedRegions2, SelectedRegions;

        HTuple tupTiao, tupMin, tupMax;
        HTuple holeSize;

        ReduceDomain(dst, midRegion, &ImageReduced);
        CError err = Algorithm::dynThre(ImageReduced, RegionDynThresh, dynThreValu, dynKernel);
        CHECKERR(err);
        Connection(RegionDynThresh, &ConnectedRegions1);

        SelectShape(ConnectedRegions1, &hanRegions,
                    ((HTuple("area").Append("width")).Append("height")),
                    "and",
                    HTuple(hanHoleSize).TupleConcat((HTuple(0).Append(0))),
                    (HTuple(maxHanArea).TupleConcat(hanHoleSize)).TupleConcat(hanHoleSize));

        CHECKEMPIMG(hanRegions, "getHanRegion::hanRegions is empty");

        Union1(hanRegions, &RegionUnion);
        DilationCircle(RegionUnion, &hanRegions, hanHoleSize / 2);
        Connection(hanRegions, &ConnectedRegions2);
        SelectShape(ConnectedRegions2, &hanRegion, "area", "and", minHanRegionSize * maxHanArea, 99999);
        CountObj(hanRegion, &holeSize);

        HTuple row1, row2, col1, col2;
        SmallestRectangle1(hanRegion, &row1, &col1, &row2, &col2);

        rects.clear();
        HObject hUnion, hRect;
        GenEmptyObj(&hUnion);
        for (int i = 0; i < row1.Length(); i++) {
            if (lenRatio < (col2[i].D() - col1[i].D()) / (row2[i].D() - row1[i].D())) {
                QRect rect(QPoint(col1[i].D(), row1[i].D()), QPoint(col2[i].D(), row2[i].D()));
                GenRectangle1(&hRect, row1[i], col1[i], row2[i], col2[i]);
                Union2(hUnion, hRect, &hUnion);
                rects.push_back(rect);
            }
        }
        Intersection(hUnion, hanRegion, &hanRegion);
        return 0;

    } catch (...) {
        return CError(PAMREAD, "CCheckHanReg::getHanRegion crashed!");
    }
}
}
