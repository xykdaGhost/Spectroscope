#include "threadmanager.h"

ThreadManager::ThreadManager() {
    edfaStatus.resize(3);
    edfaSettings.resize(3);

}

ThreadManager::~ThreadManager()
{

}

void ThreadManager::doWork()
{

    edfaSettings[0].current = 500;
    edfaSettings[0].maxCurrent = 5000;
    edfaSettings[0].mode = 2;
    edfaSettings[0].power = 150;

    edfaSettings[1].current = 500;
    edfaSettings[1].maxCurrent = 5000;
    edfaSettings[1].mode = 2;
    edfaSettings[1].power = 150;

    edfaSettings[2].current = 500;
    edfaSettings[2].maxCurrent = 5000;
    edfaSettings[2].mode = 2;
    edfaSettings[2].power = 230;

    edfaStatus[0].timeout = 1;
    edfaStatus[1].timeout = 1;
    edfaStatus[2].timeout = 1;
    edfaStatus[0].current = 150.0;
    edfaStatus[1].current = 150.0;
    edfaStatus[2].current = 150.0;
    edfaStatus[0].power = 15.0;
    edfaStatus[1].power = 15.0;
    edfaStatus[2].power = 23.0;
    edfaStatus[0].temperature = 25.6;
    edfaStatus[1].temperature = 35.6;
    edfaStatus[0].mode = 2;
    edfaStatus[1].mode = 2;
    edfaStatus[2].mode = 2;

//    QThread* udpThread = new QThread();
//    my_udp = &UDP::getInstance();
//    my_udp->moveToThread(udpThread);
//    connect(udpThread, SIGNAL(started()), my_udp, SLOT(udpDoWork()));
//    connect(udpThread, SIGNAL(finished()), udpThread, SLOT(deleteLater()));
//    udpThread->start();
}


void ThreadManager::udpDoWork()
{

}
void ThreadManager::requestUart(int id) {
    requestUartMessage(id);
}

void ThreadManager::requestOpenUartPort(QString name) {
    openUart(name);
}
