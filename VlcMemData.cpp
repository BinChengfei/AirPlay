#include "VlcMemData.h"
#include "Media.h"

#include <QMutexLocker>
#include <QDebug>

#include  <stdint.h>

#define INTPTR_PTR(x) reinterpret_cast<intptr_t>(x)
#define INTPTR_FUNC(x) reinterpret_cast<intptr_t>(&x)
#define BLOCKSIZE 256

VlcMemData::VlcMemData(QObject *parent)
    : QObject(parent)
    , _pos(0)
    , _size(0)
    , _eos(false)
    , _unlocked(false)
{
}

VlcMemData::~VlcMemData()
{
}

void VlcMemData::reset()
{
    lock();
    _buffer.clear();
    _pos = 0;
    _size = 0;
    _eos = false;
    _unlocked = false;
}

void VlcMemData::attachToMedia(VlcMedia *pMedia)
{
    if (!pMedia) {
        qWarning() << "VlcMemData::attachToMedia() invalid arg!";
        return;
    }

    lock(); // Make sure we can lock in read().

    pMedia->setOption(QString(":imem-cat=4"));
    pMedia->setOption(QString(":imem-data=") + QString::number(static_cast<qint64>(INTPTR_PTR(this))));
    pMedia->setOption(QString(":imem-get=") + QString::number(static_cast<qint64>(INTPTR_FUNC(readCallback))));
    pMedia->setOption(QString(":imem-release=") + QString::number(static_cast<qint64>(INTPTR_FUNC(readDoneCallback))));

    if(_argList.size())
        pMedia->setOptions(_argList);

    // if stream has known size, we may pass it
    // imem module will use it and pass it to demux
    if (streamSize() > 0)
        pMedia->setOption(QString(":imem-size=%1").arg(streamSize()));
}

void VlcMemData::lock()
{
    QMutexLocker lock(&_mutex);
    _unlocked = false;
}

void VlcMemData::unlock()
{
    QMutexLocker lock(&_mutex);
    _unlocked = true;
    _waitingForData.wakeAll();
}

int VlcMemData::readCallback(void *data, const char *cookie,
                             int64_t *dts, int64_t *pts, unsigned *flags, // krazy:exclude=typedefs
                             size_t *bufferSize, void **buffer)
{
    Q_UNUSED(cookie);
    Q_UNUSED(dts);
    Q_UNUSED(pts);
    Q_UNUSED(flags);

    VlcMemData *that = static_cast<VlcMemData *>(data);
    size_t length = BLOCKSIZE;
    *buffer = new char[length];
    int size = length;
    bool ret = that->read(that->currentPos(), &size, static_cast<char*>(*buffer));
    *bufferSize = static_cast<size_t>(size);

    return ret ? 0 : -1;
}

int VlcMemData::readDoneCallback(void *data, const char *cookie,
                                 size_t bufferSize, void *buffer)
{
    Q_UNUSED(data);
    Q_UNUSED(cookie);
    Q_UNUSED(bufferSize);
    delete static_cast<char *>(buffer);
    return 0;
}

quint64 VlcMemData::currentBufferSize() const
{
    return _buffer.size();
}

bool VlcMemData::read(quint64 pos, int *length, char *buffer)
{
    QMutexLocker lock(&_mutex);
    bool ret = true;

    if (_unlocked)
        return ret;

    if (currentPos() != pos)
        return false;

    if (_buffer.capacity() < *length)
        _buffer.reserve(*length);

    while (currentBufferSize() < static_cast<unsigned int>(*length)) {
        quint64 oldSize = currentBufferSize();
        _waitingForData.wait(&_mutex);

        if (oldSize == currentBufferSize()) {
            if (_eos && _buffer.isEmpty())
                return false;

            // We didn't get any more data
            *length = static_cast<int>(oldSize);
            // If we have some data to return, why tell to reader that we failed?
            // Remember that length argument is more like maxSize not requiredSize
            ret = true;
        }
    }

    memcpy(buffer, _buffer.data(), *length);
    _pos += *length;
    // trim the buffer by the amount read
    _buffer = _buffer.mid(*length);

    return ret;
}

void VlcMemData::endOfData()
{
    _eos = true;
    _waitingForData.wakeAll();
}

void VlcMemData::writeData(const QByteArray &data)
{
    QMutexLocker lock(&_mutex);
    _buffer.append(data);
    _waitingForData.wakeAll();
}

quint64 VlcMemData::currentPos() const
{
    return _pos;
}

void VlcMemData::setStreamSize(qint64 newSize)
{
    _size = newSize;
}

qint64 VlcMemData::streamSize() const
{
    return _size;
}
