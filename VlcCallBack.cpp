#include "VlcCallBack.h"
#include "VlcDevice.h"
#include "VlcMemData.h"

#include <QDebug>
#include <QtEndian>

VlcCallBack::VlcCallBack(QObject *parent) :
    RaopAudioHandler(parent)
{
}

void * VlcCallBack::audioInit(int, int channels, int samplerate)
{
    qDebug()<<__FUNCTION__;

    VlcDevice *pVlcDevice = new VlcDevice("hw:0,0");
    pVlcDevice->setAddVlcArg( QStringList() << ":demux=rawaud"
                              << QString(":rawaud-channels=%1").arg(channels)
                              << QString(":rawaud-samplerate=%1").arg(samplerate));

    VlcMemData *pMemData = new VlcMemData;
    //    pMemData->addArg(QString(":imem-codec=pcm"));
    //    pMemData->addArg(QString(":imem-channels=%1").arg(channels));
    //    pMemData->addArg(QString(":imem-samplerate=%1").arg(samplerate));

    pVlcDevice->setMemData(pMemData);
    pVlcDevice->setMediaUrl("imem://");
    pVlcDevice->open(QIODevice::ReadOnly);
    pVlcDevice->startPlay();

    _vlcDeviceList.append(pVlcDevice);

    return pVlcDevice;
}

void VlcCallBack::audioProcess(void *session, const QByteArray & buffer)
{
    //qDebug()<<__FUNCTION__;
    VlcDevice *pVlcDevice = (VlcDevice*)session;
    if(pVlcDevice && pVlcDevice->vlcData())
        pVlcDevice->vlcData()->writeData(buffer);
}

void VlcCallBack::audioDestroy(void *session)
{
    qDebug()<<__FUNCTION__;
    VlcDevice *pVlcDevice = (VlcDevice*)session;
    if(pVlcDevice) {
        _vlcDeviceList.removeAll(pVlcDevice);
        pVlcDevice->stopPlay();
        delete pVlcDevice;
    }
}

void VlcCallBack::audioFlush(void *)
{
    qDebug()<<__FUNCTION__;
}

void VlcCallBack::audioSetVolume(void *session, float volume)
{
    qDebug()<<__FUNCTION__<<volume;

    VlcDevice *pVlcDevice = (VlcDevice*)session;
    if(pVlcDevice) {
        float volPercent = volume < -30.0 ? 0 : 1 - volume/-30.0;//pow(10.0,0.05*volume);
        pVlcDevice->setVolume(volPercent * 255);
    }
}

//parse daap metadata - thx to project MythTV
static QMap<QString, QString> decodeDMAP(const QByteArray &buffer)
{
    QMap<QString, QString> result;
    quint32 offset = 8;
    while (offset < buffer.size())
    {
        QString tag;
        tag.append(buffer.mid(offset, 4));
        offset += 4;
        quint32 length = qFromBigEndian<quint32>(*(quint32 *)(buffer.constData() + offset));
        offset += sizeof(quint32);
        QString content;
        content.append(QString::fromUtf8(buffer.mid(offset, length)));
        offset += length;
        result[tag] = content;
    }
    return result;
}

void VlcCallBack::audioSetMetadata(void *, const QByteArray &buffer)
{
    qDebug()<<__FUNCTION__;

    QMap<QString, QString> metadata = decodeDMAP(buffer);

    //    if(metadata["asal"].length())
    //      tag.SetAlbum(metadata["asal"]);//album
    //    if(metadata["minm"].length())
    //      tag.SetTitle(metadata["minm"]);//title
    //    if(metadata["asar"].length())
    //      tag.SetArtist(metadata["asar"]);//artist

    qDebug()<<metadata;
}

void VlcCallBack::audioSetCoverart(void *, const QByteArray &buffer)
{
    qDebug()<<__FUNCTION__;

    //QImage::fromData(buffer, "jpg").save("/tmp/coverart.jpg");
    //qDebug()<<buffer;
}
