#include "dialoggraytransform.h"
#include "ui_dialoggraytransform.h"

DialogGrayTransform::DialogGrayTransform(QWidget *parent, int type) : QDialog(parent), ui(new Ui::DialogGrayTransform) {
    ui->setupUi(this);
    dialogType = type;

    if (type == DIALOG_GRAY_LINEAR) {
        a = c = 0;
        b = d = 255;

        ui->hintLabel->setText("Please confirm that a<=b and c<=d");

        ui->aValue->setValidator(new QIntValidator(0, 255, this));
        ui->bValue->setValidator(new QIntValidator(0, 255, this));
        ui->cValue->setValidator(new QIntValidator(0, 255, this));
        ui->dValue->setValidator(new QIntValidator(0, 255, this));

        ui->aValue->setText(QString("0"));
        ui->bValue->setText(QString("255"));
        ui->cValue->setText(QString("0"));
        ui->dValue->setText(QString("255"));
        ui->gammaValue->setEnabled(false);
    } else if (type == DIALOG_GRAY_LOG) {
        a = 0;
        b = 0.0217;
        c = 2.7183;

        ui->hintLabel->setText("log transform");

        ui->aValue->setValidator(new QDoubleValidator(0, 255.0, 4, this));
        ui->bValue->setValidator(new QDoubleValidator(0, 100.0, 4, this));
        ui->cValue->setValidator(new QDoubleValidator(0, 100.0, 4, this));

        ui->aValue->setText("0");
        ui->bValue->setText("0.0217");
        ui->cValue->setText("2.7183");
        ui->dValue->setEnabled(false);
        ui->gammaValue->setEnabled(false);
    } else if (type == DIALOG_GRAY_E) {
        a = 0;
        b = 2.7183;
        c = 0.0217;

        ui->hintLabel->setText("exp transform");

        ui->aValue->setValidator(new QDoubleValidator(0, 255.0, 4, this));
        ui->bValue->setValidator(new QDoubleValidator(0, 100.0, 4, this));
        ui->cValue->setValidator(new QDoubleValidator(0, 100.0, 4, this));

        ui->aValue->setText("0");
        ui->bValue->setText("2.7183");
        ui->cValue->setText("0.0217");
        ui->dValue->setEnabled(false);
        ui->gammaValue->setEnabled(false);
    } else {
        c = 1;
        gamma = 1;

        ui->hintLabel->setText("Gamma adjust");

        ui->gammaValue->setValidator(new QDoubleValidator(0, 100.0, 5, this));
        ui->cValue->setValidator(new QDoubleValidator(0, 100.0, 5, this));

        ui->aValue->setEnabled(false);
        ui->bValue->setEnabled(false);
        ui->cValue->setText("1.0");
        ui->dValue->setEnabled(false);
        ui->gammaValue->setText("1.0");
    }


    connect(ui->aValue, SIGNAL(textChanged(QString)), this, SLOT(setA(QString)));
    connect(ui->bValue, SIGNAL(textChanged(QString)), this, SLOT(setB(QString)));
    connect(ui->cValue, SIGNAL(textChanged(QString)), this, SLOT(setC(QString)));
    connect(ui->dValue, SIGNAL(textChanged(QString)), this, SLOT(setD(QString)));
    connect(ui->gammaValue, SIGNAL(textChanged(QString)), this, SLOT(setGamma(QString)));
    connect(ui->confirmBtn, SIGNAL(clicked(bool)), this, SLOT(confirmBtnSlot()));
    connect(this, SIGNAL(sendLinearArgs(int,int,int,int)), parent, SLOT(modifyLinearGraySlot(int, int, int, int)));
    connect(this, SIGNAL(sendLogArgs(double,double,double)), parent, SLOT(modifyLogGraySlot(double, double, double)));
    connect(this, SIGNAL(sendEArgs(double,double,double)), parent, SLOT(modifyEGraySlot(double, double, double)));
    connect(this, SIGNAL(sendGammaArgs(double,double)), parent, SLOT(modifyGammaGraySlot(double, double)));
}

DialogGrayTransform::~DialogGrayTransform() {
    delete ui;
}

