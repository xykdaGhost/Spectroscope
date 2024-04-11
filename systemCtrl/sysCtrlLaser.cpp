#include "dialogsystemctrl.h"
#include "ui_dialogsystemctrl.h"

void DialogSystemCtrl::InitCtrlModuleLaser(){
    InitLaser1ObjsInMainUI();
    for(int iTag = 1; iTag < 3; iTag++){
        qdlLaser1[iTag] = new DialogLASER1();
        Laser1ObjsInMainUI[iTag].labelWatchIfOnLaser->setStyleSheet("QLabel{background:rgba(255,0,0,130);}");
        Laser1ObjsInMainUI[iTag].labelConnectStateComLaser->setStyleSheet("QLabel{background:rgba(255,0,0,130);}");
        Laser1ObjsInMainUI[iTag].pushButtonSnglFreqSetLaser->setEnabled(false);
        Laser1ObjsInMainUI[iTag].pushButtonSnglFreqEnableLaser->setEnabled(false);
        Laser1ObjsInMainUI[iTag].pushButtonMultiFreqEnableLaser->setEnabled(false);
//        需要为4个qdl分别调用connect函数
        connectUISliderWithSpinBoxLaser(iTag);
        connectUIbuttonWithHandlerLaser(iTag);
        connectSignalsWithMainHandlerLaser(iTag);
    }
}
void DialogSystemCtrl::connectSignalsWithMainHandlerLaser(int iTag){
    //sigWatchResult
    connect(qdlLaser1[iTag], &DialogLASER1::sigWatchResultLaser,
            [=](bool bIsComOpen, bool bIsOutput, bool bIsStable, bool bIsAlive, float fQstFrq, float fQstTmp, float fQstPwr){
        if(bIsComOpen) {
            if(bIsAlive) {
                //有输出，可以显示功率，输出是否稳定，稳定后还可以开启用户手动设置
                //TextDebugOut("Laser Alive");
                if(bIsOutput){
                    //Laser1ObjsInMainUI[iTag].pushButtonEnableLaser->setText("DisableLASER1");
                    if(bIsStable){
                        Laser1ObjsInMainUI[iTag].labelWatchIfOnLaser->setText(QStringLiteral("输出稳定")); //输出稳定
                        Laser1ObjsInMainUI[iTag].labelWatchIfOnLaser->setStyleSheet("QLabel{background:rgba(0,255,0,130);}");
                    }
                    else{
                        Laser1ObjsInMainUI[iTag].labelWatchIfOnLaser->setText(QStringLiteral("输出未稳定"));
                        Laser1ObjsInMainUI[iTag].labelWatchIfOnLaser->setStyleSheet("QLabel{background:rgba(255,255,0,130);}");
                    }
                    //既然在输出那就可以显示功率
                    Laser1ObjsInMainUI[iTag].labelWatchPwrLaser->setText(QString::number(fQstPwr, 'f', 2));
                }
                else{
                    Laser1ObjsInMainUI[iTag].labelWatchPwrLaser->setText("NAN");
                    Laser1ObjsInMainUI[iTag].labelWatchIfOnLaser->setText(QStringLiteral("未输出"));
                    Laser1ObjsInMainUI[iTag].labelWatchIfOnLaser->setStyleSheet("QLabel{background:rgba(255,0,0,130);}");
                    //Laser1ObjsInMainUI[iTag].pushButtonEnableLaser->setText("EnableLASER1");
                }
                //不管有没有输出，频率都是可以显示的
                float fQstWaveln = 300000/fQstFrq;
                Laser1ObjsInMainUI[iTag].labelWatchWavelnLaser->setText(QString::number(fQstWaveln, 'f', 2));
                Laser1ObjsInMainUI[iTag].labelWatchTempLaser->setText(QString::number(fQstTmp, 'f', 2));
            }
            else {
                //激光器挂了未响应
                Laser1ObjsInMainUI[iTag].labelWatchIfOnLaser->setText(QStringLiteral("未响应"));
                Laser1ObjsInMainUI[iTag].labelWatchIfOnLaser->setStyleSheet("QLabel{background:rgba(255,0,0,130);}");
                Laser1ObjsInMainUI[iTag].labelWatchWavelnLaser->setText("NAN");
                Laser1ObjsInMainUI[iTag].labelWatchTempLaser->setText("NAN");
                Laser1ObjsInMainUI[iTag].labelWatchPwrLaser->setText("NAN");
            }
        }
        else{
            //串口就没开
        }
    });
}

