#ifndef THREADEDSLIMCLI_H
#define THREADEDSLIMCLI_H

#include <QThread>
#include <QMutexLocker>
#include <QMutex>
#include <QTcpSocket>

class threadedslimCli : public QThread
{
    Q_OBJECT
public:
    explicit threadedslimCli(QObject *parent = 0, const char *name = NULL,
                             QString serverAdd= "127.0.0.1", QByteArray mac= QByteArray( "00:00:00:00:00:04" ),
                             quint16 cliport = 9090);
/*
    bool SendCommand( QByteArray cmd );
    bool SendCommand( QByteArray cmd, QByteArray mac );

signals:
    void isConnected( void );               // we're connected to the SqueezeCenter server
    void cliError( int errmsg , const QString &message = "" );
    void cliInfo( QString msg );

protected:
    void run(void);

private:
    void DeviceMsgProcessing( void ); // messages forwarded to devices
    void SystemMsgProcessing( void ); // messages forwarded to the system for processing

    bool SetupLogin( void );

    QByteArray MacAddressOfResponse( void );
    QByteArray ResponseLessMacAddress( void );
    void RemoveNewLineFromResponse( void );

    void ProcessLoginMsg( void );
    void ProcessControlMsg( void );
    void processStatusMsg( void );
    bool waitForResponse( void );

private:
    bool isRunning;

    QTcpSocket *slimCliSocket;// socket for CLI interface

    QByteArray command;       // string to build a command (different from "currCommand" below that is used to check what the CLI sends back
    QByteArray response;      // buffer to hold CLI response
    QList<QByteArray> responseList; // command response processed into "tag - response" pairs
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
*/
};

#endif // THREADEDSLIMCLI_H
