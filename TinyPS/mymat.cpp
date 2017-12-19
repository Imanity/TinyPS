#include "mymat.h"

MyMat::MyMat() {

}

Mat MyMat::rotate_l_90(Mat &src) {
    Mat dst(src.cols, src.rows, src.type());
    for (int i = 0; i < src.cols; ++i) {
        for (int j = 0; j < src.rows; ++j) {
            dst.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(j, src.cols - i - 1);
        }
    }
    return dst;
}

Mat MyMat::rotate_r_90(Mat &src) {
    Mat dst(src.cols, src.rows, src.type());
    for (int i = 0; i < src.cols; ++i) {
        for (int j = 0; j < src.rows; ++j) {
            dst.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(src.rows - j - 1, i);
        }
    }
    return dst;
}

Mat MyMat::rotate_180(Mat &src) {
    Mat dst(src.rows, src.cols, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            dst.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(src.rows - i - 1, src.cols - j - 1);
        }
    }
    return dst;
}

Mat MyMat::rotate(Mat &src, int a) {
    double angle = -double(a) * 3.1415927 / 180.0;
    int r = src.rows;
    int c = src.cols;
    int xMin = 0, xMax = 0, yMin = 0, yMax = 0;
    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            int cornerX = c / 2 * i;
            int cornerY = r / 2 * j;
            int x = rotateVertexX(cornerX, cornerY, angle);
            int y = rotateVertexY(cornerX, cornerY, angle);
            if (x < xMin) {
                xMin = x;
            }
            if (x > xMax) {
                xMax = x;
            }
            if (y < yMin) {
                yMin = y;
            }
            if (y > yMax) {
                yMax = y;
            }
        }
    }
    Mat dst(yMax - yMin + 1, xMax - xMin + 1, src.type());
    int newR = yMax - yMin + 1;
    int newC = xMax - xMin + 1;
    Vec3b white;
    white[0] = white[1] = white[2] = 255;
    for (int i = 0; i < newR; ++i) {
        for (int j = 0; j < newC; ++j) {
            int x = -newC / 2 + j;
            int y = newR / 2 - i;
            int originX = rotateVertexX(x, y, -angle);
            int originY = rotateVertexY(x, y, -angle);
            if (originX < -c / 2 || originX > c / 2 || originY < -r / 2 || originY > r / 2) {
                dst.at<cv::Vec3b>(i, j) = white;
            } else {
                int originI = r / 2 - originY;
                int originJ = c / 2 + originX;
                if (originI < 0) {
                    originI = 0;
                }
                if (originI >= r) {
                    originI = r - 1;
                }
                if (originJ < 0) {
                    originJ = 0;
                }
                if (originJ >= c) {
                    originJ = c - 1;
                }
                dst.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(originI, originJ);
            }
        }
    }
    return dst;
}

int MyMat::rotateVertexX(int x, int y, double angle) {
    return int((double)x * cos(angle) - (double)y * sin(angle));
}

int MyMat::rotateVertexY(int x, int y, double angle) {
    return int((double)y * cos(angle) + (double)x * sin(angle));
}

Mat MyMat::flip_h(Mat &src) {
    Mat dst(src.rows, src.cols, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            dst.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(i, src.cols - j - 1);
        }
    }
    return dst;
}

Mat MyMat::flip_v(Mat &src) {
    Mat dst(src.rows, src.cols, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            dst.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(src.rows - i - 1, j);
        }
    }
    return dst;
}

Mat MyMat::cutRect(Mat &src, int x1, int x2, int y1, int y2) {
    if (x1 > x2) {
        int tmp = x1;
        x1 = x2;
        x2 = tmp;
    }
    if (y1 > y2) {
        int tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    Mat dst(y2 - y1, x2 - x1, src.type());
    for (int i = 0; i < y2 - y1; ++i) {
        for (int j = 0; j < x2 - x1; ++j) {
            dst.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(y1 + i, x1 + j);
        }
    }
    return dst;
}

Mat MyMat::cutCircle(Mat &src, int x1, int x2, int y1, int y2) {
    if (x1 > x2) {
        int tmp = x1;
        x1 = x2;
        x2 = tmp;
    }
    if (y1 > y2) {
        int tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    double a = (x2 - x1) / 2;
    double b = (y2 - y1) / 2;
    Mat dst(y2 - y1, x2 - x1, src.type());
    for (int i = 0; i < y2 - y1; ++i) {
        for (int j = 0; j < x2 - x1; ++j) {
            double x = j - a;
            double y = i - b;
            if (x * x * b * b + y * y * a * a - a * a * b * b < 0) {
                dst.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(y1 + i, x1 + j);
            } else {
                setColor(dst, i, j, 255, 255, 255);
            }
        }
    }
    return dst;
}

Mat MyMat::cutPoly(Mat &src, vector<pair<int, int>> &poly) {
    Mat dst(src.rows, src.cols, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            dst.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(i, j);
        }
    }
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            int x = j, y = i;
            if (!isInsidePoly(poly, x, y)) {
                setColor(dst, i, j, 255, 255, 255);
            }
        }
    }
    return dst;
}

