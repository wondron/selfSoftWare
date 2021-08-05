#include "algorithm.h"
#include "QList"
#include "qline.h"
#include "qdebug.h"
#include "commonDef.h"
#include "QFileInfo"
#include "QTime"

using namespace Detect;
using namespace HalconCpp;

Algorithm::Algorithm()
{

}

CError Algorithm::objIsEmpty(CONSTIMG obj)
{
    try {
        bool ini = obj.IsInitialized();
        if (!ini) {
            return 1;
        }

        HObject null;
        HTuple number;
        GenEmptyObj(&null);
        TestEqualObj(obj, null, &number); //判定是否和空对象相等。

        ini = number == 1 ? 1 : 0;
        return ini;
    }  catch (...) {
        return CError(2, "Algorithm unexpected happened!");
    }
}

CError Algorithm::tupleisEmpty(const HTuple& tuple)
{
    try {
        HTuple Length;
        TupleLength(tuple, &Length);
        bool ini = (Length.I() == 0);
        return ini;
    }  catch (...) {
        return CError(2, "Algorithm unexpected happened!");
    }
}

CCircleInfo Algorithm::getAverCircle(QList<CCircleInfo> info)
{
    CCircleInfo res;
    int num = 0;
    double totalX = 0;
    double totalY = 0;
    double totalR = 0;
    for (auto i : info) {
        totalX += i.X;
        totalY += i.Y;
        totalR += i.Radius;
        num ++;
    }

    res.X = totalX / num;
    res.Y = totalY / num;
    res.Radius = totalR / num;
    return res;
}

CError Algorithm::isFileExist(const QString filename)
{
    try {
        QFileInfo fileCheck;
        fileCheck.setFile(filename);

        if (fileCheck.exists()) {
            return 0;
        } else {
            return CError(NG, QString("* %1 * is not exist").arg(filename));
        }
    }  catch (...) {
        return CError(UNEXCEPTION, "Algorithm::isfileExist is crashed");
    }
}

QPointF Algorithm::getCrossPoint(qreal pX, qreal pY, qreal lX1, qreal lY1, qreal lX2, qreal lY2)
{
    qreal lK = (lY2 - lY1) / (lX2 - lX1);
    qreal pK = -1 / (lK);
    qreal lB = lY1 - lK * lX1;
    qreal pB = pY - pK * pX;

    qreal X = (pB - lB) / (lK - pK);
    qreal Y = lK * X + lB;

    return QPointF(X, Y);
}

LineInfo Algorithm::getCrossInfo(qreal pX, qreal pY, const LineInfo& info)
{
    LineInfo data = info;
    qreal lX1 = info.startCol;
    qreal lY1 = info.startRow;
    qreal lX2 = info.endCol;
    qreal lY2 = info.endRow;

    qreal lK = (lY2 - lY1) / (lX2 - lX1);
    qreal pK = -1 / (lK);
    qreal lB = lY1 - lK * lX1;
    qreal pB = pY - pK * pX;

    qreal X = (pB - lB) / (lK - pK);
    qreal Y = lK * X + lB;

    data.pX = pX;
    data.pY = pY;
    data.crossX = X;
    data.crossY = Y;
    data.distance = QLineF(pX, pY, X, Y).length();

    return data;
}

const QString Algorithm::getErrDescri(cint errIndex)
{
    QString info;
    switch (errIndex) {
    case LACKAREA:
        info = "lackarea";
        break;
    case CUPOS:
        info = "cupos";
        break;
    case REGIONNUM:
        info = "regionnum";
        break;
    case HANSIZE:
        info = "hanSize";
        break;
    case EMPTYOBJ:
        info = "emptyRegion";
        break;
    default:
        info = "Other";
    }
    return info;
}

