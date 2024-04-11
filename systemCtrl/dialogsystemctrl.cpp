#include "dialogsystemctrl.h"
#include "ui_dialogsystemctrl.h"

DialogSystemCtrl::DialogSystemCtrl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSystemCtrl)
{
    ui->setupUi(this);
    this->setWindowTitle(QStringLiteral("系统控制"));
    InitCtrlModuleEDFA();
    InitCtrlModuleLaser();
}

DialogSystemCtrl::~DialogSystemCtrl()
{
    delete ui;
}

void DialogSystemCtrl::mwChangeTab(int iTab){
    if(iTab == 1){
        this->setWindowTitle(QStringLiteral("EDFA控制"));
        ui->tabWidget_SysCtrl->setCurrentWidget(ui->tabSysCtrlEDFA);
    }
    else if (iTab == 2) {
        this->setWindowTitle(QStringLiteral("标定激光器控制"));
        ui->tabWidget_SysCtrl->setCurrentWidget(ui->tabSysCtrlLaserType1);
    }
    else if (iTab == 3) {
        this->setWindowTitle(QStringLiteral("主激光器控制"));
        ui->tabWidget_SysCtrl->setCurrentWidget(ui->tabSysCtrlLaserType2);
    }
    else if (iTab == 4) {
        this->setWindowTitle(QStringLiteral("主机通信设置"));
        ui->tabWidget_SysCtrl->setCurrentWidget(ui->tabCommToMain);
    }
    else if (iTab == 5) {
        this->setWindowTitle(QStringLiteral("其他设置"));
        ui->tabWidget_SysCtrl->setCurrentWidget(ui->tabOtherSettings);
    }
}
