#include "test.h"
#include "ui_test.h"
#include "halconcpp/HalconCpp.h"
#include "qfiledialog.h"
#include "qdebug.h"
#include "qmessagebox.h"
#include "commonUsage/algorithm.h"
#include "commonUsage/commonDef.h"
#include "halconGraphic/halconwidget.h"

#define SENDERR(a) \
    {QMessageBox::warning(this, "warnning", a);\
    return;}

test::test(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::test)
{
    ui->setupUi(this);
    widget = new Graphics::CImageViewHalcon();
    ui->lay_image->addWidget(widget);

    connect(widget->imageWindow(), &Graphics::halconWidget::drawLineDone, this, &test::getLineDone);
}

test::~test()
{
    delete ui;
}

void test::on_btn__clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, "select the filepath");
    widget->setFilePath(directory);
}

void test::on_pushButton_clicked()
{
    try {
        HObject img = widget->currentImage();
        L68Regions region;
        CSH68L::CCheckHanRegion hanRegion;
        QRect hanDtctRect;
        CSH68L::CGet68LRegion getregion;
        LineInfo info;
        getregion.detect(img, region, info);

        HObject hRect, hShow;
        HTuple row1, row2, col1, col2;
        HTuple area, row, col;
        SmallestRectangle1(region.blueRegion, &row1, &col1, &row2, &col2);

        int biacol = ui->spin_biaCol->value();
        int biarow = ui->spin_biaRow->value();
        int len1 = ui->spin_len1->value();
        int len2 = ui->spin_len2->value();

        hanRegion.setBiaValue(biacol, biarow, len1, len2);
        CError err = hanRegion.getHanCheckReg(region, hanDtctRect, row2.D(), col1.D());
        GenRectangle1(&hRect, hanDtctRect.top(), hanDtctRect.left(), hanDtctRect.bottom(), hanDtctRect.right());

        widget->imageWindow()->showObj(hRect);
    }  catch (...) {
        SENDERR("on_pushButton_clicked crashed!");
    }
}

void test::on_pushButton_2_clicked()
{
    try {
        HObject img = widget->currentImage();
        L68Regions region;
        CSH68L::CCheckHanRegion hanRegion;
        QRect hanDtctRect;
        CSH68L::CGet68LRegion getregion;
        LineInfo info;
        getregion.detect(img, region, info);

        HObject hRect, hShow;
        HTuple row1, row2, col1, col2;
        HTuple area, row, col;
        SmallestRectangle1(region.blueRegion, &row1, &col1, &row2, &col2);

        int biacol = ui->spin_biaCol->value();
        int biarow = ui->spin_biaRow->value();
        int len1 = ui->spin_len1->value();
        int len2 = ui->spin_len2->value();

        hanRegion.setBiaValue(biacol, biarow, len1, len2);
        CError err = hanRegion.getHanCheckReg(region, hanDtctRect, row2.D(), col1.D());
        GenRectangle1(&hRect, hanDtctRect.top(), hanDtctRect.left(), hanDtctRect.bottom(), hanDtctRect.right());

        HObject hReduce, hMean, hDynRegion, hConnect;
        HObject hSlct1, hSlct2, hSlct3;
        HTuple hNum;

        ReduceDomain(region.m_GImg, hRect, &hReduce);
        int hanSize = ui->spin_hansize->value();
        MeanImage(hReduce, &hMean, hanSize, hanSize);

        int dynVal = ui->spin_dynval->value();
        DynThreshold(hReduce, hMean, &hDynRegion, dynVal, "light");
        FillUp(hDynRegion, &hDynRegion);
        Connection(hDynRegion, &hConnect);

        int widMin = ui->spin_widthMin->value();
        int widMax = ui->spin_widthMax->value();
        int heighMin = ui->spin_heightMin->value();
        int heighMax = ui->spin_heightMax->value();
        int areaMin = ui->spin_areaMin->value();
        int areaMax = ui->spin_areaMax->value();
        float ratioMin = ui->spin_ratioMin->value();
        float ratioMax = ui->spin_ratioMax->value();
        float conMin = ui->spin_conMin->value();
        float conMax = ui->spin_conMax->value();
        float circleMin = ui->spin_circleMin->value();
        float circleMax = ui->spin_circleMax->value();

        SelectShape(hConnect, &hSlct1, (HTuple("width").Append("height")), "and",
                    (HTuple(widMin).Append(heighMin)), HTuple(widMax).Append(heighMax));

        SelectShape(hSlct1, &hSlct2, (HTuple("area").Append("ratio")), "and",
                    (HTuple(areaMin).Append(ratioMin)), (HTuple(areaMax).Append(ratioMax)));

        qDebug() << "convexity" << conMin << conMax;
        SelectShape(hSlct2, &hSlct3, "convexity", "and", conMin, conMax);

        qDebug() << "circularity" << circleMin << circleMax;
        SelectShape(hSlct3, &hSlct3, "circularity", "and", circleMin, circleMax);
        allObj = hSlct3;
        slctObj = hSlct3;

        widget->imageWindow()->showObj(hSlct3);
    }  catch (...) {
        qDebug() << "crashed";
    }
}

