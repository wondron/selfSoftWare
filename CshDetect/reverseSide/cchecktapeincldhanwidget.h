#ifndef CCHECKTAPEINCLDHANWIDGET_H
#define CCHECKTAPEINCLDHANWIDGET_H

#include <QWidget>
#include "../commonUsage/cerror.h"
#include "../commonUsage/comStruct.h"
#include "cchecktapeincldhan.h"

namespace Ui {
class CCheckTapeIncldHanWidget;
}

namespace CSHDetect {

class CCheckTapeIncldHanWidgetPrivate;

class CCheckTapeIncldHanWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CCheckTapeIncldHanWidget(QWidget *parent = nullptr);
    ~CCheckTapeIncldHanWidget();

    void setRegion(const ReverRegionS& region);
    void setRects(const QList<QRect>& rects);

    CError detect();

public slots:

private slots:
    void on_btn_pamRead_clicked();
    void on_btn_changePos_clicked();

    void on_btn_detect_clicked();

private:
    Ui::CCheckTapeIncldHanWidget *ui;
    CCheckTapeIncldHanWidgetPrivate *d;
};

}

#endif // CCHECKTAPEINCLDHANWIDGET_H
