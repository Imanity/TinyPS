#ifndef MYMAT_H
#define MYMAT_H

#define PI 3.1416

#include <vector>

#include <opencv2/opencv.hpp>

#include <QDebug>

using namespace std;
using namespace cv;

struct Color3 {
    double x, y, z;
    Color3(double a, double b, double c) : x(a), y(b), z(c) {}
};

class MyMat {
public:
    MyMat();
    Mat rotate_l_90(Mat &src);
    Mat rotate_r_90(Mat &src);
    Mat rotate_180(Mat &src);
    Mat rotate(Mat &src, int a);
    Mat flip_h(Mat &src);
    Mat flip_v(Mat &src);
    Mat cutRect(Mat &src, int x1, int x2, int y1, int y2);
    Mat cutCircle(Mat &src, int x1, int x2, int y1, int y2);
    Mat cutPoly(Mat &src, vector<pair<int, int>> &poly);
    Mat drawRect(Mat &src, int x, int y, int w, int h);
    Mat drawCircle(Mat &src, int x, int y, int w, int h);
    Mat drawLine(Mat &src, int x1, int y1, int x2, int y2);
    void setColor(Mat &src, int x, int y, uchar r, uchar g, uchar b);
    Mat modifyHSI(Mat &src, int deltaH, int deltaS, int deltaI);
    Mat halftone(Mat &src);
    Mat modifyLinearGray(Mat &src, int a, int b, int c, int d);
    Mat modifyLogGray(Mat &src, double a, double b, double c);
    Mat modifyEGray(Mat &src, double a, double b, double c);
    Mat modifyGammaGray(Mat &src, double c, double gamma);
    vector<double> getGrayHistogram(Mat &src);
    Mat equalization(Mat &src);
    Mat specificationSML(Mat &src, vector<double> newHistogram);
    Mat specificationGML(Mat &src, vector<double> newHistogram);
    Mat medianFilter(Mat &src, int n);
    Mat gaussianFilter(Mat &src, int n);
    Mat sharpen(Mat &src);
    Mat radialBlur(Mat &src);
    Mat frequencySmooth(Mat &src, int d0, int n);
    Mat frequencySharpen(Mat &src, int d0, int n);

private:
    int rotateVertexX(int x, int y, double angle);
    int rotateVertexY(int x, int y, double angle);
    bool isInsidePoly(vector<pair<int, int>> &poly, int x, int y);
    double angleBetween(double x1, double y1, double x2, double y2);
    Color3 rgb2hsi(Color3 &rgb);
    Color3 hsi2rgb(Color3 &hsi);
    pair<double, double> rotateVec(pair<double, double> v, double theta);
    void drawPot(Mat &src, int cx, int cy, int cr, int r, int g, int b);
    int rgb2gray(Color3 rgb);
    void setRGBgray(Color3 &rgb, int gray);
    int getMedian(vector<int> &histogram, int num);
    vector<int> getGaussianArray(int n);
    Mat MyDFT(Mat &src);
    Mat MyIDFT(Mat &src);
    Mat create_butterworth_lowpass_filter(Mat &src, int d0, int n);
    Mat create_butterworth_highpass_filter(Mat &src, int d0, int n);
    void shiftDFT(Mat &mag);
};

#endif // MYMAT_H
