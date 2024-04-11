#ifndef DIALOGEDFACA_H
#define DIALOGEDFACA_H

#include <QDialog>
#include "../Conn/Uart.h"
#include <QSerialPortInfo>
#include "../ThreadManager/threadmanager.h"




namespace Ui {
class DialogEdfaCA;
}

class DialogEdfaCA : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEdfaCA(QWidget *parent = nullptr);
    ~DialogEdfaCA();


private:
    Ui::DialogEdfaCA *ui;

private slots:
    void on_horizontalSliderEdfaCASetMaxCurrent_valueChanged(int value);
    void on_doubleSpinBoxEdfaCASetMaxCurrent_valueChanged(double value);
    void on_horizontalSliderEdfaCASetCurrentAndPower_valueChanged(int value);
    void on_doubleSpinBoxEdfaCASetCurrentAndPower_valueChanged(double value);
    void changeStatusValue();
    void on_comboBoxEdfaCASetMode_currentIndexChanged(int index);

};

#endif // DIALOGEDFACA_H
