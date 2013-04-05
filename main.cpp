#include <QtGui/QGuiApplication>
#include <QQuickView>
#include <QQuickItem>
#include <QQmlContext>
#include <QImage>
#include "qtquick2applicationviewer.h"

#include "squeezedefines.h"
#include "audioplayer.h"

#ifdef SQUEEZEMAINWINDOW_DEBUG
#define DEBUGF(...) qDebug() << this->objectName() << Q_FUNC_INFO << __VA_ARGS__;
#else
#define DEBUGF(...)
#endif


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView view;

    AudioPlayer *p = new AudioPlayer(0);
    p->Init(&view);
    int retVal = app.exec();
    p->Close();
    return retVal;
}
