#ifndef SLIC_H
#define SLIC_H

#include <opencv2/opencv.hpp>

#include <climits>

#include <QDebug>

using namespace cv;
using namespace std;

struct Color3d {
    double x, y, z;
    Color3d(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
};

struct CIELAB {
    int l, a, b;
    int x, y;
    CIELAB(int l_, int a_, int b_, int x_, int y_) : l(l_), a(a_), b(b_), x(x_), y(y_) {}
};

struct ClusterCenter {
    int id;
    CIELAB val;
    vector<int> points;
    ClusterCenter(int id_, CIELAB v_) : id(id_), val(v_) {}
};

struct ImgPoint {
    int id;
    CIELAB val;
    double distance;
    int label;
    ImgPoint(int id_, CIELAB v_) : id(id_), val(v_) {
        distance = DBL_MAX;
        label = -1;
    }
};

class Slic {
public:
    Mat img;
    int n;                          // img.rows * img.cols
    int k;                          // Number of clusters
    int s;                          // Distance between cluster centers
    double m;                       // Weigh the relative importance between color similarity and spatial proximity
    int nIters;                     // Times for iteration

private:
    vector<ClusterCenter> Cks;      // Cluster centers
    vector<ImgPoint> points;        // Points in image

public:
    Slic(Mat img);
    Mat output();

public:
    void setK(int k);
    void setM(double m);
    void setN(int n);

private:
    void initializePoints();
    void initializeCks();
    void updateLabels();
    void updateClusterCenter();

private:
    // Color translation
    Color3d rgb2lab(Color3d rgb);
    double gamma(double x);
    double flab(double x);
    // Get Grads
    double gray(int i, int j);
    double grad(int i, int j);
    // Compute Distance
    double distance(CIELAB x, CIELAB y);
    // Get a point
    CIELAB getPoint(int i, int j);
    int getPointId(int i, int j);
    // Get the rgb info from a cluster center
    Color3d getRGB(ClusterCenter c);
};

#endif // SLIC_H
