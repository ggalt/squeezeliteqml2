#ifndef DEVICESTATUS_H
#define DEVICESTATUS_H

#include <QObject>
#include <QByteArray>
#include <QDateTime>
#include <QQuickItem>

#include "squeezedefines.h"
#include "audioplayer.h"
#include "slimcli.h"

class DeviceStatus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QByteArray DeviceNumber READ DeviceNumber WRITE setDeviceNumber)
    Q_PROPERTY(QByteArray DeviceMAC READ DeviceMAC WRITE setDeviceMAC)
    Q_PROPERTY(QByteArray DeviceName READ DeviceName WRITE setDeviceName)
    Q_PROPERTY(QByteArray DeviceIP READ DeviceIP WRITE setDeviceIP)

    Q_PROPERTY(QByteArray Volume READ Volume WRITE setVolume)
    Q_PROPERTY(bool Mute READ Mute WRITE setMute)
    Q_PROPERTY(QByteArray Mode READ Mode WRITE setMode)
    Q_PROPERTY(QByteArray Repeat READ Repeat WRITE setRepeat)
    Q_PROPERTY(QByteArray Shuffle READ Shuffle WRITE setShuffle)
    Q_PROPERTY(QByteArray CurrentSongTime READ CurrentSongTime WRITE setCurrentSongTime)
    Q_PROPERTY(QByteArray CurrentSongDuration READ CurrentSongDuration WRITE setCurrentSongDuration)

public:
    explicit DeviceStatus(QObject *parent = 0);
    
    QString getTitle(void) { return m_title; }
    void setTitle(QString str ) { m_title = str; }
    QString getGenre (void) { return m_genre; }
    void setGenre(QString str ) { m_genre = str; }
    QString getArtist (void) { return m_artist; }
    void setArtist(QString str ) { m_artist = str; }
    QString getAlbum (void) { return m_album; }
    void setAlbum(QString str ) { m_album = str; }
    QString getTrackNum (void) { return m_tracknum; }
    void setTrackNum(QString str ) { m_tracknum = str; }
    QString getYear (void) { return m_year; }
    void setYear(QString str ) { m_year = str; }
    QString getDuration (void) { return m_duration; }
    void setDuration(QString str ) { m_duration = str; }
    QString getCoverID (void) { return m_coverid; }
    void setCoverID(QString str ) { m_coverid = str; }

signals:
    
public slots:

private:
    SlimCLI *cli;
    QQuickItem *viewer;

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
    QByteArray m_deviceCurrentSongTime; // time into current song
    QByteArray m_deviceCurrentSongDuration; // length of current song
    bool m_isPlaying;

    QByteArray m_devicePlaylistName; // name of current playlist
    int m_devicePlaylistCount; // number of tracks in current playlist
    int m_devicePlaylistIndex;  // where are we in the current playlist
    CurrentPlayList m_devicePlayList; // all info related to the current device playlist
    TrackData m_currentTrack;
    QTime m_playerTime;   // how long have we been playing?
};

// http://10.6.67.54:9000/html/images/artists_500x500.png

#endif // DEVICESTATUS_H