Mat MyMat::drawRect(Mat &src, int x, int y, int w, int h) {
    Mat dst(src.rows, src.cols, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            dst.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(i, j);
        }
    }
    for (int i = y; i < y + h; ++i) {
        setColor(dst, i, x, 0, 255, 0);
        setColor(dst, i, x + w, 0, 255, 0);
    }
    for (int j = x; j < x + w; ++j) {
        setColor(dst, y, j, 0, 255, 0);
        setColor(dst, y + h, j, 0, 255, 0);
    }
    return dst;
}

Mat MyMat::drawCircle(Mat &src, int x, int y, int w, int h) {
    Mat dst(src.rows, src.cols, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            dst.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(i, j);
        }
    }
    double a = w / 2, b = h / 2;
    double i = 0;
    double j = b;
    double d = b * b + a * a * (0.25 - b);
    while (i / (a * a) <= j / (b * b)) {
        setColor(dst, int(y + b + j), int(x + a + i), 0, 255, 0);
        setColor(dst, int(y + b + j), int(x + a - i), 0, 255, 0);
        setColor(dst, int(y + b - j), int(x + a - i), 0, 255, 0);
        setColor(dst, int(y + b - j), int(x + a + i), 0, 255, 0);
        if (d < 0) {
            ++i;
            d += b * b * (3 + 2 * i);
        } else {
            ++i;
            --j;
            d += b * b * (3 + 2 * i) + a * a * (2 - 2 * j);
        }
    }
    i = a;
    j = 0;
    d = a * a + b * b * (0.25 - a);
    while (i / (a * a) > j / (b * b)) {
        setColor(dst, int(y + b + j), int(x + a + i), 0, 255, 0);
        setColor(dst, int(y + b + j), int(x + a - i), 0, 255, 0);
        setColor(dst, int(y + b - j), int(x + a - i), 0, 255, 0);
        setColor(dst, int(y + b - j), int(x + a + i), 0, 255, 0);
        if (d < 0) {
            ++j;
            d += a * a * (3 + 2 * j);
        } else {
            --i;
            ++j;
            d += b * b * (2 - 2 * i) + a * a * (3 + 2 * j);
        }
    }
    return dst;
}

Mat MyMat::drawLine(Mat &src, int x1, int y1, int x2, int y2) {
    Mat dst(src.rows, src.cols, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            dst.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(i, j);
        }
    }
    int x, y, dx, dy, sx, sy, se;
    double k, e;
    x = x1;
    y = y1;
    dx = x2 - x1;
    dy = y2 - y1;
    sx = dx > 0 ? 1 : -1;
    sy = dy > 0 ? 1 : -1;
    k = (double)dy / (double)dx;
    e = k > 0 ? -0.5 : 0.5;
    se = k > 0 ? -1 : 1;
    if (dx == 0) {
        for (int i = 0; i <= abs(dy); ++i) {
            setColor(dst, y, x, 0, 255, 0);
            y = y + sy;
        }
    } else if (dy == 0) {
        for (int i = 0; i <= abs(dx); ++i) {
            setColor(dst, y, x, 0, 255, 0);
            x = x + sx;
        }
    } else if (abs(k) <= 1) {
        for (int i = 0; i <= abs(dx); ++i) {
            setColor(dst, y, x, 0, 255, 0);
            x = x + sx;
            e = e + k;
            if ((k > 0 && e >= 0) || (k < 0 && e <= 0)) {
                y = y + sy;
                e = e + se;
            }
        }
    } else {
        k = 1.0 / k;
        for (int j = 0; j <= abs(dy); ++j) {
            setColor(dst, y, x, 0, 255, 0);
            y = y + sy;
            e = e + k;
            if ((k > 0 && e >= 0) || (k < 0 && e <= 0)) {
                x = x + sx;
                e = e + se;
            }
        }
    }
    return dst;
}

void MyMat::setColor(Mat &src, int x, int y, uchar r, uchar g, uchar b) {
    src.at<cv::Vec3b>(x, y)[0] = b;
    src.at<cv::Vec3b>(x, y)[1] = g;
    src.at<cv::Vec3b>(x, y)[2] = r;
}

bool MyMat::isInsidePoly(vector<pair<int, int>> &poly, int x, int y) {
    double totalAngle = 0.0;
    for (int i = 0; i < poly.size(); ++i) {
        double x1 = poly[i].first - x;
        double y1 = poly[i].second - y;
        double x2 = (i == poly.size() - 1) ? (poly[0].first - x) : (poly[i + 1].first - x);
        double y2 = (i == poly.size() - 1) ? (poly[0].second - y) : (poly[i + 1].second - y);
        double a = angleBetween(x1, y1, x2, y2);
        totalAngle += a;
    }
    return (abs(totalAngle) > 6);
}

