#include "cgetregions.h"
#include "../commonUsage/algorithm.h"
#include "../commonUsage/commonDef.h"
#include "XmlRead.h"
#include "QDebug"

namespace CSHDetect {

class CGetRegionsPrivate
{
public:
    CGetRegionsPrivate(CGetRegions* parent = nullptr)
    {
        p = parent;
    }
    ~CGetRegionsPrivate() {}

    int m_width;
    int m_height;
    HObject m_oriImg;

    //总体
    int direct = LEFTRIGHT; //两极的方向
    int tapeNum = 2;   //中间区域胶带数量

    //黑棒参数
    int blkBangMaxThre = 20; //黑棒检测参数

    //黑色区域
    int darkMaxR = 30;
    int darkMaxG = 30;
    int darkMaxB = 30;

    //白色区域
    int whiteMinR = 200;
    int whiteMinG = 200;
    int whiteMinB = 200;
    int WhiteMaxArea = 1500;   //筛选掉最大的白色缺陷面积

    //底面胶带区域
    double guassSigma = 2.3;
    double guassFactor = 1.6;
    int thresMax = -3;
    double tapeHole = 10;

    //蓝色区域获取
    int blueSubVal = 51;

    //中间区域
    int rectWid = 400;
    int eroWidth = 0;
    int slctNum = 3;

