#ifndef VLCMEMDATA_H
#define VLCMEMDATA_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QStringList>

class VlcMedia;

class VlcMemData : public QObject
{
    Q_OBJECT

public:
    VlcMemData(QObject *parent = 0);
    ~VlcMemData();

    inline void addArg(const QString& arg)  {
        _argList.push_back(arg);
    }

    void attachToMedia(VlcMedia *pMedia);

    void lock();
    void unlock();

    static int readCallback(void *data, const char *cookie,
                            int64_t *dts, int64_t *pts, unsigned *flags, // krazy:exclude=typedefs
                            size_t *bufferSize, void **buffer);

    static int readDoneCallback(void *data, const char *cookie,
                                size_t bufferSize, void *buffer);

    quint64 currentBufferSize() const;
    void writeData(const QByteArray &data);
    quint64 currentPos() const;

    bool read(quint64 offset, int *length, char *buffer);

    void endOfData();
    void setStreamSize(qint64 newSize);
    qint64 streamSize() const;

    void reset();

protected:
    QByteArray _buffer;
    quint64 _pos;
    quint64 _size;
    bool _eos;
    bool _unlocked;
    QMutex _mutex;
    QWaitCondition _waitingForData;

    QStringList _argList;
};

#endif // VLCMEMDATA_H
