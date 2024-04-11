#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThread>
#include "../Network/UDP.h"
#include "QVector"

typedef struct {
    int mode;
    double current;
    double power;
    double temperature;
    int timeout;

    int setMode;
    int setPowerMode;
    int setMaxCurrent;
    int setCurrent;
    int setPower;
} EdfaCAData;

typedef struct {
    int mode;
    double current;
    double power;
    double temperature;
    int timeout;

    int setMode;
    int setPowerMode;
    int setMaxCurrent;
    int setCurrent;
    int setPower;
} EdfaCBData;

typedef struct {
    int mode;
    double current;
    double power;
    double temperature;
    int timeout;

    int setMode;
    int setPowerMode;
    int setMaxCurrent;
    int setCurrent;
    int setPower;
} EdfaLData;

typedef struct {
    int mode;
    int maxCurrent;
    int current;  //0-5000   ➗ 10.0 mA
    int power;    // 0-230or150  ➗  10.0 dBm
} EdfaSettings;

typedef struct {
    int mode;
    double current;
    double power;
    double temperature;
    int timeout;
} EdfaStatus;

typedef struct {
    int mode;
    int powerMode;
    int maxCurrent;
    int current;
    int power;
} LaserSettings;

typedef struct {
    int mode;
    int powerMode;
    int maxCurrent;
    int current;
    int power;
} LaserSatuts;


#define ACC_MODE 15
#define APC_MODE 23
#define POWERMODE_15 15
#define POWERMODE_23 23


class ThreadManager : public QObject
{
    Q_OBJECT
public:
    static ThreadManager& getInstance() {
        static ThreadManager manager;
        return manager;
    }
    ~ThreadManager();
    EdfaCAData _edfaCAData;
    EdfaCBData _edfaCBData;
    EdfaLData _edfaLData;
    void requestUart(int id);
    void requestOpenUartPort(QString name);

    QVector<EdfaStatus> edfaStatus;
    QVector<EdfaSettings> edfaSettings;

signals:
    void requestUartMessage(int id);
    void refreshEdfaCAStatus();
    void refreshEdfaCBStatus();
    void refreshEdfaLStatus();
    void refreshEDFAStatus();
    void openUart(QString name);


public slots:
    void doWork();
    void udpDoWork();


private:
    UDP* my_udp;

    ThreadManager();


};

#endif // THREADMANAGER_H
