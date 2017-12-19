#include "dialogfacedetection.h"
#include "ui_dialogfacedetection.h"

//===========================================================================
// Construction
//===========================================================================

DialogFaceDetection::DialogFaceDetection(QWidget *parent) : QDialog(parent), ui(new Ui::DialogFaceDetection) {
    ui->setupUi(this);

    connect(ui->viewBtn, SIGNAL(clicked(bool)), this, SLOT(viewSlot()));
    connect(ui->submitBtn, SIGNAL(clicked(bool)), this, SLOT(submitSlot()));
    connect(this, SIGNAL(sendResult(Mat)), parent, SLOT(updateBeautySlot(Mat)));
}

DialogFaceDetection::~DialogFaceDetection() {
    delete ui;
}

void DialogFaceDetection::setImage(Mat m) {
    img = m.clone();
    imgOrigin = m.clone();
    imgOutput = m.clone();
    imgMask = m.clone();
    imgTmp = m.clone();

    skinExtract();
    medianFilter();

    for (int i = 0; i < imgOutput.rows; ++i) {
        for (int j = 0; j < imgOutput.cols; ++j) {
            imgOutput.at<cv::Vec3b>(i, j)[0] = imgOutput.at<cv::Vec3b>(i, j)[1] = imgOutput.at<cv::Vec3b>(i, j)[2] = 255;
            imgMask.at<cv::Vec3b>(i, j)[0] = imgMask.at<cv::Vec3b>(i, j)[1] = imgMask.at<cv::Vec3b>(i, j)[2] = 255;
        }
    }

    showImg();
    showImgOutput();
}

//===========================================================================
// Show image
//===========================================================================

void DialogFaceDetection::viewSlot() {
    if (step == STEP_SELECT) {
        for (int i = 0; i < imgOutput.rows; ++i) {
            for (int j = 0; j < imgOutput.cols; ++j) {
                imgOutput.at<cv::Vec3b>(i, j)[0] = imgOutput.at<cv::Vec3b>(i, j)[1] = imgOutput.at<cv::Vec3b>(i, j)[2] = 255;
            }
        }
        showImgOutput();
    } else if (step == STEP_FILTER) {
        sigmaR = ui->filterSlider->value();
        if (sigmaR != 0) {
            bilateralFilter();
        } else {
            imgOutput = imgOrigin.clone();
        }
        showImgOutput();
    } else if (step == STEP_LIGHT) {
        deltaB = ui->lightSlider->value();
        adjustBrightness();
        showImgOutput();
    }
    ui->submitBtn->setEnabled(true);
}

void DialogFaceDetection::submitSlot() {
    if (step == STEP_SELECT) {
        ui->viewBtn->setText("预览");
        ui->hintLabel->setText("磨皮");
        ui->filterSlider->setEnabled(true);
        ui->lightSlider->setEnabled(false);
        imgMask = imgOutput.clone();
        img = imgOrigin.clone();
        for (int i = 0; i < imgOutput.rows; ++i) {
            for (int j = 0; j < imgOutput.cols; ++j) {
                imgOutput.at<cv::Vec3b>(i, j)[0] = imgOutput.at<cv::Vec3b>(i, j)[1] = imgOutput.at<cv::Vec3b>(i, j)[2] = 255;
            }
        }
    } else if (step == STEP_FILTER) {
        ui->hintLabel->setText("美白");
        ui->filterSlider->setEnabled(false);
        ui->lightSlider->setEnabled(true);
        img = imgOutput.clone();
        for (int i = 0; i < imgOutput.rows; ++i) {
            for (int j = 0; j < imgOutput.cols; ++j) {
                imgOutput.at<cv::Vec3b>(i, j)[0] = imgOutput.at<cv::Vec3b>(i, j)[1] = imgOutput.at<cv::Vec3b>(i, j)[2] = 255;
            }
        }
    } else {
        emit sendResult(imgOutput);
        this->close();
        return;
    }
    step++;
    showImg();
    showImgOutput();
    ui->submitBtn->setEnabled(false);
}

