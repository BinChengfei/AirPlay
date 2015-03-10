#include "VlcDevice.h"
#include "VlcMemData.h"

#include "Instance.h"
#include "MediaPlayer.h"
#include "Media.h"
#include "Common.h"
#include "Enums.h"
#include "Audio.h"

#include <QDebug>
#include <QTimer>

const QStringList VlcDevice::_vlcArgs = QStringList() << "--intf=dummy"
                                                      << "--ignore-config"
                                                      << "--no-reset-plugins-cache"
                                                      << "--no-media-library"
                                                      << "--no-video"
                                                      << "--no-sout-video"
                                                      << "--no-stats"
                                                      << "--no-loop"
                                                      << "--album-art=0"
                                                      << "--services-discovery=''";

const qreal VlcDevice::_vlcVolumeFx = 100 / 256.0; //200 / 256.0;

VlcDevice::VlcDevice(QObject *parent)
    : MediaDevice(parent),
      _pVlcInstance(0),
      _pVlcMediaPlayer(0),
      _pVlcMedia(0),
      _pVlcMem(0)
{
}

VlcDevice::VlcDevice(const QString& path, QObject *parent)
    : MediaDevice(parent),
      _pVlcInstance(0),
      _pVlcMediaPlayer(0),
      _pVlcMedia(0),
      _pVlcMem(0)
{
    _path = path;
}

VlcDevice::~VlcDevice()
{
    if (isOpen())
        closeDevice();
}

bool VlcDevice::open(OpenMode mode)
{
    if ((mode != QIODevice::NotOpen) && !isOpen()) {
        if (!_path.startsWith("hw:")) {
            qWarning() << "VlcDevice::open() device path is invalid!"
                       << _path;
            return false;
        }

        QIODevice::open(mode);
        QStringList args = VlcDevice::_vlcArgs;
        args << "--alsa-audio-device" << _path;

        qDebug() << "VlcDevice::open() start vlc."
                 << "args =" << args;

        _pVlcInstance = new VlcInstance(args, this);
        _pVlcMediaPlayer = new VlcMediaPlayer(_pVlcInstance);
        connect(_pVlcMediaPlayer, &VlcMediaPlayer::stateChanged, this,  &VlcDevice::vlcStateChange);
        connect(_pVlcMediaPlayer, &VlcMediaPlayer::buffering, this, &VlcDevice::vlcBuffering);
        connect(_pVlcMediaPlayer, &VlcMediaPlayer::end, this, &VlcDevice::vlcEnd);
        connect(_pVlcMediaPlayer, &VlcMediaPlayer::error, this,  &VlcDevice::vlcError);
        connect(_pVlcMediaPlayer, &VlcMediaPlayer::opening, this, &VlcDevice::vlcOpening);
        connect(_pVlcMediaPlayer, &VlcMediaPlayer::paused, this,  &VlcDevice::vlcPaused);
        connect(_pVlcMediaPlayer, &VlcMediaPlayer::playing, this, &VlcDevice::vlcPlaying);
        connect(_pVlcMediaPlayer, &VlcMediaPlayer::stopped, this, &VlcDevice::vlcStoped);

        _state = PLAY_STATE_STOP;


        qDebug() << "VlcDevice::open()";
    }

    return true;
}

void VlcDevice::setMemData(VlcMemData *pData)
{
    if (!pData) {
        qWarning() << "VlcDevice::setMemData()  invalid arg!";
        return ;
    }

    delete _pVlcMem;
    _pVlcMem = pData;
}

void VlcDevice::close()
{
    if (isOpen()) {
        QIODevice::close();

        if (state() != PLAY_STATE_STOP)
            stopPlay();

        if (_pVlcMedia)
            _pVlcMedia->deleteLater();
        _pVlcMedia = 0;
        _pVlcMediaPlayer->stop();
        _pVlcMediaPlayer->deleteLater();
        _pVlcMediaPlayer = 0;
        _pVlcInstance->deleteLater();
        _pVlcInstance = 0;


        qDebug() << "VlcDevice::close()";
    }
}

void VlcDevice::vlcStateChange()
{
        int vlcState = isOpen() ? _pVlcMediaPlayer->state() : -1;
        qDebug() << "VlcDevice::vlcStateChange()."
                 << vlcState;
}

void VlcDevice::vlcBuffering(qreal f)
{
        qDebug() << "VlcDevice::vlcBuffering()." << f;

    //FIXME
    //    if (isOpen() && (_state != PLAY_STATE_BUFFERING)) {
    //        _state = PLAY_STATE_BUFFERING;
    //        emit playStateChanged(_state);
    //    }
    emit playBuffering(f);

    //    if (qAbs(f - 100.0) < 1e(-6)) {
    //        _state = PLAY_STATE_PLAYING;
    //        emit playStateChanged(_state);
    //    }
}

void VlcDevice::vlcEnd()
{

    qDebug() << "VlcDevice::vlcEnd()!";

    if (isOpen())
        emit playFinished();
}

void VlcDevice::vlcError()
{

    qDebug() << "VlcDevice::vlcError()!";

    if (isOpen())
        emit playErrored(-1);
}

