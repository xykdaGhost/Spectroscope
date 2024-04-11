#include "dialoglaser1.h"
#include "ui_dialoglaser1.h"

DialogLASER1::DialogLASER1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLASER1)
{
    ui->setupUi(this);
    qspLASER1 = new myQSP();
    InitCMDs();

    //开启50ms中断，每20个循环为一周期
    parLASER1.qtmMainTimerLASER1 = new QTimer();
    parLASER1.qtmMainTimerLASER1->setSingleShot(false);
    parLASER1.qtmMainTimerLASER1->setTimerType(Qt::PreciseTimer);
    connect(parLASER1.qtmMainTimerLASER1, SIGNAL(timeout()), this, SLOT(handleMainTimerUpdateLASER1()));
    parLASER1.qtmMainTimerLASER1->start(50);//should be 50

    //连接响应函数与解析信号
    connect(this, SIGNAL(sigWatchResultLaser(bool, bool, bool,
                                             bool, float, float, float)),
            this, SLOT(sigWatchResultLocalHandler(bool, bool, bool,
                                                  bool, float, float, float)));

    //ui部分handleTimerUpdate
    ui->labelSnglFreqSetPwrLaser_1->setToolTip("Its Pwr ranges 6dBm~13.5dBm, stepping 0.01dBm");
    ui->labelSnglFreqSetPwrLaser_1->setToolTipDuration(30000);
    ui->labelSnglFreqSetWvlnLaser_1->setToolTip("Its Wavelength ranges 1520.9nm~1580.6nm, stepping 0.1nm");
    //    ui->labelSnglFreqSetWvlnLaser_1->setToolTip("Its Freq ranges 189.8000THz~197.2500THz, stepping 0.0001THz");
    ui->labelSnglFreqSetWvlnLaser_1->setToolTipDuration(30000);
    //ui->pushButtonSet->setToolTip("Caution: SetParam Will Disable LASER1 Output!!");
    //ui->pushButtonSet->setToolTipDuration(30000);

/*    ui->pushButtonSet->setEnabled(false);
    ui->pushButtonSpvsToggle->setEnabled(false);
    ui->pushButtonEnableScan->setEnabled(false);
    ui->pushButtonEnableLaser->setEnabled(false);*/

    parLASER1.bQstIsAlive = false;
    parLASER1.bQstIsOutput = false;
    parLASER1.bQstIsStable = false;
    parLASER1.bComIsOpened = false;

    parLASER1.bWantEnableLASER1 = false;
    parLASER1.bWantSetParLASER1 = true;
    //connectSignalsWithLocalHandler

    parLASER1.iCountPeriod = 0;

}
DialogLASER1::~DialogLASER1(){
    delete ui;
}
/********************************** 主函数窗口Mw调用的函数 ****************************************************/
QStringList DialogLASER1::handleMwScanComLASER1(){
    qspLASER1->handleScan();
    return qspLASER1->qslSerialPortName;
}
int DialogLASER1::handleMwSetComLASER1(QString comSelected, int iBaudRate){
    //QString qsBaudRate = ui->lineEditBaudRate->text();//should be done in MW
    return qspLASER1->handleConnect(comSelected, iBaudRate);
}
void DialogLASER1::handleMwToggleLASER1(){
    parLASER1.bWantEnableLASER1 = !parLASER1.bWantEnableLASER1;
}

