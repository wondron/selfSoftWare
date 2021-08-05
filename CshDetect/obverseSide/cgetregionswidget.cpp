#include "cgetregionswidget.h"
#include "ui_cgetregionswidget.h"
#include "halconGraphic/halconwidget.h"
#include "qfiledialog.h"
#include "obverseSide/cgetregions.h"
#include "qdebug.h"
#include "commonUsage/algorithm.h"
#include "qradiobutton.h"
#include "qmessagebox.h"
#include "commonUsage/commonDef.h"

#define CHECKRES(err) \
    if(err.isWrong()){\
        emit sendErr(err.msg());\
        return;\
}

using namespace Graphics;
using namespace CSHDetect;

class CGetRegionsWidgetPrivate
{
public:
    enum {DETECTSIZE = 7};
    CGetRegionsWidgetPrivate(CGetRegionsWidget *parent): p(parent){
        pam = new CSHDetect::CGetRegions();

        for(int i = 0; i < DETECTSIZE; i++){
            halconWidget* widget = new halconWidget();
            widgetList.push_back(widget);
        }
    }

    HObject img;
    HTuple imgWid;
    HTuple imgHit;
    CGetRegions* pam;
    RegionS regions;

    QList<QRadioButton *> radioBtnList;
    QList<QLayout*> layoutList;
    QList<halconWidget*> widgetList;
    QHash<int, HObject> channelImgs;
    QList<QGroupBox *> pamGroupList;

private:
    CGetRegionsWidget *p;
};

CGetRegionsWidget::CGetRegionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CGetRegionsWidget),
    d(new CGetRegionsWidgetPrivate(this))
{
    ui->setupUi(this);
    iniLayout();

    connect(this, SIGNAL(sendErr(QString)), this, SLOT(showErrMsg(const QString&)));
}

CGetRegionsWidget::~CGetRegionsWidget()
{
    delete ui;
}

void CGetRegionsWidget::setPam(CGetRegions *pam)
{
    d->pam = pam;
}

void CGetRegionsWidget::setImage(HObject &img, RegionS &regions)
{
    try {
        CHECKRES(Algorithm::objIsEmpty(img));
        d->img = img;
        GetImageSize(d->img, &d->imgWid, &d->imgHit);

        d->regions = regions;

        for(auto i : d->widgetList)
            i->setShowImage(d->img);
    }  catch (...) {
        emit sendErr("setImage func happen unexcepted!");
    }
}

const RegionS& CGetRegionsWidget::getRegons() const
{
    return d->regions;
}

void CGetRegionsWidget::setDetectImage(const QString &temPath)
{
    if (temPath.isEmpty()) return;

    try {
        ReadImage(&d->img, temPath.toStdString().c_str());
        CHECKRES(Algorithm::objIsEmpty(d->img));

        GetImageSize(d->img, &d->imgWid, &d->imgHit);
        err = d->pam->transformImg(d->img, d->regions);
        CHECKRES(err);

        for(auto i : d->widgetList)
            i->setShowImage(d->img);

    }  catch (...) {
        emit sendErr("getclicked func happen unexcepted!");
    }
}

void CGetRegionsWidget::iniLayout()
{
    d->radioBtnList.clear();
    d->radioBtnList.push_back(ui->radiobtn_Ori);
    d->radioBtnList.push_back(ui->radiobtn_R);
    d->radioBtnList.push_back(ui->radiobtn_G);
    d->radioBtnList.push_back(ui->radiobtn_B);
    d->radioBtnList.push_back(ui->radiobtn_H);
    d->radioBtnList.push_back(ui->radiobtn_S);
    d->radioBtnList.push_back(ui->radiobtn_V);

    d->layoutList.push_back(ui->layout_blackBang);
    d->layoutList.push_back(ui->layout_Dark);
    d->layoutList.push_back(ui->layout_White);
    d->layoutList.push_back(ui->layout_tape);
    d->layoutList.push_back(ui->layout_blueRegion);
    d->layoutList.push_back(ui->layout_midRect);
    d->layoutList.push_back(ui->layout_blueTape);

    d->pamGroupList.push_back(ui->grpbx_blackBang);
    d->pamGroupList.push_back(ui->grpbx_dark);
    d->pamGroupList.push_back(ui->grpbx_white);
    d->pamGroupList.push_back(ui->grpbx_tape);
    d->pamGroupList.push_back(ui->grpbx_blueRegion); //蓝色胶带预留位
    d->pamGroupList.push_back(ui->grpbx_midRect);
    d->pamGroupList.push_back(ui->grpbx_blueTape);


    for(int i = 0; i < CGetRegionsWidgetPrivate::DETECTSIZE; i++){
        d->layoutList[i]->addWidget(d->widgetList[i]);
    }

    for(auto i : d->pamGroupList)
        i->setVisible(false);

    d->pamGroupList[0]->setVisible(true);
}

int CGetRegionsWidget::getShowImgType()
{
    for(int i = 0; i < d->radioBtnList.size(); i++){
        if(d->radioBtnList[i]->isChecked())
            return i;
    }
    return 0;
}