void test::getRectDone(int row, int col, int phi, int len1, int len2)
{
    qDebug() << "get rectangle done:" << row << col << phi << len1 << len2;

    RectInfo info;
    info.row = row;
    info.col = col;
    info.phi = phi;
    info.len1 = len1;
    info.len2 = len2;

    QList<QPointF> pts;
    qreal maxValue;
    Algorithm::getEdgePoint(widget->imageWindow()->getShowImage(), info, pts, maxValue, 10, 3);

    qDebug() << "maxvalue:" << maxValue << "pts size :" << pts.size();
}

void test::getLineDone(int row1, int col1, int row2, int col2)
{
    qDebug() << "enter lineDone";
    HObject image = widget->currentImage();

    HObject hR, hG, hB, hSubImg, showPts;
    Decompose3(image, &hR, &hG, &hB);
    SubImage(hB, hR, &hSubImg, 1, 0);
    widget->imageWindow()->setShowImage(hSubImg);

    HObject showLine;
    LineInfo resinfo, dtctInfo;
    dtctInfo.startCol = col1;
    dtctInfo.startRow = row1;
    dtctInfo.endCol = col2;
    dtctInfo.endRow = row2;

    MeasureposPam pam;
    pam.pntSelect = FIRSTPNT;
    pam.sigma = 2;
    pam.recLen1 = 0;
    pam.recLen2 = 20;
    pam.threshold = 10;
    pam.transition = POSITIVE;

    CError err = Algorithm::edgeLineDtct(hSubImg, showLine, showPts, resinfo, 1, 30, dtctInfo, pam);
    widget->imageWindow()->showObj(showPts);
}


void test::on_btn_getCircle_clicked()
{
    HObject show;
    qDebug() << "enter show";

    array.baseRow = ui->spin_baseRow->value();
    array.baseCol = ui->spin_baseCol->value();
    array.rowNum = ui->spin_rowNum->value();
    array.colNum = ui->spin_colNum->value();
    array.rowInter = ui->spin_rowInter->value();
    array.colInter = ui->spin_colInter->value();
    array.radius = ui->spin_radius->value();
    array.startRowIndex = ui->spin_startRowNum->value();
    array.startColIndex = ui->spin_startColNum->value();

    CError err = Algorithm::genSpotArrayRegion(array);
    qDebug() << err.msg();
    if (err.isWrong()) return;

    widget->imageWindow()->showObj(array.arrayObj);
}

void test::on_btn_maskTest_clicked()
{
    CSH68L::CCheckHanRegion region;
    CSH68L::CGet68LRegion datas;
    L68Regions l68Region;
    HObject dst, resobj;
    int num;
    LineInfo lines;
    QRect hanDetect;
    dst = widget->currentImage();
    datas.detect(dst, l68Region, lines);
    SDotArray array;

    int biacol = ui->spin_biaCol->value();
    int biarow = ui->spin_biaRow->value();
    int len1 = ui->spin_len1->value();
    int len2 = ui->spin_len2->value();

    int hanSize = ui->spin_hansize->value();
    int dynVal = ui->spin_dynval->value();

    QList<float> widList;
    widList.push_back(ui->spin_widthMin->value());
    widList.push_back(ui->spin_widthMax->value());
    widList.push_back(ui->spin_heightMin->value());
    widList.push_back(ui->spin_heightMax->value());
    widList.push_back(ui->spin_areaMin->value());
    widList.push_back(ui->spin_areaMax->value());
    widList.push_back(ui->spin_ratioMin->value());
    widList.push_back(ui->spin_ratioMax->value());
    widList.push_back(ui->spin_conMin->value());
    widList.push_back(ui->spin_conMax->value());
    widList.push_back(ui->spin_circleMin->value());
    widList.push_back(ui->spin_circleMax->value());

    array.baseRow = ui->spin_baseRow->value();
    array.baseCol = ui->spin_baseCol->value();
    array.rowNum = ui->spin_rowNum->value();
    array.colNum = ui->spin_colNum->value();
    array.rowInter = ui->spin_rowInter->value();
    array.colInter = ui->spin_colInter->value();
    array.radius = ui->spin_radius->value();
    array.startRowIndex = ui->spin_startRowNum->value();
    array.startColIndex = ui->spin_startColNum->value();

    region.setBiaValue(biacol, biarow, len1, len2);
    region.setdynPam(dynVal, hanSize);
    region.setGenDotArrayPam(array);
    region.setRowSlectPam(ui->spin_rowNumVal->value(), ui->spin_rowRange->value());
    region.setSlctPam(widList);

    CError err = region.detect(l68Region, hanDetect, resobj, num);
    qDebug() << "size :" << num;

    if (err.isWrong()) {
        qDebug() << "err:" << err.msg();
        return;
    }

    widget->imageWindow()->showObj(resobj);
}
