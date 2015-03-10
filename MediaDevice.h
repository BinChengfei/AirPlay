#ifndef MEDIADEVICE_H
#define MEDIADEVICE_H

#include <QIODevice>

class MediaDevice : public QIODevice
{
    Q_OBJECT

public:
    enum PlayState{
        PLAY_STATE_STOP,
        PLAY_STATE_OPENING,
        PLAY_STATE_PLAYING,
        PLAY_STATE_PAUSE
    };

    explicit MediaDevice(QObject *parent = 0);

    inline QString path() const {
        return _path;
    }

    inline QString mediaUrl() const {
        return _mediaUrl;
    }

    inline int mute() const {
        return _mute;
    }

    inline int volume() const {
        return _volume;
    }

    inline qreal volumeFx() const {
        return _volumeFx;
    }

    inline int bass() const {
        return _bass;
    }

    inline int treb() const {
        return _treb;
    }

    inline int eq() const {
        return _eq;
    }

    virtual int playTime();

    inline int state() const {
        return _state;
    }

    inline bool isSequential() const {
        return true;
    }

public slots:
    //NOTE 该接口仅有在volume被设置过，才会立刻生效
    bool setVolumeFx(qreal fx);

    virtual bool setPath(const QString &path);
    virtual bool setMediaUrl(const QString &mediaUrl);
    virtual bool setMute(int m);
    virtual bool setVolume(int v);
    virtual bool setBass(int t);
    virtual bool setTreb(int b);
    virtual bool setEq(int eq);
    virtual bool setPlayTime(int t);

    virtual bool startPlay();
    virtual bool resumePlay();
    virtual bool pausePlay();
    virtual bool stopPlay();

    virtual bool openDevice();
    virtual bool closeDevice();

signals:
    void playStateChanged(int s);
    void playFinished();
    void playErrored(int e);
    void playBuffering(qreal & f);

protected:
    inline qint64 readData(char *, qint64) {
        return -1;
    }

    inline qint64 writeData(const char *, qint64) {
        return -1;
    }

protected:
    QString _path;
    QString _mediaUrl;

    int _mute;
    int _volume;
    qreal _volumeFx;
    int _bass;
    int _treb;
    int _eq;

    int _state;
};

#endif // MEDIADEVICE_H
