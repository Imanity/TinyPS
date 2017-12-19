#include "slic.h"

//===========================================================================
// Construction
//===========================================================================

Slic::Slic(Mat img) {
    this->img = img;
    n = img.rows * img.cols;
    k = 256;
    m = 20.0;
    nIters = 10;
}

//===========================================================================
// Color translation
//===========================================================================

double Slic::gamma(double x) {
    if (x > 0.04045) {
        return pow((x + 0.055) / 1.055, 2.4);
    }
    return x / 12.92;
}

double Slic::flab(double x) {
    double epsilon = 0.008856, kappa = 903.3;;
    if (x > epsilon) {
        return pow(x, 1.0 / 3.0);
    }
    return (kappa * x + 16.0) / 116.0;
}

Color3d Slic::rgb2lab(Color3d rgb) {
    double r = gamma(rgb.x / 255.0);
    double g = gamma(rgb.y / 255.0);
    double b = gamma(rgb.z / 255.0);

    double x = 0.412453 * r + 0.357580 * g + 0.180423 * b;
    double y = 0.212671 * r + 0.715160 * g + 0.072169 * b;
    double z = 0.019334 * r + 0.119193 * g + 0.950227 * b;

    double xr = x / 0.950456;
    double yr = y / 1.0;
    double zr = z / 1.088754;

    double fx = flab(xr);
    double fy = flab(yr);
    double fz = flab(zr);

    return Color3d(116.0 * fy - 16.0, 500.0 * (fx - fy), 200.0 * (fy - fz));
}

//===========================================================================
// Initialize
//===========================================================================

void Slic::setK(int k) {
    this->k = k;
}

void Slic::setM(double m) {
    this->m = m;
}

void Slic::setN(int n) {
    this->nIters = n;
}

void Slic::initializePoints() {
    points.clear();
    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            int id = i * img.cols + j;
            Color3d lab = rgb2lab(Color3d(img.at<cv::Vec3b>(i, j)[2], img.at<cv::Vec3b>(i, j)[1], img.at<cv::Vec3b>(i, j)[0]));
            points.push_back(ImgPoint(id, CIELAB(lab.x, lab.y, lab.z, i, j)));
        }
    }
}

void Slic::initializeCks() {
    Cks.clear();
    s = sqrt(n / k);
    int start = s / 2;
    k = 0;
    for (int i = start; i < img.rows; i += s) {
        for (int j = start; j < img.cols; j += s) {
            Color3d lab = rgb2lab(Color3d(img.at<cv::Vec3b>(i, j)[2], img.at<cv::Vec3b>(i, j)[1], img.at<cv::Vec3b>(i, j)[0]));
            Cks.push_back(ClusterCenter(k, CIELAB(lab.x, lab.y, lab.z, i, j)));
            ++k;
        }
    }
    // Adjust Cluster Centers
    for (int i = 0; i < Cks.size(); ++i) {
        int x = Cks[i].val.x;
        int y = Cks[i].val.y;
        double grads[3][3] = {  { grad(x - 1, y - 1),   grad(x, y - 1), grad(x + 1, y - 1) },
                                { grad(x - 1, y),       grad(x, y),     grad(x + 1, y) },
                                { grad(x - 1, y + 1),   grad(x, y + 1), grad(x + 1, y + 1) } };
        int minX, minY;
        double minGrad = DBL_MAX;
        for (int x_ = 0; x_ < 3; ++x_) {
            for (int y_ = 0; y_ < 3; ++y_) {
                if (grads[x_][y_] <= minGrad) {
                    minGrad = grads[x_][y_];
                    minX = x + x_ - 1;
                    minY = y + y_ - 1;
                }
            }
        }
        Color3d lab = rgb2lab(Color3d(img.at<cv::Vec3b>(minX, minY)[2], img.at<cv::Vec3b>(minX, minY)[1], img.at<cv::Vec3b>(minX, minY)[0]));
        Cks[i].val.x = minX;
        Cks[i].val.y = minY;
        Cks[i].val.l = lab.x;
        Cks[i].val.a = lab.y;
        Cks[i].val.b = lab.z;
    }
}

double Slic::gray(int i, int j) {
    return (img.at<cv::Vec3b>(i, j)[2] + img.at<cv::Vec3b>(i, j)[1] + img.at<cv::Vec3b>(i, j)[0]) / 3;
}

double Slic::grad(int i, int j) {
    if (i < 0 || i >= img.rows || j < 0 || j >= img.cols) {
        return DBL_MAX;
    }
    double upVal = i > 0 ? gray(i - 1, j) : 0;
    double downVal = i < img.rows - 1 ? gray(i + 1, j) : 0;
    double leftVal = j > 0 ? gray(i, j - 1) : 0;
    double rightVal = j < img.cols - 1 ? gray(i, j + 1) : 0;
    return abs(4 * gray(i, j) - (upVal + downVal + leftVal + rightVal));
}

