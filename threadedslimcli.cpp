#include "threadedslimcli.h"

threadedslimCli::threadedslimCli(QObject *parent, const char *name, QString serverAdd, QByteArray mac, quint16 cliport) :
    QThread(parent)
{
}