void DialogLASER1::handleMwSetParLASER1(float fSetWavln, float fSetPwr){
    float fSetFrq;
    fSetFrq = (float)300000 / fSetWavln;
    TextDebugOut(QString::number(fSetFrq, 'f', 4));
    TextDebugOut(QString::number(fSetPwr, 'f', 2));
    parLASER1.fSetPwr = fSetPwr;
    parLASER1.qvfSetFrq.append(fSetFrq);
    parLASER1.bWantSetParLASER1 = true;
}
void DialogLASER1::handleMwScnFrqLASER1(float fScanStart, float fScanEnd, float fStep, float fWantScanSecond, float fSetPwr, bool bIsFreqScan) {
    //Enable output, then only setting waveln is needed
    parLASER1.bWantEnableLASER1 = true;
    parLASER1.fSetPwr = fSetPwr;
    //计数多长时间后跳转频率
    parLASER1.iWantStablePeriod = fWantScanSecond * 20;
    //生成所有扫描点的队列
    parLASER1.qvfSetFrq.clear();
    //以频率作为步长
    if(bIsFreqScan) {
        if(fScanStart < fScanEnd){
            //增量式扫描点
            parLASER1.qvfSetFrq.append(fScanStart);
            while (fScanStart < fScanEnd) {
                parLASER1.qvfSetFrq.append(fScanStart);
                fScanStart += fStep;
            }
            parLASER1.qvfSetFrq.append(fScanEnd);
        }
        else if (fScanStart > fScanEnd){
            //减量式扫描点
            parLASER1.qvfSetFrq.append(fScanStart);
            fStep = - fStep;
            while (fScanStart > fScanEnd) {
                parLASER1.qvfSetFrq.append(fScanStart);
                fScanStart += fStep;
            }
            parLASER1.qvfSetFrq.append(fScanEnd);
        }
        else if(fScanStart == fScanEnd){
            //怎么会有蠢蛋在扫描模式里让起止点相等，还扫der
            parLASER1.qvfSetFrq.append(fScanEnd);
        }
    }
    //以波长作为步长
    else {
        if(fScanStart < fScanEnd){
            //增量式扫描点
            parLASER1.qvfSetFrq.append(300000/fScanStart);
            while (fScanStart < fScanEnd) {
                parLASER1.qvfSetFrq.append(300000/fScanStart);
                fScanStart += fStep;
            }
            parLASER1.qvfSetFrq.append(300000/fScanEnd);
        }
        else if (fScanStart > fScanEnd){
            //减量式扫描点
            parLASER1.qvfSetFrq.append(300000/fScanStart);
            fStep = - fStep;
            while (fScanStart > fScanEnd) {
                parLASER1.qvfSetFrq.append(300000/fScanStart);
                fScanStart += fStep;
            }
            parLASER1.qvfSetFrq.append(300000/fScanEnd);
        }
        else if(fScanStart == fScanEnd){
            //怎么会有蠢蛋在扫描模式里让起止点相等，还扫der
            parLASER1.qvfSetFrq.append(300000/fScanEnd);
        }
    }
//    while (parLASER1.qvfSetFrq.count() > 0) {
//        TextDebugOut(QString::number(parLASER1.qvfSetFrq.front(), 'f', 4));
//        parLASER1.qvfSetFrq.pop_front();
//    }//这是一截测试扫描点数序列生成是否正常的程序，会把parLASER1.qvfSetFrq依次打印在公屏上
}
/********************************** 激光器参数接收及解析函数 ****************************************************/
void DialogLASER1::handleAnlyzRecvLASER1(){
    if(qspLASER1->handleIsOpen()){
        //TextDebugOut("Com Opened");
        parLASER1.bComIsOpened = true;
        //串口已打开，收一下串口数据
        qspLASER1->handleRecv();
        //如果收到包大小为4即可视作正常接收，进行解析
        if(qspLASER1->qbaAllRead.count() == 4){
            QByteArray qbaAnalysis = qspLASER1->qbaAllRead;
            //把收到的数据以Hex打在公屏上
            TextDebugOut(qbaAnalysis.toHex());
            //激光器确认存活
            parLASER1.bQstIsAlive = true;

            //收包解析
            //激光器的回包读取第二字节即可解析
            //1、是Nop包，一般情况下54000010确认激光器存活，但是也可以用于判定激光器是否稳定工作
            if((uchar)qbaAnalysis.at(1) == 0x00){
                //44000110是激光器输出但未稳定时返回的包，一旦在这种包后面接收到了54000010即可认为激光器稳定工作
                if((uchar)qbaAnalysis.at(2) == 0x00){
                    if(parLASER1.bQstIsOutput)
                        parLASER1.bQstIsStable = true;
                }
                else if ((uchar)qbaAnalysis.at(2) == 0x01)
                    parLASER1.bQstIsStable = false;
            }
            else if ((uchar)qbaAnalysis.at(1) == 0x30) {
                //设置激光器Channel的回包，好像没什么用
            }
            //2、查询/设置激光器输出后的回包
            else if ((uchar)qbaAnalysis.at(1) == 0x32) {
                //08仅代表激光器在输出，无法判定输出是否稳定，需要结合54000010判断
                if((uchar)qbaAnalysis.at(3) == 0x08){
                    parLASER1.bQstIsOutput = true;
                }
                //00代表无输出
                else if ((uchar)qbaAnalysis.at(3) == 0x00) {
                    parLASER1.bQstIsOutput = false;
                }
            }
            //35和36都是回包，感觉也没大用，就不写了
            //3、40为查询当前输出频率的THz整数部分
            else if ((uchar)qbaAnalysis.at(1) == 0x40) {
                parLASER1.iQstFrqUpr = (uchar)qbaAnalysis.at(3);
            }
            //4、41为查询当前输出频率的THz小数部分
            else if ((uchar)qbaAnalysis.at(1) == 0x41) {
                parLASER1.iQstFrqLwr = (uchar)qbaAnalysis.at(2)*256 + (uchar)qbaAnalysis.at(3);
                parLASER1.fQstFrq = parLASER1.iQstFrqLwr / 10000.0;
                parLASER1.fQstFrq += parLASER1.iQstFrqUpr;
            }
            //5、42为输出功率
            else if ((uchar)qbaAnalysis.at(1) == 0x42) {
                parLASER1.iQstPwr = (uchar)qbaAnalysis.at(2)*256 + (uchar)qbaAnalysis.at(3);
                parLASER1.fQstPwr = parLASER1.iQstPwr / 100.0;
                //TextDebugOut(QString::number(iQstPwr));
            }
            //6、43为当前温度
            else if ((uchar)qbaAnalysis.at(1) == 0x43) {
                parLASER1.iQstTmp = (uchar)qbaAnalysis.at(2)*256 + (uchar)qbaAnalysis.at(3);
                //TextDebugOut(QString::number(iQstTmp));
                parLASER1.fQstTmp = parLASER1.iQstTmp / 100.0;
                TextDebugOut("Emitting Signal");
                TextDebugOut("Quest Result");
                TextDebugOut(QString::number(parLASER1.fQstFrq, 'f', 4));
                TextDebugOut(QString::number(parLASER1.fQstPwr, 'f', 2));
                emit sigWatchResultLaser(parLASER1.bComIsOpened, parLASER1.bQstIsOutput, parLASER1.bQstIsStable,
                                         parLASER1.bQstIsAlive, parLASER1.fQstFrq, parLASER1.fQstTmp, parLASER1.fQstPwr);
            }
        }
        else{
            //串口开着，但是什么都没收到，激光器挂了
            parLASER1.bQstIsAlive = false;
            parLASER1.bQstIsOutput = false;
            parLASER1.bQstIsStable = false;
            emit sigWatchResultLaser(parLASER1.bComIsOpened, parLASER1.bQstIsOutput, parLASER1.bQstIsStable,
                                     parLASER1.bQstIsAlive, parLASER1.fQstFrq, parLASER1.fQstTmp, parLASER1.fQstPwr);
        }
    }
    else{
        //串口关了
        //TextDebugOut("Com Closed");
        parLASER1.bQstIsAlive = false;
        parLASER1.bQstIsOutput = false;
        parLASER1.bQstIsStable = false;
        parLASER1.bComIsOpened = false;
        emit sigWatchResultLaser(parLASER1.bComIsOpened, parLASER1.bQstIsOutput, parLASER1.bQstIsStable,
                                 parLASER1.bQstIsAlive, parLASER1.fQstFrq, parLASER1.fQstTmp, parLASER1.fQstPwr);
    }
}
/********************************** 响应解析激光器参数结果，并显示至UI的函数 ****************************************************/
void DialogLASER1::sigWatchResultLocalHandler(bool bIsComOpen, bool bIsOutput, bool bIsStable,
                                              bool bIsAlive, float fQstFrq, float fQstTmp, float fQstPwr){
    if(bIsComOpen) {
        if(bIsAlive) {
            //有输出，可以显示功率，输出是否稳定，稳定后还可以开启用户手动设置
            //TextDebugOut("Laser Alive");
            if(bIsOutput){
                //ui->pushButtonEnableLaser->setText("DisableLASER1");
                if(bIsStable){
                    ui->labelWatchIfOnLaser_1->setText(QStringLiteral("输出稳定")); //输出稳定
                    //TextDebugOut("LASER1 Nrom Nop");
                    /*//如果是非扫描模式，允许用户手动设置
                    if(!bWantLASER1Scan){
                        ui->pushButtonSet->setEnabled(true);
                        ui->pushButtonSpvsToggle->setEnabled(true);
                        ui->pushButtonEnableScan->setEnabled(true);
                        ui->pushButtonEnableLaser->setEnabled(true);
                    }这是甚么代码来着*/
                }
                else{
                    TextDebugOut("LASER1 Unstable Output");
                    ui->labelWatchIfOnLaser_1->setText(QStringLiteral("输出未稳定"));
                }
                //既然在输出那就可以显示功率
                ui->labelWatchPwrLaser_1->setText(QString::number(fQstPwr, 'f', 2));
            }
            else{
                ui->labelWatchPwrLaser_1->setText("NAN");
                ui->labelWatchIfOnLaser_1->setText("No Output");
                //ui->pushButtonEnableLaser->setText("EnableLASER1");
            }
            //不管有没有输出，频率都是可以显示的
            float fQstWaveln = 300000/fQstFrq;
            ui->labelWatchWavelnLaser_1->setText(QString::number(fQstWaveln, 'f', 2));
            ui->labelWatchTempLaser_1->setText(QString::number(fQstTmp, 'f', 2));
        }
        else {
            //激光器挂了
            ui->labelWatchIfOnLaser_1->setText("Laser Offline");
            ui->labelWatchWavelnLaser_1->setText("NAN");
            ui->labelWatchTempLaser_1->setText("NAN");
            ui->labelWatchPwrLaser_1->setText("NAN");
        }
    }
    else{
        //串口就没开
    }
}
/********************************** 负责向串口待发送队列添加命令的函数 ****************************************************/
void DialogLASER1::handlePushComCMDLASER1() {
    //调用这个函数的时候一定是队列空了，应该优先满足设置的要求
    //如果希望设置激光器参数，则在发送队列中加入相关指令
    if (parLASER1.bWantSetParLASER1) {
        if(!parLASER1.qvfSetFrq.isEmpty()) {
            //把需要发送的相关数据都给他塞进去：

            //由float->int的数据转换
            float fSetFrq = parLASER1.qvfSetFrq.takeFirst();
            parLASER1.iSetPwrOut = parLASER1.fSetPwr * 100.0;
            parLASER1.iSetFrqUpr = fSetFrq;
            parLASER1.iSetFrqLwr = fSetFrq * 10000;
            parLASER1.iSetFrqLwr = parLASER1.iSetFrqLwr % 10000;
            //设置激光器功率
            parLASER1.qvbaCMDsetNorm[2].data()[3] = parLASER1.iSetPwrOut % 256;
            parLASER1.qvbaCMDsetNorm[2].data()[2] = parLASER1.iSetPwrOut / 256;
            //设置激光器频率的整数部分
            parLASER1.qvbaCMDsetNorm[3].data()[3] = parLASER1.iSetFrqUpr;
            //设置激光器频率的小数部分
            parLASER1.qvbaCMDsetNorm[4].data()[3] = parLASER1.iSetFrqLwr % 256;
            parLASER1.qvbaCMDsetNorm[4].data()[2] = parLASER1.iSetFrqLwr / 256;
            //disable first
            parLASER1.qvbaCMDsendQueue.append(parLASER1.qvbaCMDsetNorm.at(6));
            //进行设置
            parLASER1.qvbaCMDsendQueue.append(parLASER1.qvbaCMDsetNorm.at(2));
            parLASER1.qvbaCMDsendQueue.append(parLASER1.qvbaCMDsetNorm.at(3));
            parLASER1.qvbaCMDsendQueue.append(parLASER1.qvbaCMDsetNorm.at(4));
            parLASER1.qvbaCMDsendQueue.append(parLASER1.qvbaCMDsetNorm.at(0));
            //enable then
            //如果希望使能激光器输出，则在发送队列中加入相关指令
            if(parLASER1.bWantEnableLASER1)
                parLASER1.qvbaCMDsendQueue.append(parLASER1.qvbaCMDsetNorm.at(5));
            else
                parLASER1.qvbaCMDsendQueue.append(parLASER1.qvbaCMDsetNorm.at(6));
            //塞完了，别再塞了
            TextDebugOut(QString::number(fSetFrq, 'f', 4));
            TextDebugOut(QString::number(parLASER1.fSetPwr, 'f', 2));
            parLASER1.bWantSetParLASER1 = false;
        }
    }
    else {
        //如果希望使能激光器输出，则在发送队列中加入相关指令
        if(parLASER1.bWantEnableLASER1)
            parLASER1.qvbaCMDsendQueue.append(parLASER1.qvbaCMDsetNorm.at(5));
        else
            parLASER1.qvbaCMDsendQueue.append(parLASER1.qvbaCMDsetNorm.at(6));

    }
    //观察激光器参数，则在发送队列中加入相关指令
    parLASER1.qvbaCMDsendQueue += parLASER1.qvbaCMDqstNorm;
}
/********************************** 激光器定时执行的函数 ****************************************************/
void DialogLASER1::handleMainTimerUpdateLASER1(){
    //先走接收流程
    //TextDebugOut("Timer");
    handleAnlyzRecvLASER1();
    //扫频计时器
    if(parLASER1.bQstIsStable) {
        parLASER1.iCountPeriod ++;
    }
    //如果串口还开着
    if(parLASER1.bComIsOpened) {
        //如果待发送队列为空，则需要填充一些指令
        if(parLASER1.qvbaCMDsendQueue.isEmpty()) {
            //TextDebugOut("Filling CmdQue");
            if(!parLASER1.qvfSetFrq.isEmpty()) {
                //需要加上等待输出稳定时间的判断条件
                if(parLASER1.iCountPeriod >= parLASER1.iWantStablePeriod) {
                    //parLASER1.bWantEnableLASER1 = true;
                    parLASER1.bWantSetParLASER1 = true;
                    parLASER1.iCountPeriod = 0;
                }
            }
            else {
                //无待发频点
                parLASER1.iWantStablePeriod = 100000;
            }
            handlePushComCMDLASER1();
        }
        //可以发送串口指令
        if(!parLASER1.qvbaCMDsendQueue.isEmpty())
            qspLASER1->handleSend(this->SumCheck(parLASER1.qvbaCMDsendQueue.takeFirst()));
    }
}

