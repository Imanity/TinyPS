#include "dialogrotate.h"
#include "ui_dialogrotate.h"

DialogRotate::DialogRotate(QWidget *parent) : QDialog(parent), ui(new Ui::DialogRotate) {
    ui->setupUi(this);
    connect(ui->rotateSlider, SIGNAL(valueChanged(int)), parent, SLOT(rotateAngleSlot(int)));
    connect(ui->rotateBtn, SIGNAL(clicked(bool)), parent, SLOT(confirmedSlot()));
    connect(ui->rotateBtn, SIGNAL(clicked(bool)), this, SLOT(confirmRotate()));
}

DialogRotate::~DialogRotate() {
    delete ui;
}

void DialogRotate::confirmRotate() {
    ui->rotateSlider->setValue(0);
    this->close();
}
