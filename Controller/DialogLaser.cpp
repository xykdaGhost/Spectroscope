#include "DialogLaser.h"
#include "ui_DialogLaser.h"

DialogLaser::DialogLaser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLaser)
{
    ui->setupUi(this);
}

DialogLaser::~DialogLaser()
{
    delete ui;
}


void DialogLaser::on_horizontalSliderSingleFreqPwLaser_valueChanged(int value) {
    ui->doubleSpinBoxSingleFreqPwLaser->setValue((double)value/100);
}

void DialogLaser::on_doubleSpinBoxSingleFreqPwLaser_valueChanged(double value) {
    ui->horizontalSliderSingleFreqPwLaser->setValue(value*100);
}
