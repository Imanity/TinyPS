#include "dialogslic.h"
#include "ui_dialogslic.h"

DialogSlic::DialogSlic(QWidget *parent) : QDialog(parent), ui(new Ui::DialogSlic) {
    ui->setupUi(this);

    connect(ui->sliderK, SIGNAL(valueChanged(int)), this, SLOT(kvalSlot(int)));
    connect(ui->sliderM, SIGNAL(valueChanged(int)), this, SLOT(mvalSlot(int)));
    connect(ui->sliderN, SIGNAL(valueChanged(int)), this, SLOT(nvalSlot(int)));
    connect(ui->submitBtn, SIGNAL(clicked(bool)), this, SLOT(btnSlot()));
    connect(this, SIGNAL(submitVal(int,int,int)), parent, SLOT(doSuperPixelSlot(int, int, int)));
}

DialogSlic::~DialogSlic() {
    delete ui;
}

void DialogSlic::kvalSlot(int val) {
    ui->kval->setText(QString::number(val));
}

void DialogSlic::mvalSlot(int val) {
    ui->mval->setText(QString::number(val));
}

void DialogSlic::nvalSlot(int val) {
    ui->nval->setText(QString::number(val));
}

void DialogSlic::btnSlot() {
    emit submitVal(ui->sliderK->value(), ui->sliderM->value(), ui->sliderN->value());
    this->close();
}
