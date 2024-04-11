#ifndef DIALOGLASER1_H
#define DIALOGLASER1_H

#include <QTimer>
#include <QDialog>
#include <QDateTime>
#include "myqsp.h"

#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QDoubleSpinBox>

namespace Ui {
class DialogLASER1;
}

class DialogLASER1 : public QDialog
{
    Q_OBJECT
public:
    explicit DialogLASER1(QWidget *parent = nullptr);
    ~DialogLASER1();
    QStringList handleMwScanComLASER1();
    void handleMwSetParLASER1(float fSetWavln, float fSetPwr);
    int handleMwSetComLASER1(QString comSelected, int iBaudRate);
    void handleMwScnFrqLASER1(float fScanStart, float fScanEnd, float fStep, float fWantScanSecond, float fSetPwr, bool bIsFreqScan);
    void handleMwToggleLASER1();

signals:
    //以下为每当有参数/使能输出设置完成时的信号，并非每次定时器事件都更新
    //0：开始设置激光器，应该发信号禁止ui操作
    //1：激光器设置完毕，仅watch允许ui操作
    void sigSetEventLASER1(int iSetEvent);
    //以下信号包含每次定时器中断都会更新的参数
    void sigWatchResultLaser(bool bIsComOpen, bool bIsOutput, bool bIsStable,
                             bool bIsAlive, float fQstFrq, float fQstTmp, float fQstPwr);
private slots:
    void handleMainTimerUpdateLASER1();
    void handleAnlyzRecvLASER1();
    void handlePushComCMDLASER1();
    void sigWatchResultLocalHandler(bool bIsComOpen, bool bIsOutput, bool bIsStable,
                                    bool bIsAlive, float fQstFrq, float fQstTmp, float fQstPwr);

private:
    Ui::DialogLASER1 *ui;
    myQSP *qspLASER1;
    typedef struct{
        //用于记录从激光器查询到状态的变量：
        float fQstFrq, fQstTmp, fQstPwr;
        int iQstPwr, iQstTmp, iQstFrqUpr, iQstFrqLwr;
        bool bQstIsAlive, bQstIsOutput, bQstIsStable, bComIsOpened;

        //需要设置激光器的参数，会由handlePushComCMDLASER1()函数在发送时转换为具体串口指令
        float fSetPwr;
        QVector<float> qvfSetFrq;
        int iSetPwrOut, iSetFrqUpr, iSetFrqLwr;

        //用于记录用户希望的操作
        bool bWantEnableLASER1;//希望使能或者失能激光器输出
        bool bWantSetParLASER1;//希望设置激光器参数

        int iCountPeriod;
        int iWantStablePeriod;
        //激光器固定指令集
        QVector<QByteArray> qvbaCMDqstSys;
        QVector<QByteArray> qvbaCMDqstNorm;
        QVector<QByteArray> qvbaCMDsetNorm;
        QVector<QByteArray> qvbaCMDsetJump;
        QVector<QByteArray> qvbaCMDsendQueue;
        QTimer *qtmMainTimerLASER1;
    } ParamLASER1;
    ParamLASER1 parLASER1;

    //Functions
    void InitCMDs();
    void handleScanFreq();
    void TextDebugOut(QString SC);
    QByteArray SumCheck(QByteArray SC);

private slots:
    void on_pushButtonScanComLaser_1_clicked();
    void on_pushButtonOpenComLaser_1_clicked();
    void on_pushButtonSnglFreqEnableLaser_1_clicked();
    void on_pushButtonSnglFreqSetLaser_1_clicked();
    void on_pushButtonMultiFreqEnableLaser_1_clicked();

    void on_horizontalSliderSnglFreqSetPwrLaser_1_valueChanged(int value);
    void on_doubleSpinBoxSnglFreqSetPwrLaser_1_valueChanged(double arg1);
    void on_horizontalSliderSnglFreqSetWvlnLaser_1_valueChanged(int value);
    void on_doubleSpinBoxSnglFreqSetWvlnLaser_1_valueChanged(double arg1);
    void on_horizontalSliderMultiFreqSetPwrLaser_1_valueChanged(int value);
    void on_doubleSpinBoxMultiFreqSetPwrLaser_1_valueChanged(double arg1);
    void on_horizontalSliderStartWavelnLaser_1_valueChanged(int value);
    void on_doubleSpinBoxStartWavelnLaser_1_valueChanged(double arg1);
    void on_horizontalSliderEndWavelnLaser_1_valueChanged(int value);
    void on_doubleSpinBoxEndWavelnLaser_1_valueChanged(double arg1);

public:

    typedef struct{
        QPushButton* pushButtonScanComLaser;
        QPushButton* pushButtonOpenComLaser;
        QPushButton* pushButtonOpenDebuggerLaser;

        QPushButton* pushButtonSnglFreqSetLaser;
        QPushButton* pushButtonSnglFreqEnableLaser;
        QPushButton* pushButtonMultiFreqEnableLaser;

        QDoubleSpinBox* doubleSpinBoxSnglFreqSetPwrLaser;
        QDoubleSpinBox* doubleSpinBoxSnglFreqSetWvlnLaser;
        QDoubleSpinBox* doubleSpinBoxMultiFreqSetPwrLaser;
        QDoubleSpinBox* doubleSpinBoxStartWavelnLaser;
        QDoubleSpinBox* doubleSpinBoxEndWavelnLaser;
        QDoubleSpinBox* doubleSpinBoxSteplnLaser;
        QDoubleSpinBox* doubleSpinBoxStableDurLaser;

        QSlider* horizontalSliderSnglFreqSetPwrLaser;
        QSlider* horizontalSliderSnglFreqSetWvlnLaser;
        QSlider* horizontalSliderMultiFreqSetPwrLaser;
        QSlider* horizontalSliderStartWavelnLaser;
        QSlider* horizontalSliderEndWavelnLaser;

        QComboBox* comboBoxSelectComLaser;
        QLineEdit* lineEditOpenDebuggerCodeLaser;
        QLineEdit* lineEditBaudRateComLaser;

        QLabel* labelConnectStateComLaser;
        QLabel* labelWatchWavelnLaser;
        QLabel* labelWatchTempLaser;
        QLabel* labelWatchPwrLaser;
        QLabel* labelWatchIfOnLaser;
    } objsUsedInUI;
};

#endif // DIALOGLASER1_H