    //胶带宽度
    int blueTapeWid = 1500;
    int ringWid = 40;

private:
    CGetRegions* p;
};

CGetRegions::CGetRegions(): d(new CGetRegionsPrivate(this))
{

}

CGetRegions::~CGetRegions()
{
    delete d;
}

CError CGetRegions::detect(CONSTIMG obj, RegionS& res)
{
    CHECKEMPIMG(obj, "CGetRegions::detect obj is empty");

    CError err;
    d->m_oriImg = obj;
    res.m_oriImg = obj;
    err = transformImg(res.m_oriImg, res);
    CHECKERR(err);

    err = getBlackBangRoi(res, d->blkBangMaxThre);
    CHECKERR(err);

    err = getDarkRegion(res, d->darkMaxR, d->darkMaxG, d->darkMaxB);
    CHECKERR(err);

    err = getWhiteRegion(res, d->whiteMinR, d->whiteMinG, d->whiteMinB, d->WhiteMaxArea);
    CHECKERR(err);

    err = getTapeRegion(res, d->guassSigma, d->guassFactor, d->thresMax, d->tapeHole);
    CHECKERR(err);

    err = getBlueRegion(res, d->blueSubVal);
    CHECKERR(err);

    err = getMidRect(res, d->direct, d->rectWid, d->eroWidth, d->slctNum);
    CHECKERR(err);

    int size;
    err = getBlueTape(res, size, d->blueTapeWid, d->ringWid);

    res.TapeNum = size;
    if (size != d->tapeNum)
        return CError(REGIONNUM, QString("size of weld is not %1,result is %2").arg(d->tapeNum).arg(size));

    return 0;
}

CError CGetRegions::transformImg(CONSTIMG obj, RegionS& res)
{
    try {
        CHECKEMPIMG(obj, "CGetRegions::transformImg oriImg is empty!");
        res.m_oriImg = obj;

        HTuple Width, Height;
        HObject m_hsvHImg, m_hsvSImg, m_hsvVImg;
        Decompose3(obj, &res.m_RImg, &res.m_GImg, &res.m_BImg);

        TransFromRgb(res.m_RImg, res.m_GImg, res.m_BImg, &m_hsvHImg, &m_hsvSImg, &m_hsvVImg, "hsv");
        res.m_hsvHImg = m_hsvHImg;
        res.m_hsvSImg = m_hsvSImg;
        res.m_hsvVImg = m_hsvVImg;

        GetImageSize(res.m_oriImg, &Width, &Height);
        CHECKEMPTUPLE(Width, "get image size failed!");

        res.width = Width.D();
        res.height = Height.D();
        d->m_width = Width.D();
        d->m_height = Height.D();
        return 0;
    }  catch (...) {
        return CError(UNEXCEPTION, "CGetRegions::transformImg crashed!");
    }
}

CError CGetRegions::pamRead(const char* xmlfilePath)
{
    XmlRead xmlRead;

    try {
        std::map<std::string, xmlInfo> res = xmlRead.parseXML(xmlfilePath, taskName.toLocal8Bit().data());

        READPAM(d->direct, "direct", res);
        READPAM(d->tapeNum, "tapeNum", res);

        READPAM(d->blkBangMaxThre, "blkBangMaxThre", res);

        READPAM(d->darkMaxR, "darkMaxR", res);
        READPAM(d->darkMaxG, "darkMaxG", res);
        READPAM(d->darkMaxB, "darkMaxB", res);

        READPAM(d->whiteMinR, "whiteMinR", res);
        READPAM(d->whiteMinG, "whiteMinG", res);
        READPAM(d->whiteMinB, "whiteMinB", res);
        READPAM(d->WhiteMaxArea, "WhiteMaxArea", res);

        READPAM(d->guassSigma, "guassSigma", res);
        READPAM(d->guassFactor, "guassFactor", res);
        READPAM(d->thresMax, "thresMax", res);
        READPAM(d->tapeHole, "tapeHole", res);

        READPAM(d->blueSubVal, "blueSubVal", res);

        READPAM(d->rectWid, "rectWid", res);
        READPAM(d->eroWidth, "eroWidth", res);
        READPAM(d->slctNum, "slctNum", res);

        READPAM(d->blueTapeWid, "blueTapeWid", res);
        return 0;

    } catch (...) {
        return CError(PAMREAD, "task: CuPosDetecte, pamRead failed!");
    }
}

CError CGetRegions::pamRead(QMap<QString, QMap<QString, QVariant> >& pamMap)
{
    try {
        auto res = pamMap.value(taskName);

        READPAMINT(d->direct, "direct", res);
        READPAMINT(d->tapeNum, "tapeNum", res);

        READPAMINT(d->blkBangMaxThre, "blkBangMaxThre", res);

        READPAMINT(d->darkMaxR, "darkMaxR", res);
        READPAMINT(d->darkMaxG, "darkMaxG", res);
        READPAMINT(d->darkMaxB, "darkMaxB", res);

        READPAMINT(d->whiteMinR, "whiteMinR", res);
        READPAMINT(d->whiteMinG, "whiteMinG", res);
        READPAMINT(d->whiteMinB, "whiteMinB", res);
        READPAMINT(d->WhiteMaxArea, "WhiteMaxArea", res);

        READPAMFLOAT(d->guassSigma, "guassSigma", res);
        READPAMFLOAT(d->guassFactor, "guassFactor", res);
        READPAMINT(d->thresMax, "thresMax", res);
        READPAMFLOAT(d->tapeHole, "tapeHole", res);

        READPAMINT(d->blueSubVal, "blueSubVal", res);

        READPAMINT(d->rectWid, "rectWid", res);
        READPAMINT(d->eroWidth, "eroWidth", res);
        READPAMINT(d->slctNum, "slctNum", res);

        READPAMINT(d->blueTapeWid, "blueTapeWid", res);
        return 0;

    } catch (...) {
        return CError(PAMREAD, "task: CuPosDetecte, pamRead failed!");
    }
}

CError CGetRegions::getBlackBangRoi(RegionS& res, cint maxThre)
{
    try {
        HObject region, connectReg, selectReg;

        HObject maxSlctReg, openingReg, finalMaxReg;
        HTuple width, height;
        CError err;

        HObject hsvV = res.m_hsvVImg;
        CHECKEMPIMG(hsvV, "getBlackBangRoi::hsvVImg is empty");
        CHECKTHREVALUE(maxThre, "getBlackBangRoi::maxThre out of range");

        GetImageSize(hsvV, &width, &height);

        d->m_width = width.I();
        d->m_height = height.I();

        Threshold(hsvV, &region, 0, maxThre);
        Connection(region, &connectReg);
        SelectShape(connectReg, &selectReg, (HTuple("column").Append("row")),
                    "and", (width / 3).TupleConcat(height / 3), ((2 * width) / 3).TupleConcat((2 * height) / 3));

        CHECKEMPIMG(selectReg, "after select obj is empty");

        SelectShapeStd(selectReg, &maxSlctReg, "max_area", 70);
        FillUp(maxSlctReg, &maxSlctReg);

        OpeningCircle(maxSlctReg, &openingReg, 30);
        Connection(openingReg, &openingReg);

        SelectShapeStd(openingReg, &finalMaxReg, "max_area", 70);
        ShapeTrans(finalMaxReg, &res.BlackBangRegion, "convex");

        return 0;
    }  catch (...) {
        return CError(UNEXCEPTION, "CGetRegions::getBlackBangRoi crashed!");
    }
}

CError CGetRegions::getDarkRegion(RegionS& res, cint maxR, cint maxG, cint maxB)
{
    try {
        HObject  hRegion, hRegion1, hRegion2, hRegionIntersection;
        HObject RImg, GImg, BImg, darkRegion;

        RImg = res.m_RImg;
        GImg = res.m_GImg;
        BImg = res.m_BImg;

        CHECKEMPIMG(RImg, "getDarkRegion func: Rimg is empty");
        CHECKEMPIMG(GImg, "getDarkRegion func: Gimg is empty");
        CHECKEMPIMG(BImg, "getDarkRegion func: Bimg is empty");

        CHECKTHREVALUE(maxR, "getDarkRegion func: maxR out of range");
        CHECKTHREVALUE(maxG, "getDarkRegion func: maxG out of range");
        CHECKTHREVALUE(maxB, "getDarkRegion func: maxB out of range");

        Threshold(RImg, &hRegion, 0, maxR);
        Threshold(GImg, &hRegion1, 0, maxG);
        Threshold(BImg, &hRegion2, 0, maxB);

        Intersection(hRegion2, hRegion1, &hRegionIntersection);
        Intersection(hRegionIntersection, hRegion, &darkRegion);

        CHECKEMPIMG(darkRegion, "getDarkRegion func: final result is empty");
        res.DarkRegion = darkRegion;
        return 0;
    }  catch (...) {
        return CError(UNEXCEPTION, "CGetRegions::getDarkRegion crashed!");
    }
}

CError CGetRegions::getWhiteRegion(RegionS& res, cint minR, cint minG, cint minB, cint maxArea)
{
    try {
        HObject  hRegion, hRegion1, hRegion2, hRegionIntersection;
        HObject  hRegionOpening, hconnectedRegions, hslctRegion;
        HObject  hSelectedRegions2, hRectangle;

        CError err;

        CHECKEMPIMG(res.m_RImg, "getDarkRegion func: Rimg is empty");
        CHECKEMPIMG(res.m_GImg, "getDarkRegion func: Gimg is empty");
        CHECKEMPIMG(res.m_BImg, "getDarkRegion func: Bimg is empty");

        CHECKTHREVALUE(minR, "getDarkRegion func: maxR out of range");
        CHECKTHREVALUE(minG, "getDarkRegion func: maxG out of range");
        CHECKTHREVALUE(minB, "getDarkRegion func: maxB out of range");

        Threshold(res.m_RImg, &hRegion, minR, 255);
        Threshold(res.m_GImg, &hRegion1, minG, 255);
        Threshold(res.m_BImg, &hRegion2, minB, 255);

        Intersection(hRegion2, hRegion1, &hRegionIntersection);
        Intersection(hRegionIntersection, hRegion, &hRegion);
        CHECKEMPIMG(hRegion, "getWhiteRrgion func: hRegionOpening is empty");

        Connection(hRegion, &hconnectedRegions);
        SelectShape(hconnectedRegions, &hslctRegion, "area", "and", 0, maxArea);
        Difference(hRegion, hslctRegion, &res.WhiteRegion);
        return 0;

    }  catch (...) {
        return CError(UNEXCEPTION, "CGetRegions::getWhiteRrgion crashed!");
    }
}

CError CGetRegions::getTapeRegion(RegionS& res, double guassSigma, double guassFactor, cint thresMax, cint tapesize)
{
    try {
        HObject BImage = res.m_BImg;
        CHECKEMPIMG(BImage, "getTapeRegion::Bimg is empty");

        HObject  diffOfGauss, region, connectedRegions;
        HObject  slctRegion, slctRegion1, hUnion;
        HObject  RegionDilation, RegionFillUp, RegionErosion;
        HObject  hConnect, RegionClosing;

        HTuple  Area, Row, Column, Sorted;
        HTuple tapeHole = tapesize;

        DiffOfGauss(BImage, &diffOfGauss, guassSigma, guassFactor);
        Threshold(diffOfGauss, &region, -67, thresMax);
        Connection(region, &connectedRegions);

        SelectShape(connectedRegions, &slctRegion, ((HTuple("width").Append("height")).Append("holes_num")),
                    "and", ((HTuple(0).Append(0)).Append(0)), ((3 * tapeHole).TupleConcat(3 * tapeHole)).TupleConcat(0));

        SelectShape(slctRegion, &slctRegion1, "area", "and", 3 * tapeHole, 40 * tapeHole);
        SelectShape(slctRegion1, &slctRegion1, "compactness", "and", 0, 2);

        Union1(slctRegion1, &hUnion);
        DilationRectangle1(hUnion, &RegionDilation, tapeHole, tapeHole);
        FillUp(RegionDilation, &RegionFillUp);
        ErosionRectangle1(RegionFillUp, &RegionErosion, tapeHole, tapeHole);
        Connection(RegionErosion, &hConnect);
        AreaCenter(hConnect, &Area, &Row, &Column);
        TupleSort(Area, &Sorted);

        SelectShape(hConnect, &slctRegion1, "area", "and",
                    HTuple(Sorted[(Sorted.TupleLength()) - 4]),
                    HTuple(Sorted[(Sorted.TupleLength()) - 1]));

        ClosingCircle(slctRegion1, &RegionClosing, 5 * tapeHole);
        ErosionCircle(RegionClosing, &res.TapeRegion, 3.5);

        return 0;
    }  catch (...) {
        return CError(UNEXCEPTION, "CGetRegions::getTapeRegion crashed!");
    }
}

CError CGetRegions::getBlueRegion(RegionS& res, cint blueSubVal)
{
    try {

        HObject rImg, bImg, tapeRegion, blueRegion;

        rImg = res.m_RImg;
        bImg = res.m_BImg;
        tapeRegion = res.TapeRegion;

        CHECKEMPIMG(rImg, "CGetRegions::getBlueRegion: rImg is empty");
        CHECKEMPIMG(bImg, "CGetRegions::getBlueRegion: bImg is empty");
        CHECKEMPIMG(tapeRegion, "CGetRegions::getBlueRegion: tapeRegion is empty");

        HObject  hImageSub, hRegion;

        SubImage(bImg, rImg, &hImageSub, 1, 0);
        Threshold(hImageSub, &hRegion, blueSubVal, 255);

        CHECKEMPIMG(hRegion, "CGetRegions::getBlueRegion: hRegion is empty");
        Difference(hRegion, tapeRegion, &hRegion);
        res.BlueRegion = hRegion;

        return 0;
    }  catch (...) {
        return CError(UNEXCEPTION, "CGetRegions::getBlueRegion crashed!");
    }
}

CError CGetRegions::getMidRect(RegionS& res, cint direct, cint rectWid, cint eroWidth, cint slctNum)
{
    try {
        // Local iconic variables
        HObject  hRectangle, hRegionDifference, hConnectedRegions1;
        HObject  hSelectedRegions1, hRectangle1;

        HTuple  hvwidth, hvheight, hvrowEro, hvcolEro;
        HTuple  hvRow1, hvColumn1, hvRow2, hvColumn2, hvArea;
        HTuple  hvRow, hvColumn, hvSorted, hvArea1, hvRow3;
        HTuple  hvColumn3, hvrowMean, hvcolMean, tupleLen;

        if (direct == 1) {
            hvwidth = 9999;
            hvheight = rectWid;
            hvrowEro = 0;
            hvcolEro = eroWidth;
        } else {
            hvwidth = 9999;
            hvheight = rectWid;
            hvrowEro = eroWidth;
            hvcolEro = 0;
        }

        HObject hBlueRegion, hWhiteRegion;
        hBlueRegion = res.BlueRegion;
        hWhiteRegion = res.WhiteRegion;
        CHECKEMPIMG(hWhiteRegion, "CGetRegions::getMidRect: hWhiteRegion is empty");
        CHECKEMPIMG(hBlueRegion, "CGetRegions::getMidRect: hBlueRegion is empty");

        SmallestRectangle1(hBlueRegion, &hvRow1, &hvColumn1, &hvRow2, &hvColumn2);
        GenRectangle1(&hRectangle, hvRow1 + hvrowEro, hvColumn1 + hvcolEro, hvRow2 - hvrowEro,  hvColumn2 - hvcolEro);
        Difference(hRectangle, hWhiteRegion, &hRegionDifference);
        Difference(hRegionDifference, hBlueRegion, &hRegionDifference);
        Connection(hRegionDifference, &hConnectedRegions1);
        AreaCenter(hConnectedRegions1, &hvArea, &hvRow, &hvColumn);
        TupleSort(hvArea, &hvSorted);

        tupleLen = hvArea.Length();
        SelectShape(hConnectedRegions1, &hSelectedRegions1, "area", "and", HTuple(hvSorted[tupleLen - slctNum]), HTuple(hvSorted[tupleLen - 1]));
        AreaCenter(hSelectedRegions1, &hvArea1, &hvRow3, &hvColumn3);
        TupleMean(hvRow3, &hvrowMean);
        TupleMean(hvColumn3, &hvcolMean);
        GenRectangle2(&hRectangle1, hvrowMean, hvcolMean, 0, hvwidth, hvheight);
        Intersection(hRectangle, hRectangle1, &res.MidRect);
        return 0;
    }  catch (...) {
        return CError(UNEXCEPTION, "CGetRegions::getMidRect crashed!");
    }
}

CError CGetRegions::getBlueTape(RegionS& res, int& tapeNum, cint blueTapeWid, cint ringWid)
{
    try {
        HObject  hRegionIntersection, hRegionIntersection1;
        HObject  hRegionUnion, hConnectedRegions, hSelectedRegions;
        HObject  hRegionOpening1, hConnectedRegions2, hRegionFillUp;
        HObject  hRegionTrans, hRegionUnion1, hConnectedRegions1;

        HObject hMidRect, hBlueRegion, hWhiteRegion, hBlueTape;
        HTuple htapeNum;
        hMidRect = res.MidRect;
        hBlueRegion = res.BlueRegion;
        hWhiteRegion = res.WhiteRegion;

        CHECKEMPIMG(hWhiteRegion, "CGetRegions::getBlueTape: hWhiteRegion is empty");
        CHECKEMPIMG(hBlueRegion, "CGetRegions::getBlueTape: hBlueRegion is empty");
        CHECKEMPIMG(hMidRect, "CGetRegions::getBlueTape: hMidRect is empty");

        Intersection(hMidRect, hBlueRegion, &hRegionIntersection);
        Intersection(hMidRect, hWhiteRegion, &hRegionIntersection1);
        Union2(hRegionIntersection, hRegionIntersection1, &hRegionUnion);
        Connection(hRegionUnion, &hConnectedRegions);
        SelectShape(hConnectedRegions, &hSelectedRegions, "width", "and", blueTapeWid * 0.7, 99999);
        CHECKEMPIMG(hSelectedRegions, "CGetRegions::getBlueTape: hSelectedRegions is empty");

        OpeningCircle(hConnectedRegions, &hRegionOpening1, ringWid);
        Union1(hRegionOpening1, &hRegionOpening1);
        Connection(hRegionOpening1, &hConnectedRegions2);
        SelectShape(hConnectedRegions2, &hConnectedRegions2, "width", "and", blueTapeWid * 0.7, 99999);
        CHECKEMPIMG(hConnectedRegions2, "CGetRegions::getBlueTape: hConnectedRegions2 is empty");
        FillUp(hConnectedRegions2, &hRegionFillUp);

        ShapeTrans(hRegionFillUp, &hRegionTrans, "convex");
        Union1(hRegionTrans, &hRegionUnion1);
        Intersection(hRegionUnion1, hSelectedRegions, &hBlueTape);
        CHECKEMPIMG(hBlueTape, "CGetRegions::getBlueTape: hBlueTape is empty");
        res.MidBlueTape = hBlueTape;

        CountObj(hConnectedRegions2, &htapeNum);
        tapeNum = htapeNum.D();

        return 0;
    }  catch (...) {
        return CError(UNEXCEPTION, "CGetRegions::getBlueTape crashed!");
    }
}
}