void DialogFaceDetection::showImg() {
    QImage qimg;
    Mat tmpImg;
    if (img.channels() == 3) {
        cvtColor(img, tmpImg, CV_BGR2RGB);
        qimg = QImage((const unsigned char *)(tmpImg.data), tmpImg.cols, tmpImg.rows, tmpImg.cols*tmpImg.channels(), QImage::Format_RGB888);
    } else if (img.channels() == 1) {
        qimg = QImage((const unsigned char *)(img.data), img.cols, img.rows, img.cols*img.channels(), QImage::Format_ARGB32);
    } else {
        qimg = QImage((const unsigned char *)(img.data), img.cols, img.rows, img.cols*img.channels(), QImage::Format_RGB888);
    }
    ui->imgLabel1->setFixedSize(img.cols, img.rows);
    ui->imgLabel1->setPixmap(QPixmap::fromImage(qimg));
}

void DialogFaceDetection::showImgOutput() {
    QImage qimg;
    Mat tmpImg;
    if (imgOutput.channels() == 3) {
        cvtColor(imgOutput, tmpImg, CV_BGR2RGB);
        qimg = QImage((const unsigned char *)(tmpImg.data), tmpImg.cols, tmpImg.rows, tmpImg.cols*tmpImg.channels(), QImage::Format_RGB888);
    } else if (imgOutput.channels() == 1) {
        qimg = QImage((const unsigned char *)(imgOutput.data), imgOutput.cols, imgOutput.rows, imgOutput.cols*imgOutput.channels(), QImage::Format_ARGB32);
    } else {
        qimg = QImage((const unsigned char *)(imgOutput.data), imgOutput.cols, imgOutput.rows, imgOutput.cols*imgOutput.channels(), QImage::Format_RGB888);
    }
    ui->imgLabel2->setFixedSize(imgOutput.cols, imgOutput.rows);
    ui->imgLabel2->setPixmap(QPixmap::fromImage(qimg));
}

//===========================================================================
// Color translation
//===========================================================================

double DialogFaceDetection::gamma(double x) {
    if (x > 0.04045) {
        return pow((x + 0.055) / 1.055, 2.4);
    }
    return x / 12.92;
}

double DialogFaceDetection::flab(double x) {
    double epsilon = 0.008856, kappa = 903.3;;
    if (x > epsilon) {
        return pow(x, 1.0 / 3.0);
    }
    return (kappa * x + 16.0) / 116.0;
}

Color DialogFaceDetection::rgb2lab(Color rgb) {
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

    return Color(116.0 * fy - 16.0, 500.0 * (fx - fy), 200.0 * (fy - fz));
}

//===========================================================================
// Extract skin pixels by its rgb value
//===========================================================================

void DialogFaceDetection::skinExtract() {
    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            if (isSkinRGB(imgOrigin.at<cv::Vec3b>(i, j)[2], imgOrigin.at<cv::Vec3b>(i, j)[1], imgOrigin.at<cv::Vec3b>(i, j)[0])) {
                img.at<cv::Vec3b>(i, j) = imgOrigin.at<cv::Vec3b>(i, j);
            } else {
                img.at<cv::Vec3b>(i, j)[0] = img.at<cv::Vec3b>(i, j)[1] = img.at<cv::Vec3b>(i, j)[2] = 255;
            }
        }
    }
}

bool DialogFaceDetection::isSkinRGB(int r, int g, int b) {
    bool dayLight = true, flashLight = true;
    if (r <= 95 || g <= 40 || b <= 20) {
        dayLight = false;
    }
    if (max(r, max(g, b)) - min(r, min(g, b)) <= 15) {
        dayLight = false;
    }
    if (abs(r - g) <= 15) {
        dayLight = false;
    }
    if (r <= g || r <= b) {
        dayLight = false;
    }
    if (r <= 220 || g <= 210 || b <= 170) {
        flashLight = false;
    }
    if (abs(r - g) > 15) {
        flashLight = false;
    }
    if (b >= g || b >= r) {
        flashLight = false;
    }
    return (dayLight || flashLight);
}

//===========================================================================
// Median blur the img
//===========================================================================

