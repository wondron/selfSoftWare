#include "ccheckhanregion.h"
#include "qdebug.h"

namespace CSH68L {

class CCheckHanRegionPrivate
{
public:
    CCheckHanRegionPrivate()
    {

    }
    int slctPamNum = 12;

    int biaRow = 270; //偏移到中心
    int biaCol = 970;
    int len1 = 280; //检测矩形长
    int len2 = 100; //检测矩形宽

    int hanSize = 30;//焊印直径
    int ligOrDark = 0; // 0:light, 1:dark
    int dynVal = 11;

    int hanWidMin = 5; //selectshape参数
    int hanWidMax = 60;
    int hanHitMin = 5;
    int hanHitMax = 60;
    int hanAreaMin = 20;
    int hanAreaMax = 500;
    float hanRatioMin = 0.5;
    float hanRatioMax = 1.1;
    float hanConvexityMin = 0.65;
    float hanConvexityMax = 1;
    float circurityMin = 0.48;
    float circurityMax = 1;

    int hanSizeNum = 1; //焊印数量标准

    bool useGridCheck = true; //是否使用网格筛选
    int baseRow = 1120;
    int baseCol = 1995;
    int rowNum = 3;
    int colNum = 14;
    int rowInter = 37;
    int colInter = 38;
    int radius = 18;
    int startRowIndex = 0;
    int startColIndex = -2;
    int rowRange = 10; //筛选范围
    int rowSpotNum = 3;
    int closingCircle = 20;
};


CCheckHanRegion::CCheckHanRegion():
    d(new CCheckHanRegionPrivate())
{

}

CCheckHanRegion::~CCheckHanRegion()
{
    delete d;
}

CError CCheckHanRegion::pamRead(QMap<QString, QMap<QString, QVariant> >& pamMap)
{
    try {
        auto res = pamMap.value(taskName);

        READPAMINT(d->biaRow, "biaRow", res);
        READPAMINT(d->biaCol, "biaCol", res);
        READPAMINT(d->len1, "len1", res);
        READPAMINT(d->len2, "len2", res);

        READPAMINT(d->hanSize, "hanSize", res);
        READPAMINT(d->ligOrDark, "ligOrDark", res);
        READPAMINT(d->dynVal, "dynVal", res);

        READPAMINT(d->hanWidMin, "hanWidMin", res);
        READPAMINT(d->hanWidMax, "hanWidMax", res);
        READPAMINT(d->hanHitMin, "hanHitMin", res);
        READPAMINT(d->hanHitMax, "hanHitMax", res);
        READPAMINT(d->hanAreaMin, "hanAreaMin", res);
        READPAMFLOAT(d->hanRatioMin, "hanRatioMin", res);
        READPAMFLOAT(d->hanRatioMax, "hanRatioMax", res);
        READPAMFLOAT(d->hanConvexityMin, "hanConvexityMin", res);
        READPAMFLOAT(d->hanConvexityMax, "hanConvexityMin", res);

        READPAMINT(d->hanSizeNum, "hanSizeNum", res);

        READPAMINT(d->baseRow, "baseRow", res);
        READPAMINT(d->baseCol, "baseCol", res);
        READPAMINT(d->rowNum, "rowNum", res);
        READPAMINT(d->colNum, "colNum", res);
        READPAMINT(d->rowInter, "rowInter", res);
        READPAMINT(d->colInter, "colInter", res);
        READPAMINT(d->radius, "radius", res);
        READPAMINT(d->startRowIndex, "startRowIndex", res);
        READPAMINT(d->startColIndex, "startColIndex", res);

        READPAMINT(d->rowRange, "rowRange", res);
        READPAMINT(d->rowSpotNum, "rowSpotNum", res);
        READPAMINT(d->closingCircle, "closingCircle", res);

        return 0;
    }  catch (...) {
        qDebug() << "CCheckHanRegion::pamRead crashed";
        return CError(UNEXCEPTION, "CCheckHanRegion::pamRead crashed");
    }
}

CError CCheckHanRegion::detect(const L68Regions& region, QRect& hanDtctRect, HObject& hShowObj, int& hanNum)
{
    try {
        HObject hRect, hShow, hResult;
        HTuple row1, row2, col1, col2;
        HTuple area, row, col;
        SmallestRectangle1(region.blueRegion, &row1, &col1, &row2, &col2);
        CError err = getHanCheckReg(region, hanDtctRect, row2.D(), col1.D());
        if (err.isWrong())
            return err;

        err = getHanSpotNum(region, hanDtctRect, hShow, hanNum);
        if (hanNum < d->hanSizeNum)
            return CError(REGIONNUM, "HanSize num is Lack");

        qDebug() << "enter err111";
        hShowObj = hShow;
        if(!d->useGridCheck)
            return err;

        qDebug() << "enter err";
        SDotArray array;
        array.baseRow = d->baseRow;
        array.baseCol = d->baseCol;
        array.rowNum = d->rowNum;
        array.colNum = d->colNum;
        array.rowInter = d->rowInter;
        array.colInter = d->colInter;
        array.radius = d->radius;
        array.startRowIndex = d->startRowIndex;
        array.startColIndex = d->startColIndex;

        hanNum = 0;
        err = Algorithm::genSpotArrayRegion(array);
        CHECKERR(err);
        err = getRegionWithMask(hShow, hShow, array, hResult, hanNum, d->rowRange, d->rowNum, d->closingCircle);
        CHECKERR(err);
        qDebug() << "enter final";
        hShowObj = hResult;

        if (hanNum < d->hanSizeNum)
            return CError(REGIONNUM, "HanSize num is Lack");
        return err;
    }  catch (...) {
        qDebug() << "CCheckHanRegion::detect crashed";
        return CError(UNEXCEPTION, "CCheckHanRegion::detect crashed");
    }
}

void CCheckHanRegion::setBiaValue(cint colBia, cint rowBia, cint len1, cint len2)
{
    d->biaCol = colBia;
    d->biaRow = rowBia;

    d->len1 = len1;
    d->len2 = len2;
}

void CCheckHanRegion::setdynPam(int dynVal, int hanSize)
{
    d->dynVal = dynVal;
    d->hanSize = hanSize;
}

void CCheckHanRegion::setRowSlectPam(int rowNumMin, int rowRange)
{
    d->rowSpotNum = rowNumMin;
    d->rowRange = rowRange;
}

void CCheckHanRegion::setGenDotArrayPam(const SDotArray array)
{
     d->baseRow = array.baseRow;
     d->baseCol = array.baseCol;
     d->rowNum = array.rowNum;
     d->colNum = array.colNum;
     d->rowInter = array.rowInter;
     d->colInter = array.colInter;
     d->radius = array.radius;
     d->startRowIndex = array.startRowIndex;
     d->startColIndex = array.startColIndex;
}

void CCheckHanRegion::setSlctPam(QList<float> pamList)
{
    int length = pamList.length();

    if(length != d->slctPamNum){
        qDebug() << "slctPam num err:" << pamList.length();
        return;
    }

    d->hanWidMin = pamList[0];
    d->hanWidMax = pamList[1];
    d->hanHitMin =  pamList[2];
    d->hanHitMax =  pamList[3];
    d->hanAreaMin =  pamList[4];
    d->hanAreaMax =  pamList[5];
    d->hanRatioMin =  pamList[6];
    d->hanRatioMax =  pamList[7];
    d->hanConvexityMin =  pamList[8];
    d->hanConvexityMax =  pamList[9];
    d->circurityMin =  pamList[10];
    d->circurityMax = pamList[11];

}

CError CCheckHanRegion::getHanCheckReg(const L68Regions& region, QRect& rect, cint baseRow, cint baseCol)
{
    try {
        QPoint center = QPoint(baseCol + d->biaCol, baseRow + d->biaRow);
        QPoint leftTop = QPoint(center.x() - d->len1, center.y() - d->len2);
        QPoint rigtBtm = QPoint(center.x() + d->len1, center.y() + d->len2);
        QRect dtctRect = QRect(leftTop, rigtBtm);
        rect = dtctRect;
        return 0;
    }  catch (...) {
        qDebug() << "CCheckHanRegion::isHanRegExit crashed";
        return CError(UNEXCEPTION, "CCheckHanRegion::isHanRegExit crashed");
    }
}

CError CCheckHanRegion::getHanSpotNum(const L68Regions& region, const QRect rect, RHOBJ showObj, int& num)
{
    try {
        if (rect.width() < 10)
            return CError(EMPTYOBJ, "RECT size is err");

        HObject image = region.m_GImg;
        CHECKEMPIMG(image, "CError CCheckHanRegion::getHanSpotNum image is empty");

        HObject hRect, hReduce, hMean, hDynRegion, hConnect;
        HObject hSlct1, hSlct2, hSlct3;

        HTuple hNum;
        GenRectangle1(&hRect, rect.top(), rect.left(), rect.bottom(), rect.right());
        ReduceDomain(image, hRect, &hReduce);

        MeanImage(hReduce, &hMean, d->hanSize, d->hanSize);
        QString light = d->ligOrDark == 0 ? "light" : "dark";
        DynThreshold(hReduce, hMean, &hDynRegion, d->dynVal, light.toStdString().c_str());
        Connection(hDynRegion, &hConnect);
        CHECKEMPIMG(hConnect, "CCheckHanRegion::getHanSpotNum hConnect is empty")

        SelectShape(hConnect, &hSlct1, (HTuple("width").Append("height")), "and",
                    (HTuple(d->hanWidMin).Append(d->hanHitMin)), HTuple(d->hanWidMax).Append(d->hanHitMax));
        CHECKEMPIMG(hSlct1, "CCheckHanRegion::getHanSpotNum hSlct1 is empty")

        SelectShape(hSlct1, &hSlct2, (HTuple("area").Append("ratio")), "and",
                    (HTuple(d->hanAreaMin).Append(d->hanRatioMin)), (HTuple(9999).Append(d->hanRatioMax)));
        CHECKEMPIMG(hSlct2, "CCheckHanRegion::getHanSpotNum hSlct2 is empty")

        SelectShape(hSlct2, &hSlct3, "convexity", "and", d->hanConvexityMin, d->hanConvexityMax);
        CHECKEMPIMG(hSlct3, "CCheckHanRegion::getHanSpotNum hSlct3 is empty")

        SelectShape(hSlct2, &hSlct3, "circularity", "and", d->circurityMin, d->circurityMax);
        CHECKEMPIMG(hSlct3, "CCheckHanRegion::getHanSpotNum hSlct3 is empty")

        CountObj(hSlct3, &hNum);
        showObj = hSlct3;
        num = hNum.D();
        return 0;
    } catch (...) {
        qDebug() << "CCheckHanRegion::isHanRegExit crashed";
        return CError(UNEXCEPTION, "CCheckHanRegion::isHanRegExit crashed");
    }
}

CError CCheckHanRegion::getRegionWithMask(CONSTIMG slctObj, CONSTIMG allRegion, const SDotArray mask, RHOBJ resObj, int &num, cint rowRange, cint rowNum, cint closeCircle)
{
    try {
        CHECKEMPIMG(slctObj, "CCheckHanRegion::getRegionWithMask slctobj empty");
        CHECKEMPIMG(allRegion, "CCheckHanRegion::getRegionWithMask allRegion empty");
        CHECKEMPIMG(mask.arrayObj, "CCheckHanRegion::getRegionWithMask mask empty");

        HObject hSlctObj, hAllRegion, hMask, hSortedReg, hSortedReg1;
        HObject hSingle, hSlctShape, hUnionReg, hIntersect, hClosing, hAffine, hFinalReg;
        HTuple tNum, tNum1, tArea, tRow, tCol, tInterArea, tInterRow, tInterCol;
        HTuple homMat2d, homMat2dTran;

        HTuple maxNum = 0, finalNum;
        HObject maxObj;

        CError err;
        Connection(slctObj, &hSlctObj);
        Connection(allRegion, &hAllRegion);
        Connection(mask.arrayObj, &hMask);

        GenEmptyObj(&hUnionReg);
        SortRegion(hSlctObj, &hSortedReg, "upper_left", "true", "row");
        CountObj(hSortedReg, &tNum);

        for ( int i = 1; i <= tNum.D(); i++) {
            SelectObj(hSortedReg, &hSingle, i);
            AreaCenter(hSingle, &tArea, &tRow, &tCol);
            SelectShape(hSortedReg, &hSlctShape, "row", "and", tRow.D() - rowRange, tRow.D() + rowRange);
            CountObj(hSlctShape, &tNum1);
            if (tNum1.D() > rowNum) {
                Intersection(hSlctShape, hUnionReg, &hIntersect);
                AreaCenter(hIntersect, &tInterArea, &tInterRow, &tInterCol);

                err = Algorithm::tupleisEmpty(tInterArea);
                if (!err.isWrong()){
                    if(tInterArea[0].D() > 0)
                        continue;
                }

                Union2(hUnionReg, hSlctShape, &hUnionReg);
                ClosingCircle(hSlctShape, &hClosing, closeCircle);
                SortRegion(hClosing, &hSortedReg1, "upper_left", "true", "column");

                for (int index = 1; index <= tNum1.D(); index++) {
                    SelectObj(hSortedReg1, &hSlctObj, index);
                    AreaCenter(hSlctObj, &tArea, &tRow, &tCol);

                    HomMat2dIdentity(&homMat2d);
                    HomMat2dTranslate(homMat2d, tRow[0].D() - mask.baseRow, tCol[0].D() - mask.baseCol, &homMat2dTran);

                    AffineTransRegion(hMask, &hAffine, homMat2dTran, "nearest_neighbor");

                    SelectShapeProto(hAllRegion, hAffine, &hSlctObj, "overlaps_rel", 90, 100);
                    SelectShape(hSlctObj, &hSlctObj, "circularity", "and", 0.4, 1);

                    CountObj(hSlctObj, &finalNum);

                    if (finalNum[0].D() > maxNum[0].D()) {
                        maxNum = finalNum;
                        maxObj = hSlctObj;
                    }
                }
            }
        }
        CHECKEMPIMG(maxObj, "CCheckHanRegion::getRegionWithMask maxObj empty");
        Intersection(maxObj, hUnionReg, &hFinalReg);
        CountObj(hFinalReg, &finalNum);
        resObj = hFinalReg;
        num = finalNum.D();
        return 0;
    } catch (...) {
        qDebug() << "CCheckHanRegion::getRegionWithMask crashed!";
        return CError(UNEXCEPTION, "CCheckHanRegion::getRegionWithMask crashed!");
    }
}

}

