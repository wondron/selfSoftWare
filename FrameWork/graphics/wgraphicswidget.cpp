﻿#include "wgraphicswidget.h"
#include "QTimer"
#include "wgraphicsscene.h"
#include "QTime"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#include "qstring.h"
#endif
#endif
#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#else
#include <QtWidgets>
#endif
#include <QtMath>

#define ICONSIZE 30

namespace Graphics {

class WViewPrivate
{
public:
    WViewPrivate(WGraphicsWidget *qq): q(qq)
    {
        scene = nullptr;
        m_btnsObjName << "expand" << "zoomIn" << "zoomOut" << "centerOn"
                      << "originalSize" << "printer"  << "openGL"  << "antialiasing"
                      << "labelShow" << "aimSwitch";

        m_tipName   << u8"适应" << u8"放大" << u8"缩小" << u8"定位中心"
                    << u8"原始尺寸" << u8"打印" << u8"OpenGL开关" << u8"反锯齿开关"
                    << u8"辅助信息显示" << u8"瞄准线开关";

        flowBgColor = QColor(150, 150, 150);
        flowPressColor = QColor( 255, 0, 0);
    }
    ~WViewPrivate()
    {
        delete view;
    }

public:
    WGraphicsWidget *q;
    WGraphicsView *view;
    WGraphicsScene *scene;

    QString m_name;
    int m_fps;
    QString m_showText;
    QWidget *m_panel;

    QList<QString> m_btnsObjName, m_tipName;
    QList<QPushButton *> m_btnList;

    QColor flowBgColor;
    QColor flowPressColor;

    bool m_labelState = true;
    bool m_firstSetScene = true; //第一次设置scene
    bool m_panelVisibel = true;
    QLabel *m_scaleLabel;
    QLabel *m_aimLabel;
    QLabel *m_nameLabel;
    QLabel *m_fpsLabel;
};

WGraphicsWidget::WGraphicsWidget(const QString &name, QWidget *parent)
    : QFrame(parent), d(new WViewPrivate(this))
{
    setObjectName("graphicsWisget");
    setFrameStyle(Sunken | StyledPanel);
    d->scene = new WGraphicsScene();
    d->view = new WGraphicsView(this);
    d->view->setObjectName("WgraphicsView");
    setCurrentScene(d->scene);
    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    d->m_name = name;
    QSize iconSize(size, size);
    //setMinimumSize(400, 300);
    iniPanel();
    initAimWidget();
    initScaleWidget();
    initFpsLabel();

    connect(d->view, &WGraphicsView::scaleChanged, [this](qreal scale) {
        QString str = QString::number(scale * 100, 'f', 1);
        d->m_scaleLabel->setText(str + "%");
    });
}

WGraphicsView *WGraphicsWidget::view() const
{
    return d->view;
}

QString WGraphicsWidget::widgetName() const
{
    return d->m_name;
}

void WGraphicsWidget::setCurrentScene(WGraphicsScene *scene)
{
    if (!d->m_firstSetScene) {
        disconnect(d->scene, &WGraphicsScene::imageSizeChange, d->view, &WGraphicsView::fitScene);
        disconnect(d->scene, &WGraphicsScene::fpsChanged, this, &WGraphicsWidget::fpsChange);
    }

    d->m_firstSetScene = false;
    d->view->setScene(scene);
    d->scene = scene;
    connect(scene, &WGraphicsScene::imageSizeChange, d->view, &WGraphicsView::fitScene);
    connect(scene, &WGraphicsScene::fpsChanged, this, &WGraphicsWidget::fpsChange);
}

WGraphicsScene *WGraphicsWidget::currentScene() const
{
    return d->scene;
}

void WGraphicsWidget::updatePos()
{
    if (!view()) return;
    d->m_panel->setGeometry(5, 5, this->width() - ICONSIZE / 2, ICONSIZE * 1.4);

    view()->setFixedSize(width(), height());
    QRect rect = this->rect();
    d->m_aimLabel->setGeometry(rect.center().x() - d->m_aimLabel->width() / 2,
                               rect.center().y() - d->m_aimLabel->height() / 2,
                               d->m_aimLabel->width(),
                               d->m_aimLabel->height());

    d->m_scaleLabel->setGeometry(rect.width() - d->m_scaleLabel->width() - 5,
                                 rect.height() - d->m_scaleLabel->height(),
                                 d->m_scaleLabel->width(), d->m_scaleLabel->height());

    d->m_fpsLabel->setGeometry(5, 5, d->m_fpsLabel->width(), d->m_fpsLabel->height());
}

void WGraphicsWidget::setLabelShow(bool ennabel)
{
    d->m_labelState = ennabel;
    d->m_scaleLabel->setVisible(ennabel);
    d->m_fpsLabel->setVisible(ennabel);
    d->m_aimLabel->setVisible(ennabel);
    d->m_btnList[LABELSHOW]->setChecked(ennabel);
}

void WGraphicsWidget::setAimShow(bool ennabel)
{
    if (!d->scene) return;
    d->scene->setCenterImageAimVisible(ennabel);
    d->m_btnList[AIMSWITCH]->setChecked(ennabel);
}

void WGraphicsWidget::setPanelShow(const bool enabel)
{
    d->m_panelVisibel = enabel;
    d->m_panel->setVisible(enabel);
}

void WGraphicsWidget::setImage(const QImage &img)
{
    d->scene->setCenterImage(img);
}

QImage WGraphicsWidget::getImage() const
{
    return d->scene->getImage();
}

void WGraphicsWidget::setWidgetName(QString name)
{
    d->m_name = name;
}

void WGraphicsWidget::setImagePath(const QString &filePath)
{

    qDebug() << "setImage:" << filePath;
    QImage img(filePath);

    if(img.isNull()) return;
    setImage(img);
}

void WGraphicsWidget::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    if(!d->m_panelVisibel) return;
    d->m_panel->setVisible(true);
}