double MyMat::angleBetween(double x1, double y1, double x2, double y2) {
    double sinValue = (x1 * y2 - x2 * y1) / (sqrt(x1 * x1 + y1 * y1) * sqrt(x2 * x2 + y2 * y2));
    double cosValue = (x1 * x2 + y1 * y2) / (sqrt(x1 * x1 + y1 * y1) * sqrt(x2 * x2 + y2 * y2));
    if (cosValue >= 1.0) {
        return 0;
    }
    if (cosValue <= -1.0) {
        return PI;
    }
    double a = acos(cosValue);
    if (sinValue < 0) {
        a = -a;
    }
    return a;
}

Color3 MyMat::rgb2hsi(Color3 &rgb) {
    double r = rgb.x, g = rgb.y, b = rgb.z;
    double h = acos((2 * r - g - b) / (2 * sqrt((r - g) * (r - g) + (r - b) * (g - b))));
    if (g < b) {
        h = 2 * PI - h;
    }
    double s = 1 - 3 * min(r, min(g, b)) / (r + g + b);
    double i = (r + g + b) / 3;
    return Color3(h, s, i);
}

Color3 MyMat::hsi2rgb(Color3 &hsi) {
    double h = hsi.x, s = hsi.y, i = hsi.z;
    double r, g, b;
    if (h >= 0 && h < 2.0 * PI / 3.0) {
        r = i * (1 + s * cos(h) / cos(PI / 3.0 - h));
        b = i * (1 - s);
        g = 3 * i - (b + r);
    } else if (h >= 2.0 * PI / 3.0 && h < 4.0 * PI / 3.0) {
        r = i * (1 - s);
        g = i * (1 + s * cos(h - 2.0 * PI / 3.0) / cos(PI - h));
        b = 3 * i - (r + g);
    } else {
        g = i * (1 - s);
        b = i * (1 + s * cos(h - 4.0 * PI / 3.0) / cos(PI * 5.0 / 3.0 - h));
        r = 3 * i - (g + b);
    }
    r = r < 0 ? 0 : r;
    r = r > 255 ? 255 : r;
    g = g < 0 ? 0 : g;
    g = g > 255 ? 255 : g;
    b = b < 0 ? 0 : b;
    b = b > 255 ? 255 : b;
    return Color3(r, g, b);
}

Mat MyMat::modifyHSI(Mat &src, int deltaH, int deltaS, int deltaI) {
    Mat dst(src.rows, src.cols, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            Color3 currRGB = Color3(src.at<cv::Vec3b>(i, j)[2], src.at<cv::Vec3b>(i, j)[1], src.at<cv::Vec3b>(i, j)[0]);
            Color3 currHSI = rgb2hsi(currRGB);
            currHSI.x += deltaH * PI / 180.0;
            currHSI.y += deltaS / 100.0;
            currHSI.z += deltaI;
            if (currHSI.z < 0) {
                currHSI.z = 0;
            }
            if (currHSI.z > 255) {
                currHSI.z = 255;
            }
            if (currHSI.y < 0) {
                currHSI.y = 0;
            }
            if (currHSI.y > 1) {
                currHSI.y = 1;
            }
            if (currHSI.z < 0) {
                currHSI.z += 2.0 * PI;
            }
            if (currHSI.z >= 2.0 * PI) {
                currHSI.z -= 2.0 * PI;
            }
            currRGB = hsi2rgb(currHSI);
            dst.at<cv::Vec3b>(i, j)[0] = currRGB.z;
            dst.at<cv::Vec3b>(i, j)[1] = currRGB.y;
            dst.at<cv::Vec3b>(i, j)[2] = currRGB.x;
        }
    }
    return dst;
}

