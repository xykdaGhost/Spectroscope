#include "dialogedfa.h"
#include "ui_dialogedfa.h"

DialogEDFA::DialogEDFA(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEDFA)
{
    ui->setupUi(this);


//    {
//        EdfaSettings settings = ThreadManager::getInstance().edfaSettings[0];
//        if (settings.mode == 0) {
//            ui->comboBox_edfa01SetMode->set
//        }
//    }


    connect(ui->pushButton_scan, &QPushButton::clicked, this, [=] {

        ui->comboBox_port->clear();   //清空下拉框的显示
        qDebug() << "scan";
        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())   //通过 QSerialPortInfo 查询 LInux 系统下的可用串口号；forreach 是增强的for循环，适用于循环次数未知的情况
        {
            ui->comboBox_port->addItem(info.portName());//把获取到的串口号显示在下拉框，“/dev/”是串口节点，获取串口号是没办法获取到这部分的，只能获取到 “ttyS*” 这些，为了显示完整这里加了“/dev/”上去
            qDebug() << info.portName();
        }


    });


    connect(ui->pushButton_connect, &QPushButton::clicked, this, [=] {

        QString comName = ui->comboBox_port->currentText();
        qDebug() << "com name : " << comName;
        ThreadManager::getInstance().openUart(comName);

    });

    connect(&ThreadManager::getInstance(), SIGNAL(refreshEDFAStatus()), this, SLOT(changeStatusValue()), Qt::QueuedConnection);

}

DialogEDFA::~DialogEDFA()
{
    delete ui;
}

void DialogEDFA::changeStatusValue() {
    qDebug() << "refresh";
    QString _string;
    {
        EdfaStatus status =  ThreadManager::getInstance().edfaStatus[0];

        if (status.timeout == 0) {
            _string = "连接超时";

        } else {

            double d = status.current;
            _string = "工作电流: " + QString::number(d, 10, 1) + " mA";
            ui->labelEdfa01MonitorCurrent->setText(_string);

            d = status.temperature;
            _string = "温度: " +  QString::number(d, 10, 1) + " ℃";
            ui->labelEdfa01MonitorTemperature->setText(_string);


            d = status.power;
            _string = "输出光功率: " +  QString::number(d, 10, 1) + " dBm";
            ui->labelEdfa01MonitorPower->setText(_string);

            if (status.mode == 0) {
                _string = "工作模式: ACC 恒电流模式";
            } else if (status.mode == 2) {
                _string = "工作模式: APC 恒功率模式";
            }
            ui->labelEdfa01MonitorWorkMode->setText(_string);

        }
    }
    {
        EdfaStatus status =  ThreadManager::getInstance().edfaStatus[1];

        if (status.timeout == 0) {
            _string = "连接超时";

        } else {

            double d = status.current;
            _string = "工作电流: " + QString::number(d, 10, 1) + " mA";
            ui->labelEdfa02MonitorCurrent->setText(_string);

            d = status.temperature;
            _string = "温度: " +  QString::number(d, 10, 1) + " ℃";
            ui->labelEdfa02MonitorTemperature->setText(_string);


            d = status.power;
            _string = "输出光功率: " +  QString::number(d, 10, 1) + " dBm";
            ui->labelEdfa02MonitorPower->setText(_string);

            if (status.mode == 0) {
                _string = "工作模式: ACC 恒电流模式";
            } else if (status.mode == 2) {
                _string = "工作模式: APC 恒功率模式";
            }
            ui->labelEdfa02MonitorWorkMode->setText(_string);

        }
    }
    {
        EdfaStatus status =  ThreadManager::getInstance().edfaStatus[2];

        if (status.timeout == 0) {
            _string = "连接超时";

        } else {

            double d = status.current;
            _string = "工作电流: " + QString::number(d, 10, 1) + " mA";
            ui->labelEdfa03MonitorCurrent->setText(_string);


            d = status.power;
            _string = "输出光功率: " +  QString::number(d, 10, 1) + " dBm";
            ui->labelEdfa03MonitorPower->setText(_string);

            if (status.mode == 0) {
                _string = "工作模式: ACC 恒电流模式";
            } else if (status.mode == 2) {
                _string = "工作模式: APC 恒功率模式";
            }
            ui->labelEdfa03MonitorWorkMode->setText(_string);

        }
    }

}

void DialogEDFA::on_horizontalSlider_edfa01SetCurrentAndPower_valueChanged(int value) {
    ui->doubleSpinBox_edfa01SetCurrentAndPower->setValue((double)value/10);
}

void DialogEDFA::on_horizontalSlider_edfa02SetCurrentAndPower_valueChanged(int value) {
    ui->doubleSpinBox_edfa02SetCurrentAndPower->setValue((double)value/10);
}

void DialogEDFA::on_horizontalSlider_edfa03SetCurrentAndPower_valueChanged(int value) {
    ui->doubleSpinBox_edfa03SetCurrentAndPower->setValue((double)value/10);
}


void DialogEDFA::on_doubleSpinBox_edfa01SetCurrentAndPower_valueChanged(double value) {
    ui->horizontalSlider_edfa01SetCurrentAndPower->setValue(value*10);
}

void DialogEDFA::on_doubleSpinBox_edfa02SetCurrentAndPower_valueChanged(double value) {
    ui->horizontalSlider_edfa02SetCurrentAndPower->setValue(value*10);
}

void DialogEDFA::on_doubleSpinBox_edfa03SetCurrentAndPower_valueChanged(double value) {
    ui->horizontalSlider_edfa03SetCurrentAndPower->setValue(value*10);
}



//void DialogEdfaCA::on_comboBoxEdfaCASetMode_currentIndexChanged(int index) {
//    qDebug() << "id :" << index;
//    if (index == 0) {
//        ui->labelEdfaCASetCurrentAndPower->setText("设置电流");
//        ui->doubleSpinBoxEdfaCASetCurrentAndPower->setMaximum(500);
//        ui->doubleSpinBoxEdfaCASetCurrentAndPower->setMinimum(0);
//        ui->horizontalSliderEdfaCASetCurrentAndPower->setMaximum(5000);
//        ui->horizontalSliderEdfaCASetCurrentAndPower->setMinimum(0);

//        ui->doubleSpinBoxEdfaCASetCurrentAndPower->setValue(50);

//    } else if (index == 1) {
//        ui->labelEdfaCASetCurrentAndPower->setText("设置功率");
//        ui->doubleSpinBoxEdfaCASetCurrentAndPower->setMaximum(ThreadManager::getInstance()._edfaCAData.setPowerMode);
//        ui->doubleSpinBoxEdfaCASetCurrentAndPower->setMinimum(-70);
//        ui->horizontalSliderEdfaCASetCurrentAndPower->setMaximum(ThreadManager::getInstance()._edfaCAData.setPowerMode*10);
//        ui->horizontalSliderEdfaCASetCurrentAndPower->setMinimum(-700);

//        ui->doubleSpinBoxEdfaCASetCurrentAndPower->setValue(5);
//    }
//}

