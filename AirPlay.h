#ifndef AIRPLAY_H
#define AIRPLAY_H

#include "raopservice.h"
#include "dnssdservice.h"
#include "VlcCallBack.h"

#include <QObject>

class AirPlay : public QObject
{
    Q_OBJECT
public:
    explicit AirPlay(QObject *parent = 0);
    ~AirPlay();

    inline bool isRunning() const {
        return _running;
    }
    
public slots:
    inline bool start(){
        return start("AirPlay", 5000,  1,  "");
    }
    bool start(const QString& name, quint16 port,  int maxClient, const QByteArray& mac);
    void stop();

private:
    RaopService *_pRaopService;
    DnssdService *_pDnssdService;
    VlcCallBack _callbacks;

    bool _running;
};

#endif // AIRPLAY_H
