#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QByteArray>
#include <QQuickView>
#include <QModelIndex>

#include "squeezedefines.h"
#include "slimcli.h"
#include "devicestatus.h"
#include "listmodel.h"
#include "controllistitem.h"

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    explicit AudioPlayer(QObject *parent = 0);
    ~AudioPlayer();

    void Init(QQuickView *v);
    void Close(void);
    
signals:
    
public slots:
    /*
     *  signal shuffle(int state)
        signal repeat(int state)
        signal nextTrack()
        signal play(int state)
        signal prevTrack()
        signal volUp()
        signal volDown()

     */
    void cliConnected(void);

    void shuffleState(int state);
    void nextTrackClicked(void);
    void prevTrackClicked(void);
    void playState(int state);
    void volUp(void);
    void volDown(void);
    void controlViewClicked(int idx);
    void controlViewClicked(QString s);


private:
    void initInterface(void);
    void getplayerMACAddress(void);
    void loadHomeScreen(void);
    void loadMusicScreen(void);
    void loadNowPlayingScreen(void);


private:
    QString lmsUsername;
    QString lmsPassword;
    QString SqueezeBoxServerAddress;
    QString SqueezeBoxServerCLIPort;
    QString SqueezeBoxServerHttpPort;
    QString SqueezeBoxServerAudioPort;
    QString AudioDevice;
    QString PlayerName;

    QByteArray MacAddress;      // MAC address of this machine (which will become the MAC address for our player)
    QByteArray encodedMacAddress;   // percent encoded mac address for use with the CLI
    QHash<QString,ListModel*> controlHierarchy;

    QProcess *player;
    SlimCLI *cli;
    QQuickView *viewer;
    DeviceStatus *dev;
    QModelIndex *nowPlayingIndex;
};

#endif // AUDIOPLAYER_H