void WGraphicsWidget::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    if(!d->m_panelVisibel) return;
    d->m_panel->setVisible(false);
}

void WGraphicsWidget::resizeEvent(QResizeEvent *)
{
    updatePos();
}

void WGraphicsWidget::btnClicked()
{
    if (!view()) return;

    QPushButton *btn = (QPushButton *)sender();
    QString objName = btn->objectName();

    int clickNum;
    for (int i = 0 ; i < d->m_btnsObjName.size() ; i++) {
        if (objName == d->m_btnsObjName[i])
            clickNum = i;
    }

    switch (clickNum) {
    case FITBTN :
        d->view->fitScene();
        break;
    case ZOOMIN :
        d->view->zoomIn();
        break;
    case ZOOMOUT :
        d->view->zoomOut();
        break;
    case CENTERON :
        d->view->movetoCenter();
        break;
    case ORIGIN :
        d->view->zoom(1);
        break;
    case ANTIALIASE:
        d->view->setAntialiase(btn->isChecked());
        break;
    case LABELSHOW:
        setLabelShow(btn->isChecked());
        break;
    case AIMSWITCH:
        setAimShow(btn->isChecked());
        break;
    }
}

void WGraphicsWidget::fpsChange(int num)
{
    QString str = QString("fps: %2").arg(num);
    d->m_fps = num;
    d->m_fpsLabel->setFixedSize(str.size() * 10, 25);
    d->m_fpsLabel->setText(str);
}

