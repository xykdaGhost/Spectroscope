#ifndef DIALOGLASER_H
#define DIALOGLASER_H

#include <QDialog>
#include <QSlider>
#include <QDoubleSpinBox>

namespace Ui {
class DialogLaser;
}

class DialogLaser : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLaser(QWidget *parent = nullptr);
    ~DialogLaser();

private:
    Ui::DialogLaser *ui;

private slots:
    void on_horizontalSliderSingleFreqPwLaser_valueChanged(int value);
    void on_doubleSpinBoxSingleFreqPwLaser_valueChanged(double value);
};



#endif // DIALOGLASER_H
