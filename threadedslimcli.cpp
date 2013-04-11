#include <QEventLoop>
#include <Qtimer>

#include "threadedslimcli.h"
#include "squeezedefines.h"

#ifdef SLIMCLI_DEBUG
#define DEBUGF(...) qDebug() << this->objectName() << Q_FUNC_INFO << __VA_ARGS__;
#else
#define DEBUGF(...)
#endif


threadedslimCli::threadedslimCli(QObject *parent, const char *name, QString serverAdd, QByteArray mac, quint16 cliport) :
    QThread(parent)
{
    DEBUGF("threaded cli created");
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
    SendCommand("exit");  // shut down CLI interface
    slimCliSocket->flush();
    delete slimCliSocket;
}


void threadedslimCli::SendCommand( QByteArray cmd )
{
    SendCommand(cmd,macAddress);
}

void threadedslimCli::SendCommand( QByteArray cmd, QByteArray mac )
{
    DEBUGF("Sending command" << cmd << "to" << mac);
    cmd = mac +" "+cmd;
    if(!cmd.trimmed().endsWith("\n")) // need to terminate with a \n
        cmd = cmd.trimmed() + "\n";
    QMutexLocker m(&mutex);
    command = cmd;
}

QByteArray threadedslimCli::getResponse(void)   // returns first response in list
{
    DEBUGF("returning response");
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
    DEBUGF("CONNECTING TO CLI");
    DEBUGF("starting run");
    slimCliSocket = new QTcpSocket(0);

    if(!cliUsername.isEmpty() && !cliPassword.isEmpty()) { // we need to authenticate
        useAuthentication = true;
        isAuthenticated = false;  // will be reset later if we succeed
    }

    emit cliInfo(QString("Connecting to Squeezebox Server"));

    QEventLoop q;
    QTimer tT;

    tT.setSingleShot(true);
    connect(&tT, SIGNAL(timeout()), &q, SLOT(quit()),Qt::DirectConnection);
    connect(slimCliSocket, SIGNAL(connected()),
            &q, SLOT(quit()),Qt::DirectConnection);

    slimCliSocket->connectToHost(SlimServerAddr, cliPort);
    DEBUGF("connecting to" << SlimServerAddr << "on port" << cliPort);

    tT.start(5000); // 5s timeout
    q.exec();

    if(tT.isActive()){
        // download complete
        tT.stop();
    } else {
        DEBUGF("Audio Header Request Timed Out");
        return false; // returning empty string will indicate no header retrieved
    }
    emit isConnected();
    return true;
}

void threadedslimCli::stop(void)
{
    QMutexLocker m(&mutex);
    isRunning = false;
}

void threadedslimCli::run(void)
{
    // main loop looking for messages
    isRunning = true;
    while(isRunning) {
        // see if there is a message to send
        mutex.lock();
        if(!command.isEmpty()){
            DEBUGF("SENDING MESSAGE FROM CLI:" << command);
            slimCliSocket->write(command);
            slimCliSocket->flush();
            command.clear();
        }
        mutex.unlock();


        // see if there is anything to read
//        slimCliSocket->waitForReadyRead(5000);
        while(slimCliSocket->canReadLine()) {
            mutex.lock();
            responseList.append(slimCliSocket->readLine());
            mutex.unlock();
            DEBUGF("Message from CLI available:" << responseList.last().left(100));
            emit messageReady();
        }
    }
}
