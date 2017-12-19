#ifndef DIALOGGRAYHISTOGRAM_H
#define DIALOGGRAYHISTOGRAM_H

#include <QDialog>
#include <QPainter>
#include <QDebug>

#include <vector>

using namespace std;

namespace Ui {
class DialogGrayHistogram;
}

class DialogGrayHistogram : public QDialog {
    Q_OBJECT

public:
    explicit DialogGrayHistogram(QWidget *parent = 0, const vector<double> &histogram = vector<double>());
    ~DialogGrayHistogram();

protected:
    void paintEvent(QPaintEvent *e);

public slots:
    void equalization();
    void specificationSML();
    void specificationGML();

signals:
    void sml(int, double);
    void gml(int, double);

private:
    Ui::DialogGrayHistogram *ui;
    vector<double> histogram;
    double hisMax;

private:
    vector<double> generateGaussianHis();
};

#endif // DIALOGGRAYHISTOGRAM_H
