#include "Uart.h"
#include <QDebug>
#include <QThread>

Uart::Uart()
{
}

Uart::~Uart()
{
    port->close();
    port->deleteLater();
}

void Uart::init()
{
    //port->setPortName("/dev/ttyTHS0");
    port->setBaudRate(QSerialPort::Baud115200);
    port->setDataBits(QSerialPort::Data8);
    port->setStopBits(QSerialPort::OneStop);
    //port->setParity(QSerialPort::OddParity);

    connect(port, SIGNAL(readyRead()), this, SLOT(handle_data()), Qt::DirectConnection);


    // QThread* taskThread = new QThread();
    // connect(taskThread, SIGNAL(started()), this, SLOT(scheduleTask()), Qt::QueuedConnection);
    // connect(taskThread, SIGNAL(finished()), this, SLOT(deleteLater()));
    // taskThread->start();

    openUartHandler("COM5");
}


//数据接收槽函数
void Uart::handle_data()
{
    QByteArray data = port->readAll();
    qDebug() << "receive : " << data.toHex();
    //on_receive(data);

    if (data[0] != 0x43) {
        return;
    }
    switch (data[2]) {
    case 0x00:

        break;
    case 0x01:

        break;
    case 0x02:

        break;
    case 0x03:

        break;
    case 0x04:

        break;
    case 0x40:
        qDebug() << "timeout : " << data.toHex()[3];

        switch (data[3]) {

        case 0x30:
            ThreadManager::getInstance().edfaStatus[0].timeout = 0;
            ThreadManager::getInstance().refreshEdfaCAStatus();
            break;

        case 0x40:
            ThreadManager::getInstance().edfaStatus[1].timeout = 0;
            ThreadManager::getInstance().refreshEdfaCBStatus();



            break;
        case 0x50:
            ThreadManager::getInstance().edfaStatus[2].timeout = 0;
            ThreadManager::getInstance().refreshEdfaLStatus();

            break;
        default:
            break;
        }


        break;
    case 0x41:
        edfaDatahandler(data);

        break;
    case 0x50:

        break;
    default:
        break;
    }
}

void Uart::edfaDatahandler(QByteArray data) {
    ThreadManager * manager = &ThreadManager::getInstance();
    //qDebug() << "temperature :" << data.toHex();
    QVector<EdfaStatus> vector = manager->edfaStatus;

    switch (data[3]) {

    case 0x30:

        vector[0].power = ((uchar)data[4] * 256 + (uchar)data[5]) *0.1 - 70;
        vector[0].temperature = ((uchar)data[6]*256 + (uchar)data[7]) * 0.1;

        vector[0].current = ((uchar)data[8]*256 + (uchar)data[9]) * 0.1;
        vector[0].mode = (uchar)data[10];
        vector[0].timeout = 1;

        manager->refreshEDFAStatus();
        break;

    case 0x40:
        vector[1].power = ((uchar)data[4] * 256 + (uchar)data[5]) *0.1 - 70;
        vector[1].temperature = ((uchar)data[6]*256 + (uchar)data[7]) * 0.1;
        vector[1].current = ((uchar)data[8]*256 + (uchar)data[9]) * 0.1;
        vector[1].mode = (uchar)data[10];
        vector[1].timeout = 1;

        manager->refreshEDFAStatus();

        break;
    case 0x50:
        manager->_edfaLData.power = ((uchar)data[4] * 256 + (uchar)data[5]) *0.1 - 70;
        manager->_edfaLData.temperature = ((uchar)data[6]*256 + (uchar)data[7]) * 0.1;
        manager->_edfaLData.current = ((uchar)data[8]*256 + (uchar)data[9]) * 0.1;
        manager->_edfaLData.mode = (uchar)data[10];
        manager->_edfaLData.timeout = 1;

        manager->refreshEDFAStatus();
        break;
    default:
        break;
    }

}

void Uart::doWork()
{
    port = new QSerialPort();
    connect(&ThreadManager::getInstance(), SIGNAL(requestUartMessage(int)), this, SLOT(sendUartMessageHandler(int)), Qt::QueuedConnection);
    init();
    qDebug() << "init uart";
    connect(&ThreadManager::getInstance(), SIGNAL(openUart(QString)), this, SLOT(openUartHandler(QString)), Qt::QueuedConnection);

}

char BCDtoUINT (char p) {
    return ((p>>4)*10 + (p&0x0f));
}


void Uart::openUartHandler(QString name) {
    port->setPortName(name);
        if (port->open(QIODevice::ReadWrite)) {
            qDebug() << "Port have been opened";
            ask_EdfaCAHeart();
            ask_EdfaCBHeart();
            ask_EdfaLHeart();
            ask_STMHeart();
        } else {
            qDebug() << "open it failed";
        }

}


