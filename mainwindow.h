#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ThreadManager/threadmanager.h"
#include "Controller/DialogLaser.h"
#include "Conn/Uart.h"
#include "Controller/DialogEdfaCA.h"
#include "Controller/DialogEdfaCB.h"
#include "Controller/dialogedfa.h"
#include "Network/UDP.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void loadFileSignal(QString filePath);

public slots:
    void showLoadFile();

private:
    Ui::MainWindow *ui;
    ThreadManager* manager;
    Uart * myUart;
    UDP * myUDP;

};
#endif // MAINWINDOW_H