const QString Algorithm::getErrDescri(cint errIndex, cint Pos)
{
    QString info = QObject::tr(" ");
    QString pos = QObject::tr(" ");
    switch (Pos) {
    case CUTOP:
        pos = QObject::tr("CUTOP");
        break;
    case CUBTM:
        pos = QObject::tr("CUBTM");
        break;
    case ALTOP:
        pos = QObject::tr("ALTOP");
        break;
    case ALBTM:
        pos = QObject::tr("ALBTM");
        break;
    default:
        break;
    }

    switch (errIndex) {
    case LACKAREA:
        info = QObject::tr("lackarea");
        break;
    case CUPOS:
        info = QObject::tr("cupos");
        break;
    case REGIONNUM:
        info = QObject::tr("regionnum");
        break;
    case HANSIZE:
        info = QObject::tr("hanSize");
        break;
    case EMPTYOBJ:
        info = QObject::tr("emptyRegion");
        break;
    case HanRegHig:
        info = QObject::tr("height");
        break;
    case HanRegWidth:
        info = QObject::tr("width");
        break;
    case HanRegNum:
        info = QObject::tr("num");
        break;
    default:
        info = QObject::tr("Other");
    }

    QString item = pos + info;
    return item;
}

void Algorithm::saveHImage(CONSTIMG region, CONSTIMG dst, const char* filepath)
{
    try {
        bool empty = objIsEmpty(region);
        if (empty) {
            qDebug() << "Algorithm::saveHImage region is empty";
            return ;
        }

        empty = objIsEmpty(dst);
        if (empty) {
            qDebug() << "Algorithm::saveHImage dst is empty";
            return ;
        }

        HObject saveImg;
        HTuple Width, Height;
        GetImageSize(dst, &Width, &Height);
        RegionToBin(region, &saveImg, 255, 0, Width, Height);

        WriteImage(saveImg, "bmp", 0, filepath);
    }  catch (...) {
        qDebug() << "Algorithm::saveHImage crashed";
    }
}

CError Algorithm::dynThre(CONSTIMG dst, RHOBJ res, cint threVal, cint meanSize)
{
    try {
        CHECKEMPIMG(dst, "Algorithm::dynThre is empty");

        HObject ImageMean;
        MeanImage(dst, &ImageMean, meanSize, meanSize);
        DynThreshold(dst, ImageMean, &res, threVal, "light");
        ClearObj(ImageMean);
        return 0;
    } catch (...) {
        return CError(UNEXCEPTION, "Algorithm::dynThre crashed!");
    }
}

CError Algorithm::dynThre(const HObject& dst, HObject& res, cint threVal, cint meanSize, cint ligORDark)
{
    try {
        CHECKEMPIMG(dst, "Algorithm::dynThre is empty");

        HObject ImageMean;
        MeanImage(dst, &ImageMean, meanSize, meanSize);

        QString ligOrDk = ligORDark == 0 ? "light" : "dark";
        DynThreshold(dst, ImageMean, &res, threVal, ligOrDk.toStdString().c_str());
        ClearObj(ImageMean);
        return 0;
    } catch (...) {
        return CError(UNEXCEPTION, "Algorithm::dynThre crashed!");
    }
}

