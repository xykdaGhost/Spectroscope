#include "dialogsystemctrl.h"
#include "ui_dialogsystemctrl.h"

void DialogSystemCtrl::InitCtrlModuleEDFA()
{
    InitEDFAObjsInMainUI();
    for(int iTag = 1; iTag < 5; iTag++){
        qdlEDFA[iTag] = new DialogEDFA();
        EDFAObjsInMainUI[iTag].labelWatchIfOnEDFA->setStyleSheet("QLabel{background:rgba(255,0,0,130);}");
        EDFAObjsInMainUI[iTag].labelConnectStateComEDFA->setStyleSheet("QLabel{background:rgba(255,0,0,130);}");
        EDFAObjsInMainUI[iTag].pushButtonSetParamEDFA->setEnabled(false);
        EDFAObjsInMainUI[iTag].pushButtonToggleWatchStateEDFA->setEnabled(false);
        //需要为4个qdl分别调用connect函数
        connectUISliderWithSpinBoxEDFA(iTag);
        connectUIbuttonWithHandlerEDFA(iTag);
        connectSignalsWithMainHandlerEDFA(iTag);
    }
//    //实现DialogSystemCtrlUI中EDFA1模块：选中与未被选中工作模式之间颜色差异的函数
//    EDFAObjsInMainUI[iTag].labelBgPwrEDFA->setStyleSheet("QLabel{background:rgba(0,255,0,120);}");
//    EDFAObjsInMainUI[iTag].labelBgSetModEDFA->setStyleSheet("QLabel{background:rgba(0,255,0,120);}");//
//    EDFAObjsInMainUI[iTag].labelBgHintSetParamEDFA->setStyleSheet("QLabel{background:rgba(0,255,0,120);}");//
//    EDFAObjsInMainUI[iTag].labelBgCurMaxEDFA->setStyleSheet("QLabel{background:rgba(255,255,0,150);}");
//    EDFAObjsInMainUI[iTag].labelHorizontalSplitterEDFA->setStyleSheet("QLabel{background:rgba(0,0,255,150);}");

}
void DialogSystemCtrl::InitEDFAObjsInMainUI(){
    /**************************************EDFA_1**********************************/
    EDFAObjsInMainUI[1].labelWatchPwrEDFA = ui->labelWatchPwrEDFA_1;
    EDFAObjsInMainUI[1].labelWatchCurEDFA = ui->labelWatchCurEDFA_1;
    EDFAObjsInMainUI[1].labelWatchModEDFA = ui->labelWatchModEDFA_1;
    EDFAObjsInMainUI[1].labelWatchIfOnEDFA = ui->labelWatchIfOnEDFA_1;
    EDFAObjsInMainUI[1].labelWatchTempEDFA = ui->labelWatchTempEDFA_1;
    EDFAObjsInMainUI[1].labelConnectStateComEDFA = ui->labelConnectStateComEDFA_1;

    EDFAObjsInMainUI[1].pushButtonScanComEDFA = ui->pushButtonScanComEDFA_1;
    EDFAObjsInMainUI[1].pushButtonOpenComEDFA = ui->pushButtonOpenComEDFA_1;
    EDFAObjsInMainUI[1].pushButtonSetParamEDFA = ui->pushButtonSetParamEDFA_1;
    EDFAObjsInMainUI[1].pushButtonOpenDebuggerEDFA = ui->pushButtonOpenDebuggerEDFA_1;
    EDFAObjsInMainUI[1].pushButtonToggleWatchStateEDFA = ui->pushButtonToggleWatchStateEDFA_1;

    EDFAObjsInMainUI[1].horizontalSliderPwrEDFA = ui->horizontalSliderPwrEDFA_1;
    EDFAObjsInMainUI[1].horizontalSliderCurMaxEDFA = ui->horizontalSliderCurMaxEDFA_1;
    EDFAObjsInMainUI[1].horizontalSliderCurSetEDFA = ui->horizontalSliderCurSetEDFA_1;

    EDFAObjsInMainUI[1].doubleSpinBoxPwrEDFA = ui->doubleSpinBoxPwrEDFA_1;
    EDFAObjsInMainUI[1].doubleSpinBoxCurMaxEDFA = ui->doubleSpinBoxCurMaxEDFA_1;
    EDFAObjsInMainUI[1].doubleSpinBoxCurSetEDFA = ui->doubleSpinBoxCurSetEDFA_1;

    EDFAObjsInMainUI[1].comboBoxSetModEDFA = ui->comboBoxSetModEDFA_1;
    EDFAObjsInMainUI[1].comboBoxSetPwrLvEDFA = ui->comboBoxSetPwrLvEDFA_1;
    EDFAObjsInMainUI[1].comboBoxSelectComEDFA = ui->comboBoxSelectComEDFA_1;
    EDFAObjsInMainUI[1].lineEditBaudRateComEDFA = ui->lineEditBaudRateComEDFA_1;
    EDFAObjsInMainUI[1].lineEditOpenDebuggerCodeEDFA = ui->lineEditOpenDebuggerCodeEDFA_1;
    /**************************************EDFA_2**********************************/
    EDFAObjsInMainUI[2].labelWatchPwrEDFA = ui->labelWatchPwrEDFA_2;
    EDFAObjsInMainUI[2].labelWatchCurEDFA = ui->labelWatchCurEDFA_2;
    EDFAObjsInMainUI[2].labelWatchModEDFA = ui->labelWatchModEDFA_2;
    EDFAObjsInMainUI[2].labelWatchIfOnEDFA = ui->labelWatchIfOnEDFA_2;
    EDFAObjsInMainUI[2].labelWatchTempEDFA = ui->labelWatchTempEDFA_2;
    EDFAObjsInMainUI[2].labelConnectStateComEDFA = ui->labelConnectStateComEDFA_2;

    EDFAObjsInMainUI[2].pushButtonScanComEDFA = ui->pushButtonScanComEDFA_2;
    EDFAObjsInMainUI[2].pushButtonOpenComEDFA = ui->pushButtonOpenComEDFA_2;
    EDFAObjsInMainUI[2].pushButtonSetParamEDFA = ui->pushButtonSetParamEDFA_2;
    EDFAObjsInMainUI[2].pushButtonOpenDebuggerEDFA = ui->pushButtonOpenDebuggerEDFA_2;
    EDFAObjsInMainUI[2].pushButtonToggleWatchStateEDFA = ui->pushButtonToggleWatchStateEDFA_2;

    EDFAObjsInMainUI[2].horizontalSliderPwrEDFA = ui->horizontalSliderPwrEDFA_2;
    EDFAObjsInMainUI[2].horizontalSliderCurMaxEDFA = ui->horizontalSliderCurMaxEDFA_2;
    EDFAObjsInMainUI[2].horizontalSliderCurSetEDFA = ui->horizontalSliderCurSetEDFA_2;

    EDFAObjsInMainUI[2].doubleSpinBoxPwrEDFA = ui->doubleSpinBoxPwrEDFA_2;
    EDFAObjsInMainUI[2].doubleSpinBoxCurMaxEDFA = ui->doubleSpinBoxCurMaxEDFA_2;
    EDFAObjsInMainUI[2].doubleSpinBoxCurSetEDFA = ui->doubleSpinBoxCurSetEDFA_2;

    EDFAObjsInMainUI[2].comboBoxSetModEDFA = ui->comboBoxSetModEDFA_2;
    EDFAObjsInMainUI[2].comboBoxSetPwrLvEDFA = ui->comboBoxSetPwrLvEDFA_2;
    EDFAObjsInMainUI[2].comboBoxSelectComEDFA = ui->comboBoxSelectComEDFA_2;
    EDFAObjsInMainUI[2].lineEditBaudRateComEDFA = ui->lineEditBaudRateComEDFA_2;
    EDFAObjsInMainUI[2].lineEditOpenDebuggerCodeEDFA = ui->lineEditOpenDebuggerCodeEDFA_2;
    /**************************************EDFA_3**********************************/
    EDFAObjsInMainUI[3].labelWatchPwrEDFA = ui->labelWatchPwrEDFA_3;
    EDFAObjsInMainUI[3].labelWatchCurEDFA = ui->labelWatchCurEDFA_3;
    EDFAObjsInMainUI[3].labelWatchModEDFA = ui->labelWatchModEDFA_3;
    EDFAObjsInMainUI[3].labelWatchIfOnEDFA = ui->labelWatchIfOnEDFA_3;
    EDFAObjsInMainUI[3].labelWatchTempEDFA = ui->labelWatchTempEDFA_3;
    EDFAObjsInMainUI[3].labelConnectStateComEDFA = ui->labelConnectStateComEDFA_3;

    EDFAObjsInMainUI[3].pushButtonScanComEDFA = ui->pushButtonScanComEDFA_3;
    EDFAObjsInMainUI[3].pushButtonOpenComEDFA = ui->pushButtonOpenComEDFA_3;
    EDFAObjsInMainUI[3].pushButtonSetParamEDFA = ui->pushButtonSetParamEDFA_3;
    EDFAObjsInMainUI[3].pushButtonOpenDebuggerEDFA = ui->pushButtonOpenDebuggerEDFA_3;
    EDFAObjsInMainUI[3].pushButtonToggleWatchStateEDFA = ui->pushButtonToggleWatchStateEDFA_3;

    EDFAObjsInMainUI[3].horizontalSliderPwrEDFA = ui->horizontalSliderPwrEDFA_3;
    EDFAObjsInMainUI[3].horizontalSliderCurMaxEDFA = ui->horizontalSliderCurMaxEDFA_3;
    EDFAObjsInMainUI[3].horizontalSliderCurSetEDFA = ui->horizontalSliderCurSetEDFA_3;

    EDFAObjsInMainUI[3].doubleSpinBoxPwrEDFA = ui->doubleSpinBoxPwrEDFA_3;
    EDFAObjsInMainUI[3].doubleSpinBoxCurMaxEDFA = ui->doubleSpinBoxCurMaxEDFA_3;
    EDFAObjsInMainUI[3].doubleSpinBoxCurSetEDFA = ui->doubleSpinBoxCurSetEDFA_3;

    EDFAObjsInMainUI[3].comboBoxSetModEDFA = ui->comboBoxSetModEDFA_3;
    EDFAObjsInMainUI[3].comboBoxSetPwrLvEDFA = ui->comboBoxSetPwrLvEDFA_3;
    EDFAObjsInMainUI[3].comboBoxSelectComEDFA = ui->comboBoxSelectComEDFA_3;
    EDFAObjsInMainUI[3].lineEditBaudRateComEDFA = ui->lineEditBaudRateComEDFA_3;
    EDFAObjsInMainUI[3].lineEditOpenDebuggerCodeEDFA = ui->lineEditOpenDebuggerCodeEDFA_3;
    /**************************************EDFA_4**********************************/
    EDFAObjsInMainUI[4].labelWatchPwrEDFA = ui->labelWatchPwrEDFA_4;
    EDFAObjsInMainUI[4].labelWatchCurEDFA = ui->labelWatchCurEDFA_4;
    EDFAObjsInMainUI[4].labelWatchModEDFA = ui->labelWatchModEDFA_4;
    EDFAObjsInMainUI[4].labelWatchIfOnEDFA = ui->labelWatchIfOnEDFA_4;
    EDFAObjsInMainUI[4].labelWatchTempEDFA = ui->labelWatchTempEDFA_4;
    EDFAObjsInMainUI[4].labelConnectStateComEDFA = ui->labelConnectStateComEDFA_4;

    EDFAObjsInMainUI[4].pushButtonScanComEDFA = ui->pushButtonScanComEDFA_4;
    EDFAObjsInMainUI[4].pushButtonOpenComEDFA = ui->pushButtonOpenComEDFA_4;
    EDFAObjsInMainUI[4].pushButtonSetParamEDFA = ui->pushButtonSetParamEDFA_4;
    EDFAObjsInMainUI[4].pushButtonOpenDebuggerEDFA = ui->pushButtonOpenDebuggerEDFA_4;
    EDFAObjsInMainUI[4].pushButtonToggleWatchStateEDFA = ui->pushButtonToggleWatchStateEDFA_4;

    EDFAObjsInMainUI[4].horizontalSliderPwrEDFA = ui->horizontalSliderPwrEDFA_4;
    EDFAObjsInMainUI[4].horizontalSliderCurMaxEDFA = ui->horizontalSliderCurMaxEDFA_4;
    EDFAObjsInMainUI[4].horizontalSliderCurSetEDFA = ui->horizontalSliderCurSetEDFA_4;

    EDFAObjsInMainUI[4].doubleSpinBoxPwrEDFA = ui->doubleSpinBoxPwrEDFA_4;
    EDFAObjsInMainUI[4].doubleSpinBoxCurMaxEDFA = ui->doubleSpinBoxCurMaxEDFA_4;
    EDFAObjsInMainUI[4].doubleSpinBoxCurSetEDFA = ui->doubleSpinBoxCurSetEDFA_4;

    EDFAObjsInMainUI[4].comboBoxSetModEDFA = ui->comboBoxSetModEDFA_4;
    EDFAObjsInMainUI[4].comboBoxSetPwrLvEDFA = ui->comboBoxSetPwrLvEDFA_4;
    EDFAObjsInMainUI[4].comboBoxSelectComEDFA = ui->comboBoxSelectComEDFA_4;
    EDFAObjsInMainUI[4].lineEditBaudRateComEDFA = ui->lineEditBaudRateComEDFA_4;
    EDFAObjsInMainUI[4].lineEditOpenDebuggerCodeEDFA = ui->lineEditOpenDebuggerCodeEDFA_4;
}
//实现DialogSystemCtrlUI中EDFA1模块：选中与未被选中工作模式之间颜色差异的函数
//void DialogSystemCtrl::on_comboBoxSetModEDFA_1_currentIndexChanged(int index){
//    /*if(index == 0){
//        EDFAObjsInMainUI[iTag].labelBgPwrEDFA->setStyleSheet("QLabel{background:rgba(0,255,0,120);}");
//        EDFAObjsInMainUI[iTag].labelBgCurSetEDFA->setStyleSheet("QLabel{background:rgba(0,0,0,0);}");
//    }
//    else if(index == 1){
//        EDFAObjsInMainUI[iTag].labelBgPwrEDFA->setStyleSheet("QLabel{background:rgba(0,0,0,0);}");
//        EDFAObjsInMainUI[iTag].labelBgCurSetEDFA->setStyleSheet("QLabel{background:rgba(0,255,0,120);}");
//    }*/
//}
void DialogSystemCtrl::connectSignalsWithMainHandlerEDFA(int iTag){

    //sigState
    connect(qdlEDFA[iTag], &DialogEDFA::sigStateEDFA, [=](int iState){
        if(iState == 1){
            EDFAObjsInMainUI[iTag].pushButtonSetParamEDFA->setText(QStringLiteral("设置工作参数"));
            EDFAObjsInMainUI[iTag].pushButtonSetParamEDFA->setEnabled(true);
            EDFAObjsInMainUI[iTag].pushButtonToggleWatchStateEDFA->setEnabled(true);
        }
        else if (iState == 2) {
            EDFAObjsInMainUI[iTag].pushButtonToggleWatchStateEDFA->setText(QStringLiteral("停止监控"));
        }
        else if (iState == 3) {
            EDFAObjsInMainUI[iTag].pushButtonToggleWatchStateEDFA->setText(QStringLiteral("监控模块状态"));
            EDFAObjsInMainUI[iTag].labelWatchIfOnEDFA->setText(QStringLiteral("未监控"));
            EDFAObjsInMainUI[iTag].labelWatchIfOnEDFA->setStyleSheet("QLabel{background:rgba(255,0,0,130);}");
        }
        else if (iState == -1) {
            EDFAObjsInMainUI[iTag].pushButtonToggleWatchStateEDFA->setText(QStringLiteral("串口未打开"));
            EDFAObjsInMainUI[iTag].labelWatchIfOnEDFA->setText(QStringLiteral("监控已停止"));
            EDFAObjsInMainUI[iTag].labelWatchIfOnEDFA->setStyleSheet("QLabel{background:rgba(255,0,0,130);}");
        }
    });

    //sigWatchResult
    connect(qdlEDFA[iTag], &DialogEDFA::sigWatchResultEDFA,
            [=](bool bIsAlive, float fEDFACur, float fEDFATmp, float fEDFAPwr){
        if(bIsAlive){
            EDFAObjsInMainUI[iTag].labelWatchIfOnEDFA->setText(QStringLiteral("已连接"));
            EDFAObjsInMainUI[iTag].labelWatchIfOnEDFA->setStyleSheet("QLabel{background:rgba(0,255,0,130);}");
        }
        else{
            EDFAObjsInMainUI[iTag].labelWatchIfOnEDFA->setText(QStringLiteral("未响应"));
            EDFAObjsInMainUI[iTag].labelWatchIfOnEDFA->setStyleSheet("QLabel{background:rgba(255,0,0,130);}");
        }
        EDFAObjsInMainUI[iTag].labelWatchPwrEDFA->setText(QString::number(fEDFAPwr, 'f', 1));
        EDFAObjsInMainUI[iTag].labelWatchCurEDFA->setText(QString::number(fEDFATmp, 'f', 1));
        EDFAObjsInMainUI[iTag].labelWatchTempEDFA->setText(QString::number(fEDFACur, 'f', 1));
    });
}