Mat MyMat::halftone(Mat &src) {
    Mat dst(src.rows, src.cols, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            setColor(dst, i, j, 255, 255, 255);
        }
    }
    double angles[3] = { PI / 4, PI / 3, PI / 7 };
    int cmy[3][3] = { { 0, 255, 255 }, { 255, 0, 255 }, { 255, 255, 0 } };
    double d = 10;
    double r = src.rows, c = src.cols;
    for (int colorId = 0; colorId < 3; ++colorId) {
        double theta = angles[colorId];
        pair<double, double> p0 = rotateVec(make_pair(0, 0), theta);
        pair<double, double> p1 = rotateVec(make_pair(c, 0), theta);
        pair<double, double> p2 = rotateVec(make_pair(c, r), theta);
        pair<double, double> p3 = rotateVec(make_pair(0, r), theta);
        double xMin = min(min(p0.first, p1.first), min(p2.first, p3.first));
        double xMax = max(max(p0.first, p1.first), max(p2.first, p3.first));
        double yMin = min(min(p0.second, p1.second), min(p2.second, p3.second));
        double yMax = max(max(p0.second, p1.second), max(p2.second, p3.second));
        double x_ = xMin, y_ = yMin;
        while (x_ <= xMax) {
            while (y_ <= yMax) {
                pair<double, double> p = rotateVec(make_pair(x_, y_), -theta);
                double channel = 0, channelMax = 0;
                int x = p.first, y = p.second;
                y_ += d;
                if (x < 0 || x >= c || y < 0 || y >= r) {
                    continue;
                }

                for (int i = x - d / 2; i <= x + d / 2; ++i) {
                    for (int j = y - d / 2; j <= y + d / 2; ++j) {
                        if (i < 0 || i >= c || j < 0 || j >= r) {
                            continue;
                        }
                        channelMax += 1;
                        channel += double(src.at<cv::Vec3b>(j, i)[2 - colorId]) / 255.0;
                    }
                }
                double rate = 1.0 - channel / channelMax;
                int cr = d * rate / 2;
                drawPot(dst, y, x, cr, cmy[colorId][0], cmy[colorId][1], cmy[colorId][2]);
            }
            x_ += d;
            y_ = yMin;
        }
    }
    return dst;
}

pair<double, double> MyMat::rotateVec(pair<double, double> v, double theta) {
    double x = v.first, y = v.second;
    return make_pair(x * cos(theta) - y * sin(theta), x * sin(theta) + y * cos(theta));
}

void MyMat::drawPot(Mat &src, int cx, int cy, int cr, int r, int g, int b) {
    for (int i = -cr; i <= cr; ++i) {
        for (int j = -cr; j <= cr; ++j) {
            if (i * i + j * j <= cr * cr) {
                int x = cx + i;
                int y = cy + j;
                if (x < 0 || x >= src.rows || y < 0 || y >= src.cols) {
                    continue;
                }
                src.at<cv::Vec3b>(x, y)[0] += (b - 255);
                src.at<cv::Vec3b>(x, y)[1] += (g - 255);
                src.at<cv::Vec3b>(x, y)[2] += (r - 255);
            }
        }
    }
}

int MyMat::rgb2gray(Color3 rgb) {
    return (rgb.x + rgb.y + rgb.z) / 3;
}

void MyMat::setRGBgray(Color3 &rgb, int gray) {
    /*
    int originGray = rgb2gray(rgb);
    rgb.x += (gray - originGray);
    rgb.y += (gray - originGray);
    rgb.z += (gray - originGray);
    rgb.x = rgb.x > 255 ? 255 : rgb.x;
    rgb.x = rgb.x < 0 ? 0 : rgb.x;
    rgb.y = rgb.y > 255 ? 255 : rgb.y;
    rgb.y = rgb.y < 0 ? 0 : rgb.y;
    rgb.z = rgb.z > 255 ? 255 : rgb.z;
    rgb.z = rgb.z < 0 ? 0 : rgb.z;
    */
    Color3 hsi = rgb2hsi(rgb);
    hsi.z = gray;
    rgb = hsi2rgb(hsi);
}

Mat MyMat::modifyLinearGray(Mat &src, int a, int b, int c, int d) {
    Mat dst(src.rows, src.cols, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            Color3 currRGB = Color3(src.at<cv::Vec3b>(i, j)[2], src.at<cv::Vec3b>(i, j)[1], src.at<cv::Vec3b>(i, j)[0]);
            int currGray = rgb2gray(currRGB);
            int newGray = 0;
            if (currGray < a) {
                newGray = c * currGray / a;
            } else if (currGray >= b) {
                newGray = (255 - d) * (currGray - b) / (255 - b) + d;
            } else {
                newGray = (d - c) * (currGray - a) / (b - a) + c;
            }
            newGray = newGray > 255 ? 255 : newGray;
            newGray = newGray < 0 ? 0 : newGray;
            setRGBgray(currRGB, newGray);
            dst.at<cv::Vec3b>(i, j)[0] = currRGB.z;
            dst.at<cv::Vec3b>(i, j)[1] = currRGB.y;
            dst.at<cv::Vec3b>(i, j)[2] = currRGB.x;
        }
    }
    return dst;
}

Mat MyMat::modifyLogGray(Mat &src, double a, double b, double c) {
    Mat dst(src.rows, src.cols, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            Color3 currRGB = Color3(src.at<cv::Vec3b>(i, j)[2], src.at<cv::Vec3b>(i, j)[1], src.at<cv::Vec3b>(i, j)[0]);
            int currGray = rgb2gray(currRGB);
            int newGray = (double)a + log((double)currGray + 1) / (b * log(c));
            newGray = newGray > 255 ? 255 : newGray;
            newGray = newGray < 0 ? 0 : newGray;
            setRGBgray(currRGB, newGray);
            dst.at<cv::Vec3b>(i, j)[0] = currRGB.z;
            dst.at<cv::Vec3b>(i, j)[1] = currRGB.y;
            dst.at<cv::Vec3b>(i, j)[2] = currRGB.x;
        }
    }
    return dst;
}