CError Algorithm::edgeLineDtct(CONSTIMG dst, RHOBJ showLine, RHOBJ showPoints, LineInfo& resLine, cint direct, cint dtctTime,
                               const LineInfo dtctRegion, const MeasureposPam measurePam)
{
    try {
        CHECKEMPIMG(dst, "Algorithm::edgeLineDtct input image is empty!");

        HObject  Contour;
        int interval;
        HTuple  phi, colRatio, rowRatio;
        HTuple  Width, Height, edgeCol, edgeRow, indx;
        HTuple  row, col, MeasureHandle, RowEdge, ColumnEdge;
        HTuple  Amplitude, Distance, Nr, Nc, Dist;
        HTuple  RowBegin, ColBegin, RowEnd, ColEnd;

        HTuple transition, pntSlct;

        int endCol = dtctRegion.endCol.D();
        int endRow = dtctRegion.endRow.D();
        int startCol = dtctRegion.startCol.D();
        int startRow = dtctRegion.startRow.D();

        if (direct == 1) {
            phi = 3.1415926 / 2;
            interval = (endCol - startCol) / (dtctTime - 1);
            colRatio = 1;
            rowRatio = 0;
        } else {
            phi = 0;
            interval = (endRow - startRow) / (dtctTime - 1);
            colRatio = 0;
            rowRatio = 1;
        }
        GetImageSize(dst, &Width, &Height);

        switch (measurePam.transition) {
        case ALLTRANS:
            transition = "all";
            break;
        case POSITIVE:
            transition = "positive";
            break;
        case NAGETIVE:
            transition = "negative";
            break;
        default:
            transition = "all";
        }

        switch (measurePam.pntSelect) {
        case ALL:
            pntSlct = "all";
            break;
        case FIRSTPNT:
            pntSlct = "first";
            break;
        case LASTPNT:
            pntSlct = "last";
            break;
        default:
            pntSlct = "first";
            break;
        }

        edgeCol = HTuple();
        edgeRow = HTuple();
        {
            HTuple end_val16 = dtctTime - 1;
            HTuple step_val16 = 1;
            for (indx = 0; indx.Continue(end_val16, step_val16); indx += step_val16) {
                row = startRow + ((interval * rowRatio) * indx);
                col = startCol + ((interval * colRatio) * indx);
                GenMeasureRectangle2(row, col, phi, measurePam.recLen1, measurePam.recLen2, Width, Height, "nearest_neighbor", &MeasureHandle);
                MeasurePos(dst, MeasureHandle, measurePam.sigma, measurePam.threshold, transition,
                           pntSlct, &RowEdge, &ColumnEdge, &Amplitude, &Distance);

                edgeRow = edgeRow.TupleConcat(RowEdge);
                edgeCol = edgeCol.TupleConcat(ColumnEdge);
            }
        }
        GenContourPolygonXld(&Contour, edgeRow, edgeCol);
        GenCrossContourXld(&showPoints, edgeRow, edgeCol, 10, 1);
        FitLineContourXld(Contour, "tukey", -1, 0, 5, 2, &RowBegin, &ColBegin, &RowEnd, &ColEnd, &Nr, &Nc, &Dist);
        CHECKEMPTUPLE(RowBegin, "Algorithm::edgeLineDtct not get the line");
        showLine = Contour;

        resLine.startRow = RowBegin[0];
        resLine.startCol = ColBegin[0];
        resLine.endRow   = RowEnd[0];
        resLine.endCol   = ColEnd[0];
        resLine.angle = (resLine.endRow.D() - resLine.startRow.D()) / (resLine.endCol.D() - resLine.startCol.D()) * 180.0 / 3.1415926;

        CloseMeasure(MeasureHandle);
        return 0;
    }  catch (...) {
        return CError(UNEXCEPTION, "edgeLineDtct algorithm crashed!");
    }
}

