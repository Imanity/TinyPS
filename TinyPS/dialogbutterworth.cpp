#include "dialogbutterworth.h"
#include "ui_dialogbutterworth.h"

DialogButterworth::DialogButterworth(QWidget *parent) : QDialog(parent), ui(new Ui::DialogButterworth) {
    ui->setupUi(this);

    connect(ui->d0Slider, SIGNAL(valueChanged(int)), this, SLOT(updateD0Val(int)));
    connect(ui->nSlider, SIGNAL(valueChanged(int)), this, SLOT(updateNVal(int)));
    connect(ui->submitBtn, SIGNAL(clicked(bool)), this, SLOT(btnSlot()));
    connect(this, SIGNAL(submitVal(int,int)), parent, SLOT(butterworthSlot(int, int)));
}

DialogButterworth::~DialogButterworth() {
    delete ui;
}

void DialogButterworth::updateD0Val(int val) {
    ui->d0val->setText(QString::number(val));
}

void DialogButterworth::updateNVal(int val) {
    ui->nval->setText(QString::number(val));
}

void DialogButterworth::btnSlot() {
    emit submitVal(ui->d0Slider->value(), ui->nSlider->value());
    this->close();
}
