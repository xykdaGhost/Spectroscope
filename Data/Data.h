#ifndef DATA_H
#define DATA_H

#define ACC_MODE 2
#define APC_MODE 0


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
} EdfaStatus;


class Data
{
public:
    Data();

private:
    EdfaStatus * edfaStauts;


};

#endif // DATA_H
