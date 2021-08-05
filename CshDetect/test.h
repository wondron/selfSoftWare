#ifndef TEST_H
#define TEST_H

#include <QWidget>
#include "halconGraphic/halconwidget.h"
#include "commonWidget/cimageviewhalcon.h"
#include "L68Side/csingledtct.h"
#include "L68Side/cget68lregion.h"
#include "L68Side/ccheckcuexit.h"
#include "L68Side/cchecktapeangle.h"
#include "L68Side/ccheckhanregion.h"

namespace Ui {
class test;
}

class test : public QWidget
{
    Q_OBJECT

public:
    explicit test(QWidget *parent = nullptr);
    ~test();

private slots:
    void on_btn__clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void getRectDone(int row, int col, int phi, int len1, int len2);
    void getLineDone(int row1, int col1, int row2, int col2);

    void on_btn_getCircle_clicked();

    void on_btn_maskTest_clicked();

private:
    Ui::test *ui;
    Graphics::CImageViewHalcon* widget;

    SDotArray array;
    HObject allObj;
    HObject slctObj;
};

#endif // TEST_H
