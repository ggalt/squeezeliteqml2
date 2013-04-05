#ifndef DEVICESTATUS_H
#define DEVICESTATUS_H

#include <QObject>
#include <QByteArray>
#include <QDateTime>
#include <QQuickItem>

#include "squeezedefines.h"


class DeviceStatus : public QObject
{
    Q_OBJECT

public:
    explicit DeviceStatus(QObject *parent = 0);
    
signals:
    void issueCommand(QString cmd);
    
public slots:
    void processDeviceStatusMsg(QByteArray msg);

private:
    /*
* NOTE: almost all of the variables are exchanged with the server as
* string-based commands and so we store virutually everything as
* QByteArrays rather than as the actual item represented (e.g., an 'int')
* so that server communication doesn't require a bunch of conversions back
* and forth -- except in rare instances.
*/
    QByteArray m_deviceNumber; // device number in "Device count" (i.e., if there are 3 devices, which number (0,1,2) is this?)
    QByteArray m_deviceMAC; // MAC address of the device
    QByteArray m_deviceName; // Name as listed in server (e.g., "SoftSqueeze" or "Kitchen Device")
    QByteArray m_deviceIP; // IP address of device

    QByteArray m_deviceVol; // volume (0-100)
    bool m_deviceMute; // is device muted
    QByteArray m_deviceMode; // one of the following: "play", "stop" or "pause"
    QByteArray m_deviceRepeatMode;
    QByteArray m_deviceShuffleMode;
    bool m_isPlaying;

    QByteArray m_devicePlaylistName; // name of current playlist
    int m_devicePlaylistCount; // number of tracks in current playlist
    int m_devicePlaylistIndex;  // where are we in the current playlist
    CurrentPlayList m_devicePlayList; // all info related to the current device playlist
    QTime m_playerTime;   // how long have we been playing?

    TrackData m_currentTrack;
    QByteArray m_deviceCurrentSongTime; // time into current song
    QByteArray m_deviceCurrentSongDuration; // length of current song
};

// http://10.6.67.54:9000/html/images/artists_500x500.png

#endif // DEVICESTATUS_H