void DialogLASER1::on_pushButtonMultiFreqEnableLaser_1_clicked()
{
    parLASER1.qvfSetFrq.clear();
    ui->textBrowser->clear();
    handleMwScnFrqLASER1(ui->doubleSpinBoxStartWavelnLaser_1->value(),
                         ui->doubleSpinBoxEndWavelnLaser_1->value(),
                         ui->doubleSpinBoxSteplnLaser_1->value(),
                         ui->doubleSpinBoxStableDurLaser_1->value(),
                         ui->doubleSpinBoxMultiFreqSetPwrLaser_1->value(),
                         false);
}

void DialogLASER1::on_pushButtonSnglFreqSetLaser_1_clicked()
{
    ui->labelWatchIfOnLaser_1->setText(QStringLiteral("正在设置参数")); //输出稳定
    this->handleMwSetParLASER1(ui->doubleSpinBoxSnglFreqSetWvlnLaser_1->value(),
                               ui->doubleSpinBoxSnglFreqSetPwrLaser_1->value());
}
void DialogLASER1::on_pushButtonSnglFreqEnableLaser_1_clicked(){
    handleMwToggleLASER1();
    if(parLASER1.bWantEnableLASER1){
//        ui->labelWatchIfOnLaser_1->setText(QStringLiteral("正在设置参数")); //输出稳定
        ui->labelWatchIfOnLaser_1->setText("Toggle Output State");
    }
}