void DialogGrayTransform::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);

    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter.drawLine(QPoint(20, 275), QPoint(275, 275));
    painter.drawLine(QPoint(20, 275), QPoint(20, 20));

    painter.drawText(QPoint(10, 10), QString("g(x, y)"));
    painter.drawText(QPoint(280, 280), QString("f(x, y)"));
    painter.drawText(QPoint(10, 285), QString("O"));

    if (dialogType == DIALOG_GRAY_LINEAR) {
        painter.drawText(QPoint(20 + a, 285), QString("a"));
        painter.drawText(QPoint(20 + b, 285), QString("b"));
        painter.drawText(QPoint(10, 275 - c), QString("c"));
        painter.drawText(QPoint(10, 275 - d), QString("d"));

        painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
        painter.drawLine(QPoint(20, 275), QPoint(20 + a, 275 - c));
        painter.drawLine(QPoint(20 + a, 275 - c), QPoint(20 + b, 275 - d));
        painter.drawLine(QPoint(20 + b, 275 - d), QPoint(275, 20));

        painter.setPen(QPen(Qt::green, 1, Qt::DashDotLine));
        painter.drawLine(QPoint(20 + a, 275 - c), QPoint(20 + a, 275));
        painter.drawLine(QPoint(20 + a, 275 - c), QPoint(20, 275 - c));
        painter.drawLine(QPoint(20 + b, 275 - d), QPoint(20 + b, 275));
        painter.drawLine(QPoint(20 + b, 275 - d), QPoint(20, 275 - d));
    } else if (dialogType == DIALOG_GRAY_LOG) {
        painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
        for (int x = 0; x <= 255; ++x) {
            int y = (double)a + log(x + 1) / (b * log(c));
            y = y > 255 ? 255 : y;
            y = y < 0 ? 0 : y;
            painter.drawPoint(20 + x, 275 - y);
        }
    } else if (dialogType == DIALOG_GRAY_E) {
        painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
        for (int x = 0; x <= 255; ++x) {
            int y = pow(b, c * (x - a)) - 1;
            y = y > 255 ? 255 : y;
            y = y < 0 ? 0 : y;
            painter.drawPoint(20 + x, 275 - y);
        }
    } else {
        painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
        for (int x = 0; x <= 255; ++x) {
            int y = c * pow(x, gamma);
            y = y > 255 ? 255 : y;
            y = y < 0 ? 0 : y;
            painter.drawPoint(20 + x, 275 - y);
        }
    }
}

void DialogGrayTransform::setA(QString value) {
    bool ok;
    a = value.toDouble(&ok);
    validateInput();
    update();
}

void DialogGrayTransform::setB(QString value) {
    bool ok;
    b = value.toDouble(&ok);
    validateInput();
    update();
}

void DialogGrayTransform::setC(QString value) {
    bool ok;
    c = value.toDouble(&ok);
    validateInput();
    update();
}

void DialogGrayTransform::setD(QString value) {
    bool ok;
    d = value.toDouble(&ok);
    validateInput();
    update();
}

void DialogGrayTransform::setGamma(QString value) {
    bool ok;
    gamma = value.toDouble(&ok);
    validateInput();
    update();
}

void DialogGrayTransform::validateInput() {
    if (dialogType == DIALOG_GRAY_LINEAR) {
        if (a <= b && c <= d) {
            ui->confirmBtn->setEnabled(true);
        } else {
            ui->confirmBtn->setEnabled(false);
        }
    } else {
        ui->confirmBtn->setEnabled(true);
    }
}

void DialogGrayTransform::confirmBtnSlot() {
    switch (dialogType) {
    case DIALOG_GRAY_LINEAR:
        emit sendLinearArgs((int)a, (int)b, (int)c, (int)d);
        break;
    case DIALOG_GRAY_LOG:
        emit sendLogArgs(a, b, c);
        break;
    case DIALOG_GRAY_E:
        emit sendEArgs(a, b, c);
        break;
    case DIALOG_GRAY_GAMMA:
        emit sendGammaArgs(c, gamma);
        break;
    default:
        break;
    }
    this->close();
}
