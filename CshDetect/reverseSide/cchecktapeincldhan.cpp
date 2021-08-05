#include "cchecktapeincldhan.h"
#include "qdebug.h"
#include "../commonUsage/commonDef.h"
#include "XmlRead.h"


namespace CSHDetect {

class CCheckTapeIncldHanPrivate
{
public:
    CCheckTapeIncldHanPrivate()
    {

    }

    QList<RectInfo> testInfo;
    ReverRegionS m_regions;

    int minThre = 0;
    int maxThre = 220;
};


CCheckTapeIncldHan::CCheckTapeIncldHan():
    d(new CCheckTapeIncldHanPrivate())
{

}

CCheckTapeIncldHan::~CCheckTapeIncldHan()
{
    delete d;
}

CError CCheckTapeIncldHan::detect(CONSTIMG dst, const QList<QRect>& rects, QList<QRect>& gdRects, QList<QRect>& ngRects)
{
    try {
        CError err = checkRegionShape(dst, rects, d->testInfo, gdRects, ngRects, d->minThre, d->maxThre);
        if (err.isWrong())
            return CError(UNEXCEPTION, err.msg());

        if (ngRects.size())
            return CError(LACKAREA, "lack tape Area");

        return 0;
    }  catch (...) {
        return CError(UNEXCEPTION, "CCheckTapeIncldHan::detect crashed");
    }
}

CError CCheckTapeIncldHan::pamRead(const char* xmlfilePath)
{
    XmlRead xmlRead;

    try {
        std::map<std::string, xmlInfo> res = xmlRead.parseXML(xmlfilePath, taskName.toLocal8Bit().data());

        d->testInfo.clear();
        RectInfo info;
        info.phi = 0;
        info.XBia = 0;
        READPAM(info.YBia, "YBia1", res);
        d->testInfo.push_back(info);
        READPAM(info.YBia, "YBia2", res);
        d->testInfo.push_back(info);
        READPAM(info.YBia, "YBia3", res);
        d->testInfo.push_back(info);
        READPAM(info.YBia, "YBia4", res);
        d->testInfo.push_back(info);

        READPAM(d->minThre, "minThre", res);
        READPAM(d->maxThre, "maxThre", res);

        //for(auto i : d->testInfo)
        //    qDebug() <<"infolist:" << i.XBia << i.YBia;

        //qDebug() << d->minThre;
        //qDebug() << d->maxThre;

        return 0;

    } catch (...) {
        return CError(PAMREAD, "task: CCheckHanReg, pamRead failed!");
    }
}

CError CCheckTapeIncldHan::pamRead(QMap<QString, QMap<QString, QVariant> > &pamMap)
{
    try {
        auto res = pamMap.value(taskName);

        d->testInfo.clear();
        RectInfo info;
        info.phi = 0;
        info.XBia = 0;
        READPAMINT(info.YBia, "YBia1", res);
        d->testInfo.push_back(info);
        READPAMINT(info.YBia, "YBia2", res);
        d->testInfo.push_back(info);
        READPAMINT(info.YBia, "YBia3", res);
        d->testInfo.push_back(info);
        READPAMINT(info.YBia, "YBia4", res);
        d->testInfo.push_back(info);

        READPAMINT(d->minThre, "minThre", res);
        READPAMINT(d->maxThre, "maxThre", res);

        //for(auto i : d->testInfo)
        //    qDebug() <<"infolist:" << i.XBia << i.YBia;

        //qDebug() << d->minThre;
        //qDebug() << d->maxThre;

        return 0;

    } catch (...) {
        return CError(PAMREAD, "task: CCheckHanReg, pamRead failed!");
    }
}

CError CCheckTapeIncldHan::checkRegionShape(CONSTIMG dst, const QList<QRect>& hanRect, const QList<RectInfo>& detectInfo,
        QList<QRect>& gdRect, QList<QRect>& ngRect, cint minThre, cint maxThre)
{
    try {
        if (hanRect.size() > detectInfo.size())
            return CError(PAMVALUE, "CCheckTapeIncldHan::checkRegionShape hanRect size less than detectInfo size");

        HObject showObj;
        CError err;

        QPoint basePt;
        QRect dtctRect, biaRect;
        RectInfo info;
        QList<RectInfo> inRects;
        QList<RectInfo> gdRects;
        QList<RectInfo> ngRects;

        for (int i = 0; i < hanRect.size() ; i++) {
            Algorithm::getRectByQuadrant(hanRect, dtctRect, i);
            basePt = dtctRect.center();
            info = detectInfo[i];
            info.len1 = hanRect[i].width() / 2;
            info.len2 = hanRect[i].height() / 2;
            info.phi = 0;

            inRects.clear();
            inRects.push_back(info);

            qDebug() << "quadrant:" << i << ": " << dtctRect.topLeft() << dtctRect.bottomRight();
            qDebug() << "info.len" << info.len1 << info.len2;

            gdRects.clear();
            ngRects.clear();
            err = Algorithm::detectRegionExit(dst, showObj, inRects, gdRects, ngRects, basePt, minThre, maxThre, 0, QSize(hanRect[i].width() - 10, 1));

            if (err.isWrong()) {
                QString testInfo = QString("CCheckTapeIncldHan::checkRegionShape index %1, err: %2").arg(i).arg(err.msg());
                return CError(PAMVALUE, testInfo);
            }

            if (gdRects.size()) {
                info = gdRects[0];
                biaRect = QRect(QPoint(info.col - info.len1, info.row - info.len2), QSize(2 * info.len1, 2 * info.len2));
                gdRect.push_back(biaRect);
            } else {
                info = ngRects[0];
                biaRect = QRect(QPoint(info.col - info.len1, info.row - info.len2), QSize(2 * info.len1, 2 * info.len2));
                ngRect.push_back(biaRect);
            }
        }
        return 0;
    }  catch (...) {
        qDebug() << "CSHDetect::CCheckTapeIncldHan crashed";
        return CError(UNEXCEPTION, "CSHDetect::CCheckTapeIncldHan crashed");
    }
}
}