void DialogSystemCtrl::connectUIbuttonWithHandlerEDFA(int iTag){
    /************************这段使用lambda函数，负责扫描串口并向串口选择栏中增加可选状态******************/
    connect(EDFAObjsInMainUI[iTag].comboBoxSetPwrLvEDFA, &QComboBox::currentTextChanged, [=](){
        if(EDFAObjsInMainUI[iTag].comboBoxSetPwrLvEDFA->currentIndex() == 0){
            //15dBm version
            EDFAObjsInMainUI[iTag].doubleSpinBoxPwrEDFA->setMaximum(15);
            EDFAObjsInMainUI[iTag].doubleSpinBoxCurSetEDFA->setMaximum(500);
            EDFAObjsInMainUI[iTag].doubleSpinBoxCurMaxEDFA->setMaximum(500);
            EDFAObjsInMainUI[iTag].horizontalSliderPwrEDFA->setMaximum(150);
            EDFAObjsInMainUI[iTag].horizontalSliderCurSetEDFA->setMaximum(5000);
            EDFAObjsInMainUI[iTag].horizontalSliderCurMaxEDFA->setMaximum(5000);
        }
        else if(EDFAObjsInMainUI[iTag].comboBoxSetPwrLvEDFA->currentIndex() == 1){
            //15dBm version
            EDFAObjsInMainUI[iTag].doubleSpinBoxPwrEDFA->setMaximum(23);
            EDFAObjsInMainUI[iTag].doubleSpinBoxCurSetEDFA->setMaximum(1000);
            EDFAObjsInMainUI[iTag].doubleSpinBoxCurMaxEDFA->setMaximum(1000);
            EDFAObjsInMainUI[iTag].horizontalSliderPwrEDFA->setMaximum(230);
            EDFAObjsInMainUI[iTag].horizontalSliderCurSetEDFA->setMaximum(10000);
            EDFAObjsInMainUI[iTag].horizontalSliderCurMaxEDFA->setMaximum(10000);
        }

    });

    /************************这段使用lambda函数，负责扫描串口并向串口选择栏中增加可选状态******************/
    connect(EDFAObjsInMainUI[iTag].pushButtonSetParamEDFA, &QPushButton::clicked, [=](){
        EDFAObjsInMainUI[iTag].pushButtonSetParamEDFA->setText(QStringLiteral("正在设置"));
        EDFAObjsInMainUI[iTag].pushButtonSetParamEDFA->setEnabled(false);
        EDFAObjsInMainUI[iTag].pushButtonToggleWatchStateEDFA->setEnabled(false);
        EDFAObjsInMainUI[iTag].labelWatchModEDFA->setText(EDFAObjsInMainUI[iTag].comboBoxSetModEDFA->currentText());
        qdlEDFA[iTag]->handleMwSetEDFA(EDFAObjsInMainUI[iTag].comboBoxSetModEDFA->currentIndex(),
                                  EDFAObjsInMainUI[iTag].doubleSpinBoxCurMaxEDFA->value(),
                                  EDFAObjsInMainUI[iTag].doubleSpinBoxCurSetEDFA->value(),
                                  EDFAObjsInMainUI[iTag].doubleSpinBoxPwrEDFA->value());
    });
    /************************这段使用lambda函数，负责扫描串口并向串口选择栏中增加可选状态******************/
    connect(EDFAObjsInMainUI[iTag].pushButtonToggleWatchStateEDFA, &QPushButton::clicked, [=](){
        qdlEDFA[iTag]->handleMwToggleWatchEDFA();
    });
    /************************这段使用lambda函数，负责扫描串口并向串口选择栏中增加可选状态******************/
    connect(EDFAObjsInMainUI[iTag].pushButtonOpenDebuggerEDFA, &QPushButton::clicked, [=](){
        if(EDFAObjsInMainUI[1].lineEditOpenDebuggerCodeEDFA->text() == "6666")
            qdlEDFA[iTag]->show();
    });
    /************************这段使用lambda函数，负责扫描串口并向串口选择栏中增加可选状态******************/
    connect(EDFAObjsInMainUI[iTag].pushButtonScanComEDFA, &QPushButton::clicked, [=](){
        EDFAObjsInMainUI[iTag].comboBoxSelectComEDFA->clear();
        EDFAObjsInMainUI[iTag].comboBoxSelectComEDFA->addItems(qdlEDFA[iTag]->handleMwScanComEDFA());
    });
    /************************这段使用lambda函数，负责打开串口及改变Mw按钮状态***************************/
    connect(EDFAObjsInMainUI[iTag].pushButtonOpenComEDFA, &QPushButton::clicked, [=](){
        int status = qdlEDFA[iTag]->handleMwSetComEDFA(
        EDFAObjsInMainUI[iTag].comboBoxSelectComEDFA->currentText(), EDFAObjsInMainUI[iTag].lineEditBaudRateComEDFA->text().toInt());

        if(status == 0){
            EDFAObjsInMainUI[iTag].pushButtonSetParamEDFA->setEnabled(false);
            EDFAObjsInMainUI[iTag].pushButtonToggleWatchStateEDFA->setEnabled(false);
            EDFAObjsInMainUI[iTag].pushButtonOpenComEDFA->setText(QStringLiteral("打开串口"));
            EDFAObjsInMainUI[iTag].labelConnectStateComEDFA->setText(QStringLiteral("现已关闭"));
            EDFAObjsInMainUI[iTag].labelConnectStateComEDFA->setStyleSheet("QLabel{background:rgba(255,0,0,130);}");
        }
        else if (status == 1){
            EDFAObjsInMainUI[iTag].pushButtonSetParamEDFA->setEnabled(true);
            EDFAObjsInMainUI[iTag].pushButtonToggleWatchStateEDFA->setEnabled(true);
            EDFAObjsInMainUI[iTag].pushButtonOpenComEDFA->setText(QStringLiteral("关闭串口"));
            EDFAObjsInMainUI[iTag].labelConnectStateComEDFA->setText(QStringLiteral("现已打开"));
            EDFAObjsInMainUI[iTag].labelConnectStateComEDFA->setStyleSheet("QLabel{background:rgba(0,255,0,130);}");
        }
        else if(status == -1)
            EDFAObjsInMainUI[iTag].labelConnectStateComEDFA->setText(QStringLiteral("打开失败!"));
        else if(status == -2)
            EDFAObjsInMainUI[iTag].labelConnectStateComEDFA->setText(QStringLiteral("选择串口！"));
    });
}
void DialogSystemCtrl::connectUISliderWithSpinBoxEDFA(int iTag){
    /************************这段使用lambda函数，负责将Slider与SpinBox之间的值进行同步***************************/
    connect(EDFAObjsInMainUI[iTag].doubleSpinBoxCurMaxEDFA,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            [=](){
        EDFAObjsInMainUI[iTag].horizontalSliderCurMaxEDFA->setValue((int)(EDFAObjsInMainUI[iTag].doubleSpinBoxCurMaxEDFA->value()*10));
    });
    connect(EDFAObjsInMainUI[iTag].horizontalSliderCurMaxEDFA, &QSlider::valueChanged, [=](int value){
        EDFAObjsInMainUI[iTag].doubleSpinBoxCurMaxEDFA->setValue((float)value/10);
    });
    connect(EDFAObjsInMainUI[iTag].doubleSpinBoxPwrEDFA,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            [=](){
        EDFAObjsInMainUI[iTag].horizontalSliderPwrEDFA->setValue((int)(EDFAObjsInMainUI[iTag].doubleSpinBoxPwrEDFA->value()*10));
    });
    connect(EDFAObjsInMainUI[iTag].horizontalSliderPwrEDFA, &QSlider::valueChanged, [=](int value){
        EDFAObjsInMainUI[iTag].doubleSpinBoxPwrEDFA->setValue((float)value/10);
    });
    connect(EDFAObjsInMainUI[iTag].doubleSpinBoxCurSetEDFA,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            [=](){
        EDFAObjsInMainUI[iTag].horizontalSliderCurSetEDFA->setValue((int)(EDFAObjsInMainUI[iTag].doubleSpinBoxCurSetEDFA->value()*10));
    });
    connect(EDFAObjsInMainUI[iTag].horizontalSliderCurSetEDFA, &QSlider::valueChanged, [=](int value){
        EDFAObjsInMainUI[iTag].doubleSpinBoxCurSetEDFA->setValue((float)value/10);
    });
    /***********************End*****************/
}
