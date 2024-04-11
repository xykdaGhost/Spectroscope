#include "DialogEdfaCB.h"
#include "ui_DialogEdfaCB.h"

DialogEdfaCB::DialogEdfaCB(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEdfaCB)
{
    ui->setupUi(this);
    connect(ui->pushButtonEdfaCBUartListRefresh, &QPushButton::clicked, this, [=] {

        ui->comboBoxEdfaCBUartList->clear();   //清空下拉框的显示
        qDebug() << "scan";
        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())   //通过 QSerialPortInfo 查询 LInux 系统下的可用串口号；forreach 是增强的for循环，适用于循环次数未知的情况
        {
            ui->comboBoxEdfaCBUartList->addItem(info.portName());//把获取到的串口号显示在下拉框，“/dev/”是串口节点，获取串口号是没办法获取到这部分的，只能获取到 “ttyS*” 这些，为了显示完整这里加了“/dev/”上去
            qDebug() << info.portName();
        }

    });

    connect(ui->pushButtonEdfaCBUartConnect, &QPushButton::clicked, this, [=] {

        QString comName = ui->comboBoxEdfaCBUartList->currentText();
        ThreadManager::getInstance().openUart(comName);

    });


    connect(ui->pushButtonEdfaCBSendParam, &QPushButton::clicked, this, [=] {
        qDebug() << "click";
        ThreadManager::getInstance().requestUart(1);

    });

    connect(&ThreadManager::getInstance(), SIGNAL(refreshEdfaCBStatus()), this, SLOT(changeStatusValue()), Qt::QueuedConnection);

}

DialogEdfaCB::~DialogEdfaCB()
{
    delete ui;

}

void DialogEdfaCB::changeStatusValue() {
    QString _string;
    if (ThreadManager::getInstance()._edfaCBData.timeout == 0) {
        _string = "连结超时";
        ui->labelEdfaCBMonitorWorkMode->setText(_string);
        return;

    }

    double d = ThreadManager::getInstance()._edfaCBData.current;
    _string = "工作电流: " + QString::number(d, 10, 1) + " mA";
    ui->labelEdfaCBMonitorCurrent->setText(_string);


    d = ThreadManager::getInstance()._edfaCBData.temperature;
    _string = "温度: " +  QString::number(d, 10, 1) + " ℃";
    ui->labelEdfaCBMonitorTemperature->setText(_string);


    d = ThreadManager::getInstance()._edfaCBData.power;
    _string = "输出光功率: " +  QString::number(d, 10, 1) + " dBm";
    ui->labelEdfaCBMonitorPower->setText(_string);


}

void DialogEdfaCB::on_horizontalSliderEdfaCBSetMaxCurrent_valueChanged(int value) {
    ui->doubleSpinBoxEdfaCBSetMaxCurrent->setValue((double)value/10);

}

void DialogEdfaCB::on_doubleSpinBoxEdfaCBSetMaxCurrent_valueChanged(double value) {
    ui->horizontalSliderEdfaCBSetMaxCurrent->setValue(value*10);

}

void DialogEdfaCB::on_horizontalSliderEdfaCBSetCurrentAndPower_valueChanged(int value) {
    ui->doubleSpinBoxEdfaCBSetCurrentAndPower->setValue((double)value/10);
}

void DialogEdfaCB::on_doubleSpinBoxEdfaCBSetCurrentAndPower_valueChanged(double value) {
    ui->horizontalSliderEdfaCBSetCurrentAndPower->setValue(value*10);
}


void DialogEdfaCB::on_comboBoxEdfaCBSetMode_currentIndexChanged(int index) {
    qDebug() << "id :" << index;
    if (index == 0) {
        ui->labelEdfaCBSetCurrentAndPower->setText("设置电流");
        ui->doubleSpinBoxEdfaCBSetCurrentAndPower->setMaximum(500);
        ui->doubleSpinBoxEdfaCBSetCurrentAndPower->setMinimum(0);
        ui->horizontalSliderEdfaCBSetCurrentAndPower->setMaximum(5000);
        ui->horizontalSliderEdfaCBSetCurrentAndPower->setMinimum(0);

        ui->doubleSpinBoxEdfaCBSetCurrentAndPower->setValue(50);

    } else if (index == 1) {
        ui->labelEdfaCBSetCurrentAndPower->setText("设置功率");
        ui->doubleSpinBoxEdfaCBSetCurrentAndPower->setMaximum(ThreadManager::getInstance()._edfaCBData.setPowerMode);
        ui->doubleSpinBoxEdfaCBSetCurrentAndPower->setMinimum(-70);
        ui->horizontalSliderEdfaCBSetCurrentAndPower->setMaximum(ThreadManager::getInstance()._edfaCBData.setPowerMode*10);
        ui->horizontalSliderEdfaCBSetCurrentAndPower->setMinimum(-700);

        ui->doubleSpinBoxEdfaCBSetCurrentAndPower->setValue(5);
    }
}
