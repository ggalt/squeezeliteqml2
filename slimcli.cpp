#include <QtAlgorithms>
#include "slimcli.h"

#ifdef SLIMCLI_DEBUG
#define DEBUGF(...) qDebug() << this->objectName() << Q_FUNC_INFO << __VA_ARGS__;
#else
#define DEBUGF(...)
#endif

//#define PATH "./qtsqueezeimage.dat"


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ---------------------------- OBJECT INITIALIZATION ---------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

SlimCLI::SlimCLI(QObject *parent, const char *name,
                  QString serverAdd, QByteArray mac,
                  quint16 cliport) : QObject(parent)
{
    setObjectName("SlimCLI");
    SlimServerAddr = serverAdd;
    cliPort = cliport;          // default, but user can reset
    macAddress = mac;

    MaxRequestSize = "100";     // max size of any cli request (used for limiting each request for albums, artists, songs, etc., so we don't time out or overload things)
    iTimeOut = 5000;           // number of milliseconds before CLI blocking requests time out
    useAuthentication = false;  // assume we don't need it unless we do
    isAuthenticated = true;     // we will assume that authentication is not used (and therefore we have been authenticated!!)
    maintainConnection = true;
    myCliState = CLI_DISCONNECTED;
}

SlimCLI::~SlimCLI(){
    maintainConnection = false;
    disconnect(slimCliSocket, SIGNAL(disconnected()),      this, SLOT(LostConnection()));
    SendCommand ("exit");  // shut down CLI interface
    slimCliSocket->flush();
    delete slimCliSocket;
}


void SlimCLI::Init(void)
{
    DEBUGF("");
    slimCliSocket = new QTcpSocket(this);

    if(!cliUsername.isEmpty() && !cliPassword.isEmpty()) { // we need to authenticate
        useAuthentication = true;
        isAuthenticated = false;  // will be reset later if we succeed
    }

    connect(slimCliSocket, SIGNAL(connected()),
             this, SLOT(CLIConnectionOpen()));
    connect(slimCliSocket, SIGNAL(error(QAbstractSocket::SocketError)), \
             this, SLOT(ConnectionError(QAbstractSocket::SocketError)));

    emit cliInfo(QString("Connecting to Squeezebox Server"));
    Connect();    // connect to host
    //NOTE: WE HAVE NOT YET CONNECTED THE READYREAD SIGNAL TO THE MSGWAITING SLOT SO THAT WE CAN DO SOME BLOCKING CALLS TO THE CLI
}

