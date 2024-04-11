#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QMessageBox>
#include <iostream>
#include <QIODevice>
#include <QFileDialog>
#include <QDebug>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,manager(&ThreadManager::getInstance())
    ,myUart(&Uart::getInstance())
    ,myUDP(&UDP::getInstance())
{
    ui->setupUi(this);

    DialogEDFA * dialogEdfa = new DialogEDFA;
    //dialogEdfa->show();

    //create thread
    QThread* managerThread = new QThread();
    manager->moveToThread(managerThread);
    connect(managerThread, SIGNAL(started()), manager, SLOT(doWork()));
    connect(managerThread, SIGNAL(finished()), managerThread, SLOT(deleteLater()));
    managerThread->start();

    QThread* uartThread = new QThread();
    myUart->moveToThread(uartThread);
    connect(uartThread, SIGNAL(started()), myUart, SLOT(doWork()));
    connect(uartThread, SIGNAL(finished()), uartThread, SLOT(deleteLater()));
    uartThread->start();

    QThread* udpThread = new QThread();
    myUDP->moveToThread(udpThread);
    connect(udpThread, SIGNAL(started()), myUDP, SLOT(doWork()));
    connect(udpThread, SIGNAL(finished()), udpThread, SLOT(deleteLater()));
    udpThread->start();


    DialogLaser * dialogLaser = new DialogLaser;




    //connect ui input
    connect(ui->action_laser, &QAction::triggered, [=](){

        dialogLaser->show();
    });

    connect(ui->action_EDFA, &QAction::triggered, [=](){

        dialogEdfa -> show();
    });

    connect(ui->action_load, &QAction::triggered, [=](){
        QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("加载文件"),
            "",
            //tr("images(*.png *jpeg *bmp);;video files(*.avi *.mp4 *.wmv);;All files(*.*)")
            tr("波形文件(*.csv)"));
        if (fileName.isEmpty()) {
            //QMessageBox::warning(this, "Warning!", "Failed to open the video!");
        }
        else {

        }
        qDebug() << "fileName: " << fileName;
        QFile file(fileName);
        QStringList lines;
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream_text(&file);
            QVector<pair<float, float>> qvpffTemp;
            while (!stream_text.atEnd())
            {
                lines.push_back(stream_text.readLine());
            }
            float fstrn;
            float ftime = 0;
            float fDeltatime = 10.00/24.50;

            for (int j = 0; j < lines.size(); j++)//lines.size();
            {
                fstrn = lines.at(j).toFloat() / 2.37;
                qvpffTemp.append(make_pair(ftime, fstrn));
                ftime += fDeltatime;
            }
            file.close();
            qDebug() << "open";
            ui->widget_xyview->setdata(qvpffTemp);
        }

    });

    manager->refreshEDFAStatus();

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::showLoadFile(){
    connect(this, &MainWindow::loadFileSignal, [=](QString filePath){
//        ui->textBrowser->append(filePath);
        QFile file(filePath);
        QStringList lines;
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream_text(&file);
            QVector<pair<float, float>> qvpffTemp;
            while (!stream_text.atEnd())
            {
                lines.push_back(stream_text.readLine());
            }
            float fstrn;
            float ftime = 0;
            float fDeltatime = 10.00/24.50;

            for (int j = 0; j < lines.size(); j++)//lines.size();
            {
                fstrn = lines.at(j).toFloat() / 2.37;
                qvpffTemp.append(make_pair(ftime, fstrn));
                ftime += fDeltatime;
            }
            file.close();
            qDebug() << "open";
            ui->widget_xyview->setdata(qvpffTemp);
        }

    });
}
