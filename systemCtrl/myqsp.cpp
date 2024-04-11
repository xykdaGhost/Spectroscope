#include "myqsp.h"

myQSP::myQSP(QObject *parent) : QSerialPort(parent)
{

}

void myQSP::handleScan()
{
    qslSerialPortName.clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qslSerialPortName << info.portName();
        //qDebug()<<"serialPortName:"<<info.portName();
    }
}
int myQSP::handleIsOpen(){
    //串口已打开是1，没开返回0
    if(this->isOpen())
        return 1;
    else
        return 0;
}
int myQSP::handleConnect(QString tgtCOM, int iBaudRate){
    if(this->isOpen())
    {
        this->clear();
        this->close();
        return 0;
        //closed
    }
    else if(tgtCOM != nullptr)
    {
        this->setPortName(tgtCOM);
        if(!this->open(QIODevice::ReadWrite))
        {
            //failed to open
            return -1;
        }
        else
        {
            //设置波特率和读写方向QSerialPort::Baud115200
            this->setBaudRate(iBaudRate, QSerialPort::AllDirections);
            this->setDataBits(QSerialPort::Data8);		//数据位为8位
            this->setFlowControl(QSerialPort::NoFlowControl);//无流控制
            this->setParity(QSerialPort::NoParity);	//无校验位
            this->setStopBits(QSerialPort::OneStop); //一位停止位
            return 1;
        //successfully opened
        }
    }
    else {
        //
        return -2;
    }
}

int myQSP::handleSend(QByteArray tst){
    if(tst.size() != 0){
        this->write(tst);
    }
    return tst.size();
}

void myQSP::handleRecv(){
    qbaAllRead.clear();
    qbaAllRead = this->readAll();
    emit handleRecvCplt();
    //return qbaReadAll;
}
