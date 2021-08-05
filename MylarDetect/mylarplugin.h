#ifndef MYLARPLUGIN_H
#define MYLARPLUGIN_H


#include <QObject>
#include "QtPlugin"
#include "EchoInterface.h"

class MylarPlugin : public QObject, public EchoInterface
{
    Q_OBJECT

#if(QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_PLUGIN_METADATA(IID EchoInterface_iid)
#endif

    Q_INTERFACES(EchoInterface)

public:
    explicit MylarPlugin(QObject *parent = 0);
    void echo(QObject *parent);
};

#endif // MYLARPLUGIN_H
