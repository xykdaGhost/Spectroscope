#include "dialogedfa.h"
#include "ui_dialogedfa.h"

DialogEDFA::DialogEDFA(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEDFA)
{
    ui->setupUi(this);
    qspEDFA = new myQSP();
    InitCMDs();

    qtmEDFAWch = new QTimer();
    qtmEDFAWch->setTimerType(Qt::PreciseTimer);
    qtmEDFAWch->setSingleShot(false);
    connect(qtmEDFAWch, SIGNAL(timeout()), this, SLOT(handleWatchEDFA()));
    connect(this, SIGNAL(sigWatchResultEDFA(bool, float, float, float)),
            this, SLOT(handleWatchResultEDFA(bool, float, float, float)));

    qtmEDFASet = new QTimer();
    qtmEDFASet->setSingleShot(true);
    qtmEDFASet->setTimerType(Qt::PreciseTimer);
    connect(qtmEDFASet, SIGNAL(timeout()), this, SLOT(handleSendSettingCommandEDFA()));
    connect(this, SIGNAL(sigStateEDFA(int)), this, SLOT(handleSigRespondEDFA(int)));

    ui->comboBoxMod->addItem("APC Mod");
    ui->comboBoxMod->addItem("ACC Mod");

    iManualState = 50;
    bNotSettingEDFA = 1;
    bWatchingEDFA = 0;
    bSpvsCMDCntPlusOne = 0;

    qthMultiTh = new QThread();

}

DialogEDFA::~DialogEDFA()
{
    delete ui;
}

