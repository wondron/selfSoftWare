#include "test.h"
#include "ui_test.h"
#include "halconcpp/HalconCpp.h"
#include "qfiledialog.h"
#include "qdebug.h"
#include "qmessagebox.h"
#include "commonUsage/algorithm.h"
#include "commonUsage/commonDef.h"
#include "halconGraphic/halconwidget.h"
#include "cmylarmatch.h"

#define SENDERR(a) \
    {QMessageBox::warning(this, "warnning", a);\
    return;}

test::test(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::test)
{
    ui->setupUi(this);
    widget = new Graphics::CImageViewHalcon();
    widget->imageWindow()->setColor("green");
    ui->lay_image->addWidget(widget);\
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
        MylarDetect::CMylarMatch match;
        MylarDetect::SMylarMatchData data;
        match.setGlueNum(2);
        match.setMylarNum(0);
        match.setModelFilePath("D:/work/mylar");

        HObject hShow;
        HObject image = widget->currentImage();
        match.detect(image, data);
        widget->imageWindow()->showObj(data.glueRegionList[0]);

        qDebug() << "data: " << data.row << data.col << data.angle;
    }  catch (...) {
        qDebug() << "on_pushButton_clicked crashed!";
    }
}

void test::on_pushButton_2_clicked()
{
    try {

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
}

void test::on_btn_maskTest_clicked()
{

}
