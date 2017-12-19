#include "dialogfilter.h"
#include "ui_dialogfilter.h"

DialogFilter::DialogFilter(QWidget *parent) : QDialog(parent), ui(new Ui::DialogFilter) {
    ui->setupUi(this);

    connect(ui->val, SIGNAL(valueChanged(int)), this, SLOT(updateSlider(int)));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(applyBtnSlot()));
    connect(this, SIGNAL(sendSize(int)), parent, SLOT(filterSlot(int)));
}

DialogFilter::~DialogFilter() {
    delete ui;
}

void DialogFilter::applyBtnSlot() {
    emit sendSize(ui->val->value());
    this->close();
}

void DialogFilter::updateSlider(int val) {
    ui->valLabel->setText(QString::number(val));
}
