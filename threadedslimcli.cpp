#include "threadedslimcli.h"
#ifdef SLIMCLI_DEBUG
#define DEBUGF(...) qDebug() << this->objectName() << Q_FUNC_INFO << __VA_ARGS__;
#else
#define DEBUGF(...)
#endif


threadedslimCli::threadedslimCli(QObject *parent, const char *name, QString serverAdd, QByteArray mac, quint16 cliport) :
    QThread(parent)
{
    setObjectName("threadedSlimCLI");
    SlimServerAddr = serverAdd;
    cliPort = cliport;          // default, but user can reset
    macAddress = mac;

    MaxRequestSize = "100";     // max size of any cli request (used for limiting each request for albums, artists, songs, etc., so we don't time out or overload things)
    iTimeOut = 5000;           // number of milliseconds before CLI blocking requests time out
    useAuthentication = false;  // assume we don't need it unless we do
    isAuthenticated = true;     // we will assume that authentication is not used (and therefore we have been authenticated!!)
}

threadedslimCli::~threadedslimCli()
{
    disconnect(slimCliSocket, SIGNAL(disconnected()),      this, SLOT(LostConnection()));
    SendCommand ("exit");  // shut down CLI interface
    slimCliSocket->flush();
    delete slimCliSocket;
}


void threadedslimCli::SendCommand( QByteArray cmd )
{
    SendCommand(cmd,macAddress);
}

void threadedslimCli::SendCommand( QByteArray cmd, QByteArray mac )
{
    if(!cmd.trimmed().endsWith("\n")) // need to terminate with a \n
        cmd = cmd.trimmed() + "\n";
    QMutexLocker m(&mutex);
    command = cmd;
}

QByteArray threadedslimCli::getResponse(void)   // returns first response in list
{
    QMutexLocker m(&mutex);
    if(!responseList.isEmpty()) {
        QByteArray resp = responseList.first();
        responseList.pop_front();
        return resp;
    }
    else
        return QByteArray();    // return an empty QByteArray if there is nothing to read
}

//void threadedslimCli::DeviceMsgProcessing( void ); // messages forwarded to devices
//void threadedslimCli::SystemMsgProcessing( void ); // messages forwarded to the system for processing
//bool threadedslimCli::SetupLogin( void );
//QByteArray threadedslimCli::MacAddressOfResponse( void );
//QByteArray threadedslimCli::ResponseLessMacAddress( void );
//void threadedslimCli::RemoveNewLineFromResponse( void );
//void threadedslimCli::ProcessLoginMsg( void );
//void threadedslimCli::ProcessControlMsg( void );
//void threadedslimCli::processStatusMsg( void );

bool threadedslimCli::Connect(void)
{
    QEventLoop q;
    QTimer tT;

    tT.setSingleShot(true);
    connect(&tT, SIGNAL(timeout()), &q, SLOT(quit()),Qt::DirectConnection);
    connect(slimCliSocket, SIGNAL(connected()),
            &q, SLOT(quit()),Qt::DirectConnection);

    slimCliSocket->connectToHost(SlimServerAddr, cliPort);

    tT.start(5000); // 5s timeout
    q.exec();

    if(tT.isActive()){
        // download complete
        tT.stop();
    } else {
        VERBOSE( VB_IMPORTANT, LOC + "Audio Header Request Timed Out");
        return QString(""); // returning empty string will indicate no header retrieved
    }
}

void threadedslimCli::stop(void)
{
    QMutexLocker m(&mutex);
    isRunning = false;
}

void threadedslimCli::run(void)
{
    DEBUGF("");
    slimCliSocket = new QTcpSocket(this);

    if(!cliUsername.isEmpty() && !cliPassword.isEmpty()) { // we need to authenticate
        useAuthentication = true;
        isAuthenticated = false;  // will be reset later if we succeed
    }

    emit cliInfo(QString("Connecting to Squeezebox Server"));

    // main loop looking for messages
    isRunning = true;
    while(isRunning) {
        {   // see if there is a message to send
            QMutexLocker m(&mutex);
            if(!command.isEmpty()){
                slimCliSocket->write(command);
                command.clear();
            }
        }

        // see if there is anything to read
        if(slimCliSocket->canReadLine()) {
            QMutexLocker m(&mutex);
            responseList.append(slimCliSocket->readLine());
            emit messageReady();
        }
    }
}