void WGraphicsWidget::iniPanel()
{
    qDebug() << "enter inipanel";
    d->m_panel = new QWidget(this);
    d->m_panel->setObjectName("flowPanel");

    //用布局顶住,左侧弹簧
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(2);
    layout->setMargin(0);
    layout->addStretch();
    d->m_panel->setLayout(layout);
    d->m_panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QStringList strList = qApp->applicationDirPath().split('/');
    QString filepath;

    for(auto i = strList.begin(); i != strList.end() - 1; i++){
        filepath += *i +"/";
    }
    filepath += "FrameWork/graphics/image/";

    //循环添加顶部按钮
    for (int i = 0; i < d->m_btnsObjName.count(); i++) {
        QPushButton *btn = new QPushButton;
        btn->setIconSize(QSize(ICONSIZE, ICONSIZE));

        QString str = filepath + d->m_btnsObjName[i] + ".png";

        qDebug() << "icon path:" << str;
        QPixmap image = QPixmap(str);
        qDebug() << "pixmap is null: " << image.isNull();

        btn->setIcon(QIcon(QPixmap(str)));
        btn->setToolTip(d->m_tipName[i]);

        //设置标识,用来区别按钮
        btn->setObjectName(d->m_btnsObjName.at(i));
        if (d->m_btnsObjName[i] == "openGL" || d->m_btnsObjName[i] == "antialiasing")
            btn->setCheckable(true);
        if (d->m_btnsObjName[i] == "labelShow" || d->m_btnsObjName[i] == "aimSwitch") {
            btn->setCheckable(true);
            btn->setChecked(true);
        }

        //设置固定宽度
        btn->setFixedWidth(ICONSIZE + 8);
        btn->setFixedHeight(ICONSIZE + 8);

        //设置拉伸策略使得填充
        btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

        //设置焦点策略为无焦点,避免单击后焦点跑到按钮上
        btn->setFocusPolicy(Qt::NoFocus);

        //将按钮加到布局中
        layout->addWidget(btn);
        d->m_btnList.push_back(btn);

        //绑定按钮单击事件,用来发出信号通知
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(btnClicked()));
    }
    initPanelStyle();
}

void WGraphicsWidget::initPanelStyle()
{
    //设置样式以便区分,可以自行更改样式,也可以不用样式
    QStringList qss;
    QString rgba = QString("rgba(%1, %2, %3, 170)").arg(d->flowBgColor.red()).arg(d->flowBgColor.green()).arg(d->flowBgColor.blue());
    //qss.append(QString("#flowPanel{background:%1;}").arg(rgba));
    qss.append(QString("QPushButton{color:rgba(0,0,0,0);}"));
    qss.append(QString("QPushButton:pressed{color:rgb(255,0,0);}"));
    //qss.append(QString("QPushButton{border:none;padding:0px;background:rgba(0,0,0,0);}"));
    //qss.append(QString("QPushButton:pressed{color:%1;}").arg(flowPressColor.name()));
    d->m_panel->setStyleSheet(qss.join(""));
}

void WGraphicsWidget::initScaleWidget()
{
    d->m_scaleLabel = new QLabel("100%", this);
    d->m_scaleLabel->setObjectName("scaleLabel");
    QFont font("Arial");
    font.setBold(true);
    font.setPixelSize(12);
    d->m_scaleLabel->setFixedSize(50, 25);
    d->m_scaleLabel->setFont(font);
    d->m_scaleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    d->m_scaleLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    d->m_scaleLabel->setStyleSheet("QLabel{background-color:rgb(100,100,100);color:rgb(0,200,0);border: 2px solid rgb(100,200,100); border-radius: 5px;}");
}

void WGraphicsWidget::initAimWidget()
{
    d->m_aimLabel = new QLabel(this);
    d->m_aimLabel->setObjectName("aimLabel");
    d->m_aimLabel->installEventFilter(this);
    d->m_aimLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    QImage img(":/icon/image/aims.png");
    d->m_aimLabel->setFixedSize(img.width(), img.height());
    d->m_aimLabel->setPixmap(QPixmap::fromImage(img));
}

/*
void WGraphicsWidget::initNameLabel()
{
    d->m_nameLabel = new QLabel(this);
    QFont font("Arial");
    font.setBold(true);
    font.setPixelSize(13);
    d->m_nameLabel->setFixedSize(d->m_name.size() * 8, 25);
    d->m_nameLabel->setFont(font);
    d->m_nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    d->m_nameLabel->setStyleSheet("color:rgb(0,200,0);");
}*/

void WGraphicsWidget::initFpsLabel()
{
    d->m_fpsLabel = new QLabel(this);
    d->m_fpsLabel->setObjectName("fpsLabel");
    QFont font("Arial");
    font.setBold(true);
    font.setPixelSize(13);
    d->m_fpsLabel->setFixedSize(d->m_name.size() * 10, 25);
    d->m_fpsLabel->setFont(font);
    d->m_fpsLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    d->m_fpsLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    d->m_fpsLabel->setStyleSheet("color:rgb(0,200,0);");
}
}
