#ifndef UART_H
#define UART_H

#include <QObject>
#include <QSerialPort>
#include <QDateTime>
#include <QStorageInfo>
#include <QProcess>
#include "../ThreadManager/threadmanager.h"

class Uart : public QObject
{
    Q_OBJECT
public:
    static Uart& getInstance() {
        static Uart uart;
        return uart;
    }
    ~Uart();
    void init();



public slots:
    void handle_data();
    void doWork();
    void sendUartMessageHandler(int id);
    void openUartHandler(QString name);
    void scheduleTask();


private:
    explicit Uart();
    QSerialPort * port;
    volatile bool readFlag;
    bool writeFlag;

    void closePort();
    void openPort();

    void on_receive(QByteArray tmpdata);
//    void ask_setEdfaParam(uchar id);

    void ask_STMHeart();
    void ask_EdfaCAHeart();
    void ask_EdfaCBHeart();
    void ask_EdfaLHeart();

    void ask_EdfaCAParamSet();
    void ask_EdfaCBParamSet();
    void ask_EdfaLParamSet();

    void edfaDatahandler(QByteArray data);
};

#endif // UART_H
