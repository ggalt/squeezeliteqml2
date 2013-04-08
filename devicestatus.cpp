#include "devicestatus.h"

#ifdef SLIMDEVICE_DEBUG
#define DEBUGF(...) qDebug() << this->objectName() << Q_FUNC_INFO << __VA_ARGS__;
#else
#define DEBUGF(...)
#endif


DeviceStatus::DeviceStatus(QObject *parent) :
    QObject(parent)
{
    //    QByteArray m_deviceVol; // volume (0-100)
    //    bool m_deviceMute; // is device muted
    //    QByteArray m_deviceMode; // one of the following: "play", "stop" or "pause"
    //    QByteArray m_deviceRepeatMode;
    //    QByteArray m_deviceShuffleMode;
    //    bool m_isPlaying;

    //    QByteArray m_devicePlaylistName; // name of current playlist
    //    int m_devicePlaylistCount; // number of tracks in current playlist
    //    int m_devicePlaylistIndex;  // where are we in the current playlist
    //    CurrentPlayList m_devicePlayList; // all info related to the current device playlist
    //    QTime m_playerTime;   // how long have we been playing?

    //    TrackData m_currentTrack;
    //    QByteArray m_deviceCurrentSongTime; // time into current song
    //    QByteArray m_deviceCurrentSongDuration; // length of current song
}

void DeviceStatus::Init(void)
{
    DEBUGF("Device Init");
    m_playerTime.start();
    // this command gets genre, artist, album, title, year, duration, artwork_id (to get cover image)
    issueCommand(QByteArray("status 0 1000 tags:g,a,l,t,e,y,d,c \n"));
}

void DeviceStatus::processDeviceStatusMsg(QByteArray msg)
{
    DEBUGF( "StatusSetupMessage start");
    /*
   * we can't yet remove the percent encoding because some of the msgs like
   * "playlist index" have a space (%20) in them and it will throw off the sectioning of the string
   * using mgs.split below if we do it now, but we do need to convert the %3A to a ':'
  */
    m_devicePlayList.clear();
    msg.replace( "%3A", ":" );
    QList<QByteArray> MsgList = msg.split( ' ' );    // put all of the status messages into an array for processing
    bool playlistInfo = false;

    QListIterator<QByteArray> i( MsgList );
    DEBUGF( "list has size: " << MsgList.count() );

    // note we start on the 4th field because the first 4 are <"status"> <"0"> <"1000"> <"tags:"> and we can ignore them
    for( int c = 0; c < 4 && i.hasNext(); c++ )
        i.next();

    int t = 0;
    while( i.hasNext() ) {
        t++;
        QString s = QString( i.next() );
        DEBUGF( "List item [ " << t-1 << " ] is: " << s );
        if( !playlistInfo ) {
            if( s.section( ':', 0, 0 ) == "mode" )
                m_deviceMode = QByteArray::fromPercentEncoding( s.section( ':', 1, 1 ).toLatin1() );
            else if( s.section( ':', 0, 0 ) == "time" )
                m_deviceCurrentSongTime = QByteArray::fromPercentEncoding( s.section( ':', 1, 1 ).toLatin1() );
            else if( s.section( ':', 0, 0 ) == "duration" )
                m_deviceCurrentSongDuration = QByteArray::fromPercentEncoding( s.section( ':', 1, 1 ).toLatin1() );
            else if( s.section( ':', 0, 0 ) == "mixer%20volume" )
                m_deviceVol = QByteArray::fromPercentEncoding( s.section( ':', 1, 1 ).toLatin1() );
            else if( s.section( ':', 0, 0 ) == "playlist%20repeat" )
                m_deviceRepeatMode = QByteArray::fromPercentEncoding( s.section( ':', 1, 1 ).toLatin1() );
            else if( s.section( ':', 0, 0 ) == "playlist%20shuffle" )
                m_deviceShuffleMode = QByteArray::fromPercentEncoding( s.section( ':', 1, 1 ).toLatin1() );
            else if( s.section( ':', 0, 0 ) == "playlist_cur_index" )
                m_devicePlaylistIndex = s.section( ':', 1, 1 ).toInt();
            else if( s.section( ':', 0, 0 ) == "playlist_tracks" ) {  // OK, we've gotten to the portion of the program where the playlist info is ready
                m_devicePlaylistCount = s.section( ':', 1, 1 ).toInt();
                playlistInfo = true;
            }
        }
        else {
            if( s.section( ':', 0, 0 ) == "playlist%20index" ) {
                m_devicePlayList.append( TrackData() );
                m_devicePlayList.last().playlist_index = QByteArray::fromPercentEncoding( s.section( ':', 1, 1 ).toLatin1() );
            }
            else if( s.section( ':', 0, 0 ) == "title" )
                m_devicePlayList.last().title = QByteArray::fromPercentEncoding( s.section( ':', 1, 1 ).toLatin1() );
            else if( s.section( ':', 0, 0 ) == "album_id")
                m_devicePlayList.last().album_id = QByteArray::fromPercentEncoding(s.section( ':', 1, 1 ).toLatin1() );
            else if( s.section( ':', 0, 0 ) == "genre" )
                m_devicePlayList.last().genre = QByteArray::fromPercentEncoding( s.section( ':', 1, 1 ).toLatin1() );
            else if( s.section( ':', 0, 0 ) == "artist" )
                m_devicePlayList.last().artist = QByteArray::fromPercentEncoding( s.section( ':', 1, 1 ).toLatin1() );
            else if( s.section( ':', 0, 0 ) == "album" )
                m_devicePlayList.last().album = QByteArray::fromPercentEncoding( s.section( ':', 1, 1 ).toLatin1() );
            else if( s.section( ':', 0, 0 ) == "tracknum" )
                m_devicePlayList.last().tracknum = QByteArray::fromPercentEncoding( s.section( ':', 1, 1 ).toLatin1() );
            else if( s.section( ':', 0, 0 ) == "year" )
                m_devicePlayList.last().year = QByteArray::fromPercentEncoding( s.section( ':', 1, 1 ).toLatin1() );
            else if( s.section( ':', 0, 0 ) == "duration" )
                m_devicePlayList.last().duration = QByteArray::fromPercentEncoding( s.section( ':', 1, 1 ).toLatin1() );
            else if( s.section( ':', 0, 0 ) == "coverid" )
                m_devicePlayList.last().coverid = QByteArray::fromPercentEncoding( s.section( ':', 1, 1 ).toLatin1() );
            else if( s.section( ':', 0, 0 ) == "id" )
                m_devicePlayList.last().song_id = QByteArray::fromPercentEncoding( s.section( ':', 1, 1 ).toLatin1() );
        }
    }
    DEBUGF("Device Status processing ends");
    emit issueCommand(QByteArray("mode ? \n"));
    emit playlistIndexChange(QVariant(1));
    qDebug() << "index message sent";
}