Mat MyMat::modifyEGray(Mat &src, double a, double b, double c) {
    Mat dst(src.rows, src.cols, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            Color3 currRGB = Color3(src.at<cv::Vec3b>(i, j)[2], src.at<cv::Vec3b>(i, j)[1], src.at<cv::Vec3b>(i, j)[0]);
            int currGray = rgb2gray(currRGB);
            int newGray = pow(b, c * (currGray - a)) - 1;
            newGray = newGray > 255 ? 255 : newGray;
            newGray = newGray < 0 ? 0 : newGray;
            setRGBgray(currRGB, newGray);
            dst.at<cv::Vec3b>(i, j)[0] = currRGB.z;
            dst.at<cv::Vec3b>(i, j)[1] = currRGB.y;
            dst.at<cv::Vec3b>(i, j)[2] = currRGB.x;
        }
    }
    return dst;
}

Mat MyMat::modifyGammaGray(Mat &src, double c, double gamma) {
    Mat dst(src.rows, src.cols, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            Color3 currRGB = Color3(src.at<cv::Vec3b>(i, j)[2], src.at<cv::Vec3b>(i, j)[1], src.at<cv::Vec3b>(i, j)[0]);
            int currGray = rgb2gray(currRGB);
            int newGray = c * pow(currGray, gamma);
            newGray = newGray > 255 ? 255 : newGray;
            newGray = newGray < 0 ? 0 : newGray;
            setRGBgray(currRGB, newGray);
            dst.at<cv::Vec3b>(i, j)[0] = currRGB.z;
            dst.at<cv::Vec3b>(i, j)[1] = currRGB.y;
            dst.at<cv::Vec3b>(i, j)[2] = currRGB.x;
        }
    }
    return dst;
}

vector<double> MyMat::getGrayHistogram(Mat &src) {
    vector<double> res(256, 0);
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            Color3 currRGB = Color3(src.at<cv::Vec3b>(i, j)[2], src.at<cv::Vec3b>(i, j)[1], src.at<cv::Vec3b>(i, j)[0]);
            int currGray = rgb2gray(currRGB);
            res[currGray] += 1;
        }
    }
    for (int i = 0; i < res.size(); ++i) {
        res[i] /= (double)(src.rows * src.cols);
    }
    return res;
}

Mat MyMat::equalization(Mat &src) {
    Mat dst(src.rows, src.cols, src.type());
    vector<double> histogram = getGrayHistogram(src);
    vector<int> newGrayArr(256, 0);
    double s = 0;
    for (int i = 0; i < 256; ++i) {
        s += histogram[i];
        newGrayArr[i] = int(s * 255.0);
    }
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            Color3 currRGB = Color3(src.at<cv::Vec3b>(i, j)[2], src.at<cv::Vec3b>(i, j)[1], src.at<cv::Vec3b>(i, j)[0]);
            int currGray = rgb2gray(currRGB);
            int newGray = newGrayArr[currGray];
            setRGBgray(currRGB, newGray);
            dst.at<cv::Vec3b>(i, j)[0] = currRGB.z;
            dst.at<cv::Vec3b>(i, j)[1] = currRGB.y;
            dst.at<cv::Vec3b>(i, j)[2] = currRGB.x;
        }
    }
    return dst;
}

Mat MyMat::specificationSML(Mat &src, vector<double> newHistogram) {
    Mat dst(src.rows, src.cols, src.type());
    vector<double> originHistogram = getGrayHistogram(src);
    vector<int> newGrayArr(256, 0);
    vector<double> srcHis(256, 0);
    vector<double> dstHis(256, 0);
    srcHis[0] = originHistogram[0];
    dstHis[0] = newHistogram[0];
    for (int i = 1; i < 256; ++i) {
        srcHis[i] = srcHis[i - 1] + originHistogram[i];
    }
    for (int i = 1; i < 256; ++i) {
        dstHis[i] = dstHis[i - 1] + newHistogram[i];
    }
    for (int i = 0; i < 256; ++i) {
        double minDis = abs(srcHis[i] - dstHis[0]);
        for (int j = 0; j < 256; ++j) {
            double currDis = abs(srcHis[i] - dstHis[j]);
            if (currDis <= minDis) {
                minDis = currDis;
                newGrayArr[i] = j;
            } else {
                break;
            }
        }
    }
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            Color3 currRGB = Color3(src.at<cv::Vec3b>(i, j)[2], src.at<cv::Vec3b>(i, j)[1], src.at<cv::Vec3b>(i, j)[0]);
            int currGray = rgb2gray(currRGB);
            int newGray = newGrayArr[currGray];
            setRGBgray(currRGB, newGray);
            dst.at<cv::Vec3b>(i, j)[0] = currRGB.z;
            dst.at<cv::Vec3b>(i, j)[1] = currRGB.y;
            dst.at<cv::Vec3b>(i, j)[2] = currRGB.x;
        }
    }
    return dst;
}

