#ifndef VLCCALLBACK_H
#define VLCCALLBACK_H

#include "raopcallbacks.h"

class VlcDevice;
class VlcCallBack : public RaopAudioHandler
{
    Q_OBJECT
public:
    explicit VlcCallBack(QObject *parent = 0);
    
    void *audioInit(int bits, int channels, int samplerate);
    void audioProcess(void *session, const QByteArray &buffer);
    void audioDestroy(void *session);

    void audioFlush(void *session);
    void audioSetVolume(void *session, float volume);
    void audioSetMetadata(void *session, const QByteArray & buffer);
    void audioSetCoverart(void *session, const QByteArray & buffer);

private:
    QList<VlcDevice *> _vlcDeviceList;
};

#endif // VLCCALLBACK_H
