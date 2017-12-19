#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>

#include "dialogrotate.h"
#include "dialoghsl.h"
#include "dialoggraytransform.h"
#include "dialoggrayhistogram.h"
#include "dialogfilter.h"
#include "dialogbutterworth.h"
#include "dialogslic.h"
#include "dialogfacedetection.h"
#include "mymat.h"
#include "slic.h"

#define MEDIAN_FILTER 0
#define GAUSSIAN_FILTER 1
#define BUTTERWORTH_SMOOTH 0
#define BUTTERWORTH_SHARPEN 1

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openActionSlot();
    void saveActionSlot();
    void rotateL90ActionSlot();
    void rotateR90ActionSlot();
    void rotate180ActionSlot();
    void rotateFreeActionSlot();
    void horizontalFlipActionSlot();
    void verticalFlipActionSlot();
    void cutRecActionSlot();
    void cutCircleActionSlot();
    void cutPolyActionSlot();
    void rotateAngleSlot(int angle);
    void confirmedSlot();
    void hslActionSlot();
    void modifyHueSlot(int val);
    void modifySaturationSlot(int val);
    void modifyIntensitySlot(int val);
    void halftoneSlot();
    void grayActionSlot();
    void grayLogSlot();
    void grayESlot();
    void grayGammaSlot();
    void modifyLinearGraySlot(int a, int b, int c, int d);
    void modifyLogGraySlot(double a, double b, double c);
    void modifyEGraySlot(double a, double b, double c);
    void modifyGammaGraySlot(double c, double gamma);
    void grayHistogramActionSlot();
    void equalizationSlot();
    void specificationSMLSlot(int mu, double sigma);
    void specificationGMLSlot(int mu, double sigma);
    void medianFilterSlot();
    void gaussianFilterSlot();
    void filterSlot(int size);
    void sharpenSlot();
    void radialBlurSlot();
    void butterworthSlot(int d0, int n);
    void frequencySmoothSlot();
    void frequencySharpenSlot();
    void superpixelSlot();
    void doSuperPixelSlot(int k, int m, int n);
    void beautyFaceSlot();
    void updateBeautySlot(Mat m);

private:
    void showPic();
    void cutRectPainter();
    vector<double> generateGaussianHis(int mu, double sigma);

protected:
    bool isRectCutting, isCircleCutting, isPolyCutting;
    int x1, x2, y1, y2;
    vector<pair<int, int>> points;
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private:
    Mat img;
    Mat lastImg, lastImgForPolyCut;
    MyMat m;
    int deltaH, deltaS, deltaI;
    int filterType;
    int butterworthType;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