bool SlimCLI::SetupLogin(void){
    DEBUGF("");
    command.clear();
    QString cmd = QString ("login %1 %2\n")
            .arg(QString(cliUsername.toLatin1()))
            .arg(QString(cliPassword.toLatin1()));
    command.append(cmd);

    if(!waitForResponse()) // NOTE: WE HAVE NO NEED TO PROCESS THE RESPONSE SINCE SUCCESS GIVES NO INFO AND FAILURE TRIGGERS A DISCONNECT
        return false;
    else
        return true;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ---------------------------- COMMUNICATION WITH SERVER AND SLOTS -------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

void SlimCLI::Connect(void){
    if(!slimCliSocket->isValid()) {
        DEBUGF("invalid socket");
    }
    QString myMsg = QString("Connecting to %1 on port %2").arg(SlimServerAddr).arg(cliPort);
    DEBUGF(myMsg);
    slimCliSocket->connectToHost(SlimServerAddr, cliPort);
    DEBUGF("CONNECTION MESSAGE SENT");
}

bool SlimCLI::CLIConnectionOpen(void){
    DEBUGF("");
    // NOTE: these commands are sent from here rather than using the SendCommand() function because we DO NOT want to place the MAC address in front of them
    // we'll use a blocking call for authentication and grabbing the available players because it's easier and it's quick

    myCliState = CLI_CONNECTED;
    emit cliInfo(QString("Establishing Connection to Squeezebox Server"));
    if(useAuthentication)
        if(!SetupLogin()) {
            emit cliError(0, QString("No login"));
            return false;
        }

    isAuthenticated = true;   // we've made it to here, so we are authenticated (either through u/p combo, or because there is no u/p

    connect(slimCliSocket, SIGNAL(readyRead()),
            this, SLOT(msgWaiting()));
    connect(slimCliSocket, SIGNAL(disconnected()),
            this, SLOT(LostConnection()));

    // alert everyone we're connected
    DEBUGF("Device Init");
    emit isConnected();
    return true;
}

void SlimCLI::LostConnection(void){
    DEBUGF("");
    // just in case we want to restart things, let's first disconnect the signals and slots that will be established at successful connection
    disconnect(slimCliSocket, SIGNAL(readyRead()), 0, 0);
    disconnect(slimCliSocket, SIGNAL(disconnected()), 0, 0);

    if(!isAuthenticated && useAuthentication) // we probably lost the connection due to a bad password
        emit cliError(NOLOGIN);
    if(maintainConnection) {
        Connect();
    }
}

void SlimCLI::ConnectionError(int err){
    DEBUGF(QString("Connection error: %1").arg(err));
    emit cliError(CONNECTION_ERROR);

}

void SlimCLI::ConnectionError(QAbstractSocket::SocketError err){
    DEBUGF(QString("Connection error: %1").arg(slimCliSocket->errorString()));
    emit cliError(CONNECTION_ERROR);
}

void SlimCLI::SentBytes(int b){
    DEBUGF("Bytes written to socket: " << b);
}

bool SlimCLI::SendCommand(QByteArray cmd)
{
    return SendCommand(cmd, macAddress);
}

bool SlimCLI::SendCommand(QByteArray cmd, QByteArray mac)
{
    DEBUGF("Sending command:" << mac << cmd);
    // NOTE:: SendCommand assumes that the command string has been filled in and already been put in URL escape form
    // and that a MAC address is at the beginning of the command (if needed only!!)
    if(!cmd.isNull())
    {
        command.clear();
        command = mac +" " + cmd;
    }

    if(!command.trimmed().endsWith("\n")) // need to terminate with a \n
        command = command.trimmed() + "\n";

    if(slimCliSocket->write(command) > 0) {
        slimCliSocket->flush();
        return true;
    }
    else
        return false;
}

QByteArray SlimCLI::GetBlockingCommandResponse(QByteArray c)
{
    DEBUGF("");
    if(SendBlockingCommand(c))
        return response;
    else
        return NULL;
}

bool SlimCLI::SendBlockingCommand(QByteArray cmd)
{
    return SendBlockingCommand(cmd, macAddress);
}

bool SlimCLI::SendBlockingCommand(QByteArray cmd, QByteArray mac)
{
    DEBUGF("");
    // NOTE:: SendCommand assumes that the command string has been filled in and already been put in URL escape form
    // and that a MAC address is at the beginning of the command (if needed only!!)
    // REMEMBER, THIS BLOCKS UNTIL A RETURN IS RECEIVED!! USE WITH CARE

    if(!cmd.isNull())
    {
        command.clear();
        command = cmd;
    }

    if(!command.trimmed().endsWith("\n")) // need to terminate with a \n
        command = command.trimmed() + "\n";

    return waitForResponse();
}

bool SlimCLI::waitForResponse(void){
    DEBUGF("");
    slimCliSocket->write(command);
    if(!slimCliSocket->waitForReadyRead())
        return false;
    QTime t;
    t.start();

    QByteArray lineBuf = "";

    while(t.elapsed() < iTimeOut) {
        if(slimCliSocket->canReadLine()) {    // most times, we're going to stop here, but if the socket receives a large amount of data, it seems to need to dump it in chucks, hence the line below.
            lineBuf += slimCliSocket->readLine();
            break;
        }
        else {  // if we haven't received the \n yet, take what we've got and loop.  Sometimes the socket seems to get clogged with data otherwise.  We'll capture the rest of the line above, if it comes before we time out.
            lineBuf += slimCliSocket->read(slimCliSocket->bytesAvailable());
            DEBUGF("LESS THAN A FULL LINE RECEIVED: " << lineBuf);
        }
    }

    if(t.elapsed() >= iTimeOut) {
        QString myMsg = QString("Read line of %1 length, with the following info: %2").arg(lineBuf.size()).arg (QString(lineBuf));
        QString errmsg = QString("Connection timed out with current timeout limit of %1 milliseconds.  Consider increasing.").arg(iTimeOut);
        emit cliError(CONNECTION_TIMEOUT, errmsg);
        return false;
    }
    response = lineBuf;
    RemoveNewLineFromResponse();

    return true;
}

bool SlimCLI::msgWaiting(void)
{
    DEBUGF("");
    QTime t;
    t.start();
    bool readSomething = false;
    while(slimCliSocket->bytesAvailable() && t.elapsed() < iTimeOut) {
        if(slimCliSocket->canReadLine()) {
            response = slimCliSocket->readLine();
            readSomething=true;
            RemoveNewLineFromResponse();
            QRegExp MACrx("[0-9a-fA-F][0-9a-fA-F]%3A[0-9a-fA-F][0-9a-fA-F]%3A[0-9a-fA-F][0-9a-fA-F]%3A[0-9a-fA-F][0-9a-fA-F]%3A[0-9a-fA-F][0-9a-fA-F]%3A[0-9a-fA-F][0-9a-fA-F]");
            if(MACrx.indexIn(QString(response)) >= 0) { // if it starts with a MAC address, send it to a device for processing
                DeviceMsgProcessing();
            }
            else {
                SystemMsgProcessing();
            }
        }
    }
    return readSomething;
}

// ------------------------------------------------------------------------------------------------
// ---------------------------- MESSAGE PROCESSING ------------------------------------------------
// ------------------------------------------------------------------------------------------------


void SlimCLI::DeviceMsgProcessing(void)
{
    DEBUGF("Device Message: " << response.left(200));

    if(macAddress.toLower() == MacAddressOfResponse().toLower()) {
        QByteArray resp = ResponseLessMacAddress();
        DEBUGF("MSG with MAC removed:" << resp.left(200));
        if(resp.startsWith("status")) {
            emit DeviceStatusMessage(resp);
        }
        else {
            emit PlaylistInteractionMessage(resp);
        }
    }
    else {  // wait!  Whose MAC address is this?
        DEBUGF(QString("Unknown MAC address: %1 our address is %2").arg(QString(MacAddressOfResponse())).arg(QString(macAddress)));
    }
}

void SlimCLI::SystemMsgProcessing(void)
{
    DEBUGF("SYSTEM MESSAGE: " << response);
}

void SlimCLI::ProcessLoginMsg(void)
{
    DEBUGF("");
    if(response.left(5) == "login")
        isAuthenticated = true;
}

void SlimCLI::ProcessControlMsg(void)
{
    DEBUGF("CONTROLLING MODE message received: " << response);
    responseList = response.split(' ');   // break this up into fields delimited by spaces
    if(response.left(7) == "artists") {  // it's processing artist information
        if(responseList.at(3).left(9) == "artist_id") {
            for(int c = 3; c < responseList.size(); c++) {
            }
        }
        return;
    }

    if(response.left(6) == "albums" )  // its processing album information
        return;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ---------------------------- HELPER FUNCTIONS --------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


void SlimCLI::SetMACAddress(QString addr)
{
    DEBUGF("");
    if(!addr.contains("%3A")) // not escape encoded
        macAddress = addr.toLatin1().toPercentEncoding();
    //        macAddress = addr.toAscii().toPercentEncoding();
    else
//        macAddress = addr.toAscii();
        macAddress = addr.toLatin1();
}

void SlimCLI::SetSlimServerAddress(QString addr){
    DEBUGF("");
    SlimServerAddr = addr;
}

void SlimCLI::SetCliPort(int port){
    cliPort = port;
}

void SlimCLI::SetCliPort(QString port){
    cliPort = port.toInt();
}

void SlimCLI::SetCliUsername(QString username){
    cliUsername = username;
}

void SlimCLI::SetCliPassword(QString password){
    cliPassword = password;
}

QByteArray SlimCLI::MacAddressOfResponse(void)
{
    DEBUGF("");
    if(response.contains("%3A"))
        return response.left(27).trimmed().toLower();
    else
        return QByteArray();
}

QByteArray SlimCLI::ResponseLessMacAddress(void)
{
    DEBUGF("");
    if(response.contains("%3A"))
        return response.right(response.length() - 27).trimmed();
    else
        return response.trimmed();
}

void SlimCLI::RemoveNewLineFromResponse(void)
{
    DEBUGF("");
    while(response.contains('\n'))
        response.replace(response.indexOf('\n'), 1, " ");

}

/* vim: set expandtab tabstop=4 shiftwidth=4: */