Mat MyMat::specificationGML(Mat &src, vector<double> newHistogram) {
    Mat dst(src.rows, src.cols, src.type());
    vector<double> originHistogram = getGrayHistogram(src);
    vector<int> newGrayArr(256, 0);
    vector<double> srcHis(256, 0);
    vector<double> dstHis(256, 0);
    srcHis[0] = originHistogram[0];
    dstHis[0] = newHistogram[0];
    for (int i = 1; i < 256; ++i) {
        srcHis[i] = srcHis[i - 1] + originHistogram[i];
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
                newGrayArr[j] = i;
            }
            lastStart = start;
            lastEnd = end;
            start = lastEnd + 1;
        }
    }
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            Color3 currRGB = Color3(src.at<cv::Vec3b>(i, j)[2], src.at<cv::Vec3b>(i, j)[1], src.at<cv::Vec3b>(i, j)[0]);
            int currGray = rgb2gray(currRGB);
            int newGray = newGrayArr[currGray];
            setRGBgray(currRGB, newGray);
            dst.at<cv::Vec3b>(i, j)[0] = currRGB.z;
            dst.at<cv::Vec3b>(i, j)[1] = currRGB.y;
            dst.at<cv::Vec3b>(i, j)[2] = currRGB.x;
        }
    }
    return dst;
}

Mat MyMat::medianFilter(Mat &src, int n) {
    if (src.rows < n || src.cols < n) {
        Mat newSrc(src.rows, src.cols, src.type());
        newSrc = src;
        return newSrc;
    }
    // Mat dst(src.rows - n + 1, src.cols - n + 1, src.type());
    Mat dst(src.rows, src.cols, src.type());
    int align = n / 2;
    int th = n * n / 2 + 1;
    for (int channel = 0; channel < 3; ++channel) {
        for (int top = 0; top < src.rows - n + 1; ++top) {
            vector<int> histogram(256, 0);
            // get first histogram
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    histogram[src.at<cv::Vec3b>(i + top, j)[channel]]++;
                }
            }
            // get first med
            int med = getMedian(histogram, n * n);
            int mNum = 0;
            for (int i = 0; i <= med; ++i) {
                mNum += histogram[i];
            }
            // set value
            dst.at<cv::Vec3b>(top + align - 1, 0 + align - 1)[channel] = med;
            // window move right
            for (int j = 1; j < src.cols - n + 1; ++j) {
                // Remove left column
                for (int i = 0; i < n; ++i) {
                    int gl = src.at<cv::Vec3b>(i + top, j - 1)[channel];
                    histogram[gl]--;
                    if (gl <= med) {
                        mNum--;
                    }
                }
                // Add right column
                for (int i = 0; i < n; ++i) {
                    int gr = src.at<cv::Vec3b>(i + top, j + n - 1)[channel];
                    histogram[gr]++;
                    if (gr <= med) {
                        mNum++;
                    }
                }
                // Update med
                if (mNum > th) {
                    while (mNum > th) {
                        mNum -= histogram[med];
                        med--;
                    }
                } else if (mNum < th) {
                    while (mNum < th) {
                        med++;
                        mNum += histogram[med];
                    }
                }
                // set value
                dst.at<cv::Vec3b>(top + align - 1, j + align - 1)[channel] = med;
            }
        }
        // Get the border value
        for (int j = 0; j < src.cols; ++j) {
            for (int i = 0; i < align; ++i) {
                vector<int> v;
                for (int k = max(0, i - align); k < min(src.rows, i + n - align); ++k) {
                    for (int l = max(0, j - align); l < min(src.cols, j + n - align); ++l) {
                        v.push_back(src.at<cv::Vec3b>(k, l)[channel]);
                    }
                }
                dst.at<cv::Vec3b>(i, j)[channel] = v[int(v.size() / 2) - 1];
            }
            for (int i = src.rows - n + align; i < src.rows; ++i) {
                vector<int> v;
                for (int k = max(0, i - align); k < min(src.rows, i + n - align); ++k) {
                    for (int l = max(0, j - align); l < min(src.cols, j + n - align); ++l) {
                        v.push_back(src.at<cv::Vec3b>(k, l)[channel]);
                    }
                }
                dst.at<cv::Vec3b>(i, j)[channel] = v[int(v.size() / 2) - 1];
            }
        }
        for (int i = 0; i < src.rows; ++i) {
            for (int j = 0; j < align; ++j) {
                vector<int> v;
                for (int k = max(0, i - align); k < min(src.rows, i + n - align); ++k) {
                    for (int l = max(0, j - align); l < min(src.cols, j + n - align); ++l) {
                        v.push_back(src.at<cv::Vec3b>(k, l)[channel]);
                    }
                }
                dst.at<cv::Vec3b>(i, j)[channel] = v[int(v.size() / 2) - 1];
            }
            for (int j = src.cols - n + align; j < src.cols; ++j) {
                vector<int> v;
                for (int k = max(0, i - align); k < min(src.rows, i + n - align); ++k) {
                    for (int l = max(0, j - align); l < min(src.cols, j + n - align); ++l) {
                        v.push_back(src.at<cv::Vec3b>(k, l)[channel]);
                    }
                }
                dst.at<cv::Vec3b>(i, j)[channel] = v[int(v.size() / 2) - 1];
            }
        }
    }
    return dst;
}