void DialogSystemCtrl::connectUIbuttonWithHandlerLaser(int iTag){
    /************************这段使用lambda函数，负责扫描串口并向串口选择栏中增加可选状态******************/
    connect(Laser1ObjsInMainUI[iTag].pushButtonMultiFreqEnableLaser, &QPushButton::clicked, [=](){
        qdlLaser1[iTag]->handleMwScnFrqLASER1(Laser1ObjsInMainUI[iTag].doubleSpinBoxStartWavelnLaser->value(),
                                              Laser1ObjsInMainUI[iTag].doubleSpinBoxEndWavelnLaser->value(),
                                              Laser1ObjsInMainUI[iTag].doubleSpinBoxSteplnLaser->value(),
                                              Laser1ObjsInMainUI[iTag].doubleSpinBoxStableDurLaser->value(),
                                              Laser1ObjsInMainUI[iTag].doubleSpinBoxMultiFreqSetPwrLaser->value(),
                                              false);
        Laser1ObjsInMainUI[iTag].labelWatchIfOnLaser->setText(QStringLiteral("正在设置"));
        Laser1ObjsInMainUI[iTag].labelWatchIfOnLaser->setStyleSheet("QLabel{background:rgba(255,0,0,130);}");
    });
    /************************这段使用lambda函数，负责扫描串口并向串口选择栏中增加可选状态******************/
    connect(Laser1ObjsInMainUI[iTag].pushButtonSnglFreqEnableLaser, &QPushButton::clicked, [=](){
        qdlLaser1[iTag]->handleMwToggleLASER1();
    });
    /************************这段使用lambda函数，负责扫描串口并向串口选择栏中增加可选状态******************/
    connect(Laser1ObjsInMainUI[iTag].pushButtonSnglFreqSetLaser, &QPushButton::clicked, [=](){
        qdlLaser1[iTag]->handleMwSetParLASER1(Laser1ObjsInMainUI[iTag].doubleSpinBoxSnglFreqSetWvlnLaser->value(),
                                              Laser1ObjsInMainUI[iTag].doubleSpinBoxSnglFreqSetPwrLaser->value());
    });
    /************************这段使用lambda函数，负责扫描串口并向串口选择栏中增加可选状态******************/
    connect(Laser1ObjsInMainUI[iTag].pushButtonOpenDebuggerLaser, &QPushButton::clicked, [=](){
        if(Laser1ObjsInMainUI[iTag].lineEditOpenDebuggerCodeLaser->text() == "6666")
            qdlLaser1[iTag]->show();
    });
    /************************这段使用lambda函数，负责扫描串口并向串口选择栏中增加可选状态******************/
    connect(Laser1ObjsInMainUI[iTag].pushButtonScanComLaser, &QPushButton::clicked, [=](){
        Laser1ObjsInMainUI[iTag].comboBoxSelectComLaser->clear();
        Laser1ObjsInMainUI[iTag].comboBoxSelectComLaser->addItems(qdlLaser1[iTag]->handleMwScanComLASER1());
    });
    /************************这段使用lambda函数，负责打开串口及改变Mw按钮状态***************************/
    connect(Laser1ObjsInMainUI[iTag].pushButtonOpenComLaser, &QPushButton::clicked, [=](){
        int status = qdlLaser1[iTag]->handleMwSetComLASER1(
                    Laser1ObjsInMainUI[iTag].comboBoxSelectComLaser->currentText(),
                    Laser1ObjsInMainUI[iTag].lineEditBaudRateComLaser->text().toInt());

        if(status == 0){
            Laser1ObjsInMainUI[iTag].pushButtonSnglFreqSetLaser->setEnabled(false);
            Laser1ObjsInMainUI[iTag].pushButtonSnglFreqEnableLaser->setEnabled(false);
            Laser1ObjsInMainUI[iTag].pushButtonMultiFreqEnableLaser->setEnabled(false);

            Laser1ObjsInMainUI[iTag].pushButtonOpenComLaser->setText(QStringLiteral("打开串口"));
            Laser1ObjsInMainUI[iTag].labelConnectStateComLaser->setText(QStringLiteral("现已关闭"));
            Laser1ObjsInMainUI[iTag].labelConnectStateComLaser->setStyleSheet("QLabel{background:rgba(255,0,0,130);}");
        }
        else if (status == 1){
            Laser1ObjsInMainUI[iTag].pushButtonSnglFreqSetLaser->setEnabled(true);
            Laser1ObjsInMainUI[iTag].pushButtonSnglFreqEnableLaser->setEnabled(true);
            Laser1ObjsInMainUI[iTag].pushButtonMultiFreqEnableLaser->setEnabled(true);

            Laser1ObjsInMainUI[iTag].pushButtonOpenComLaser->setText(QStringLiteral("关闭串口"));
            Laser1ObjsInMainUI[iTag].labelConnectStateComLaser->setText(QStringLiteral("现已打开"));
            Laser1ObjsInMainUI[iTag].labelConnectStateComLaser->setStyleSheet("QLabel{background:rgba(0,255,0,130);}");
        }
        else if(status == -1){
            Laser1ObjsInMainUI[iTag].labelConnectStateComLaser->setText(QStringLiteral("打开失败!"));
            Laser1ObjsInMainUI[iTag].labelConnectStateComLaser->setStyleSheet("QLabel{background:rgba(255,0,0,130);}");
        }
        else if(status == -2){
            Laser1ObjsInMainUI[iTag].labelConnectStateComLaser->setText(QStringLiteral("选择串口！"));
            Laser1ObjsInMainUI[iTag].labelConnectStateComLaser->setStyleSheet("QLabel{background:rgba(255,0,0,130);}");
        }
    });
}

