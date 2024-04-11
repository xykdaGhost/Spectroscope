#ifndef DIALOGSYSTEMCTRL_H
#define DIALOGSYSTEMCTRL_H

#include <QDialog>
#include "dialogedfa.h"
#include "dialoglaser1.h"
namespace Ui {
class DialogSystemCtrl;
}

class DialogSystemCtrl : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSystemCtrl(QWidget *parent = 0);
    ~DialogSystemCtrl();
    void mwChangeTab(int iTab);

private:
    Ui::DialogSystemCtrl *ui;


    /********************EDFA在主函数中的组件 Begin *****************/
private:
    DialogEDFA *qdlEDFA[5];
    DialogEDFA::objsUsedInUI EDFAObjsInMainUI[5];
    void InitCtrlModuleEDFA();
    void InitEDFAObjsInMainUI();
private slots:
    //将UI组件连接的函数，及UI组件的响应函数
    void connectUISliderWithSpinBoxEDFA(int iTag);
    void connectUIbuttonWithHandlerEDFA(int iTag);
    //负责响应UI中串口信号的函数
    void connectSignalsWithMainHandlerEDFA(int iTag);
    /********************EDFA在主函数中的组件 End *****************/




    /********************Laser在主函数中的组件 Begin *****************/
    void on_pushButtonOpenDebuggerLaser_1_clicked();

private:
    DialogLASER1 *qdlLaser1[3];
    DialogLASER1::objsUsedInUI Laser1ObjsInMainUI[3];

private slots:
    void InitCtrlModuleLaser();
    void InitLaser1ObjsInMainUI();
    void connectUISliderWithSpinBoxLaser(int iTag);
    void connectUIbuttonWithHandlerLaser(int iTag);
    //负责响应UI中串口信号的函数
    void connectSignalsWithMainHandlerLaser(int iTag);

    /********************Laser在主函数中的组件 End *****************/
};

#endif // DIALOGSYSTEMCTRL_H
