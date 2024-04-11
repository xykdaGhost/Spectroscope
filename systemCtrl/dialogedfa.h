#ifndef DIALOGEDFA_H
#define DIALOGEDFA_H

#include <QTimer>
#include <QDialog>
#include <QThread>
#include <QDateTime>
#include "myqsp.h"

#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QDoubleSpinBox>

namespace Ui {
class DialogEDFA;
}

class DialogEDFA : public QDialog
{
    Q_OBJECT
signals:
    void sigStateEDFA(int iState);
    void sigWatchResultEDFA(bool bIsModAlive, float fQstCur, float fQstTmp, float fQstPwr);

public:
    explicit DialogEDFA(QWidget *parent = nullptr);
    ~DialogEDFA();

    QStringList handleMwScanComEDFA();
    int handleMwSetComEDFA(QString comSelected, int iBaudRate);
    void handleMwToggleWatchEDFA();
    void handleMwSetEDFA(int iSetMod, float fMaxCur, float fSetCur, float fSetPwr);

    typedef struct{
        QLabel *labelWatchPwrEDFA;
        QLabel *labelWatchCurEDFA;
        QLabel *labelWatchModEDFA;
        QLabel *labelWatchIfOnEDFA;
        QLabel *labelWatchTempEDFA;
        QLabel *labelConnectStateComEDFA;

        QPushButton *pushButtonScanComEDFA;
        QPushButton *pushButtonOpenComEDFA;
        QPushButton *pushButtonSetParamEDFA;
        QPushButton *pushButtonOpenDebuggerEDFA;
        QPushButton *pushButtonToggleWatchStateEDFA;

        QSlider *horizontalSliderPwrEDFA;
        QSlider *horizontalSliderCurMaxEDFA;
        QSlider *horizontalSliderCurSetEDFA;

        QDoubleSpinBox *doubleSpinBoxPwrEDFA;
        QDoubleSpinBox *doubleSpinBoxCurMaxEDFA;
        QDoubleSpinBox *doubleSpinBoxCurSetEDFA;

        QComboBox *comboBoxSetModEDFA;
        QComboBox *comboBoxSetPwrLvEDFA;
        QComboBox *comboBoxSelectComEDFA;
        QLineEdit *lineEditBaudRateComEDFA;
        QLineEdit *lineEditOpenDebuggerCodeEDFA;
    } objsUsedInUI;

private:
    int iCurrentCMD;
    int iManualState;
    bool bWatchingEDFA;
    bool bNotSettingEDFA;
    bool bSpvsCMDCntPlusOne;
    QThread* qthMultiTh;
    typedef struct{
        bool bIsModAlive;
        float fQstCur;
        float fQstTmp;
        float fQstPwr;

        int iSetMod;
        float fSetCur;
        float fMaxCur;
        float fSetPwr;
    } ParamEDFA;
    ParamEDFA parEDFA;

    Ui::DialogEDFA *ui;
    myQSP *qspEDFA;
    QTimer *qtmEDFASet;
    QTimer *qtmEDFAWch;
    QVector<QByteArray> qvbaCMDTXs;
    QVector<QByteArray> qvbaCMDRXs;

    //Functions
    void InitCMDs();
    QByteArray SumCheck(QByteArray SC);

private slots:
    void on_pushButtonScan_clicked();
    void on_pushButtonOpen_clicked();
    void on_pushButtonSet_clicked();
    void on_pushButtonSpvsToggle_clicked();

    //iMod = 0 for APC, 1 for ACC;
    void handleSendSettingCommandEDFA();
    void handleSigRespondEDFA(int iState);
    void handleWatchEDFA();
    void handleWatchResultEDFA(bool bIsAlive, float fEDFACur, float fEDFATmp, float fEDFAPwr);
};

#endif // DIALOGEDFA_H
