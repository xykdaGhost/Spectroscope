#ifndef UDP_H
#define UDP_H
#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

#include <QDebug>

class UDP : public QObject
{
    Q_OBJECT
public:

    static UDP& getInstance() {
        static UDP udp;
        return udp;
    }
    void InitSocket();//初始化套接字


public slots:
    void ReadPendingDataframs();//读取消息
    void doWork();


private:
    QUdpSocket *mUdpSocket;//UDP套接字
    QHostAddress mGroupAdress;//组播地址
    int mType; //记录UDP消息传送模式 0:单播 1:广播  2:组播（多播）


};

#endif // UDPSERVER_H

