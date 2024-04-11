#ifndef MYQSP_H
#define MYQSP_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class myQSP : public QSerialPort
{
    Q_OBJECT
public:
    explicit myQSP(QObject *parent = nullptr);
    void handleScan();
    int handleConnect(QString tgtCOM, int iBaudRate);
    int handleSend(QByteArray tst);
    int handleIsOpen();//串口已打开是1，没开返回0
    QStringList qslSerialPortName;
    QByteArray qbaAllRead;

signals:
    void handleRecvCplt();

public slots:
    void handleRecv();

};

#endif // MYQSP_H
