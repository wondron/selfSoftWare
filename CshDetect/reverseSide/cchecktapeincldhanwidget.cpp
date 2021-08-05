#include "cchecktapeincldhanwidget.h"
#include "ui_cchecktapeincldhanwidget.h"

#include "halconGraphic/halconwidget.h"
#include "qmessagebox.h"
#include "commonUsage/algorithm.h"
#include "commonUsage/commonDef.h"
#include "qdebug.h"

#define SENDERR(a) \
    {QMessageBox::warning(this, "warnning", a);\
    return 1;}

namespace CSHDetect {

class CCheckTapeIncldHanWidgetPrivate
{
public:

    CCheckTapeIncldHanWidgetPrivate()
    {
        m_widget = new Graphics::halconWidget();
        m_pam = new CCheckTapeIncldHan();
    }

    QList<QRect> rects;
    ReverRegionS m_region;
    Graphics::halconWidget* m_widget;
    HObject m_img;
    CCheckTapeIncldHan* m_pam;

    bool isSetRegion = false;
};

CCheckTapeIncldHanWidget::CCheckTapeIncldHanWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::CCheckTapeIncldHanWidget),
    d(new CCheckTapeIncldHanWidgetPrivate())
{
    ui->setupUi(this);

    ui->vlay_widget->addWidget(d->m_widget);
}

CCheckTapeIncldHanWidget::~CCheckTapeIncldHanWidget()
{
    delete ui;
}

void CCheckTapeIncldHanWidget::setRegion(const ReverRegionS& region)
{
    CError err = Algorithm::objIsEmpty(region.m_oriImg);
    if (err.isWrong()) return;

    d->m_region = region;
    d->isSetRegion = true;
    d->m_widget->setShowImage(d->m_region.m_oriImg);
}

void CCheckTapeIncldHanWidget::setRects(const QList<QRect>& rects)
{
    d->rects = rects;
    if (!rects.size()) return;

    try {
        HObject hRect, hUnion;

        GenEmptyObj(&hUnion);

        for (QRect rect : rects) {
            GenRectangle1(&hRect, rect.top(), rect.left(), rect.bottom(), rect.right());
            Union2(hUnion, hRect, &hUnion);
        }
        d->m_widget->showObj(hUnion);
    }  catch (...) {
        qDebug() << "setRects crashed";
    }
}

CError CCheckTapeIncldHanWidget::detect()
{
    if (!d->isSetRegion)
        SENDERR("not set regions");

    QList<QRect> ngRect, okRect;
    d->m_pam->pamRead("D:\\work\\vsDetect\\detection\\bin\\conf\\algorithm\\cshdefault\\detection-old.xml");
    qDebug() << "pamread done";
    CError err = d->m_pam->detect(d->m_region.m_oriImg, d->rects, okRect, ngRect);
    qDebug() << "err code" << err.code() << err.msg() << okRect.size() << ngRect.size();

    if(err.isWrong())
        SENDERR(QString("Err: %1").arg(err.msg()));
    for(auto i : okRect)
        qDebug() << "rect::" << i.topLeft() << i.bottomRight();

    HObject hRect, hUnion;
    GenEmptyObj(&hUnion);
    for (int i = 0 ; i < 4; i++) {
    //GenRectangle1(&hRect, okRect[0].top(), okRect[0].left(), okRect[0].bottom(), okRect[0].right());
        GenRectangle1(&hRect, okRect[i].top(), okRect[i].left(), okRect[i].bottom(), okRect[i].right());
        Union2(hUnion, hRect, &hUnion);
    }
    d->m_widget->showObj(hUnion);
    return 0;
}

void CCheckTapeIncldHanWidget::on_btn_pamRead_clicked()
{
    d->m_pam->pamRead("D:\\work\\vsDetect\\detection\\bin\\conf\\algorithm\\cshdefault\\detection.xml");
}


void CCheckTapeIncldHanWidget::on_btn_changePos_clicked()
{
    if (!d->rects.size()) return;

    try {
        HObject hRect, hUnion;

        GenEmptyObj(&hUnion);

        QList<int> bias;
        QList<QRect> rects = d->rects;
        bias.push_back(ui->spin_YBia1->value());
        bias.push_back(ui->spin_YBia2->value());
        bias.push_back(ui->spin_YBia3->value());
        bias.push_back(ui->spin_YBia4->value());
        int bia = 0;

        for (int i = 0 ; i < 4; i++) {
            bia = bias[i];
            GenRectangle1(&hRect, rects[i].top() + bia, rects[i].left(), rects[i].bottom() + bia, rects[i].right());
            Union2(hUnion, hRect, &hUnion);
        }
        d->m_widget->showObj(hUnion);
    }  catch (...) {
        qDebug() << "setRects crashed";
    }
}


void CCheckTapeIncldHanWidget::on_btn_detect_clicked()
{
    detect();
}

}

