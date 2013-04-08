#ifndef PLAYERINTERFACE_H
#define PLAYERINTERFACE_H

#include <QObject>
#include <QQuickView>

class playerInterface : public QQuickView
{
    Q_OBJECT
public:
    explicit playerInterface(QWindow *parent = 0);
    void Init(void);
    void loadHomeScreen(void);
    void loadMusicScreen(void);
    void loadNowPlayingScreen(void);

signals:
    
public slots:
    
};

#endif // PLAYERINTERFACE_H