//===========================================================================
// Assignment
//===========================================================================

void Slic::updateLabels() {
    for (int i = 0; i < Cks.size(); ++i) {
        if (Cks[i].id < 0) {
            continue;
        }
        Cks[i].points.clear();
        int _x = Cks[i].val.x >= s ? (Cks[i].val.x - s) : 0;
        int x_ = Cks[i].val.x < img.rows - s ? (Cks[i].val.x + s) : (img.rows - 1);
        int _y = Cks[i].val.y >= s ? (Cks[i].val.y - s) : 0;
        int y_ = Cks[i].val.y < img.cols - s ? (Cks[i].val.y + s) : (img.cols - 1);
        for (int x = _x; x < x_; ++x) {
            for (int y = _y; y < y_; ++y) {
                double d = distance(Cks[i].val, getPoint(x, y));
                if (d < points[getPointId(x, y)].distance) {
                    points[getPointId(x, y)].distance = d;
                    points[getPointId(x, y)].label = Cks[i].id;
                }
            }
        }
    }
}

void Slic::updateClusterCenter() {
    for (int i = 0; i < points.size(); ++i) {
        int id = points[i].label;
        if (id < 0) {
            continue;
        }
        Cks[id].points.push_back(i);
    }
    for (int i = 0; i < Cks.size(); ++i) {
        if (Cks[i].points.size() == 0) {
            Cks[i].id = -1;
        } else {
            int l_ = 0, a_ = 0, b_ = 0, x_ = 0, y_ = 0;
            for (int j = 0; j < Cks[i].points.size(); ++j) {
                int pId = Cks[i].points[j];
                ImgPoint p = points[pId];
                l_ += p.val.l;
                a_ += p.val.a;
                b_ += p.val.b;
                x_ += p.val.x;
                y_ += p.val.y;
            }
            Cks[i].val.l = l_ / Cks[i].points.size();
            Cks[i].val.a = a_ / Cks[i].points.size();
            Cks[i].val.b = b_ / Cks[i].points.size();
            Cks[i].val.x = x_ / Cks[i].points.size();
            Cks[i].val.y = y_ / Cks[i].points.size();
        }
    }
}

double Slic::distance(CIELAB x, CIELAB y) {
    double dc = sqrt(pow(x.l - y.l, 2) + pow(x.a - y.a, 2) + pow(x.b - y.b, 2));
    double ds = sqrt(pow(x.x - y.x, 2) + pow(x.y - y.y, 2));
    return sqrt(pow(dc, 2) + pow(ds * m / (double)s, 2));
}

CIELAB Slic::getPoint(int i, int j) {
    Color3d lab = rgb2lab(Color3d(img.at<cv::Vec3b>(i, j)[2], img.at<cv::Vec3b>(i, j)[1], img.at<cv::Vec3b>(i, j)[0]));
    return CIELAB(lab.x, lab.y, lab.z, i, j);
}

int Slic::getPointId(int i, int j) {
    return i * img.cols + j;
}

//===========================================================================
// Output Mat
//===========================================================================

Mat Slic::output() {
    initializePoints();
    initializeCks();
    for (int i = 0; i < nIters; ++i) {
        updateLabels();
        updateClusterCenter();
    }

    // Output Mat
    Mat dst(img.rows, img.cols, img.type());
    for (int i = 0; i < Cks.size(); ++i) {
        Color3d rgb = getRGB(Cks[i]);
        for (int j = 0; j < Cks[i].points.size(); ++j) {
            int id = Cks[i].points[j];
            dst.at<cv::Vec3b>(points[id].val.x, points[id].val.y)[2] = rgb.x;
            dst.at<cv::Vec3b>(points[id].val.x, points[id].val.y)[1] = rgb.y;
            dst.at<cv::Vec3b>(points[id].val.x, points[id].val.y)[0] = rgb.z;
        }
    }
    return dst;
}

Color3d Slic::getRGB(ClusterCenter c) {
    if (c.points.size() == 0) {
        return Color3d(0, 0, 0);
    }
    int r = 0, g = 0, b = 0;
    for (int i = 0; i < c.points.size(); ++i) {
        int id = c.points[i];
        r += img.at<cv::Vec3b>(points[id].val.x, points[id].val.y)[2];
        g += img.at<cv::Vec3b>(points[id].val.x, points[id].val.y)[1];
        b += img.at<cv::Vec3b>(points[id].val.x, points[id].val.y)[0];
    }
    r /= c.points.size();
    g /= c.points.size();
    b /= c.points.size();
    return Color3d(r, g, b);
}