CError Algorithm::useGridGetRegion(CONSTIMG dst, RHOBJ resRegion, cint gridWid, cint gridHigt, cint minThre, cint eroValue, cint slctNum)
{
    try {
        if (slctNum <= 0)
            return CError(PAMVALUE, "Algorithm useGridGetRegion:: select num shold large than 0");

        CHECKEMPIMG(dst, "Algorithm::edgeLineDtct input image is empty!");

        HObject  RegionGrid, ImageReduced, Region;
        HObject  RegionFillUp, RegionErosion, ConnectedRegions1;
        HObject  ConnectedRegions, SelectedRegions;

        HTuple  Width, Height, Area, Row;
        HTuple  Column, Sorted;

        CHECKEMPIMG(dst, "Algorithm::useGridGetRegion input image is empty");
        GetImageSize(dst, &Width, &Height);

        GenGridRegion(&RegionGrid, gridWid, gridHigt, "lines", Width, Height);
        ReduceDomain(dst, RegionGrid, &ImageReduced);
        Threshold(ImageReduced, &Region, minThre, 255);

        FillUp(Region, &RegionFillUp);
        if (eroValue > 0)
            ErosionCircle(RegionFillUp, &RegionFillUp, eroValue);

        ErosionCircle(RegionFillUp, &RegionErosion, 1);
        Connection(RegionErosion, &ConnectedRegions);
        AreaCenter(ConnectedRegions, &Area, &Row, &Column);
        CHECKEMPTUPLE(Column, "Algorithm::useGridGetRegion dnot get the region after threshold");

        TupleSort(Area, &Sorted);
        SelectShape(ConnectedRegions, &SelectedRegions, "area", "and", HTuple(Sorted[(Area.TupleLength()) - slctNum]),
                    HTuple(Sorted[(Area.TupleLength()) - 1]) + 10);
        Union1(SelectedRegions, &resRegion);
        return 0;
    }  catch (...) {
        return CError(UNEXCEPTION, "Algorithm::useGridGetRegion crashed!");
    }
}

CError Algorithm::getRegionByQuadrant(const RHOBJ dstObj, RHOBJ region, cint quadrant)
{
    try {
        CHECKEMPIMG(dstObj, "Algorithm::getRegionByQuadrant dstObj is empty");

        HObject detect, finalRegion;
        Connection(dstObj, &detect);

        HTuple area, row, col, rowMean, colMean;

        AreaCenter(detect, &area, &row, &col);
        TupleMean(col, &colMean);
        TupleMean(row, &rowMean);

        int row1 = 0;
        int row2 = 99999;
        int col1 = 0;
        int col2 = 99999;
        int midRow = rowMean.D();
        int midCol = colMean.D();

        switch (quadrant) {
        case 0:
            row2 = midRow;
            col1 = midCol;
            break;
        case 1:
            row2 = midRow;
            col2 = midCol;
            break;
        case 2:
            row1 = midRow;
            col2 = midCol;
            break;
        case 3:
            row1 = midRow;
            col1 = midCol;
            break;
        default:
            break;
        }

        GenEmptyRegion(&finalRegion);
        SelectShape(detect, &finalRegion, (HTuple("row").Append("column")),
                    "and", (HTuple(row1).Append(col1)), (HTuple(row2).Append(col2)));

        CHECKEMPIMG(finalRegion, "Algorithm::getRegionByQuadrant slected region is empty");

        AreaCenter(finalRegion, &area, &row, &col);
        if (area.Length() != 1)
            return CError(REGIONNUM, QString("Algorithm::getRegionByQuadrant slected is not 1: %1").arg(area.Length()));

        region = finalRegion;
        return 0;
    } catch (...) {
        return CError(UNEXCEPTION, "Algorithm::getRegionByQuadrant crashed!");
    }
}

CError Algorithm::getRectByQuadrant(const QList<QRect> rects, QRect& rectInfo, cint quadrant)
{
    if (rects.size() != 4)
        return CError(PAMVALUE, "Algorithm::getRectByQuadrant rects size is not 4");

    float totalX = 0, totalY = 0;
    float meanX, meanY;

    for (auto rect : rects) {
        totalX += rect.x();
        totalY += rect.y();
    }

    meanX = totalX / 4.0;
    meanY = totalY / 4.0;

    int row1 = 0;
    int row2 = 99999;
    int col1 = 0;
    int col2 = 99999;
    int midRow = meanY;
    int midCol = meanX;

    switch (quadrant) {
    case 0:
        row2 = midRow;
        col1 = midCol;
        break;
    case 1:
        row2 = midRow;
        col2 = midCol;
        break;
    case 2:
        row1 = midRow;
        col2 = midCol;
        break;
    case 3:
        row1 = midRow;
        col1 = midCol;
        break;
    default:
        break;
    }
    for (auto rect : rects) {
        if (rect.y() < row2 && rect.y() > row1 && rect.x() < col2 && rect.x() > col1)
            rectInfo = rect;
    }
    return 0;
}