//下面的代码也不用动，是管串口扫描和打开的
void DialogLASER1::on_pushButtonScanComLaser_1_clicked()
{
    qspLASER1->handleScan();
    ui->comboBoxSelectComLaser_1->clear();
    ui->comboBoxSelectComLaser_1->addItems(qspLASER1->qslSerialPortName);
}
void DialogLASER1::on_pushButtonOpenComLaser_1_clicked()
{
    QString qsBaudRate = ui->lineEditBaudRateComLaser_1->text();
    int status = qspLASER1->handleConnect(ui->comboBoxSelectComLaser_1->currentText(), qsBaudRate.toInt());
    if(status == 0)
    {
        ui->pushButtonOpenComLaser_1->setText("Open");
        ui->labelConnectStateComLaser_1->setText("Now Closed");

        TextDebugOut(ui->comboBoxSelectComLaser_1->currentText());
        TextDebugOut("Closed at ");
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
        TextDebugOut(current_date);
    }
    else if (status == 1)
    {
        ui->pushButtonOpenComLaser_1->setText("Colse");
        ui->labelConnectStateComLaser_1->setText("Now Opened");

        TextDebugOut(ui->comboBoxSelectComLaser_1->currentText());
        TextDebugOut("Opened at ");
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
        TextDebugOut(current_date);
        TextDebugOut("Start Supervising");
    }
    else if(status == -1)
        ui->labelConnectStateComLaser_1->setText("Failed to Open");
    else if(status == -2)
        ui->labelConnectStateComLaser_1->setText("Select a COM!");
}
//用于区别希望输出调试信息与否
void DialogLASER1::TextDebugOut(QString SC){
    if(ui->checkBox->isChecked())
        ui->textBrowser->append(SC);
}
/********************************** Laser指令的相关函数 Begin *****************************************************************/
//初始化LASER1相关指令
void DialogLASER1::InitCMDs(){
    //一、激光器参数设置指令及其初始化Start Questing
    //部分设置相关指令具有动态的参数，因此需要独立计算头4位的校验和
    //激光器频道及输出与否为固定指令及校验和
    parLASER1.qvbaCMDsetNorm.append(QByteArray::fromHex("00000000"));//0、确认有LASER1活着，对设置指令集来说没什么用
    parLASER1.qvbaCMDsetNorm.append(QByteArray::fromHex("31300001"));//1、设置激光器频道，默认为1
    //设置激光器输出功率，校验和要动态计算
    parLASER1.qvbaCMDsetNorm.append(QByteArray::fromHex("01310000"));//2、设置输出功率
    //分两步设置激光器频率，校验和要动态计算
    parLASER1.qvbaCMDsetNorm.append(QByteArray::fromHex("01350000"));//3、设置频率THz部分
    parLASER1.qvbaCMDsetNorm.append(QByteArray::fromHex("01360000"));//4、设置频率100MHz部分

    //二、激光器使能指令
    parLASER1.qvbaCMDsetNorm.append(QByteArray::fromHex("81320008"));//5、使能激光器输出
    parLASER1.qvbaCMDsetNorm.append(QByteArray::fromHex("01320000"));//6、失能激光器输出

    //三、激光器实时参数查询指令及其初始化
    parLASER1.qvbaCMDqstNorm.append(QByteArray::fromHex("00000000"));//0、确认是否有LASER1活着
    parLASER1.qvbaCMDqstNorm.append(QByteArray::fromHex("10320000"));//1、查询激光器是否输出
    parLASER1.qvbaCMDqstNorm.append(QByteArray::fromHex("40400000"));//2、查询频率THz部分
    parLASER1.qvbaCMDqstNorm.append(QByteArray::fromHex("50410000"));//3、查询频率100MHz部分
    parLASER1.qvbaCMDqstNorm.append(QByteArray::fromHex("60420000"));//4、查询激光器的输出功率，输出未使能时为8000
    parLASER1.qvbaCMDqstNorm.append(QByteArray::fromHex("70430000"));//5、查询激光器温度


    //四、激光器系统参数查询指令及其初始化    50 41 00 00
    //寄存器50~55为系统的参数，一般闲的没事用不着，在此记录一下以备不时之需
    //查询激光器的最大/最小可设置功率
    parLASER1.qvbaCMDqstSys.append(QByteArray::fromHex("50500000"));//查询最小可设置功率，结果258对应600即6dBm
    parLASER1.qvbaCMDqstSys.append(QByteArray::fromHex("40510000"));//查询最大可设置功率，结果546对应1350即13.5dBm
    //查询激光器的最大/最小可设置输出频率，经实测为189.800THz~197.250THz
    parLASER1.qvbaCMDqstSys.append(QByteArray::fromHex("70520000"));//查询最小频率THz部分，结果00BD；对应189THz
    parLASER1.qvbaCMDqstSys.append(QByteArray::fromHex("60530000"));//查询最小频率100MHz部分，结果1F40即8000；对应800GHz
    parLASER1.qvbaCMDqstSys.append(QByteArray::fromHex("10540000"));//查询最大频率THz部分，结果00C5；对应197THz
    parLASER1.qvbaCMDqstSys.append(QByteArray::fromHex("00550000"));//查询最大频率100MHz部分，结果09C4即2500；对应250GHz

}
//用于生成信息包第一个字节前4位校验
QByteArray DialogLASER1::SumCheck(QByteArray SC){
    uchar bip8 = ((uchar)SC.at(0) & 0x0F) ^ (uchar)SC.at(1) ^ (uchar)SC.at(2) ^ (uchar)SC.at(3);
    uchar bip4 = (bip8 & 0xF0) ^ ((bip8 & 0x0F) << 4);
    bip4 += ((uchar)SC.at(0) & 0x0F);
    SC.data()[0] = bip4;
    return SC;
}
/********************************** Laser调试界面的相关函数 End *****************************************************************/
void DialogLASER1::on_horizontalSliderSnglFreqSetWvlnLaser_1_valueChanged(int value){
        ui->doubleSpinBoxSnglFreqSetWvlnLaser_1->setValue((float)value/10);
}
void DialogLASER1::on_doubleSpinBoxSnglFreqSetWvlnLaser_1_valueChanged(double arg1){
        ui->horizontalSliderSnglFreqSetWvlnLaser_1->setValue(arg1*10);
}

