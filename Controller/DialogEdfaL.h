#ifndef DIALOGEDFAL_H
#define DIALOGEDFAL_H

#include <QDialog>
#include "../Conn/Uart.h"
#include <QSerialPortInfo>
#include "../ThreadManager/threadmanager.h"




namespace Ui {
class DialogEdfaL;
}

class DialogEdfaL : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEdfaL(QWidget *parent = nullptr);
    ~DialogEdfaL();


private:
    Ui::DialogEdfaL *ui;

private slots:
    void on_horizontalSliderEdfaLSetMaxCurrent_valueChanged(int value);
    void on_doubleSpinBoxEdfaLSetMaxCurrent_valueChanged(double value);
    void on_horizontalSliderEdfaLSetCurrentAndPower_valueChanged(int value);
    void on_doubleSpinBoxEdfaLSetCurrentAndPower_valueChanged(double value);
    void changeStatusValue();
    void on_comboBoxEdfaLSetMode_currentIndexChanged(int index);

};

#endif // DIALOGEDFAL_H