CError Algorithm::detectRegionExit(CONSTIMG dstObj, RHOBJ showObj, const QList<RectInfo>& inRect, QList<RectInfo>& gdRect, QList<RectInfo>& ngRect,
                                   const QPoint basePt, cint minThre, cint maxThre, const qreal percent, const QSize minSize)
{
    try {
        CHECKEMPIMG(dstObj, "Algorithm::detectRegionExit dstObj empty");

        int rectSize = inRect.size();
        if (rectSize == 0)
            return CError(PAMVALUE, "Algorithm::detectRegionExit inRect is empty");

        gdRect.clear();
        ngRect.clear();

        HTuple row, col, phi, len1, len2;
        HTuple row1, row2, col1, col2;
        HTuple area, cols, rows;
        HObject hRect, hReduceImg, hThreRg;

        GenEmptyObj(&showObj);
        double resRatio = 0;
        RectInfo info;
        for (auto i : inRect) {
            row = basePt.y() + i.YBia;
            col = basePt.x() + i.XBia;
            phi = i.phi;
            len1 = i.len1;
            len2 = i.len2;

            info = i;
            info.row = row.D();
            info.col = col.D();
            GenRectangle2(&hRect, row, col, phi, len1, len2);
            ReduceDomain(dstObj, hRect, &hReduceImg);

            Threshold(hReduceImg, &hThreRg, minThre, maxThre);
            Union2(showObj, hThreRg, &showObj);
            AreaCenter(hThreRg, &area, &rows, &cols);
            SmallestRectangle1(hThreRg, &row1, &col1, &row2, &col2);

            bool isOK = (minSize.width() < (col2.D() - col1.D())) &&
                        (minSize.height() < (row2.D() - row1.D()));

            if (objIsEmpty(hThreRg))
                resRatio = 0;
            else
                resRatio = area.D() / (len1.D() * len2.D() * 4);

            if ((resRatio >= percent) && isOK) {
                gdRect.push_back(info);
            } else {
                ngRect.push_back(info);
            }
        }
        return 0;
    }  catch (...) {
        qDebug() << "Algorithm::detectRegionExit crashed";
        return CError(UNEXCEPTION, "Algorithm::detectRegionExit crashed");
    }
}

CError Algorithm::getEdgePoint(CONSTIMG dst, RectInfo rectInfo, QList<QPointF>& pts, qreal& maxVa, const qreal sigma, const qreal maxThre)
{
    try {
        CHECKEMPIMG(dst, "Algorithm::detectRegionExit dstObj empty");

        HTuple hGrayList, hminY, hMaxY, hWidth, hHeight;
        HTuple hMeasureHandle, hFunction, hSmooth, hFirst, hAbsFirst, hSecond, hZeroCross, hYValue;
        HObject hRect;
        pts.clear();

        GetImageSize(dst, &hWidth, &hHeight);
        hGrayList = HTuple();
        GenMeasureRectangle2(rectInfo.row, rectInfo.col, rectInfo.phi, rectInfo.len1, rectInfo.len2,
                             hWidth, hHeight, "nearest_neighbor", &hMeasureHandle);
        MeasureProjection(dst, hMeasureHandle, &hGrayList);

        //获取函数
        CreateFunct1dArray(hGrayList, &hFunction);
        SmoothFunct1dGauss(hFunction, sigma, &hSmooth);
        DerivateFunct1d(hSmooth, "first", &hFirst);
        DerivateFunct1d(hSmooth, "second", &hSecond);

        //获取函数的最大值
        AbsFunct1d(hFirst, &hAbsFirst);
        YRangeFunct1d(hAbsFirst, &hminY, &hMaxY);
        maxVa = hMaxY.D();

        //获取二阶函数的0交点
        ZeroCrossingsFunct1d(hSecond, &hZeroCross);
        CHECKEMPTUPLE(hZeroCross, "Algorithm::getEdgePoint hZeroCross is empty");

        for (int i = 0; i < hZeroCross.Length(); i++) {
            GetYValueFunct1d(hFirst, HTuple(hZeroCross[i]), "constant", &hYValue);
            hYValue = hYValue.TupleAbs();
            if (hYValue >= maxThre)
                pts.push_back(QPointF(hZeroCross[i].D(), hYValue.D()));
        }
        return 0;
    }  catch (...) {
        qDebug() << "Algorithm::getEdgePoint crashed";
        return CError(UNEXCEPTION, "Algorithm::getEdgePoint crashed");
    }
}