void DialogFaceDetection::medianFilter() {
    Mat dst(img.rows, img.cols, img.type());
    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            for (int channel = 0; channel < 3; ++channel) {
                vector<int> v;
                for (int i_ = i - 2; i_ <= i + 2; ++i_) {
                    for (int j_ = j - 2; j_ <= j + 2; ++j_) {
                        if (i_ >= 0 && j_ >= 0 && i_ < img.rows && j_ < img.cols) {
                            v.push_back(img.at<cv::Vec3b>(i_, j_)[channel]);
                        }
                    }
                }
                if (!v.size()) {
                    continue;
                }
                sort(v.begin(), v.end());
                dst.at<cv::Vec3b>(i, j)[channel] = v[v.size() / 2];
            }
        }
    }
    img = dst;
}

//===========================================================================
// Selection
//===========================================================================

void DialogFaceDetection::mousePressEvent(QMouseEvent *e) {
    if (step != STEP_SELECT) {
        return;
    }
    int x = ui->imgLabel1->mapFromGlobal(e->globalPos()).y();
    int y = ui->imgLabel1->mapFromGlobal(e->globalPos()).x();
    if (x < 0 || x >= img.rows || y < 0 || y >= img.cols) {
        return;
    }
    isDragging = true;
    rectX1 = x;
    rectY1 = y;
    imgTmp = img.clone();
}

void DialogFaceDetection::mouseReleaseEvent(QMouseEvent *e) {
    if (step != STEP_SELECT) {
        return;
    }
    if (!isDragging) {
        return;
    }
    isDragging = false;
    int x = ui->imgLabel1->mapFromGlobal(e->globalPos()).y();
    int y = ui->imgLabel1->mapFromGlobal(e->globalPos()).x();
    x = x < 0 ? 0 : x;
    x = x >= img.rows ? (img.rows - 1) : x;
    y = y < 0 ? 0 : y;
    y = y >= img.cols ? (img.cols - 1) : y;
    rectX2 = x;
    rectY2 = y;
    img = imgTmp.clone();
    if (rectX1 == rectX2 || rectY1 == rectY2) {
        return;
    }
    int xStep = (rectX2 > rectX1) ? 1 : -1;
    int yStep = (rectY2 > rectY1) ? 1 : -1;
    for (int i = rectX1; i != rectX2; i += xStep) {
        for (int j = rectY1; j != rectY2; j += yStep) {
            imgOutput.at<cv::Vec3b>(i, j) = img.at<cv::Vec3b>(i, j);
        }
    }
    showImg();
    showImgOutput();
}

void DialogFaceDetection::mouseMoveEvent(QMouseEvent *e) {
    if (step != STEP_SELECT) {
        return;
    }
    if (!isDragging) {
        return;
    }
    int x = ui->imgLabel1->mapFromGlobal(e->globalPos()).y();
    int y = ui->imgLabel1->mapFromGlobal(e->globalPos()).x();
    x = x < 0 ? 0 : x;
    x = x >= img.rows ? (img.rows - 1) : x;
    y = y < 0 ? 0 : y;
    y = y >= img.cols ? (img.cols - 1) : y;
    img = imgTmp.clone();
    int xStep = (x > rectX1) ? 1 : -1;
    int yStep = (y > rectY1) ? 1 : -1;
    if (rectX1 != x) {
        for (int i = rectX1; i != x; i += xStep) {
            img.at<cv::Vec3b>(i, y)[0] = img.at<cv::Vec3b>(i, y)[2] = 0;
            img.at<cv::Vec3b>(i, y)[1] = 255;
            img.at<cv::Vec3b>(i, rectY1)[0] = img.at<cv::Vec3b>(i, rectY1)[2] = 0;
            img.at<cv::Vec3b>(i, rectY1)[1] = 255;
        }
    }
    if (rectY1 != y) {
        for (int j = rectY1; j != y; j += yStep) {
            img.at<cv::Vec3b>(x, j)[0] = img.at<cv::Vec3b>(x, j)[2] = 0;
            img.at<cv::Vec3b>(x, j)[1] = 255;
            img.at<cv::Vec3b>(rectX1, j)[0] = img.at<cv::Vec3b>(rectX1, j)[2] = 0;
            img.at<cv::Vec3b>(rectX1, j)[1] = 255;
        }
    }
    showImg();
}

