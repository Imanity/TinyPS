#ifndef DIALOGGRAYTRANSFORM_H
#define DIALOGGRAYTRANSFORM_H

#include <QDialog>
#include <QPaintEvent>
#include <QPainter>
#include <QPoint>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QDebug>

#define DIALOG_GRAY_LINEAR 0
#define DIALOG_GRAY_LOG 1
#define DIALOG_GRAY_E 2
#define DIALOG_GRAY_GAMMA 3

namespace Ui {
class DialogGrayTransform;
}

class DialogGrayTransform : public QDialog {
    Q_OBJECT

public:
    explicit DialogGrayTransform(QWidget *parent = 0, int type = DIALOG_GRAY_LINEAR);
    ~DialogGrayTransform();

public slots:
    void setA(QString value);
    void setB(QString value);
    void setC(QString value);
    void setD(QString value);
    void setGamma(QString value);
    void confirmBtnSlot();

protected:
    void paintEvent(QPaintEvent *e);

private:
    void validateInput();

private:
    Ui::DialogGrayTransform *ui;
    double a, b, c, d, gamma;
    int dialogType;

signals:
    void sendLinearArgs(int a, int b, int c, int d);
    void sendLogArgs(double a, double b, double c);
    void sendEArgs(double a, double b, double c);
    void sendGammaArgs(double c, double gamma);
};

#endif // DIALOGGRAYTRANSFORM_H
