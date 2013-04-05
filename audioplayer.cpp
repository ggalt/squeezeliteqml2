#include <QList>
#include <QListIterator>
#include <QNetworkInterface>
#include <QQmlContext>

#include "audioplayer.h"

#ifdef AUDIOPLAYER_DEBUG
#define DEBUGF(...) qDebug() << this->objectName() << Q_FUNC_INFO << __VA_ARGS__;
#else
#define DEBUGF(...)
#endif



AudioPlayer::AudioPlayer(QObject *parent) :
    QObject(parent)
{
}

AudioPlayer::~AudioPlayer()
{
    Close();
}

void AudioPlayer::Close(void)
{
    DEBUGF("");
    if(player) {
        DEBUGF("CLOSING PLAYER");
        player->close();
        player->waitForFinished(5000);
        player->deleteLater();
    }
}

void AudioPlayer::Init(QQuickView *v)
{
    viewer = v;     // connection so that we can manipulate it later and update the interface

    QSettings *mySettings = new QSettings("squeezelitegui", "squeezelitegui");

    if( !mySettings->contains("Version") || mySettings->value("Version")!=DATAVERSION) {     // no settings file, so create one
        mySettings->setValue("Version", DATAVERSION);
        mySettings->setValue("Server/Address","127.0.0.1");
        //        mySettings->setValue("Server/Address","10.6.67.54");
        mySettings->setValue("Server/AudioPort","3483");
        mySettings->setValue("Server/CLIPort", "9090");
        mySettings->setValue("Server/HttpPort", "9000");
        mySettings->setValue("Server/Username", "");
        mySettings->setValue("Server/Password", "");
        mySettings->setValue("Audio/Device","default");
        mySettings->setValue("Player/Name","squeezelite");
        mySettings->sync();
    }

    lmsUsername = mySettings->value("Server/Username","").toString();
    lmsPassword = mySettings->value("Server/Password","").toString();
    SqueezeBoxServerAddress = mySettings->value("Server/Address","127.0.0.1").toString();
    SqueezeBoxServerAudioPort = mySettings->value("Server/AudioPort","3483").toString();
    SqueezeBoxServerCLIPort = mySettings->value("Server/CLIPort", "9090").toString();
    SqueezeBoxServerHttpPort = mySettings->value("Server/HttpPort", "9000").toString();
    AudioDevice = mySettings->value("Audio/Device","").toString();
    PlayerName = mySettings->value("Player/Name", "squeezelite").toString();

    getplayerMACAddress();  // get the MAC address we are going to use

    player = new QProcess(this);

#ifdef Q_OS_LINUX
    QString program = "squeezelite";
    program += (QString(" -m " + QString( MacAddress ))) + (QString(" -n " + PlayerName)) + (QString(" -o " + AudioDevice)) + " " + SqueezeBoxServerAddress;
#else
    QString program = QString('"')+QString("c:\\program files (x86)\\squeezelite\\squeezelite-win")+QString('"');
    program += (QString(" -m " + QString( MacAddress ))) + (QString(" -n " + PlayerName)) + (QString(" -o " + AudioDevice)) + " " + SqueezeBoxServerAddress;
#endif

    DEBUGF( "player command " << program);

    player->start( program );
    DEBUGF("ERROR STRING IF ANY" << player->error() << player->errorString() );
    //    player->waitForStarted(2000);


    //    // initialize the CLI interface.  Make sure that you've set the appropriate server address and port
    cli = new SlimCLI(this, "cli", SqueezeBoxServerAddress, MacAddress, SqueezeBoxServerCLIPort.toInt());
    cli->Init();
    initInterface();
}

void AudioPlayer::shuffleState(int state)
{
    DEBUGF("");
}

void AudioPlayer::nextTrackClicked(void)
{
    cli->SendCommand(QByteArray("button fwd.single\n"));
    DEBUGF("");
}

void AudioPlayer::prevTrackClicked(void)
{
    cli->SendCommand(QByteArray("button rew\n"));
    DEBUGF("");
}

void AudioPlayer::playState(int state)
{
    if (state==0) {   // pause state
        cli->SendCommand(QByteArray("play"));
    }
    else if( state == 1) {   // play state
        cli->SendCommand(QByteArray("pause"));
    }

    DEBUGF("PlayerState" << state)
}

void AudioPlayer::volUp(void)
{
    cli->SendCommand(QByteArray("button volup\n"));
}

void AudioPlayer::volDown(void)
{
    cli->SendCommand(QByteArray("button voldown\n"));
}

void AudioPlayer::controlViewClicked(int idx)
{
    DEBUGF('control view clicked with name:' << idx);
}

void AudioPlayer::controlViewClicked(void)
{
    DEBUGF("control view clicked with name:");
}

void AudioPlayer::initInterface(void)
{
    DEBUGF("");
    loadHomeScreen();
    viewer->setSource(QUrl::fromLocalFile("qml/squeezeliteqml2/squeezeliteqml2main.qml"));

    QQuickItem *v = viewer->rootObject();
    connect(v,SIGNAL(play(int)), this,SLOT(playState(int)));
    connect(v,SIGNAL(nextTrack()), this,SLOT(nextTrackClicked()));
    connect(v,SIGNAL(prevTrack()), this,SLOT(prevTrackClicked()));
    connect(v,SIGNAL(volUp()), this,SLOT(volUp()));
    connect(v,SIGNAL(volDown()), this,SLOT(volDown()));
    connect(v,SIGNAL(controlClicked()), this,SLOT(controlViewClicked()));

    viewer->show();
}

void AudioPlayer::loadHomeScreen(void)
{
      ListModel *model = new ListModel(new ControlListItem, this);
      model->appendRow(new ControlListItem("Music", QSize(30,350), QString("http://127.0.0.1:9000/html/images/artists_40x40.png"), model));
      model->appendRow(new ControlListItem("Internet Radio", QSize(30,350), QString("http://127.0.0.1:9000/plugins/cache/icons/radiomusic_40x40.png"), model));
      model->appendRow(new ControlListItem("My Apps", QSize(30,350), QString("http://127.0.0.1:9000/plugins/AppGallery/html/images/icon_40x40.png"), model));
      model->appendRow(new ControlListItem("Favorites", QSize(30,350), QString("http://127.0.0.1:9000/html/images/favorites_40x40.png"), model));
      model->appendRow(new ControlListItem("Extras", QSize(30,350), QString("http://127.0.0.1:9000/html/images/alarm_40x40.png"), model));

      viewer->rootContext()->setContextProperty("controlListModel", model);
}

void AudioPlayer::getplayerMACAddress( void )
{
    DEBUGF("");
    MacAddress = QByteArray( "00:00:00:00:00:04" );

    QList<QNetworkInterface> netList = QNetworkInterface::allInterfaces();
    QListIterator<QNetworkInterface> i( netList );

    QNetworkInterface t;

    while( i.hasNext() ) {  // note: this grabs the first functional, non-loopback address there is.  It may not the be interface on which you really connect to the slimserver
        t = i.next();
        if( !t.flags().testFlag( QNetworkInterface::IsLoopBack ) &&
                t.flags().testFlag( QNetworkInterface::IsUp ) &&
                t.flags().testFlag( QNetworkInterface::IsRunning ) ) {
            MacAddress = t.hardwareAddress().toLatin1().toLower();
            //            MacAddress = t.hardwareAddress().toAscii().toLower();
            return;
        }
    }
}