//===========================================================================
// Bilateral Filter
//===========================================================================

void DialogFaceDetection::bilateralFilter() {
    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            if (imgMask.at<cv::Vec3b>(i, j)[0] != 255 || imgMask.at<cv::Vec3b>(i, j)[1] != 255 || imgMask.at<cv::Vec3b>(i, j)[2] != 255) {
                bilateralFilterPixel(imgOutput, i, j);
            } else {
                imgOutput.at<cv::Vec3b>(i, j) = imgOrigin.at<cv::Vec3b>(i, j);
            }
        }
    }
}

void DialogFaceDetection::bilateralFilterPixel(Mat &dst, int x, int y) {
    int x_ = (x - windowSize) < 0 ? 0 : (x - windowSize);
    int _x = (x + windowSize) >= img.rows ? (img.rows - 1) : (x + windowSize);
    int y_ = (y - windowSize) < 0 ? 0 : (y - windowSize);
    int _y = (y + windowSize) >= img.cols ? (img.cols - 1) : (y + windowSize);
    double r_sum = 0, g_sum = 0, b_sum = 0;
    double w_sum = 0;
    for (int i = x_; i <= _x; ++i) {
        for (int j = y_; j <= _y; ++j) {
            double fi_d = pow(M_E, -(pow(i - x, 2) + pow(j - y, 2)) / (2 * sigmaD * sigmaD));
            Color lab1 = rgb2lab(Color(imgOrigin.at<cv::Vec3b>(i, j)[2], imgOrigin.at<cv::Vec3b>(i, j)[1], imgOrigin.at<cv::Vec3b>(i, j)[0]));
            Color lab2 = rgb2lab(Color(imgOrigin.at<cv::Vec3b>(x, y)[2], imgOrigin.at<cv::Vec3b>(x, y)[1], imgOrigin.at<cv::Vec3b>(x, y)[0]));
            double fi_r = pow(M_E, -(pow(lab1.x - lab2.x, 2) + pow(lab1.y - lab2.y, 2) + pow(lab1.z - lab2.z, 2)) / (2 * sigmaR * sigmaR));
            double w = fi_d * fi_r;
            r_sum += w * imgOrigin.at<cv::Vec3b>(i, j)[2];
            g_sum += w * imgOrigin.at<cv::Vec3b>(i, j)[1];
            b_sum += w * imgOrigin.at<cv::Vec3b>(i, j)[0];
            w_sum += w;
        }
    }
    dst.at<cv::Vec3b>(x, y)[0] = b_sum / w_sum;
    dst.at<cv::Vec3b>(x, y)[1] = g_sum / w_sum;
    dst.at<cv::Vec3b>(x, y)[2] = r_sum / w_sum;
}

//===========================================================================
// Brightness
//===========================================================================

void DialogFaceDetection::adjustBrightness() {
    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            if (imgMask.at<cv::Vec3b>(i, j)[0] != 255 || imgMask.at<cv::Vec3b>(i, j)[1] != 255 || imgMask.at<cv::Vec3b>(i, j)[2] != 255) {
                adjustBrightnessPixel(imgOutput, i, j);
            } else {
                imgOutput.at<cv::Vec3b>(i, j) = imgOrigin.at<cv::Vec3b>(i, j);
            }
        }
    }
}

void DialogFaceDetection::adjustBrightnessPixel(Mat &dst, int x, int y) {
    int r = img.at<cv::Vec3b>(x, y)[2];
    int g = img.at<cv::Vec3b>(x, y)[1];
    int b = img.at<cv::Vec3b>(x, y)[0];
    r = (r + deltaB) > 255 ? 255 : (r + deltaB);
    g = (g + deltaB) > 255 ? 255 : (g + deltaB);
    b = (b + deltaB) > 255 ? 255 : (b + deltaB);
    dst.at<cv::Vec3b>(x, y)[2] = r;
    dst.at<cv::Vec3b>(x, y)[1] = g;
    dst.at<cv::Vec3b>(x, y)[0] = b;
}
