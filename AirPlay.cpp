#include "AirPlay.h"

#include <QDebug>

AirPlay::AirPlay(QObject *parent) :
    QObject(parent),
    _pRaopService(new RaopService(this)),
    _pDnssdService(new DnssdService(this)),
    _running(false)
{
}

bool AirPlay::start(const QString& name, quint16 port, int maxClient, const QByteArray& mac)
{
    if(_running)
        return true;

    // Initialize the service
    bool initSuccess = false;
    initSuccess = _pRaopService->init(maxClient, &_callbacks);
    if(!initSuccess) {
        qWarning() << __FUNCTION__<< "Error initializing raop service";
        return false;
    }
    initSuccess &= _pDnssdService->init();
    if(!initSuccess) {
        qWarning() << __FUNCTION__<< "Error initializing dnssd service";
        return false;
    }

    static const char defaultMac[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB };
    _pRaopService->start(port, mac.isEmpty() ? QByteArray(defaultMac, sizeof(defaultMac)) : mac);
    _pDnssdService->registerRaop(name, port, mac.isEmpty() ? QByteArray(defaultMac, sizeof(defaultMac)) : mac);

    qDebug()<<__FUNCTION__;

    return true;
}

void AirPlay::stop()
{
    if(!_running)
        return;

    _pDnssdService->unregisterRaop();
    _pRaopService->stop();

    qDebug()<<__FUNCTION__;
}

AirPlay::~AirPlay()
{
    stop();
}
