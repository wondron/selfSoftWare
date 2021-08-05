#ifndef WONDRONDLG_H
#define WONDRONDLG_H

#include <QMainWindow>
#include "EchoInterface.h"
#include "commonUsage/cerror.h"

QT_BEGIN_NAMESPACE
namespace Ui { class wondronDlg; }
QT_END_NAMESPACE

class wondronDlg : public QMainWindow
{
    Q_OBJECT

public:
    wondronDlg(QWidget *parent = nullptr);
    ~wondronDlg();

private slots:
    //void sendEcho();

private:
    CError loadPlugin();
    EchoInterface *echoInterface;

private:
    Ui::wondronDlg *ui;
};
#endif // WONDRONDLG_H
