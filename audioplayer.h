#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QByteArray>

#include "squeezedefines.h"
#include "slimcli.h"

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    explicit AudioPlayer(QObject *parent = 0);
    ~AudioPlayer();

    void Init(void);
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
    void shuffleState(int state);
    void nextTrackClicked(void);
    void prevTrackClicked(void);
    void playState(int state);
    void volUp(void);
    void volDown(void);

private:
    void getplayerMACAddress(void);

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

    QProcess *player;
    SlimCLI *cli;
};

#endif // AUDIOPLAYER_H