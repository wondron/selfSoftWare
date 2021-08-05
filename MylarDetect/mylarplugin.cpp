#include "mylarplugin.h"
#include "cmylarmatch.h"
#include "test.h"

MylarPlugin::MylarPlugin(QObject *parent):
    QObject(parent)
{

}

void MylarPlugin::echo(QObject *parent)
{
    Q_UNUSED(parent);
    qDebug()<<"enter MylarPlugin";

    test * widget = new test();
    widget->show();
}

#if(QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
Q_EXPORT_PLUGIN2(EchoPlugin, EchoPlugin);
#endif
