#include "wondrondlg.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    wondronDlg w;
    w.show();
    return a.exec();
}
