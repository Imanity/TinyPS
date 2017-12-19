#include "dialoggrayhistogram.h"
#include "ui_dialoggrayhistogram.h"

DialogGrayHistogram::DialogGrayHistogram(QWidget *parent, const vector<double> &histogram) : QDialog(parent), ui(new Ui::DialogGrayHistogram) {
    ui->setupUi(this);

    this->histogram = histogram;

    ui->smlBtn->setEnabled(false);
    ui->gmlBtn->setEnabled(false);

    ui->muVal->setValidator(new QIntValidator(0, 255, this));
    ui->sigmaVal->setValidator(new QDoubleValidator(0, 10.0, 4, this));

    connect(ui->equalizationBtn, SIGNAL(clicked(bool)), this, SLOT(equalization()));
    connect(ui->equalizationBtn, SIGNAL(clicked(bool)), parent, SLOT(equalizationSlot()));
    connect(ui->smlBtn, SIGNAL(clicked(bool)), this, SLOT(specificationSML()));
    connect(ui->gmlBtn, SIGNAL(clicked(bool)), this, SLOT(specificationGML()));
    connect(this, SIGNAL(sml(int,double)), parent, SLOT(specificationSMLSlot(int, double)));
    connect(this, SIGNAL(gml(int,double)), parent, SLOT(specificationGMLSlot(int, double)));
}

DialogGrayHistogram::~DialogGrayHistogram() {
    delete ui;
}

void DialogGrayHistogram::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);

    QPainter painter(this);

    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter.drawLine(QPoint(20, 275), QPoint(275, 275));
    painter.drawLine(QPoint(20, 275), QPoint(20, 20));

    painter.drawText(QPoint(10, 10), QString("p(rk)"));
    painter.drawText(QPoint(280, 280), QString("Nk"));
    painter.drawText(QPoint(10, 285), QString("O"));

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::blue);

    hisMax = 0;
    for (int i = 0; i < histogram.size(); ++i) {
        if (histogram[i] > hisMax) {
            hisMax = histogram[i];
        }
    }
    for (int i = 0; i < histogram.size(); ++i) {
        painter.drawRect(QRect(QPoint(20 + i, 275 - 255 * histogram[i] / hisMax), QPoint(20 + (i + 1), 275)));
    }
}

void DialogGrayHistogram::equalization() {
    vector<double> newHis(256, 0);
    double s = 0;
    for (int i = 0; i < histogram.size(); ++i) {
        s += histogram[i];
        int newGray = int(s * 255.0);
        newHis[newGray] += histogram[i];
    }
    histogram = newHis;
    ui->gmlBtn->setEnabled(true);
    ui->smlBtn->setEnabled(true);
    update();
}

void DialogGrayHistogram::specificationSML() {
    emit sml(ui->muVal->text().toInt(), ui->sigmaVal->text().toDouble());
    vector<double> newHistogram = generateGaussianHis();
    vector<double> newHis(256, 0);
    vector<double> srcHis(256, 0);
    vector<double> dstHis(256, 0);
    srcHis[0] = histogram[0];
    dstHis[0] = newHistogram[0];
    for (int i = 1; i < 256; ++i) {
        srcHis[i] = srcHis[i - 1] + histogram[i];
    }
    for (int i = 1; i < 256; ++i) {
        dstHis[i] = dstHis[i - 1] + newHistogram[i];
    }
    for (int i = 0; i < 256; ++i) {
        double minDis = abs(srcHis[i] - dstHis[0]);
        int minID = 0;
        for (int j = 0; j < 256; ++j) {
            double currDis = abs(srcHis[i] - dstHis[j]);
            if (currDis <= minDis) {
                minDis = currDis;
                minID = j;
            } else {
                break;
            }
        }
        newHis[minID] += histogram[i];
    }
    histogram = newHis;
    ui->gmlBtn->setEnabled(false);
    ui->smlBtn->setEnabled(false);
    update();
}

void DialogGrayHistogram::specificationGML() {
    emit gml(ui->muVal->text().toInt(), ui->sigmaVal->text().toDouble());
    vector<double> newHistogram = generateGaussianHis();
    vector<double> newHis(256, 0);
    vector<double> srcHis(256, 0);
    vector<double> dstHis(256, 0);
    srcHis[0] = histogram[0];
    dstHis[0] = newHistogram[0];
    for (int i = 1; i < 256; ++i) {
        srcHis[i] = srcHis[i - 1] + histogram[i];
    }
    for (int i = 1; i < 256; ++i) {
        dstHis[i] = dstHis[i - 1] + newHistogram[i];
    }
    int start = 0, end = 0, lastStart = 0, lastEnd = 0;
    for (int i = 0; i < 256; ++i) {
        double minDis = abs(dstHis[i] - srcHis[0]);
        for (int j = 0; j < 256; ++j) {
            double currDis = abs(dstHis[i] - srcHis[j]);
            if (currDis <= minDis) {
                end = j;
                minDis = currDis;
            }
        }
        if (start != lastStart || end != lastEnd) {
            for (int j = start; j <= end; ++j) {
                newHis[i] += histogram[j];
            }
            lastStart = start;
            lastEnd = end;
            start = lastEnd + 1;
        }
    }
    histogram = newHis;
    ui->gmlBtn->setEnabled(false);
    ui->smlBtn->setEnabled(false);
    update();
}

vector<double> DialogGrayHistogram::generateGaussianHis() {
    int mu = ui->muVal->text().toInt();
    double sigma = ui->sigmaVal->text().toDouble();
    vector<double> his(256, 0);
    double s = 0;
    for (int i = 0; i < 256; ++i) {
        his[i] = pow(2.7183, -(double((i - mu) * (i - mu))) / (2 * sigma * sigma)) / sigma;
        s += his[i];
    }
    for (int i = 0; i < 256; ++i) {
        his[i] /= s;
    }
    return his;
}