int MyMat::getMedian(vector<int> &histogram, int n) {
    int id = n * n / 2 + 1;
    for (int i = 0; i < histogram.size(); ++i) {
        id -= histogram[i];
        if (id <= 0) {
            return i;
        }
    }
    return ((int)histogram.size() - 1);
}

Mat MyMat::gaussianFilter(Mat &src, int n) {
    if (src.rows < n || src.cols < n) {
        Mat newSrc(src.rows, src.cols, src.type());
        newSrc = src;
        return newSrc;
    }
    Mat dstTmp(src.rows, src.cols, src.type());
    Mat dst(src.rows, src.cols, src.type());
    // Get Gaussian window
    vector<int> window = getGaussianArray(n - 1);
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += window[i];
    }
    for (int channel = 0; channel < 3; channel++) {
        // horizontal
        for (int i = 0; i < src.rows; ++i) {
            for (int j = 0; j < src.cols; ++j) {
                int left = j - n / 2;
                int right = j + n - n / 2 - 1;
                int s = 0, sum = 0;
                for (int k = left; k <= right; ++k) {
                    if (k >= 0 && k < src.cols) {
                        int id = k - left;
                        s += window[id] * src.at<cv::Vec3b>(i, k)[channel];
                        sum += window[id];
                    }
                }
                dstTmp.at<cv::Vec3b>(i, j)[channel] = s / sum;
            }
        }
        // vertical
        for (int j = 0; j < src.cols; ++j) {
            for (int i = 0; i < src.rows; ++i) {
                int top = i - n / 2;
                int bottom = i + n - n / 2 - 1;
                int s = 0, sum = 0;
                for (int k = top; k <= bottom; ++k) {
                    if (k >= 0 && k < src.rows) {
                        int id = k - top;
                        s += window[id] * dstTmp.at<cv::Vec3b>(k, j)[channel];
                        sum += window[id];
                    }
                }
                dst.at<cv::Vec3b>(i, j)[channel] = s / sum;
            }
        }
    }
    return dst;
}

vector<int> MyMat::getGaussianArray(int n) {
    vector<int> row(n + 1, 0);
    row[0] = 1;
    for (int i = 1; i <= n; ++i) {
        int j = 0;
        for (; j < i - 1; ++j) {
            row[j] += row[j + 1];
        }
        row[j] = 1;
        for (; j >= 0; --j) {
            row[j + 1] = row[j];
        }
        row[0] = 1;
    }
    return row;
}

Mat MyMat::sharpen(Mat &src) {
    double w[3][3] = { { 0, -1, 0 }, { -1, 5, -1 }, { 0, -1, 0 } };
    Mat dst(src.rows, src.cols, src.type());
    for (int channel = 0; channel < 3; ++channel) {
        for (int i = 0; i < src.rows; ++i) {
            for (int j = 0; j < src.cols; ++j) {
                double s = 0, sum = 0;
                for (int i_ = i - 1; i_ <= i + 1; ++i_) {
                    for (int j_ = j - 1; j_ <= j + 1; ++j_) {
                        if (i_ >= 0 && j_ >= 0 && i_ < src.rows && j_ < src.cols) {
                            s += (double)src.at<cv::Vec3b>(i_, j_)[channel] * w[i_ - i + 1][j_ - j + 1];
                            sum += w[i_ - i + 1][j_ - j + 1];
                        }
                    }
                }
                int c = s / sum;
                c = c > 255 ? 255 : c;
                c = c < 0 ? 0 : c;
                dst.at<cv::Vec3b>(i, j)[channel] = c;
            }
        }
    }
    return dst;
}