void DialogLASER1::on_horizontalSliderSnglFreqSetPwrLaser_1_valueChanged(int value){
    ui->doubleSpinBoxSnglFreqSetPwrLaser_1->setValue((float)value/100);
}
void DialogLASER1::on_doubleSpinBoxSnglFreqSetPwrLaser_1_valueChanged(double arg1){
    ui->horizontalSliderSnglFreqSetPwrLaser_1->setValue(arg1*100);
}

void DialogLASER1::on_horizontalSliderMultiFreqSetPwrLaser_1_valueChanged(int value){
    ui->doubleSpinBoxMultiFreqSetPwrLaser_1->setValue((float)value/100);
}
void DialogLASER1::on_doubleSpinBoxMultiFreqSetPwrLaser_1_valueChanged(double arg1){
    ui->horizontalSliderMultiFreqSetPwrLaser_1->setValue(arg1*100);
}

void DialogLASER1::on_horizontalSliderStartWavelnLaser_1_valueChanged(int value){
    ui->doubleSpinBoxStartWavelnLaser_1->setValue((float)value/10);
}
void DialogLASER1::on_doubleSpinBoxStartWavelnLaser_1_valueChanged(double arg1){
    ui->horizontalSliderStartWavelnLaser_1->setValue(arg1*10);
}

void DialogLASER1::on_horizontalSliderEndWavelnLaser_1_valueChanged(int value){
    ui->doubleSpinBoxEndWavelnLaser_1->setValue((float)value/10);
}
void DialogLASER1::on_doubleSpinBoxEndWavelnLaser_1_valueChanged(double arg1){
    ui->horizontalSliderEndWavelnLaser_1->setValue(arg1*10);
}