CError Algorithm::genSpotArrayRegion(SDotArray& array)
{
    try {
        HObject circles;
        HObject singCircle;

        int startRowNum = array.startRowIndex;
        int rowNum = array.rowNum;
        int startColNum = array.startColIndex;
        int colNum = array.colNum;
        int baseRow = array.baseRow;
        int rowInter = array.rowInter;
        int baseCol = array.baseCol;
        int colInter = array.colInter;
        int radius = array.radius;

        QTime times;
        GenEmptyObj(&circles);
        for ( int i = startRowNum; i < rowNum + startRowNum; i++) {
            for (int j = startColNum; j < colNum + startColNum; j++) {
                qDebug() << "row col:" << baseRow + i* rowInter  << baseCol + j* colInter;
                GenCircle(&singCircle, baseRow + i * rowInter, baseCol + j * colInter, radius);
                Union2(circles, singCircle, &circles);
            }
        }
        array.arrayObj = circles;
        return 0;
    }  catch (...) {
        qDebug() << "algorithm:: gencircleregion crashed";
        return CError(UNEXCEPTION, "algorithm::gencircleregion crashed!");
    }
}

CError Algorithm::metrologyLineMeasure(const HObject& dst, const MetrologyPam dtctPam, LineInfo& line, HObject& hShow, HTuple rows, HTuple cols)
{
    try {
        CHECKEMPIMG(dst, "algorithm::metrologyLineMeasure dst is empty");

        HTuple tMetrologyHandle, tMetroLine, tWid, tHig;
        GetImageSize(dst, &tWid, &tHig);

        CreateMetrologyModel(&tMetrologyHandle);
        SetMetrologyModelImageSize(tMetrologyHandle, tWid, tHig);

        AddMetrologyObjectLineMeasure(tMetrologyHandle, line.startRow, line.startCol, line.endRow, line.endCol,
                                      dtctPam.measureLen1, dtctPam.measureLen2, dtctPam.sigma,
                                      dtctPam.thre, HTuple(), HTuple(), &tMetroLine);

        QString transition, ptSlct;

        switch (dtctPam.transition) {
        case 0:
            transition = "all";
            break;
        case 1:
            transition = "positive";
            break;
        case 2:
            transition = "negative";
            break;
        default:
            transition = "all";
            break;
        }

        switch (dtctPam.ptSlect) {
        case 0:
            ptSlct = "all";
            break;
        case 1:
            ptSlct = "first";
            break;
        case 2:
            ptSlct = "last";
            break;
        default:
            ptSlct = "all";
            break;
        }

        SetMetrologyObjectParam(tMetrologyHandle, tMetroLine, "num_instances", dtctPam.numInstance);
        SetMetrologyObjectParam(tMetrologyHandle, tMetroLine, "measure_transition", "positive");
        SetMetrologyObjectParam(tMetrologyHandle, tMetroLine, "min_score", dtctPam.sigma);
        SetMetrologyObjectParam(tMetrologyHandle, tMetroLine, "measure_select", "last");

        return 0;
    }  catch (...) {
        qDebug() << "Algorithm::metrologyLineMeasure crashed";
        return CError(UNEXCEPTION, "Algorithm::metrologyLineMeasure crashed");
    }
}
