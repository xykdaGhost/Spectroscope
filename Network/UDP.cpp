#include "UDP.h"


//UDP::UDP()
//{
//    //    mType = 0;//Unicast
//    //    mType = 1;//Broadcast


//}

void UDP::doWork()
{
    mType = 0;
    InitSocket();
    qDebug() << "init uart";
}

void UDP::InitSocket()
{
    //初始化socket，设置组播地址
    mUdpSocket = new QUdpSocket;
    mGroupAdress.setAddress("239.2.2.222");
    mType = 0;
    if(mType == 0 || mType == 1)
    {
        //绑定本地IP和端口号
        mUdpSocket->bind(6666);
    }
    else if(mType == 2)
    {
        if(mUdpSocket->bind(QHostAddress::AnyIPv4,8888,QUdpSocket::ShareAddress))
        {
            //加入组播地址
            mUdpSocket->joinMulticastGroup(mGroupAdress);
            qDebug()<<("Join Multicast Adrress [")<<mGroupAdress.toString()
                     <<("] Successful!");
        }
    }
    else
    {
        qDebug()<< "mType is error! ";
        return;
    }

    connect(mUdpSocket,&QUdpSocket::readyRead,this,[=]{
        ReadPendingDataframs();
    });
}



void UDP::ReadPendingDataframs()
{
    QByteArray _data;
    _data.resize(mUdpSocket->pendingDatagramSize());
    if(mType == 0)//Unicast
    {
        QHostAddress *_peerHostAddress = new QHostAddress("127.0.0.2");
        quint16 _port = 6666;
        while(mUdpSocket->hasPendingDatagrams())
        {
            mUdpSocket->readDatagram(_data.data(),_data.size(),_peerHostAddress,&_port);//接收指定IP和端口的udp报文
            qDebug()<<"Unicast ==> Receive data : "<<QString::fromLatin1(_data);
        }
    }
    else if(mType == 1)//Broadcast
    {
        QHostAddress _peerHostAddress;
        quint16 _port;
        while(mUdpSocket->hasPendingDatagrams())
        {
            mUdpSocket->readDatagram(_data.data(),_data.size(),&_peerHostAddress,&_port);//接收同一子网的udp报文
            qDebug()<<"Broadcast ==> Receive data : "<<QString::fromLatin1(_data);
        }
    }
    else if(mType == 2)//Multicast
    {
        QHostAddress _peerHostAddress;
        quint16 _port;
        while(mUdpSocket->hasPendingDatagrams())
        {
            mUdpSocket->readDatagram(_data.data(),_data.size(),&_peerHostAddress,&_port);//接收同组的udp报文
            qDebug()<<"Multicast ==> Receive data : "<<QString::fromLatin1(_data);
        }
    }
    else
    {
        qDebug()<< "mType is error! ";
        return;
    }
}
