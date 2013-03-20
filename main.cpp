#include <QtGui/QGuiApplication>
#include <QQuickItem>
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

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/squeezeliteqml2/squeezeliteqml2main.qml"));
    viewer.showExpanded();
    QQuickItem *v = viewer.rootObject();

    AudioPlayer *p = new AudioPlayer(0);
    p->Init();
/*
 *  signal shuffle(int state)
    signal repeat(int state)
    signal nextTrack()
    signal play(int state)
    signal prevTrack()
    signal volUp()
    signal volDown()

 */
//    app.connect(v,SIGNAL(shuffle(int state)),p,SLOT(playState(int)));
//    app.connect(v,SIGNAL(repeat(int state)),p,SLOT(playState(int)));
    app.connect(v,SIGNAL(play(int)),p,SLOT(playState(int)));
    app.connect(v,SIGNAL(nextTrack()),p,SLOT(nextTrackClicked()));
    app.connect(v,SIGNAL(prevTrack()),p,SLOT(prevTrackClicked()));
    app.connect(v,SIGNAL(volUp()),p,SLOT(volUp()));
    app.connect(v,SIGNAL(volDown()),p,SLOT(volDown()));
    int retVal = app.exec();
    p->Close();
    return retVal;
}
