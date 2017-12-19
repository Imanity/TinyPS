#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    isRectCutting = false;
    isCircleCutting = false;
    isPolyCutting = false;

    x1 = y1 = -1;

    ui->imgLabel->installEventFilter(this);

    filterType = MEDIAN_FILTER;

    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openActionSlot()));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(saveActionSlot()));
    connect(ui->actionLRotate, SIGNAL(triggered(bool)), this, SLOT(rotateL90ActionSlot()));
    connect(ui->actionRRotate, SIGNAL(triggered(bool)), this, SLOT(rotateR90ActionSlot()));
    connect(ui->actionRotate, SIGNAL(triggered(bool)), this, SLOT(rotate180ActionSlot()));
    connect(ui->actionFreeRotate, SIGNAL(triggered(bool)), this, SLOT(rotateFreeActionSlot()));
    connect(ui->actionHorFlip, SIGNAL(triggered(bool)), this, SLOT(horizontalFlipActionSlot()));
    connect(ui->actionVerFlip, SIGNAL(triggered(bool)), this, SLOT(verticalFlipActionSlot()));
    connect(ui->actionRecCut, SIGNAL(triggered(bool)), this, SLOT(cutRecActionSlot()));
    connect(ui->actionCircleCut, SIGNAL(triggered(bool)), this, SLOT(cutCircleActionSlot()));
    connect(ui->actionPolyCut, SIGNAL(triggered(bool)), this, SLOT(cutPolyActionSlot()));
    connect(ui->actionHSL, SIGNAL(triggered(bool)), this, SLOT(hslActionSlot()));
    connect(ui->actionColorHalftone, SIGNAL(triggered(bool)), this, SLOT(halftoneSlot()));
    connect(ui->actionGray, SIGNAL(triggered(bool)), this, SLOT(grayActionSlot()));
    connect(ui->actionGrayLog, SIGNAL(triggered(bool)), this, SLOT(grayLogSlot()));
    connect(ui->actionGrayE, SIGNAL(triggered(bool)), this, SLOT(grayESlot()));
    connect(ui->actionGrayGamma, SIGNAL(triggered(bool)), this, SLOT(grayGammaSlot()));
    connect(ui->actionHistogram, SIGNAL(triggered(bool)), this, SLOT(grayHistogramActionSlot()));
    connect(ui->actionMedianFilter, SIGNAL(triggered(bool)), this, SLOT(medianFilterSlot()));
    connect(ui->actionGaussianFilter, SIGNAL(triggered(bool)), this, SLOT(gaussianFilterSlot()));
    connect(ui->actionSharpen, SIGNAL(triggered(bool)), this, SLOT(sharpenSlot()));
    connect(ui->actionRadialBlur, SIGNAL(triggered(bool)), this, SLOT(radialBlurSlot()));
    connect(ui->actionFrequencySmooth, SIGNAL(triggered(bool)), this, SLOT(frequencySmoothSlot()));
    connect(ui->actionFrequencySharpen, SIGNAL(triggered(bool)), this, SLOT(frequencySharpenSlot()));
    connect(ui->actionSuperPixel, SIGNAL(triggered(bool)), this, SLOT(superpixelSlot()));
    connect(ui->actionFace, SIGNAL(triggered(bool)), this, SLOT(beautyFaceSlot()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::showPic() {
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
    ui->imgLabel->setFixedSize(img.cols, img.rows);
    ui->imgLabel->setPixmap(QPixmap::fromImage(qimg));
}

void MainWindow::openActionSlot() {
    QString filename = QFileDialog::getOpenFileName(this, "Open Pic", "", "*.jpg *.png *.bnp", 0);
    img = imread(filename.toStdString());
    lastImg = img;
    showPic();
}

void MainWindow::saveActionSlot() {
    QString filename = QFileDialog::getSaveFileName(this, "Save Pic", "", "*.jpg *.png *.bmp", 0);
    if (filename.size() == 0) {
        return;
    }
    imwrite(filename.toStdString(), img);
}

void MainWindow::rotateL90ActionSlot() {
    img = m.rotate_l_90(img);
    showPic();
}

void MainWindow::rotateR90ActionSlot() {
    img = m.rotate_r_90(img);
    showPic();
}

void MainWindow::rotate180ActionSlot() {
    img = m.rotate_180(img);
    showPic();
}

void MainWindow::rotateFreeActionSlot() {
    lastImg = img;
    DialogRotate *popup = new DialogRotate(this);
    popup->show();
}

void MainWindow::horizontalFlipActionSlot() {
    img = m.flip_h(img);
    showPic();
}

void MainWindow::verticalFlipActionSlot() {
    img = m.flip_v(img);
    showPic();
}

void MainWindow::cutRecActionSlot() {
    this->setCursor(Qt::CrossCursor);
    isRectCutting = true;
}

void MainWindow::cutCircleActionSlot() {
    this->setCursor(Qt::CrossCursor);
    isCircleCutting = true;
}

void MainWindow::cutPolyActionSlot() {
    this->setCursor(Qt::CrossCursor);
    lastImg = img;
    lastImgForPolyCut = img;
    isPolyCutting = true;
}

void MainWindow::rotateAngleSlot(int angle) {
    img = m.rotate(lastImg, angle);
    showPic();
}

void MainWindow::confirmedSlot() {
    lastImg = img;
}

void MainWindow::hslActionSlot() {
    lastImg = img;
    deltaH = deltaS = deltaI = 0;
    DialogHSL *popup = new DialogHSL(this);
    popup->show();
}

void MainWindow::modifyHueSlot(int val) {
    deltaH = val;
    img = m.modifyHSI(lastImg, deltaH, deltaS, deltaI);
    showPic();
}

void MainWindow::modifySaturationSlot(int val) {
    deltaS = val;
    img = m.modifyHSI(lastImg, deltaH, deltaS, deltaI);
    showPic();
}

void MainWindow::modifyIntensitySlot(int val) {
    deltaI = val;
    img = m.modifyHSI(lastImg, deltaH, deltaS, deltaI);
    showPic();
}

void MainWindow::halftoneSlot() {
    img = m.halftone(img);
    showPic();
}

void MainWindow::grayActionSlot() {
    lastImg = img;
    DialogGrayTransform *popup = new DialogGrayTransform(this, DIALOG_GRAY_LINEAR);
    popup->show();
}

void MainWindow::grayLogSlot() {
    lastImg = img;
    DialogGrayTransform *popup = new DialogGrayTransform(this, DIALOG_GRAY_LOG);
    popup->show();
}

void MainWindow::grayESlot() {
    lastImg = img;
    DialogGrayTransform *popup = new DialogGrayTransform(this, DIALOG_GRAY_E);
    popup->show();
}

void MainWindow::grayGammaSlot() {
    lastImg = img;
    DialogGrayTransform *popup = new DialogGrayTransform(this, DIALOG_GRAY_GAMMA);
    popup->show();
}

void MainWindow::modifyLinearGraySlot(int a, int b, int c, int d) {
    img = m.modifyLinearGray(lastImg, a, b, c, d);
    showPic();
}

void MainWindow::modifyLogGraySlot(double a, double b, double c) {
    img = m.modifyLogGray(lastImg, a, b, c);
    showPic();
}

void MainWindow::modifyEGraySlot(double a, double b, double c) {
    img = m.modifyEGray(lastImg, a, b, c);
    showPic();
}

void MainWindow::modifyGammaGraySlot(double c, double gamma) {
    img = m.modifyGammaGray(lastImg, c, gamma);
    showPic();
}

void MainWindow::grayHistogramActionSlot() {
    lastImg = img;
    vector<double> histogram = m.getGrayHistogram(img);
    DialogGrayHistogram *popup = new DialogGrayHistogram(this, histogram);
    popup->show();
}

void MainWindow::equalizationSlot() {
    img = m.equalization(img);
    showPic();
}

void MainWindow::specificationSMLSlot(int mu, double sigma) {
    vector<double> histogram = generateGaussianHis(mu, sigma);
    img = m.specificationSML(img, histogram);
    showPic();
}

void MainWindow::specificationGMLSlot(int mu, double sigma) {
    vector<double> histogram = generateGaussianHis(mu, sigma);
    img = m.specificationGML(img, histogram);
    showPic();
}

void MainWindow::sharpenSlot() {
    img = m.sharpen(img);
    showPic();
}

void MainWindow::radialBlurSlot() {
    img = m.radialBlur(img);
    showPic();
}

vector<double> MainWindow::generateGaussianHis(int mu, double sigma) {
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

void MainWindow::medianFilterSlot() {
    filterType = MEDIAN_FILTER;
    DialogFilter *popup = new DialogFilter(this);
    popup->show();
}

void MainWindow::gaussianFilterSlot() {
    filterType = GAUSSIAN_FILTER;
    DialogFilter *popup = new DialogFilter(this);
    popup->show();
}

void MainWindow::filterSlot(int size) {
    if (filterType == MEDIAN_FILTER) {
        img = m.medianFilter(img, size);
        showPic();
    } else {
        img = m.gaussianFilter(img, size);
        showPic();
    }
}

void MainWindow::frequencySmoothSlot() {
    butterworthType = BUTTERWORTH_SMOOTH;
    DialogButterworth *popup = new DialogButterworth(this);
    popup->show();
}

void MainWindow::frequencySharpenSlot() {
    butterworthType = BUTTERWORTH_SHARPEN;
    DialogButterworth *popup = new DialogButterworth(this);
    popup->show();
}

void MainWindow::butterworthSlot(int d0, int n) {
    if (butterworthType == BUTTERWORTH_SMOOTH) {
        img = m.frequencySmooth(img, d0, n);
    } else {
        img = m.frequencySharpen(img, d0, n);
    }
    showPic();
}

void MainWindow::superpixelSlot() {
    DialogSlic *popup = new DialogSlic(this);
    popup->show();
}

void MainWindow::doSuperPixelSlot(int k, int m, int n) {
    Slic s(img);
    s.setK(k);
    s.setM(m);
    s.setN(n);
    img = s.output();
    showPic();
}

void MainWindow::beautyFaceSlot() {
    DialogFaceDetection *popup = new DialogFaceDetection(this);
    Mat tmp = img.clone();
    popup->setImage(tmp);
    popup->show();
}

void MainWindow::updateBeautySlot(Mat m) {
    img = m.clone();
    showPic();
}

void MainWindow::mousePressEvent(QMouseEvent *e) {
    if (isRectCutting || isCircleCutting) {
        lastImg = img;
        x1 = ui->imgLabel->mapFromGlobal(e->globalPos()).x();
        y1 = ui->imgLabel->mapFromGlobal(e->globalPos()).y();
        x1 = x1 < 0 ? 0 : x1;
        y1 = y1 < 0 ? 0 : y1;
        x1 = x1 >= img.cols ? img.cols - 1 : x1;
        y1 = y1 >= img.rows ? img.rows - 1 : y1;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e) {
    if (isRectCutting) {
        x2 = ui->imgLabel->mapFromGlobal(e->globalPos()).x();
        y2 = ui->imgLabel->mapFromGlobal(e->globalPos()).y();
        x2 = x2 < 0 ? 0 : x2;
        y2 = y2 < 0 ? 0 : y2;
        x2 = x2 >= img.cols ? img.cols - 1 : x2;
        y2 = y2 >= img.rows ? img.rows - 1 : y2;
        if (x1 == x2 || y1 == y2) {
            return;
        }
        this->setCursor(Qt::ArrowCursor);
        isRectCutting = false;
        img = m.cutRect(lastImg, x1, x2, y1, y2);
        x1 = y1 = -1;
        lastImg = img;
        showPic();
    } else if (isCircleCutting) {
        x2 = ui->imgLabel->mapFromGlobal(e->globalPos()).x();
        y2 = ui->imgLabel->mapFromGlobal(e->globalPos()).y();
        x2 = x2 < 0 ? 0 : x2;
        y2 = y2 < 0 ? 0 : y2;
        x2 = x2 >= img.cols ? img.cols - 1 : x2;
        y2 = y2 >= img.rows ? img.rows - 1 : y2;
        if (x1 == x2 || y1 == y2) {
            return;
        }
        this->setCursor(Qt::ArrowCursor);
        isCircleCutting = false;
        img = m.cutCircle(lastImg, x1, x2, y1, y2);
        x1 = y1 = -1;
        lastImg = img;
        showPic();
    } else if (isPolyCutting) {
        int x = ui->imgLabel->mapFromGlobal(e->globalPos()).x();
        int y = ui->imgLabel->mapFromGlobal(e->globalPos()).y();
        x = x < 0 ? 0 : x;
        y = y < 0 ? 0 : y;
        x = x >= img.cols ? img.cols - 1 : x;
        y = y >= img.rows ? img.rows - 1 : y;
        if (points.size() != 0) {
            int lastX = points[points.size() - 1].first;
            int lastY = points[points.size() - 1].second;
            if (abs(x - points[0].first) <= 3 && abs(y - points[0].second) <= 3) {
                this->setCursor(Qt::ArrowCursor);
                img = m.cutPoly(lastImg, points);
                lastImg = lastImgForPolyCut = img;
                isPolyCutting = false;
                showPic();
                points.clear();
            } else {
                lastImgForPolyCut = m.drawLine(lastImgForPolyCut, lastX, lastY, x, y);
                img = lastImgForPolyCut;
                showPic();
                points.push_back(make_pair(x, y));
            }
        } else {
            points.push_back(make_pair(x, y));
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e) {
    if (isRectCutting) {
        if (x1 < 0 || y1 < 0) {
            return;
        }
        int x = ui->imgLabel->mapFromGlobal(e->globalPos()).x();
        int y = ui->imgLabel->mapFromGlobal(e->globalPos()).y();
        x = x < 0 ? 0 : x;
        y = y < 0 ? 0 : y;
        x = x >= img.cols ? img.cols - 1 : x;
        y = y >= img.rows ? img.rows - 1 : y;
        img = m.drawRect(lastImg, min(x, x1), min(y, y1), abs(x - x1), abs(y - y1));
        showPic();
    } else if (isCircleCutting) {
        if (x1 < 0 || y1 < 0) {
            return;
        }
        int x = ui->imgLabel->mapFromGlobal(e->globalPos()).x();
        int y = ui->imgLabel->mapFromGlobal(e->globalPos()).y();
        x = x < 0 ? 0 : x;
        y = y < 0 ? 0 : y;
        x = x >= img.cols ? img.cols - 1 : x;
        y = y >= img.rows ? img.rows - 1 : y;
        img = m.drawCircle(lastImg, min(x, x1), min(y, y1), abs(x - x1), abs(y - y1));
        showPic();
    } else if (isPolyCutting) {
        if (points.size() != 0) {
            int lastX = points[points.size() - 1].first;
            int lastY = points[points.size() - 1].second;
            int x = ui->imgLabel->mapFromGlobal(e->globalPos()).x();
            int y = ui->imgLabel->mapFromGlobal(e->globalPos()).y();
            x = x < 0 ? 0 : x;
            y = y < 0 ? 0 : y;
            x = x >= img.cols ? img.cols - 1 : x;
            y = y >= img.rows ? img.rows - 1 : y;
            img = m.drawLine(lastImgForPolyCut, lastX, lastY, x, y);
            showPic();
            if (abs(x - points[0].first) <= 3 && abs(y - points[0].second) <= 3) {
                this->setCursor(Qt::PointingHandCursor);
            } else {
                this->setCursor(Qt::CrossCursor);
            }
        }
    }
}
