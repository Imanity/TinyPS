#include "dialoghsl.h"
#include "ui_dialoghsl.h"

DialogHSL::DialogHSL(QWidget *parent) : QDialog(parent), ui(new Ui::DialogHSL) {
    ui->setupUi(this);

    connect(ui->hSlider, SIGNAL(valueChanged(int)), parent, SLOT(modifyHueSlot(int)));
    connect(ui->sSlider, SIGNAL(valueChanged(int)), parent, SLOT(modifySaturationSlot(int)));
    connect(ui->lSlider, SIGNAL(valueChanged(int)), parent, SLOT(modifyIntensitySlot(int)));
}

DialogHSL::~DialogHSL() {
    delete ui;
}
