#ifndef SLIMCLI_H
#define SLIMCLI_H

// QT4 Headers
#include <QObject>
#include <QTcpSocket>
#include <QBuffer>
#include <QTimer>
#include <QVector>
#include <QUrl>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QByteArray>
#include <QList>
#include <QPixmap>
//#include <QFile>
//#include <QDataStream>

#include "squeezedefines.h"

/*
  * NOTE: This class establishes an object to communicate with a SqueezeCenter Server
  * via the SqueezeCenter command line interface (usually located at port 9090).  You
  * MUST set the IP address of the SqueezeCenter server and the port BEFORE you call init().
  * Otherwise it will default to the localhost and port 9090.
*/

class SlimCLI : public QObject {
    Q_OBJECT
//    Q_PROPERTY(QByteArray mac READ getMacAddress WRITE setMacAddress)
//    Q_PROPERTY(QString serverAddr READ getSqueezeServerAddress WRITE setSqueezeServerAddress)

public:
    SlimCLI( QObject *parent=0, const char *name = NULL,
             QString serverAdd= "127.0.0.1", QByteArray mac= QByteArray( "00:00:00:00:00:04" ),
             quint16 cliport = 9090);
    ~SlimCLI();

    void Init( void );
    void Connect( void );

    bool SendBlockingCommand( QByteArray cmd );
    bool SendBlockingCommand( QByteArray cmd, QByteArray mac );
    QByteArray GetBlockingCommandResponse( QByteArray cmd );    // send a blocking command and get the response back

    QByteArray GetResponse( void ) { return response; }
    QByteArray MacAddressOfResponse( void );
    QByteArray ResponseLessMacAddress( void );
    void RemoveNewLineFromResponse( void );

    void    SetMACAddress( QString addr );
    QByteArray GetMACAddress( void ) { return macAddress; }

//    void SetServerInfo(SlimServerInfo *s) { serverInfo = s; }
//    SlimServerInfo *GetServerInfo(void) { return serverInfo; }
    // ------------------------------------------------------------------------------------------
    void    SetSlimServerAddress( QString addr );
    QString GetSlimServerAddress( void ) { return SlimServerAddr; }
    // ------------------------------------------------------------------------------------------
    void    SetCliPort( int port );
    void    SetCliPort( QString port );
    int     GetCliPort( void ) { return cliPort; }
    // ------------------------------------------------------------------------------------------
    void    SetCliUsername( QString username );
    QString GetCliUsername( void ) { return cliUsername; }
    // ------------------------------------------------------------------------------------------
    void    SetCliPassword( QString password );
    QString GetCliPassword( void ) { return cliPassword; }
    // ------------------------------------------------------------------------------------------
    void    SetCliMaxRequestSize( QByteArray max ) { MaxRequestSize = max; }
    void    SetCliTimeOutPeriod( QString t ) { iTimeOut = t.toInt(); }    // NOTE: NO ERROR CHECKING!!!
    // ------------------------------------------------------------------------------------------
    bool    NeedAuthentication( void ) { return useAuthentication; }
    // ------------------------------------------------------------------------------------------
    void EmitCliInfo(QString msg) { emit cliInfo(msg);}

signals:
    void isConnected( void );               // we're connected to the SqueezeCenter server
    void cliError( int errmsg , const QString &message = "" );
    void cliInfo( QString msg );
    void FinishedInitializingDevices( void );

    void DeviceStatusMessage(QByteArray msg);

public slots:
    bool SendCommand( QByteArray cmd );
    bool SendCommand( QByteArray cmd, QByteArray mac );

private:
    QTcpSocket *slimCliSocket;// socket for CLI interface

    QByteArray command;       // string to build a command (different from "currCommand" below that is used to check what the CLI sends back
    QByteArray response;      // buffer to hold CLI response
    QList<QByteArray> responseList; // command response processed into "tag - response" pairs

    SlimServerInfo *serverInfo;   // convenience pointer

    QByteArray macAddress;       // NOTE: this is stored in URL escaped form, since that is how we mostly use it.  If you need it in plain text COPY IT to another string and use QUrl::decode() on that string.
    QString SlimServerAddr;   // server IP address
    quint16 cliPort;          // port to use for cli, usually 9090, but we allow the user to change this

    QString cliUsername;      // username for cli if needed
    QString cliPassword;      // password for cli if needed **NOTE: DANGER, DANGER this is done in clear text, so don't use a password you care about!!
    bool useAuthentication;   // test for using authentication
    bool isAuthenticated;     // have we been authenticated?
    QByteArray MaxRequestSize;      // max size of any cli request (used for limiting each request for albums, artists, songs, etc., so we don't time out or overload things)
    int iTimeOut;             // number of milliseconds before CLI blocking requests time out

    bool maintainConnection;  // do we reconnect if the connection is ended (set to false when shutting down so we don't attempt a reconnect
    bool bConnection;         // is connected?

    cliState myCliState;       // state of cli / setup process

    void DeviceMsgProcessing( void ); // messages forwarded to devices
    void SystemMsgProcessing( void ); // messages forwarded to the system for processing

    bool SetupLogin( void );

    void ProcessLoginMsg( void );
    void ProcessControlMsg( void );
    void processStatusMsg( void );
    bool waitForResponse( void );

private slots:
    bool msgWaiting( void );          // we have a message waiting from the server
    bool CLIConnectionOpen( void );   // CLI interface successfully established
    void LostConnection( void );      // lost connection (check if we want reconnect)
    void ConnectionError( int err );  // error message sent with connection
    void ConnectionError( QAbstractSocket::SocketError );
    void SentBytes( int b );          // bytes sent to SqueezeCenter
};


#endif // SLIMCLI_H
