#include "MediaDevice.h"

#include <QDebug>

MediaDevice::MediaDevice(QObject *parent)
    : QIODevice(parent),
      _mute(0),
      _volume(160),
      _volumeFx(1.0),
      _bass(7),
      _treb(7),
      _eq(0),
      _state(PLAY_STATE_STOP)
{
}

bool MediaDevice::setPath(const QString &path)
{
    if (path.isEmpty()) {
        qWarning() << "MediaDevice::setPath(), path is empty";
        return false;
    }

    if (isOpen()) {
        closeDevice();
        _path = path;


            qDebug() << "MediaDevice::setPath()." << path;

        return openDevice();
    } else {
        _path = path;


            qDebug() << "MediaDevice::setPath()." << path;

        return true;
    }
}

bool MediaDevice::setMediaUrl(const QString &mediaUrl)
{
    if (mediaUrl.isEmpty()) {
        qWarning() << "MediaDevice::setMediaUrl() faile! mediaUrl is empty";
        return false;
    }

    if (isOpen()) {
        closeDevice();
        _mediaUrl = mediaUrl;


            qDebug() << "MediaDevice::setMediaUrl()." << _mediaUrl;

        return openDevice();
    } else {
        _mediaUrl = mediaUrl;


            qDebug() << "MediaDevice::setMediaUrl()." << _mediaUrl;

        return true;
    }
}


bool MediaDevice::setMute(int)
{
    return  false;
}

bool MediaDevice::setVolume(int)
{
    return  false;
}

bool MediaDevice::setVolumeFx(qreal fx)
{
    _volumeFx = qBound(qreal(0.0), fx, qreal(1.0));
    if (_volume >= 0)
        return setVolume(_volume);
    else
        return true;
}

bool MediaDevice::setBass(int)
{
    return  false;
}

bool MediaDevice::setTreb(int)
{
    return  false;
}

bool MediaDevice::setEq(int)
{
    return  false;
}

int MediaDevice::playTime()
{
    return  0;
}

bool MediaDevice::setPlayTime(int)
{
    return  false;
}

bool MediaDevice::startPlay()
{
    return  false;
}

bool MediaDevice::pausePlay()
{
    return  false;
}

bool MediaDevice::resumePlay()
{
    return  false;
}

bool MediaDevice::stopPlay()
{
    return  false;
}

bool MediaDevice::openDevice()
{
    return isOpen() ? true : open(QIODevice::ReadWrite);
}

bool MediaDevice::closeDevice()
{
    if (isOpen())
        close();

    return true;
}
