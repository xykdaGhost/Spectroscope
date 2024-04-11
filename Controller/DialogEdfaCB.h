#ifndef DIALOGEDFACB_H
#define DIALOGEDFACB_H

#include <QDialog>
#include "../Conn/Uart.h"
#include <QSerialPortInfo>
#include "../ThreadManager/threadmanager.h"




namespace Ui {
class DialogEdfaCB;
}

class DialogEdfaCB : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEdfaCB(QWidget *parent = nullptr);
    ~DialogEdfaCB();


private:
    Ui::DialogEdfaCB *ui;

private slots:
    void on_horizontalSliderEdfaCBSetMaxCurrent_valueChanged(int value);
    void on_doubleSpinBoxEdfaCBSetMaxCurrent_valueChanged(double value);
    void on_horizontalSliderEdfaCBSetCurrentAndPower_valueChanged(int value);
    void on_doubleSpinBoxEdfaCBSetCurrentAndPower_valueChanged(double value);
    void changeStatusValue();
    void on_comboBoxEdfaCBSetMode_currentIndexChanged(int index);

};

#endif // DIALOGEDFACB_H
