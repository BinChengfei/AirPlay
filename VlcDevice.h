#ifndef VLCDEVICE_H
#define VLCDEVICE_H

#include "MediaDevice.h"
#include "Enums.h"

#include <QProcess>

class VlcInstance;
class VlcMediaPlayer;
class VlcMedia;
class VlcMemData;

//vlc thread设备
class VlcDevice : public MediaDevice
{
    Q_OBJECT

public:
    explicit VlcDevice(QObject *parent = 0);
    explicit VlcDevice(const QString& path, QObject *parent = 0);
    ~VlcDevice();

    bool setMute(int m);
    bool setVolume(int v);
    bool setTreb(int t);
    bool setBass(int b);
    bool setEq(int eq);
    int playTime();
    bool setPlayTime(int t);

    bool startPlay();
    bool resumePlay();
    bool pausePlay();
    bool stopPlay();

    bool open(OpenMode mode);
    void close();

    inline QStringList addVlcArg() const {
        return _addVlcArg;
    }
    inline void setAddVlcArg(const QStringList & arg) {
        _addVlcArg = arg;
    }
    inline void addVlcArg(const QString & arg) {
        _addVlcArg.push_back(arg);
    }

    inline VlcMemData* vlcData() const {
        return _pVlcMem;
    }

public slots:
    void setMemData(VlcMemData *pData);

protected slots:
    void vlcStateChange();
    void vlcBuffering(qreal f);
    void vlcEnd();
    void vlcError();
    void vlcOpening();
    void vlcPaused();
    void vlcPlaying();
    void vlcStoped();

protected:
    VlcInstance *_pVlcInstance;
    VlcMediaPlayer *_pVlcMediaPlayer;
    VlcMedia *_pVlcMedia;
    VlcMemData *_pVlcMem;
    QStringList _addVlcArg;

    static const QStringList _vlcArgs;
    static const qreal _vlcVolumeFx;
};

#endif // VLCDEVICE_H