void CGetRegionsWidget::on_btn_getImage_clicked()
{
    QString temPath = QFileDialog::getOpenFileName(this, "open file");
    if (temPath.isEmpty()) return;

    try {
        ReadImage(&d->img, temPath.toStdString().c_str());
        GetImageSize(d->img, &d->imgWid, &d->imgHit);
        CHECKRES(Algorithm::objIsEmpty(d->img));

        err = d->pam->transformImg(d->img, d->regions);
        CHECKRES(err);

        for(auto i : d->widgetList)
            i->setShowImage(d->img);

    }  catch (...) {
        emit sendErr("getclicked func happen unexcepted!");
    }

}

void CGetRegionsWidget::on_btn_test_clicked()
{
    err = Algorithm::objIsEmpty(d->img);

    CGetRegions result;
    err = result.detect(d->img, d->regions);

    if(err.isWrong()){
        emit sendErr(err.msg());
        return;
    }

    emit detectDone();
    d->widgetList[0]->showObj(d->regions.BlackBangRegion);
    d->widgetList[1]->showObj(d->regions.DarkRegion);
    d->widgetList[2]->showObj(d->regions.WhiteRegion);
    d->widgetList[3]->showObj(d->regions.TapeRegion);
    d->widgetList[4]->showObj(d->regions.BlueRegion);
    d->widgetList[5]->showObj(d->regions.MidRect);
    d->widgetList[6]->showObj(d->regions.MidBlueTape);
}

void CGetRegionsWidget::showErrMsg(const QString &msg)
{
    QMessageBox::warning(this, "error", msg);
}

void CGetRegionsWidget::on_btn_showImg_clicked()
{
    int index =getShowImgType();
    int widgetIndex = ui->tabWidget->currentIndex();

    d->widgetList[widgetIndex]->setShowImage(d->channelImgs.value(index));
}

void CGetRegionsWidget::on_tabWidget_currentChanged(int index)
{
    for(int i =0; i < CGetRegionsWidgetPrivate::DETECTSIZE; i++){
        if(i == index)
            d->widgetList[i]->frashWindow();
        d->pamGroupList[i]->setVisible((i == index));
    }
}

void CGetRegionsWidget::on_btn_savePam_clicked()
{

}

void CGetRegionsWidget::on_btn_blackBang_clicked()
{
    err = d->pam->getBlackBangRoi(d->regions, ui->spin_blkBangMaxThre->value());
    CHECKRES(err);

    int i = ui->tabWidget->currentIndex();
    d->widgetList[i]->showObj(d->regions.BlackBangRegion);
}

void CGetRegionsWidget::on_btn_dark_clicked()
{
    err = d->pam->getDarkRegion(d->regions, ui->spin_darkMaxR->value(), ui->spin_darkMaxG->value(), ui->spin_darkMaxB->value());
    CHECKRES(err);
    int i = ui->tabWidget->currentIndex();
    d->widgetList[i]->showObj(d->regions.DarkRegion);
}

void CGetRegionsWidget::on_btn_white_clicked()
{
    err = d->pam->getWhiteRegion(d->regions, ui->spin_whiteMinR->value(), ui->spin_whiteMinG->value(), ui->spin_whiteMinB->value(), 1500);
    CHECKRES(err);
    int i = ui->tabWidget->currentIndex();
    d->widgetList[i]->showObj(d->regions.WhiteRegion);
}

void CGetRegionsWidget::on_btn_Tape_clicked()
{
    err = d->pam->getTapeRegion(d->regions, ui->dspin_guassSigma->value(), ui->dspin_guassFactor->value(), ui->spin_threMax->value(), ui->spin_holeSize->value());
    CHECKRES(err);
    int i = ui->tabWidget->currentIndex();
    d->widgetList[i]->showObj(d->regions.TapeRegion);
}

void CGetRegionsWidget::on_btn_blueRegion_clicked()
{
    err = d->pam->getBlueRegion(d->regions, ui->spin_blueSubVal->value());
    CHECKRES(err);
    int i = ui->tabWidget->currentIndex();
    d->widgetList[i]->showObj(d->regions.BlueRegion);
}

void CGetRegionsWidget::on_btn_midRect_clicked()
{
    err = d->pam->getMidRect(d->regions, ui->spin_direct->value(), ui->spin_rectWid->value(), ui->spin_eroWidth->value(), ui->spin_slctNum->value());
    CHECKRES(err);
    int i = ui->tabWidget->currentIndex();
    d->widgetList[i]->showObj(d->regions.MidRect);
}

void CGetRegionsWidget::on_btn_blueTape_clicked()
{
    int size;
    err = d->pam->getBlueTape(d->regions, size, ui->spin_blueTapeWid->value(), ui->spin_ringWid->value());

    CHECKRES(err);
    ui->spin_bluetapeSize->setValue(size);
    int i = ui->tabWidget->currentIndex();
    d->widgetList[i]->showObj(d->regions.MidBlueTape);
}
