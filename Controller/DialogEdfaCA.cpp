#include "DialogEdfaCA.h"
#include "ui_DialogEdfaCA.h"
#include "sstream"
#include <iostream>
#include <QDebug>

DialogEdfaCA::DialogEdfaCA(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEdfaCA)
{
    ui->setupUi(this);
    connect(ui->pushButtonEdfaCAUartListRefresh, &QPushButton::clicked, this, [=] {

        ui->comboBoxEdfaCAUartList->clear();   //清空下拉框的显示
        qDebug() << "scan";
        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())   //通过 QSerialPortInfo 查询 LInux 系统下的可用串口号；forreach 是增强的for循环，适用于循环次数未知的情况
        {
            ui->comboBoxEdfaCAUartList->addItem(info.portName());//把获取到的串口号显示在下拉框，“/dev/”是串口节点，获取串口号是没办法获取到这部分的，只能获取到 “ttyS*” 这些，为了显示完整这里加了“/dev/”上去
            qDebug() << info.portName();
        }

    });

    connect(ui->pushButtonEdfaCAUartConnect, &QPushButton::clicked, this, [=] {

        QString comName = ui->comboBoxEdfaCAUartList->currentText();
        qDebug() << "com name : " << comName;
        ThreadManager::getInstance().openUart(comName);

    });


    connect(ui->pushButtonEdfaCASendParam, &QPushButton::clicked, this, [=] {
        qDebug() << "click";
        ThreadManager::getInstance().requestUart(1);

    });

    connect(&ThreadManager::getInstance(), SIGNAL(refreshEdfaCAStatus()), this, SLOT(changeStatusValue()), Qt::QueuedConnection);

}

DialogEdfaCA::~DialogEdfaCA()
{
    delete ui;

}

void DialogEdfaCA::changeStatusValue() {
    QString _string;
    if (ThreadManager::getInstance()._edfaCAData.timeout == 0) {
        _string = "连结超时";
        ui->labelEdfaCAMonitorWorkMode->setText(_string);
        return;
    }



    double d = ThreadManager::getInstance()._edfaCAData.current;
    _string = "工作电流: " + QString::number(d, 10, 1) + " mA";
    ui->labelEdfaCAMonitorCurrent->setText(_string);


    d = ThreadManager::getInstance()._edfaCAData.temperature;
    _string = "温度: " +  QString::number(d, 10, 1) + " ℃";
    ui->labelEdfaCAMonitorTemperature->setText(_string);


    d = ThreadManager::getInstance()._edfaCAData.power;
    _string = "输出光功率: " +  QString::number(d, 10, 1) + " dBm";
    ui->labelEdfaCAMonitorPower->setText(_string);


}

void DialogEdfaCA::on_horizontalSliderEdfaCASetMaxCurrent_valueChanged(int value) {
    ui->doubleSpinBoxEdfaCASetMaxCurrent->setValue((double)value/10);

}

void DialogEdfaCA::on_doubleSpinBoxEdfaCASetMaxCurrent_valueChanged(double value) {
    ui->horizontalSliderEdfaCASetMaxCurrent->setValue(value*10);

}

void DialogEdfaCA::on_horizontalSliderEdfaCASetCurrentAndPower_valueChanged(int value) {
    ui->doubleSpinBoxEdfaCASetCurrentAndPower->setValue((double)value/10);
}

void DialogEdfaCA::on_doubleSpinBoxEdfaCASetCurrentAndPower_valueChanged(double value) {
    ui->horizontalSliderEdfaCASetCurrentAndPower->setValue(value*10);
}


void DialogEdfaCA::on_comboBoxEdfaCASetMode_currentIndexChanged(int index) {
    qDebug() << "id :" << index;
    if (index == 0) {
        ui->labelEdfaCASetCurrentAndPower->setText("设置电流");
        ui->doubleSpinBoxEdfaCASetCurrentAndPower->setMaximum(500);
        ui->doubleSpinBoxEdfaCASetCurrentAndPower->setMinimum(0);
        ui->horizontalSliderEdfaCASetCurrentAndPower->setMaximum(5000);
        ui->horizontalSliderEdfaCASetCurrentAndPower->setMinimum(0);

        ui->doubleSpinBoxEdfaCASetCurrentAndPower->setValue(50);

    } else if (index == 1) {
        ui->labelEdfaCASetCurrentAndPower->setText("设置功率");
        ui->doubleSpinBoxEdfaCASetCurrentAndPower->setMaximum(ThreadManager::getInstance()._edfaCAData.setPowerMode);
        ui->doubleSpinBoxEdfaCASetCurrentAndPower->setMinimum(-70);
        ui->horizontalSliderEdfaCASetCurrentAndPower->setMaximum(ThreadManager::getInstance()._edfaCAData.setPowerMode*10);
        ui->horizontalSliderEdfaCASetCurrentAndPower->setMinimum(-700);

        ui->doubleSpinBoxEdfaCASetCurrentAndPower->setValue(5);
    }
}