void VlcDevice::vlcOpening()
{

    qDebug() << "VlcDevice::vlcOpening()!";

    if (isOpen() && (_state != PLAY_STATE_OPENING)) {
        _state = PLAY_STATE_OPENING;
        emit playStateChanged(_state);
    }
}

void VlcDevice::vlcPaused()
{

    qDebug() << "VlcDevice::vlcPaused()!";

    if (isOpen() && (_state != PLAY_STATE_PAUSE)) {
        _state = PLAY_STATE_PAUSE;
        emit playStateChanged(_state);
    }
}

void  VlcDevice::vlcPlaying()
{
    qDebug() << "VlcDevice::vlcPlaying()!";

    if (isOpen() && (_state !=  PLAY_STATE_PLAYING)) {
        _state =  PLAY_STATE_PLAYING;
        emit playStateChanged(_state);
    }
}

void  VlcDevice::vlcStoped()
{

    qDebug() << "VlcDevice::vlcStoped()!";

    if (isOpen()) {
        //        stopPlay();
        //        _state = PLAY_STATE_STOP;
        //        emit playStateChanged(_state);
    }
}

bool VlcDevice::setMute(int m)
{
    if (!isOpen()) {
        _mute = m;
        return true;
    }

    VlcAudio *pAudio = _pVlcMediaPlayer->audio();
    if (pAudio->getMute() != static_cast<bool>(m)) {
        pAudio->toggleMute();
        _mute = pAudio->getMute();
    }


    qDebug() << "VlcDevice::setMute()." << _mute;

    return true;
}

bool VlcDevice::setVolume(int v)
{
    int tmpVolume = qBound(0, v, 256);

    if (!isOpen()) {
        _volume = tmpVolume;
        return true;
    }

    VlcAudio *pAudio = _pVlcMediaPlayer->audio();
    pAudio->setVolume(tmpVolume  *  _vlcVolumeFx);
    _volume = tmpVolume;


    qDebug() << "VlcDevice::setVolume()." << _volume;

    return true;
}

bool VlcDevice::setTreb(int t)
{
    _treb =  qBound(0, t, 15);


    qDebug() << "VlcDevice::setTreb()." << _treb;

    return true;
}

bool VlcDevice::setBass(int b)
{
    _bass = qBound(0, b, 15);


    qDebug() << "VlcDevice::setBass()." << _bass;

    return true;
}

bool VlcDevice::setEq(int eq)
{
    _eq = eq;


    qDebug() << "VlcDevice::setEq()." << _eq;

    return true;
}

int VlcDevice::playTime()
{
    if (!isOpen())
        return 0;

    return  _pVlcMediaPlayer->time() / 1000;
}

bool VlcDevice::setPlayTime(int t)
{
    if (!isOpen())
        return  false;

    _pVlcMediaPlayer->setTime(t * 1000);


    qDebug() << "VlcDevice::setPlayTime()." << t;

    return true;
}

bool VlcDevice::startPlay()
{
    if (!isOpen()) {

        qWarning() << "VlcDevice::startPlay(). not open!";

        return false;
    }

    if (state() != PLAY_STATE_STOP)
        return true;


    qDebug() << "VlcDevice::startPlay().";

    if (_pVlcMedia)
        _pVlcMedia->deleteLater();

    _pVlcMedia = new VlcMedia(_mediaUrl, _mediaUrl.startsWith('/'), _pVlcInstance);
    if (_addVlcArg.size())
        _pVlcMedia->setOptions(_addVlcArg);
    if (_pVlcMem)
        _pVlcMem->attachToMedia(_pVlcMedia);

    if (_mute >= 0)
        setMute(_mute);
    if (_bass >= 0)
        setBass(_bass);
    if (_treb >= 0)
        setTreb(_treb);
    if (_eq >= 0)
        setEq(_eq);
    if (_volume >= 0)
        setVolume(_volume);

    _pVlcMediaPlayer->open(_pVlcMedia);
    _state =  PLAY_STATE_OPENING;

    emit playStateChanged(_state);

    return true;
}

bool VlcDevice::pausePlay()
{
    if (!isOpen() || (state() == PLAY_STATE_STOP)) {
        qWarning() << "VlcDevice::pausePlay() not open or not play!";
        return false;
    }

    if (state() == PLAY_STATE_PAUSE)
        return true;

    _pVlcMediaPlayer->pause();
    _state = PLAY_STATE_PAUSE;

    emit playStateChanged(_state);


    qDebug() << "VlcDevice::pausePlay().";

    return true;
}

bool VlcDevice::resumePlay()
{
    if (!isOpen() || (state() == PLAY_STATE_STOP)) {
        qWarning() << "VlcDevice::resumePlay() not open or not play!";
        return false;
    }

    if (state() == PLAY_STATE_PLAYING)
        return true;

    _pVlcMediaPlayer->play();
    _state = PLAY_STATE_PLAYING;

    emit playStateChanged(_state);


    qDebug() << "VlcDevice::resumePlay().";

    return true;
}

bool VlcDevice::stopPlay()
{
    if (!isOpen())
        return false;

    if (state() == PLAY_STATE_STOP)
        return true;

    _pVlcMediaPlayer->stop();
    _state = PLAY_STATE_STOP;

    emit playStateChanged(_state);


    qDebug() << "VlcDevice::stopPlay().";

    return true;
}
