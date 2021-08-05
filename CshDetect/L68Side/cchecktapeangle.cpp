#include "cchecktapeangle.h"
#include "qdebug.h"

namespace CSH68L {

class CCheckTapeAnglePrivate
{
public:
    CCheckTapeAnglePrivate()
    {

    }

    int minTapeWid = 200; //小蓝胶的最小宽度
    int minTapeHig = 80;  //小蓝胶的最小高度

    float subMulti = 3.0;

    float sigma = 1.0;
    int ptsSlct = FIRSTPNT;
    int rectLen1 = 200;
    int rectLen2 = 30;
    int threshold = 70;
    int transite = POSITIVE;

    float angleUp = 12.0;
    float angleDn = -12.0;
};

CCheckTapeAngle::CCheckTapeAngle():
    d(new CCheckTapeAnglePrivate())
{

}

CCheckTapeAngle::~CCheckTapeAngle()
{
    delete d;
}

CError CCheckTapeAngle::pamRead(QMap<QString, QMap<QString, QVariant> > &pamMap)
{
    try {
        auto res = pamMap.value(taskName);

        READPAMFLOAT(d->subMulti, "subMulti", res);

        READPAMINT(d->ptsSlct, "ptsSlct", res);
        READPAMFLOAT(d->sigma, "sigma", res);
        READPAMINT(d->rectLen1, "rectLen1", res);
        READPAMINT(d->rectLen2, "rectLen2", res);
        READPAMINT(d->threshold, "threshold", res);
        READPAMINT(d->transite, "transite", res);

        READPAMFLOAT(d->angleUp, "angleUp", res);
        READPAMFLOAT(d->angleDn, "angleDn", res);
        return 0;

    } catch (...) {
        return CError(PAMREAD, "task: CCheckHanReg, pamRead failed!");
    }
}

CError CCheckTapeAngle::detect(const L68Regions& region, QRect& tapeRect, LineInfo& res, bool &isFinal)
{
    try {
        HObject showRect;
        isFinal = false;

        CError err = checkTapeSize(region, tapeRect);
        CHECKERR(err);

        //err = getLineAngle(region, res, showRect);
        //CHECKERR(err);

        //isFinal = true;
        //if(res.angle < d->angleDn || res.angle > d->angleUp)
        //    return CError(ANGLE, "tape angle out of range");
        return 0;
    }  catch (...) {
        qDebug() << "cchecketapeangle::detect crashed";
        return CError(UNEXCEPTION, "CCheckTapeAngle::detect crashed");
    }
}

CError CCheckTapeAngle::checkTapeSize(const L68Regions &region, QRect &tapeRect)
{
    try {
        CHECKEMPIMG(region.tapeRegion, "CCheckTapeAngle::checkTapeSize tapeRegion is empty");

        HObject hRect;
        HTuple row1, col1, row2, col2;
        SmallestRectangle1(region.tapeRegion, &row1, &col1, &row2, &col2);

        int width = col2.D() - col1.D();
        int hight = row2.D() - row1.D();
        qDebug() << "width: "<< width << "hight: " << hight;

        if(width < d->minTapeWid || hight < d->minTapeHig)
            return CError(LACKAREA, "lack tape region");

        tapeRect = QRect(col1.D(), row1.D(), width, hight);
        return 0;
    }  catch (...) {
        qDebug() << "cchecketapeangle::checkTapeSize crashed";
        return CError(UNEXCEPTION, "CCheckTapeAngle::checkTapeSize crashed");
    }
}

CError CCheckTapeAngle::getLineAngle(const L68Regions& region, LineInfo& res, RHOBJ detectRegion)
{
    try {
        CHECKEMPIMG(region.tapeRegion, "getLineAngle:: region:taperegion is empty");
        CHECKEMPIMG(region.m_oriImg, " CCheckTapeAngle::getLineAngle moriimage is empty");

        HObject showLine, showPts;
        HObject hRect, hReduce;
        HObject hR, hG, hB, hSubImg;
        HTuple row1, col1, row2, col2, hWid, hHig;
        SmallestRectangle1(region.tapeRegion, &row1, &col1, &row2, &col2);

        hWid = col2 - col1;
        hHig = row2 - row1;

        GenRectangle1(&hRect, row2 - hHig / 3, col1, row2 + hHig / 3, col2);
        ReduceDomain(region.m_oriImg, hRect, &hReduce);
        Decompose3(hReduce, &hR, &hG, &hB);
        SubImage(hB, hG, &hSubImg, d->subMulti, 0);

        LineInfo dtctInfo;
        dtctInfo.startCol = col1;
        dtctInfo.startRow = row2;
        dtctInfo.endCol = col2;
        dtctInfo.endRow = row2;

        MeasureposPam pam;
        pam.pntSelect = d->ptsSlct;
        pam.sigma = d->sigma;
        pam.recLen1 = d->rectLen1;
        pam.recLen2 = d->rectLen2;
        pam.threshold = d->threshold;
        pam.transition = d->transite;

        CError err = Algorithm::edgeLineDtct(hSubImg, showLine, showPts, res, 1, 30, dtctInfo, pam);
        detectRegion = showPts;
        qDebug() << "getLineAngle done";
        CHECKERR(err);

        return 0;
    }  catch (...) {
        qDebug() << "getLineAngle crashed;";
        return CError(UNEXCEPTION, "getLineAngle crashed");
    }
}

}