void Uart::scheduleTask() {
    while (port->isOpen()) {}
    ask_STMHeart();
    QThread::sleep(500);
}
uchar checkByteArray(QByteArray data, int start, int end) {
    int sum = 0;
    for (int i = start; i <= end; i++) {
        sum += data[i];
    }
    return (uchar) (sum % 256);
}

void Uart::ask_STMHeart() {
    QByteArray data;
    data.resize(5);
    data[0] = 0x42;
    data[1] = 0x01;
    data[2] = 0x00;
    data[3] = 0x00;
    data[4] = 0x24;

    port->write(data, 5);
}
void Uart::ask_EdfaCAHeart() {
    QByteArray data;
    data.resize(5);
    data[0] = 0x42;
    data[1] = 0x01;
    data[2] = 0x02;
    data[3] = 0x00;
    data[4] = 0x24;

    port->write(data, 5);

}
void Uart::ask_EdfaCBHeart() {
    QByteArray data;
    data.resize(5);
    data[0] = 0x42;
    data[1] = 0x01;
    data[2] = 0x03;
    data[3] = 0x00;
    data[4] = 0x24;

    port->write(data, 5);

}
void Uart::ask_EdfaLHeart() {
    QByteArray data;
    data.resize(5);
    data[0] = 0x42;
    data[1] = 0x01;
    data[2] = 0x04;
    data[3] = 0x00;
    data[4] = 0x24;

    port->write(data, 5);

}

void Uart::ask_EdfaCAParamSet() {
    QByteArray data;
    data.resize(10);
    data[0] = 0x42;
    data[1] = 0x08;
    data[2] = 0x30;
    data[3] = (uchar) ThreadManager::getInstance().edfaSettings[0].mode;
    data[4] = (uchar) (ThreadManager::getInstance().edfaSettings[0].current/10);
    data[5] = (uchar) (ThreadManager::getInstance().edfaSettings[0].current%10);
    data[6] = (uchar) (ThreadManager::getInstance().edfaSettings[0].power/10);
    data[7] = (uchar) (ThreadManager::getInstance().edfaSettings[0].power%10);
    data[8] = checkByteArray(data, 3, 7);
    data[9] = 0x24;
}
void Uart::ask_EdfaCBParamSet() {
    QByteArray data;
    data.resize(10);
    data[0] = 0x42;
    data[1] = 0x08;
    data[2] = 0x40;
    data[3] = (uchar) ThreadManager::getInstance()._edfaCAData.setMode;
    data[4] = (uchar) (ThreadManager::getInstance()._edfaCAData.setCurrent/10);
    data[5] = (uchar) (ThreadManager::getInstance()._edfaCAData.setCurrent%10);
    data[6] = (uchar) (ThreadManager::getInstance()._edfaCAData.setPower/10);
    data[7] = (uchar) (ThreadManager::getInstance()._edfaCAData.setPower%10);
    data[8] = checkByteArray(data, 3, 7);
    data[9] = 0x24;

}
void Uart::ask_EdfaLParamSet() {
    QByteArray data;
    data.resize(10);
    data[0] = 0x42;
    data[1] = 0x08;
    data[2] = 0x50;
    data[3] = (uchar) ThreadManager::getInstance()._edfaCAData.setMode;
    data[4] = (uchar) (ThreadManager::getInstance()._edfaCAData.setCurrent/10);
    data[5] = (uchar) (ThreadManager::getInstance()._edfaCAData.setCurrent%10);
    data[6] = (uchar) (ThreadManager::getInstance()._edfaCAData.setPower/10);
    data[7] = (uchar) (ThreadManager::getInstance()._edfaCAData.setPower%10);
    data[8] = checkByteArray(data, 3, 7);
    data[9] = 0x24;

}



//void Uart::ask_setEdfaParam(uchar id) {
//    QByteArray data;
//    data.resize(10);
//    data[0] = 0x42;
//    data[1] = 0x08;
//    data[2] = 0x30;
//    data[3] = (uchar) ThreadManager::getInstance()._edfaCAData.setMode;
//    data[4] = (uchar) (ThreadManager::getInstance()._edfaCAData.setCurrent/10);
//    data[5] = (uchar) (ThreadManager::getInstance()._edfaCAData.setCurrent%10);
//    data[6] = (uchar) (ThreadManager::getInstance()._edfaCAData.setPower/10);
//    data[7] = (uchar) (ThreadManager::getInstance()._edfaCAData.setPower%10);
//    data[8] = checkByteArray(data, 3, 7);
//    data[9] = 0x24;

//    port->write(data, 10);
//    qDebug() << "sned data: " << data;
//}

void Uart::sendUartMessageHandler(int id) {
    qDebug() << "request uart";
    switch (id) {
    case 1:
        //ask_setEdfaParam(1);
        break;
    default:
        break;
    }

}

