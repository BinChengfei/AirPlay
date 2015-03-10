 #include "qtsinglecoreapplication.h"
#include "AirPlay.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QtSingleCoreApplication app(argc, argv);
    if (app.isRunning()) {
        qWarning()<<"AirPlay already is running!";
        return 0;
    }

    AirPlay airPlay;
    if(airPlay.start()) {
        return app.exec();
    }else {
        qWarning()<<"AirPlay start faile!";
        return 1;
    }
}