/**********************************主窗口Mw调用的相关函数*****************************************************************/
void DialogEDFA::handleMwSetEDFA(int iSetMod, float fMaxCur, float fSetCur, float fSetPwr){
    parEDFA.iSetMod = iSetMod;
    parEDFA.fMaxCur = fMaxCur;
    parEDFA.fSetCur = fSetCur;
    parEDFA.fSetPwr = fSetPwr;
    //串口开启才能查询
    if(qspEDFA->isOpen()){
        //如果开启了系统状态查询，就给他关了
        if(bWatchingEDFA)
            handleMwToggleWatchEDFA();
        //防止Quest打断
        bNotSettingEDFA = 0;
        //开启新的线程调用handleSendSettingCommandEDFA，设置系统的工作模式
        handleSendSettingCommandEDFA();
    }
}
void DialogEDFA::handleMwToggleWatchEDFA(){
    if(bNotSettingEDFA){
        //没在进行设置
        bWatchingEDFA = !bWatchingEDFA;
        bSpvsCMDCntPlusOne = 0;
        qspEDFA->handleRecv();//清空一下系统接收
        if(bWatchingEDFA)            {
            qtmEDFAWch->start(200);
            emit sigStateEDFA(2);
        }
        else {
            qtmEDFAWch->stop();
            emit sigStateEDFA(3);
        }
    }
}
QStringList DialogEDFA::handleMwScanComEDFA(){
    qspEDFA->handleScan();
    return qspEDFA->qslSerialPortName;
}
int DialogEDFA::handleMwSetComEDFA(QString comSelected, int iBaudRate){
    //QString qsBaudRate = ui->lineEditBaudRate->text();//should be done in MW
    return qspEDFA->handleConnect(comSelected, iBaudRate);
}
/**********************************发送设置指令/接收+解析查询指令相关函数，解析有结果会发送sig***********************************/
//需要外置检测系统是否在设置状态
void DialogEDFA::handleWatchEDFA(){
    if(qspEDFA->isOpen()){
        QByteArray qbaSpvsRX;
        if(ui->checkBox->isChecked())
            ui->textBrowser->append("Watching");
        //没在设置，串口也打开了，可以监控模块状态，清空接收
        qspEDFA->handleRecv();
        //接收结束，应该发送下一条指令
        qspEDFA->handleSend(qvbaCMDTXs.at(3+bSpvsCMDCntPlusOne));
        bSpvsCMDCntPlusOne = !bSpvsCMDCntPlusOne;
        //如果收到的串口数据够长，可以进行解析，否则会出现越界
        if(qspEDFA->qbaAllRead.count() >= 10){
            //这个EDFA还活着，bool bIsModAlive = true指示EDFA还有响应
            parEDFA.bIsModAlive = true;
            //对原始数据进行解析
            qbaSpvsRX = qspEDFA->qbaAllRead;
            if(qbaSpvsRX.at(4) == 0x11)
            {
                int iEDFACur = (uchar)qbaSpvsRX.at(5)*256 + (uchar)qbaSpvsRX.at(6);
                parEDFA.fQstCur = (float)iEDFACur/10;

                int iEDFATmp = (uchar)qbaSpvsRX.at(9)*256 + (uchar)qbaSpvsRX.at(10);
                parEDFA.fQstTmp = (float)iEDFATmp/10;
            }
            else if(qbaSpvsRX.at(4) == 0x20)
            {
                int iEDFAPwr = (uchar)qbaSpvsRX.at(7)*256 + (uchar)qbaSpvsRX.at(8);
                parEDFA.fQstPwr = (float)iEDFAPwr/10 - 70;

            }
            //如果是调试模式，将十六进制显示出来
            if(ui->checkBox->isChecked()){
                qbaSpvsRX.clear();
                qbaSpvsRX = qspEDFA->qbaAllRead.toHex();
                ui->textBrowser->append(qbaSpvsRX);
            }
        }
        //如果没收到=模块无响应，寄了
        else
            parEDFA.bIsModAlive = false;

        //完成了一次系统参数查询，应该发送Signal
        emit sigWatchResultEDFA(parEDFA.bIsModAlive, parEDFA.fQstCur, parEDFA.fQstTmp, parEDFA.fQstPwr);
    }
    else {
        //串口未打开
        emit sigStateEDFA(-1);
    }
}
void DialogEDFA::handleSendSettingCommandEDFA(){
    if(ui->checkBox->isChecked())
        ui->textBrowser->append(QByteArray::number(iManualState));
    qspEDFA->handleRecv();
    if(iManualState == 50){
        if(parEDFA.iSetMod == 1){
            qvbaCMDTXs[5].data()[6] = 0x00;
            qvbaCMDTXs[5].data()[8] = 0x00;
            iManualState = 70;
        }//iMod = 0 for APC Mod
        else if (parEDFA.iSetMod == 0) {
            qvbaCMDTXs[5].data()[6] = 0x02;
            qvbaCMDTXs[5].data()[8] = 0x02;
            iManualState = 110;
        }
        qspEDFA->handleSend(SumCheck(qvbaCMDTXs.at(5)));
        qtmEDFASet->start(50);
    }
    else if (iManualState == 70) {
        int iSetCur = 10 * parEDFA.fSetCur;
        qvbaCMDTXs[7].data()[6] = iSetCur/256;
        qvbaCMDTXs[7].data()[8] = iSetCur%256;
        qspEDFA->handleSend(SumCheck(qvbaCMDTXs.at(7)));
        iManualState = 100;
        qtmEDFASet->start(50);
    }
    else if (iManualState == 100) {
        int iSetCur = 10 * parEDFA.fMaxCur;
        qvbaCMDTXs[10].data()[6] = iSetCur/256;
        qvbaCMDTXs[10].data()[8] = iSetCur%256;
        qspEDFA->handleSend(SumCheck(qvbaCMDTXs.at(10)));

        iManualState = 0;
        qtmEDFASet->start(50);
    }
    else if (iManualState == 110) {
        int iSetCur = 10 * parEDFA.fSetPwr + 700;
        qvbaCMDTXs[11].data()[5] = iSetCur/256;
        qvbaCMDTXs[11].data()[6] = iSetCur%256;
        qspEDFA->handleSend(SumCheck(qvbaCMDTXs.at(11)));

        iManualState = 0;
        qtmEDFASet->start(50);
    }
    else if (iManualState == 0) {
        //所有设置过程中需要发送的指令全部发送完毕，Mw应该开始Watch系统参数，恢复Disabled按钮
        iManualState = 50;
        emit sigStateEDFA(1);
    }
}
/*******************************************调试窗口响应sig，与UI显示相关参数的函数*********************************************/
void DialogEDFA::handleSigRespondEDFA(int iState){
    if(iState == 1){
        bNotSettingEDFA = 1;
        ui->pushButtonSet->setEnabled(true);
        ui->pushButtonSpvsToggle->setEnabled(true);
        ui->pushButtonSet->setText("Set");
        on_pushButtonSpvsToggle_clicked();
    }
    else if (iState == 2) {
        ui->pushButtonSpvsToggle->setText("Periodic Update State:Y");//监控模块状态
    }
    else if (iState == 3) {
        ui->pushButtonSpvsToggle->setText("Periodic Update State:N");//停止监控模块状态
    }
    else if (iState == -1) {
        ui->pushButtonSpvsToggle->setText("Periodic Update State:N");//未开启串口
    }
}
void DialogEDFA::handleWatchResultEDFA(bool bIsAlive, float fEDFACur, float fEDFATmp, float fEDFAPwr){
    if(ui->checkBox->isChecked()){
        ui->textBrowser->append(QByteArray::number(bIsAlive));
        ui->textBrowser->append("Watched");
    }
    ui->labelCur->setText(QString::number(fEDFACur, 'f', 1));
    ui->labelTmp->setText(QString::number(fEDFATmp, 'f', 1));
    ui->labelPwr->setText(QString::number(fEDFAPwr, 'f', 1));
}
/**************************************调试窗口串口扫描/打开，及参数设置切换查询状态相关函数*****************************************/
void DialogEDFA::on_pushButtonSet_clicked(){
    ui->pushButtonSet->setEnabled(false);
    ui->pushButtonSpvsToggle->setEnabled(false);
    ui->pushButtonSet->setText("Setting");
    handleMwSetEDFA(ui->comboBoxMod->currentIndex(),
                    ui->doubleSpinBoxMaxCur->value(),
                    ui->doubleSpinBoxSetCur->value(),
                    ui->doubleSpinBoxSetPwr->value());
}
//管切换是否监视系统状态的
void DialogEDFA::on_pushButtonSpvsToggle_clicked(){
    handleMwToggleWatchEDFA();
}
void DialogEDFA::on_pushButtonScan_clicked(){
    qspEDFA->handleScan();
    ui->comboBox->clear();
    ui->comboBox->addItems(qspEDFA->qslSerialPortName);
}
void DialogEDFA::on_pushButtonOpen_clicked(){
    QString qsBaudRate = ui->lineEditBaudRate->text();
    int status = qspEDFA->handleConnect(ui->comboBox->currentText(), qsBaudRate.toInt());
    if(status == 0){
        if(bWatchingEDFA)
            on_pushButtonSpvsToggle_clicked();

        ui->pushButtonOpen->setText("Open");
        ui->labelConnect->setText("Now Closed");

        ui->textBrowser->append(ui->comboBox->currentText());
        ui->textBrowser->append("Closed at ");
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
        ui->textBrowser->append(current_date);
    }
    else if (status == 1){
        ui->pushButtonOpen->setText("Colse");
        ui->labelConnect->setText("Now Opened");

        ui->textBrowser->append(ui->comboBox->currentText());
        ui->textBrowser->append("Opened at ");
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
        ui->textBrowser->append(current_date);
        ui->textBrowser->append("Start Supervising");
    }
    else if(status == -1)
        ui->labelConnect->setText("Failed to Open");
    else if(status == -2)
        ui->labelConnect->setText("Select a COM!");
}
/*************************************************EDFA指令相关*********************************************************/
//初始化EDFA相关指令
void DialogEDFA::InitCMDs(){
    //初始化TX指令
    qvbaCMDTXs.append("000000");
    qvbaCMDTXs.append("000000");
    qvbaCMDTXs.append("000000");
    //3~4:系统参数监视，已加校验和位
    qvbaCMDTXs.append(QByteArray::fromHex("EFEF03FF11F1"));//3、读取980nm泵浦的参数
    qvbaCMDTXs.append(QByteArray::fromHex("EFEF03FF2000"));//4、读光功率
    //5~6:Mode Set
    qvbaCMDTXs.append(QByteArray::fromHex("EFEF0AFF5320002000200D15"));//5、设置工作模式
    qvbaCMDTXs.append(QByteArray::fromHex("EFEF05FF51202073"));//6、读取当前工作模式设置值/校验和0x
    //7~10:ACC Mode
    qvbaCMDTXs.append(QByteArray::fromHex("EFEF0AFF5222002300200D15"));//7、设置ACC模式下980nm泵浦工作电流
    qvbaCMDTXs.append(QByteArray::fromHex("EFEF0AFF5122237D"));//8、读取ACC模式下980nm泵浦工作电流设置值
    qvbaCMDTXs.append(QByteArray::fromHex("EFEF0AFF5103043F"));//9、读取ACC模式下980nm泵浦工作电流设置最大值
    qvbaCMDTXs.append(QByteArray::fromHex("EFEF0AFF5426002700200D15"));//10、设置ACC模式下980nm泵浦工作电流最大值
    //11~13:APC Mode
    qvbaCMDTXs.append(QByteArray::fromHex("EFEF05FF800000"));//11、Set APC Power
    qvbaCMDTXs.append(QByteArray::fromHex("EFEF03FF8161"));//12、读取 Set APC Power 设置值
    qvbaCMDTXs.append(QByteArray::fromHex("EFEF03FF82"));//13、读取 Set APC Power 设置值最小值和最大值
    /*for(int iQBAs = qvbaCMDTXs.count()-1; iQBAs >= 0; iQBAs --){
        int iSumCheck = 0;
        for(int iChars = qvbaCMDTXs[iQBAs].count(); iChars >= 0; iChars--)
            iSumCheck += qvbaCMDTXs[iQBAs].data()[iChars];
        qvbaCMDTXs[iQBAs].append(iSumCheck);
    }*/
    //初始化RX指令
    qvbaCMDRXs.append("000000");
    qvbaCMDRXs.append("000000");
    qvbaCMDRXs.append("000000");
    qvbaCMDRXs.append(QByteArray::fromHex("EDFA51525354"));
}
QByteArray DialogEDFA::SumCheck(QByteArray SC){
    int iSumCheck = 0;
    for(int iChars = SC.count(); iChars >= 0; iChars--)
        iSumCheck += SC.data()[iChars];
    SC.append(iSumCheck);
    return SC;
}
