#ifndef THREADEDSLIMCLI_H
#define THREADEDSLIMCLI_H

#include <QThread>
#include <QMutexLocker>
#include <QMutex>
#include <QTcpSocket>
#include <QList>

class threadedslimCli : public QThread
{
    Q_OBJECT
public:
    explicit threadedslimCli(QObject *parent = 0, const char *name = NULL,
                             QString serverAdd= "127.0.0.1", QByteArray mac= QByteArray( "00:00:00:00:00:04" ),
                             quint16 cliport = 9090);
    ~threadedslimCli();

    void SendCommand( QByteArray cmd );
    void SendCommand( QByteArray cmd, QByteArray mac );
    QByteArray getResponse(void);   // returns first response in list
    void stop(void);

signals:
    void isConnected( void );               // we're connected to the SqueezeCenter server
    void cliError( int errmsg , const QString &message = "" );
    void cliInfo( QString msg );
    void messageReady(void);

protected:
    void run(void);

private:
    bool Connect(void);
//    void DeviceMsgProcessing( void ); // messages forwarded to devices
//    void SystemMsgProcessing( void ); // messages forwarded to the system for processing

//    bool SetupLogin( void );

//    QByteArray MacAddressOfResponse( void );
//    QByteArray ResponseLessMacAddress( void );
//    void RemoveNewLineFromResponse( void );

//    void ProcessLoginMsg( void );
//    void ProcessControlMsg( void );
//    void processStatusMsg( void );
//    bool waitForResponse( void );

private:
    bool isRunning;
    QMutex mutex;

    QTcpSocket *slimCliSocket;// socket for CLI interface

    QByteArray command;       // string to build a command (different from "currCommand" below that is used to check what the CLI sends back
    QList<QByteArray> responseList; // list of responses received by CLI -- need a list so we don't lose any responses
    QByteArray macAddress;       // NOTE: this is stored in URL escaped form, since that is how we mostly use it.  If you need it in plain text COPY IT to another string and use QUrl::decode() on that string.
    QString SlimServerAddr;   // server IP address
    quint16 cliPort;          // port to use for cli, usually 9090, but we allow the user to change this

    QString cliUsername;      // username for cli if needed
    QString cliPassword;      // password for cli if needed **NOTE: DANGER, DANGER this is done in clear text, so don't use a password you care about!!
    bool useAuthentication;   // test for using authentication
    bool isAuthenticated;     // have we been authenticated?
    QByteArray MaxRequestSize;      // max size of any cli request (used for limiting each request for albums, artists, songs, etc., so we don't time out or overload things)
    int iTimeOut;             // number of milliseconds before CLI blocking requests time out

    bool bConnection;         // is connected?
};

#endif // THREADEDSLIMCLI_H