void DialogSystemCtrl::on_pushButtonOpenDebuggerLaser_1_clicked()
{
    qdlLaser1[1]->show();
}
void DialogSystemCtrl::InitLaser1ObjsInMainUI(){
    Laser1ObjsInMainUI[1].pushButtonScanComLaser = ui->pushButtonScanComLaser_1;
    Laser1ObjsInMainUI[1].pushButtonOpenComLaser = ui->pushButtonOpenComLaser_1;
    Laser1ObjsInMainUI[1].pushButtonOpenDebuggerLaser = ui->pushButtonOpenDebuggerLaser_1;

    Laser1ObjsInMainUI[1].pushButtonSnglFreqEnableLaser = ui->pushButtonSnglFreqEnableLaser_1;
    Laser1ObjsInMainUI[1].pushButtonSnglFreqSetLaser = ui->pushButtonSnglFreqSetLaser_1;
    Laser1ObjsInMainUI[1].pushButtonMultiFreqEnableLaser = ui->pushButtonMultiFreqEnableLaser_1;

    Laser1ObjsInMainUI[1].doubleSpinBoxSnglFreqSetPwrLaser = ui->doubleSpinBoxSnglFreqSetPwrLaser_1;
    Laser1ObjsInMainUI[1].doubleSpinBoxSnglFreqSetWvlnLaser = ui->doubleSpinBoxSnglFreqSetWvlnLaser_1;
    Laser1ObjsInMainUI[1].doubleSpinBoxMultiFreqSetPwrLaser = ui->doubleSpinBoxMultiFreqSetPwrLaser_1;
    Laser1ObjsInMainUI[1].doubleSpinBoxStartWavelnLaser = ui->doubleSpinBoxStartWavelnLaser_1;
    Laser1ObjsInMainUI[1].doubleSpinBoxEndWavelnLaser = ui->doubleSpinBoxEndWavelnLaser_1;
    Laser1ObjsInMainUI[1].doubleSpinBoxSteplnLaser = ui->doubleSpinBoxSteplnLaser_1;
    Laser1ObjsInMainUI[1].doubleSpinBoxStableDurLaser = ui->doubleSpinBoxStableDurLaser_1;

    Laser1ObjsInMainUI[1].horizontalSliderSnglFreqSetPwrLaser = ui->horizontalSliderSnglFreqSetPwrLaser_1;
    Laser1ObjsInMainUI[1].horizontalSliderSnglFreqSetWvlnLaser = ui->horizontalSliderSnglFreqSetWvlnLaser_1;
    Laser1ObjsInMainUI[1].horizontalSliderMultiFreqSetPwrLaser = ui->horizontalSliderMultiFreqSetPwrLaser_1;
    Laser1ObjsInMainUI[1].horizontalSliderStartWavelnLaser = ui->horizontalSliderStartWavelnLaser_1;
    Laser1ObjsInMainUI[1].horizontalSliderEndWavelnLaser = ui->horizontalSliderEndWavelnLaser_1;

    Laser1ObjsInMainUI[1].comboBoxSelectComLaser = ui->comboBoxSelectComLaser_1;
    Laser1ObjsInMainUI[1].lineEditOpenDebuggerCodeLaser = ui->lineEditOpenDebuggerCodeLaser_1;
    Laser1ObjsInMainUI[1].lineEditBaudRateComLaser = ui->lineEditBaudRateComLaser_1;

    Laser1ObjsInMainUI[1].labelConnectStateComLaser = ui->labelConnectStateComLaser_1;
    Laser1ObjsInMainUI[1].labelWatchWavelnLaser = ui->labelWatchWavelnLaser_1;
    Laser1ObjsInMainUI[1].labelWatchTempLaser = ui->labelWatchTempLaser_1;
    Laser1ObjsInMainUI[1].labelWatchPwrLaser = ui->labelWatchPwrLaser_1;
    Laser1ObjsInMainUI[1].labelWatchIfOnLaser = ui->labelWatchIfOnLaser_1;

    Laser1ObjsInMainUI[2].pushButtonScanComLaser = ui->pushButtonScanComLaser_2;
    Laser1ObjsInMainUI[2].pushButtonOpenComLaser = ui->pushButtonOpenComLaser_2;
    Laser1ObjsInMainUI[2].pushButtonOpenDebuggerLaser = ui->pushButtonOpenDebuggerLaser_2;

    Laser1ObjsInMainUI[2].pushButtonSnglFreqEnableLaser = ui->pushButtonSnglFreqEnableLaser_2;
    Laser1ObjsInMainUI[2].pushButtonSnglFreqSetLaser = ui->pushButtonSnglFreqSetLaser_2;
    Laser1ObjsInMainUI[2].pushButtonMultiFreqEnableLaser = ui->pushButtonMultiFreqEnableLaser_2;

    Laser1ObjsInMainUI[2].doubleSpinBoxSnglFreqSetPwrLaser = ui->doubleSpinBoxSnglFreqSetPwrLaser_2;
    Laser1ObjsInMainUI[2].doubleSpinBoxSnglFreqSetWvlnLaser = ui->doubleSpinBoxSnglFreqSetWvlnLaser_2;
    Laser1ObjsInMainUI[2].doubleSpinBoxMultiFreqSetPwrLaser = ui->doubleSpinBoxMultiFreqSetPwrLaser_2;
    Laser1ObjsInMainUI[2].doubleSpinBoxStartWavelnLaser = ui->doubleSpinBoxStartWavelnLaser_2;
    Laser1ObjsInMainUI[2].doubleSpinBoxEndWavelnLaser = ui->doubleSpinBoxEndWavelnLaser_2;
    Laser1ObjsInMainUI[2].doubleSpinBoxSteplnLaser = ui->doubleSpinBoxSteplnLaser_2;
    Laser1ObjsInMainUI[2].doubleSpinBoxStableDurLaser = ui->doubleSpinBoxStableDurLaser_2;

    Laser1ObjsInMainUI[2].horizontalSliderSnglFreqSetPwrLaser = ui->horizontalSliderSnglFreqSetPwrLaser_2;
    Laser1ObjsInMainUI[2].horizontalSliderSnglFreqSetWvlnLaser = ui->horizontalSliderSnglFreqSetWvlnLaser_2;
    Laser1ObjsInMainUI[2].horizontalSliderMultiFreqSetPwrLaser = ui->horizontalSliderMultiFreqSetPwrLaser_2;
    Laser1ObjsInMainUI[2].horizontalSliderStartWavelnLaser = ui->horizontalSliderStartWavelnLaser_2;
    Laser1ObjsInMainUI[2].horizontalSliderEndWavelnLaser = ui->horizontalSliderEndWavelnLaser_2;

    Laser1ObjsInMainUI[2].comboBoxSelectComLaser = ui->comboBoxSelectComLaser_2;
    Laser1ObjsInMainUI[2].lineEditOpenDebuggerCodeLaser = ui->lineEditOpenDebuggerCodeLaser_2;
    Laser1ObjsInMainUI[2].lineEditBaudRateComLaser = ui->lineEditBaudRateComLaser_2;

    Laser1ObjsInMainUI[2].labelConnectStateComLaser = ui->labelConnectStateComLaser_2;
    Laser1ObjsInMainUI[2].labelWatchWavelnLaser = ui->labelWatchWavelnLaser_2;
    Laser1ObjsInMainUI[2].labelWatchTempLaser = ui->labelWatchTempLaser_2;
    Laser1ObjsInMainUI[2].labelWatchPwrLaser = ui->labelWatchPwrLaser_2;
    Laser1ObjsInMainUI[2].labelWatchIfOnLaser = ui->labelWatchIfOnLaser_2;
}