Mat MyMat::radialBlur(Mat &src) {
    Mat dst(src.rows, src.cols, src.type());
    double dMax = sqrt(src.rows * src.rows / 4 + src.cols * src.cols / 4);
    int sizeMax = 50;
    for (int channel = 0; channel < 3; ++channel) {
        for (int i = 0; i < src.rows; ++i) {
            for (int j = 0; j < src.cols; ++j) {
                double s = 0, sum = 0;
                double d = sqrt((i - src.rows / 2) * (i - src.rows / 2) + (j - src.cols / 2) * (j - src.cols / 2));
                int size = d * sizeMax / dMax;
                size = size < 2 ? 2 : size;
                size = size > sizeMax ? sizeMax : size;
                for (int i_ = i - size / 2; i_ <= i + size - 1 - size / 2; ++i_) {
                    for (int j_ = j - size / 2; j_ <= j + size - 1 - size / 2; ++j_) {
                        if (i_ >= 0 && j_ >= 0 && i_ < src.rows && j_ < src.cols) {
                            pair<int, int> a = make_pair(i, j);
                            pair<int, int> b = make_pair(i_, j_);
                            pair<int, int> c = make_pair(src.rows / 2, src.cols / 2);
                            double area = abs(a.first * b.second + b.first * c.second + c.first * a.second -
                                              c.first * b.second - b.first * a.second - a.first * c.second);
                            double w = 1.0 - (area / sqrt((c.first - a.first) * (c.first - a.first) +
                                                          (c.second - a.second) * (c.second - a.second)));
                            w = w < 0 ? 0 : w;
                            s += (double)src.at<cv::Vec3b>(i_, j_)[channel] * w;
                            sum += w;
                        }
                    }
                }
                int c = s / sum;
                c = c > 255 ? 255 : c;
                c = c < 0 ? 0 : c;
                dst.at<cv::Vec3b>(i, j)[channel] = c;
            }
        }
    }
    return dst;
}

void MyMat::shiftDFT(Mat &mag) {
    mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));

    Mat tmp;

    int cx = mag.cols / 2, cy = mag.rows / 2;

    Mat q0(mag, Rect(0, 0, cx, cy));
    Mat q1(mag, Rect(cx, 0, cx, cy));
    Mat q2(mag, Rect(0, cy, cx, cy));
    Mat q3(mag, Rect(cx, cy, cx, cy));

    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

Mat MyMat::MyDFT(Mat &src) {
    int m = getOptimalDFTSize(src.rows);
    int n = getOptimalDFTSize(src.cols);

    Mat padded;
    copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
    Mat complexImg;
    merge(planes, 2, complexImg);

    dft(complexImg, complexImg);

    return complexImg;
}

Mat MyMat::MyIDFT(Mat &src) {
    Mat invDFT, invDFTcvt;
    idft(src, invDFT, DFT_SCALE | DFT_REAL_OUTPUT);
    invDFT.convertTo(invDFTcvt, CV_8U);
    return invDFTcvt;
}

Mat MyMat::create_butterworth_lowpass_filter(Mat &src, int d0, int n) {
    Mat filter = src.clone();
    Mat tmp = Mat(src.rows, src.cols, CV_32F);
    Mat tmp_ = Mat(src.rows, src.cols, CV_32F);
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            double d = sqrt(pow((i - src.rows / 2), 2) + pow((j - src.cols / 2), 2));
            double h = 1.0 / (1.0 + pow(d / (double)d0, 2.0 * n));
            tmp.at<float>(i, j) = h;
            tmp_.at<float>(i, j) = 0;
        }
    }
    Mat toMerge[] = { tmp, tmp_ };
    merge(toMerge, 2, filter);
    return filter;
}

Mat MyMat::create_butterworth_highpass_filter(Mat &src, int d0, int n) {
    Mat filter = src.clone();
    Mat tmp = Mat(src.rows, src.cols, CV_32F);
    Mat tmp_ = Mat(src.rows, src.cols, CV_32F);
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            double d = sqrt(pow((i - src.rows / 2), 2) + pow((j - src.cols / 2), 2));
            double h = 1.0 / (1.0 + pow((double)d0 / d, 2.0 * n));
            tmp.at<float>(i, j) = h + 0.4;
            tmp_.at<float>(i, j) = 0;
        }
    }
    Mat toMerge[] = { tmp, tmp_ };
    merge(toMerge, 2, filter);
    return filter;
}

Mat MyMat::frequencySmooth(Mat &src, int d0, int n) {
    Mat dst;
    vector<Mat> channels;
    split(src, channels);
    for (int i = 0; i < 3; ++i) {
        Mat complexImg = MyDFT(channels[i]);
        shiftDFT(complexImg);

        Mat filter = create_butterworth_lowpass_filter(complexImg, d0, n);
        mulSpectrums(complexImg, filter, complexImg, 0);

        shiftDFT(complexImg);
        channels[i] = MyIDFT(complexImg);
    }
    merge(channels, dst);
    return dst;
}

Mat MyMat::frequencySharpen(Mat &src, int d0, int n) {
    Mat dst;
    vector<Mat> channels;
    split(src, channels);
    for (int i = 0; i < 3; ++i) {
        Mat complexImg = MyDFT(channels[i]);
        shiftDFT(complexImg);

        Mat filter = create_butterworth_highpass_filter(complexImg, d0, n);
        mulSpectrums(complexImg, filter, complexImg, 0);

        shiftDFT(complexImg);
        channels[i] = MyIDFT(complexImg);
    }
    merge(channels, dst);
    dst = equalization(dst);
    return dst;
}
