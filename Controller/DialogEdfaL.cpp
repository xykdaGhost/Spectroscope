#include "DialogEdfaL.h"
#include "ui_DialogEdfaL.h"

DialogEdfaL::DialogEdfaL(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEdfaL)
{
    ui->setupUi(this);
    connect(ui->pushButtonEdfaLUartListRefresh, &QPushButton::clicked, this, [=] {

        ui->comboBoxEdfaLUartList->clear();   //清空下拉框的显示
        qDebug() << "scan";
        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())   //通过 QSerialPortInfo 查询 LInux 系统下的可用串口号；forreach 是增强的for循环，适用于循环次数未知的情况
        {
            ui->comboBoxEdfaLUartList->addItem(info.portName());//把获取到的串口号显示在下拉框，“/dev/”是串口节点，获取串口号是没办法获取到这部分的，只能获取到 “ttyS*” 这些，为了显示完整这里加了“/dev/”上去
            qDebug() << info.portName();
        }

    });

    connect(ui->pushButtonEdfaLUartConnect, &QPushButton::clicked, this, [=] {

        QString comName = ui->comboBoxEdfaLUartList->currentText();
        ThreadManager::getInstance().openUart(comName);

    });


    connect(ui->pushButtonEdfaLSendParam, &QPushButton::clicked, this, [=] {
        qDebug() << "click";
        ThreadManager::getInstance().requestUart(1);

    });

    connect(&ThreadManager::getInstance(), SIGNAL(refreshEdfaLStatus()), this, SLOT(changeStatusValue()), Qt::QueuedConnection);

}

DialogEdfaL::~DialogEdfaL()
{
    delete ui;

}

void DialogEdfaL::changeStatusValue() {
    QString _string;
    if (ThreadManager::getInstance()._edfaLData.timeout == 0) {
        _string = "连结超时";
        ui->labelEdfaLMonitorWorkMode->setText(_string);
        return;
    }

    double d = ThreadManager::getInstance()._edfaLData.current;
    _string = "工作电流: " + QString::number(d, 10, 1) + " ℃";
    ui->labelEdfaLMonitorCurrent->setText(_string);


    d = ThreadManager::getInstance()._edfaLData.temperature;
    _string = "";
    ui->labelEdfaLMonitorTemperature->setText(_string);


    d = ThreadManager::getInstance()._edfaLData.power;
    _string = "输出光功率: " +  QString::number(d, 10, 1) + " dBm";
    ui->labelEdfaLMonitorPower->setText(_string);


}

void DialogEdfaL::on_horizontalSliderEdfaLSetMaxCurrent_valueChanged(int value) {
    ui->doubleSpinBoxEdfaLSetMaxCurrent->setValue((double)value/10);

}

void DialogEdfaL::on_doubleSpinBoxEdfaLSetMaxCurrent_valueChanged(double value) {
    ui->horizontalSliderEdfaLSetMaxCurrent->setValue(value*10);

}

void DialogEdfaL::on_horizontalSliderEdfaLSetCurrentAndPower_valueChanged(int value) {
    ui->doubleSpinBoxEdfaLSetCurrentAndPower->setValue((double)value/10);
}

void DialogEdfaL::on_doubleSpinBoxEdfaLSetCurrentAndPower_valueChanged(double value) {
    ui->horizontalSliderEdfaLSetCurrentAndPower->setValue(value*10);
}


void DialogEdfaL::on_comboBoxEdfaLSetMode_currentIndexChanged(int index) {
    qDebug() << "id :" << index;
    if (index == 0) {
        ui->labelEdfaLSetCurrentAndPower->setText("设置电流");
        ui->doubleSpinBoxEdfaLSetCurrentAndPower->setMaximum(500);
        ui->doubleSpinBoxEdfaLSetCurrentAndPower->setMinimum(0);
        ui->horizontalSliderEdfaLSetCurrentAndPower->setMaximum(5000);
        ui->horizontalSliderEdfaLSetCurrentAndPower->setMinimum(0);

        ui->doubleSpinBoxEdfaLSetCurrentAndPower->setValue(50);

    } else if (index == 1) {
        ui->labelEdfaLSetCurrentAndPower->setText("设置功率");
        ui->doubleSpinBoxEdfaLSetCurrentAndPower->setMaximum(ThreadManager::getInstance()._edfaLData.setPowerMode);
        ui->doubleSpinBoxEdfaLSetCurrentAndPower->setMinimum(-70);
        ui->horizontalSliderEdfaLSetCurrentAndPower->setMaximum(ThreadManager::getInstance()._edfaLData.setPowerMode*10);
        ui->horizontalSliderEdfaLSetCurrentAndPower->setMinimum(-700);

        ui->doubleSpinBoxEdfaLSetCurrentAndPower->setValue(5);
    }
}
