#include "ccheckcuexit.h"
#include "qdebug.h"

namespace CSH68L {

class CCheckCuExitPrivate
{
public:
    CCheckCuExitPrivate()
    {

    }

    int deblowTop = 50; //大蓝胶以下的顶部
    int deblowButton = 150;//大蓝胶以下矩形的底部
    int cuWidth = 500;
    int subRed = 40;
private:
};

CCheckCuExit::CCheckCuExit():
    d(new CCheckCuExitPrivate())
{

}

CCheckCuExit::~CCheckCuExit()
{
    delete d;
}

CError CCheckCuExit::pamRead(QMap<QString, QMap<QString, QVariant> > &pamMap)
{
    try {
        auto res = pamMap.value(taskName);

        READPAMINT(d->deblowTop, "deblowTop", res);

        READPAMINT(d->deblowButton, "deblowButton", res);
        READPAMINT(d->cuWidth, "cuWidth", res);

        READPAMINT(d->subRed, "subRed", res);
        return 0;

    } catch (...) {
        return CError(PAMREAD, "task: CCheckHanReg, pamRead failed!");
    }
}

CError CCheckCuExit::detect(const L68Regions& region, bool& isCu, QRect& rect)
{
    try {
        CError err = getCuRect(region, rect);
        CHECKERR(err);

        isCu = (d->cuWidth < rect.width());
        return 0;
    }  catch (...) {
        qDebug() << "CCheckCuExit::detect crashed";
        return CError(UNEXCEPTION, "CCheckCuExit::detect crashed");
    }
}

CError CCheckCuExit::getCuRect(const L68Regions& region, QRect& rect)
{
    try {
        CHECKEMPIMG(region.blueRegion, "region.blueregion is empty");
        CHECKEMPIMG(region.m_oriImg, "region.m_oriImg is empty");

        HObject hRect, hReduce, hR, hG, hB, hSubImg;
        HObject hThre, hSlct;
        HTuple row1, col1, row2, col2;
        SmallestRectangle1(region.blueRegion, &row1, &col1, &row2, &col2);

        GenRectangle1(&hRect, row2 + d->deblowTop, col1, row2 + d->deblowButton, col2);
        ReduceDomain(region.m_oriImg, hRect, &hReduce);

        Decompose3(hReduce, &hR, &hG, &hB);
        SubImage(hR, hB, &hSubImg, 1, 0);
        Threshold(hSubImg, &hThre, d->subRed, 255);
        Connection(hThre, &hThre);
        SelectShapeStd(hThre, &hSlct, "max_area", 70);
        CHECKEMPIMG(hSlct, "CCheckCuExit::getCuRect hSlct is empty");

        SmallestRectangle1(hSlct, &row1, &col1, &row2, &col2);
        rect = QRect(QPoint(col1.D(), row1.D()), QPoint(col2.D(), row2.D()));

        return 0;
    }  catch (...) {
        qDebug() << "CCheckCuExit::getCuRect crashed";
        return CError(UNEXCEPTION, "CCheckCuExit::getCuRect crashed");
    }
}

}


