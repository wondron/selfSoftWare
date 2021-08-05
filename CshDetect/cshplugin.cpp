#include "cshplugin.h"
#include "test.h"

CshPlugin::CshPlugin(QObject *parent):
    QObject(parent)
{

}

void CshPlugin::echo(QObject *parent)
{
    Q_UNUSED(parent);
    qDebug()<<"enter AiPlugin";

    //test * widget = new test();
    //widget->show();
}

#if(QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
Q_EXPORT_PLUGIN2(EchoPlugin, EchoPlugin);
#endif
