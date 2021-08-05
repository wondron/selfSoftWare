#include "cget68lregion.h"
#include "qdebug.h"
namespace CSH68L {

class CGet68LRegionPrivate
{
public:
    CGet68LRegionPrivate()
    {

    }

    int gridWid = 40;
    int gridHig = 40;
    float bMultiVal = 1.0; //r-b的差值倍数
    int subBlue = 20; //大面蓝胶的蓝色和红色差值
    int whiteRed = 128;

    float tapeMultiVal = 1.0; //r-b的差值倍数

    //蓝胶检测区域的大致获取
    int upRange = 200;     //小蓝胶上部缩进
    int belowRange = 1000; //小蓝胶下延申能够包含住蓝胶的距离
    int leftRange = 200;   //左边缩进
    int rightRange = 200;  //右边缩进

    int tapeGridWid = 40;
    int tapeGridHit = 40;
    int tapeSubBlue = 15; //小蓝胶的蓝色和红色差值
};

CGet68LRegion::CGet68LRegion():
    d(new CGet68LRegionPrivate())
{

}

CGet68LRegion::~CGet68LRegion()
{
    delete d;
}

CError CGet68LRegion::pamRead(QMap<QString, QMap<QString, QVariant> >& pamMap)
{
    try {
        auto res = pamMap.value(taskName);

        READPAMINT(d->gridWid, "gridWid", res);
        READPAMINT(d->gridHig, "gridHig", res);
        READPAMFLOAT(d->bMultiVal, "bMultiVal", res);
        READPAMINT(d->subBlue, "subBlue", res);
        READPAMINT(d->whiteRed, "whiteRed", res);

        READPAMFLOAT(d->tapeMultiVal, "tapeMultiVal", res);
        READPAMINT(d->belowRange, "belowRange", res);
        READPAMINT(d->tapeGridWid, "tapeGridWid", res);
        READPAMINT(d->tapeGridHit, "tapeGridHit", res);
        READPAMINT(d->tapeSubBlue, "tapeSubBlue", res);

        return 0;

    } catch (...) {
        return CError(PAMREAD, "task: CCheckHanReg, pamRead failed!");
    }
}

CError CGet68LRegion::detect(CONSTIMG img, L68Regions& regions, LineInfo& line)
{
    try {
        CError err;
        err = getBlueRegion(img, regions, line);
        CHECKERR(err);

        HObject showObj;
        err = getTapeRegion(img, regions, showObj);
        CHECKERR(err);
        return 0;
    }  catch (...) {
        qDebug() << "CGetRegions::detect crashed";
        return CError(UNEXCEPTION, "CGetRegions::detect crashed");
    }
}

CError CGet68LRegion::getBlueRegion(CONSTIMG img, L68Regions& regions, LineInfo& line)
{
    try {
        CHECKEMPIMG(img, "CGetRegions::getBlueRegion::img is empty");
        regions.m_oriImg = img;
        HObject hR, hG, hB, hSubImg;
        HObject hReduce, hThred, hCuRegion, hBlueRegion;
        HTuple row1, row2, col1, col2;
        Decompose3(img, &hR, &hG, &hB);
        SubImage(hB, hR, &hSubImg, d->bMultiVal, 0);

        regions.m_BImg = hB;
        regions.m_GImg = hG;
        regions.m_RImg = hR;
        regions.m_SubImg = hSubImg;

        CError err = Algorithm::useGridGetRegion(hSubImg, hCuRegion, d->gridWid, d->gridHig, d->subBlue, 0, 1);
        CHECKERR(err);

        ReduceDomain(hR, hCuRegion, &hReduce);
        Threshold(hReduce, &hThred, d->whiteRed, 255);
        Difference(hCuRegion, hThred, &hBlueRegion);
        CHECKEMPIMG(hBlueRegion, "CGetRegions::getBlueRegion::hBlueRegion is empty");
        Connection(hBlueRegion, &hBlueRegion);
        SelectShapeStd(hBlueRegion, &hBlueRegion, "max_area", 70);
        regions.blueRegion = hBlueRegion;

        SmallestRectangle1(regions.blueRegion, &row1, &col1, &row2, &col2);

        line.startCol = col1;
        line.startRow = row2;
        line.endCol = col2;
        line.endRow = row2;

        return 0;
    }  catch (...) {
        qDebug() << "CGetRegions::getBlueRegion crashed";
        return CError(UNEXCEPTION, "CGetRegions::getBlueRegion crashed");
    }
}

CError CGet68LRegion::getTapeRegion(CONSTIMG img, L68Regions& regions, RHOBJ showReg)
{
    try {
        CHECKEMPIMG(img, "CGetRegions::getTapeRegion::img is empty");
        CHECKEMPIMG(regions.blueRegion, "CGetRegions::getTapeRegion::blueRegion is empty");

        HObject hR, hG, hB, hSubImg;
        HObject hRect, hReduce, hGrid;
        HTuple row1, col1, row2, col2;
        SmallestRectangle1(regions.blueRegion, &row1, &col1, &row2, &col2);
        GenRectangle1(&hRect, row2 + d->upRange, col1 + d->leftRange, row2 + d->belowRange, col2 - d->rightRange);
        ReduceDomain(img, hRect, &hReduce);

        Decompose3(hReduce, &hR, &hG, &hB);
        SubImage(hB, hR, &hSubImg, d->tapeMultiVal, 0);
        CError err = Algorithm::useGridGetRegion(hSubImg, hGrid, d->tapeGridWid,
                     d->tapeGridHit, d->tapeSubBlue, 0, 1);
        CHECKERR(err);
        if (Algorithm::objIsEmpty(hGrid).isWrong())
            return CError(LACKAREA, "tape Region is empty");

        regions.tapeRegion = hGrid;
        showReg = regions.blueRegion;
        return 0;
    }  catch (...) {
        qDebug() << "CGetRegions::getTapeRegion crashed";
        return CError(UNEXCEPTION, "CGetRegions::getTapeRegion crashed");
    }
}
}