void DialogSystemCtrl::connectUISliderWithSpinBoxLaser(int iTag){
    connect(Laser1ObjsInMainUI[iTag].doubleSpinBoxSnglFreqSetWvlnLaser,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            [=](){
        Laser1ObjsInMainUI[iTag].horizontalSliderSnglFreqSetWvlnLaser
                ->setValue((int)(Laser1ObjsInMainUI[iTag].doubleSpinBoxSnglFreqSetWvlnLaser->value()*10));
    });
    connect(Laser1ObjsInMainUI[iTag].horizontalSliderSnglFreqSetWvlnLaser, &QSlider::valueChanged, [=](int value){
        Laser1ObjsInMainUI[iTag].doubleSpinBoxSnglFreqSetWvlnLaser->setValue((float)value/10);
    });

    connect(Laser1ObjsInMainUI[iTag].doubleSpinBoxStartWavelnLaser,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            [=](){
        Laser1ObjsInMainUI[iTag].horizontalSliderStartWavelnLaser
                ->setValue((int)(Laser1ObjsInMainUI[iTag].doubleSpinBoxStartWavelnLaser->value()*10));
    });
    connect(Laser1ObjsInMainUI[iTag].horizontalSliderStartWavelnLaser, &QSlider::valueChanged, [=](int value){
        Laser1ObjsInMainUI[iTag].doubleSpinBoxStartWavelnLaser->setValue((float)value/10);
    });

    connect(Laser1ObjsInMainUI[iTag].doubleSpinBoxEndWavelnLaser,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            [=](){
        Laser1ObjsInMainUI[iTag].horizontalSliderEndWavelnLaser
                ->setValue((int)(Laser1ObjsInMainUI[iTag].doubleSpinBoxEndWavelnLaser->value()*10));
    });
    connect(Laser1ObjsInMainUI[iTag].horizontalSliderEndWavelnLaser, &QSlider::valueChanged, [=](int value){
        Laser1ObjsInMainUI[iTag].doubleSpinBoxEndWavelnLaser->setValue((float)value/10);
    });

    connect(Laser1ObjsInMainUI[iTag].doubleSpinBoxSnglFreqSetPwrLaser,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            [=](){
        Laser1ObjsInMainUI[iTag].horizontalSliderSnglFreqSetPwrLaser
                ->setValue((int)(Laser1ObjsInMainUI[iTag].doubleSpinBoxSnglFreqSetPwrLaser->value()*100));
    });
    connect(Laser1ObjsInMainUI[iTag].horizontalSliderSnglFreqSetPwrLaser, &QSlider::valueChanged, [=](int value){
        Laser1ObjsInMainUI[iTag].doubleSpinBoxSnglFreqSetPwrLaser->setValue((float)value/100);
    });

    connect(Laser1ObjsInMainUI[iTag].doubleSpinBoxMultiFreqSetPwrLaser,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            [=](){
        Laser1ObjsInMainUI[iTag].horizontalSliderMultiFreqSetPwrLaser
                ->setValue((int)(Laser1ObjsInMainUI[iTag].doubleSpinBoxMultiFreqSetPwrLaser->value()*100));
    });
    connect(Laser1ObjsInMainUI[iTag].horizontalSliderMultiFreqSetPwrLaser, &QSlider::valueChanged, [=](int value){
        Laser1ObjsInMainUI[iTag].doubleSpinBoxMultiFreqSetPwrLaser->setValue((float)value/100);
    });
}
