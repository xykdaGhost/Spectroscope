#ifndef DIALOGEDFA_H
#define DIALOGEDFA_H

#include <QDialog>
#include "QSerialPortInfo"
#include "QSerialPort"
#include "../ThreadManager/threadmanager.h"

namespace Ui {
class DialogEDFA;
}

class DialogEDFA : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEDFA(QWidget *parent = nullptr);
    ~DialogEDFA();

private:
    Ui::DialogEDFA *ui;

public slots:
    void changeStatusValue();

private slots:

    void on_horizontalSlider_edfa01SetCurrentAndPower_valueChanged(int value);
    void on_horizontalSlider_edfa02SetCurrentAndPower_valueChanged(int value);
    void on_horizontalSlider_edfa03SetCurrentAndPower_valueChanged(int value);

    void on_doubleSpinBox_edfa01SetCurrentAndPower_valueChanged(double value);
    void on_doubleSpinBox_edfa02SetCurrentAndPower_valueChanged(double value);
    void on_doubleSpinBox_edfa03SetCurrentAndPower_valueChanged(double value);



};

#endif // DIALOGEDFA_H
