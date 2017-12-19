#ifndef DIALOGFACEDETECTION_H
#define DIALOGFACEDETECTION_H

#include <QDialog>
#include <QMouseEvent>
#include <QDebug>

#include <vector>
#include <queue>
#include <algorithm>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define M_E 2.7183

#define STEP_SELECT 0
#define STEP_FILTER 1
#define STEP_LIGHT 2

namespace Ui {
class DialogFaceDetection;
}

struct Color {
    double x, y, z;
    Color(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
};

class DialogFaceDetection : public QDialog {
    Q_OBJECT

public:
    explicit DialogFaceDetection(QWidget *parent = 0);
    ~DialogFaceDetection();

    void setImage(Mat m);
    void skinExtract();
    void medianFilter();
    void bilateralFilter();
    void adjustBrightness();

public slots:
    void viewSlot();
    void submitSlot();

signals:
    void sendResult(Mat m);

private:
    Ui::DialogFaceDetection *ui;
    Mat img;
    Mat imgOrigin;
    Mat imgOutput;
    Mat imgMask;
    Mat imgTmp;
    double sigmaD = 3.0;
    double sigmaR = 10.0;
    int windowSize = 3;
    int step = STEP_SELECT;
    int deltaB = 10;
    bool isDragging = false;
    int rectX1 = 0, rectX2 = 0, rectY1 = 0, rectY2 = 0;

private:
    void showImg();
    void showImgOutput();
    bool isSkinRGB(int r, int g, int b);
    void bilateralFilterPixel(Mat &dst, int x, int y);
    void adjustBrightnessPixel(Mat &dst, int x, int y);
    // Color translation
    Color rgb2lab(Color rgb);
    double gamma(double x);
    double flab(double x);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // DIALOGFACEDETECTION_H
