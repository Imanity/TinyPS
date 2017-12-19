# TinyPS
An Image Processing software. Assignment for multimedia course.
***

### Requirements
* Qt 5.9.1
* OpenCV 3.3.0

### Intro
* 图像的翻转/自由旋转: 基于坐标运算实现

![rotate](/gallary/rotate.png)

* 图像的裁剪: 矩形/椭圆/多边形裁剪

![cut](/gallary/cut.png)

* 亮度/色调/饱和度调整: 基于像素HSI特征调整

![color](/gallary/color.png)

* 彩色半调

![halftone](/gallary/halftone.png)

* 线性/非线性变换

![linear](/gallary/lenear.png)

* 直方图均衡化/规定化

![histogram](/gallary/histogram.png)

* 中值滤波/高斯滤波/锐化

![gauss](/gallary/gauss.png)

* 径向模糊

![blur](/gallary/blur.png)

* 频域平滑/锐化: 基于快速傅里叶变换实现

![dft](/gallary/dft.png)

* 晶格化: 基于SLIC算法[1]实现

![slic](/gallary/slic.png)

* 人像美肤: 基于肤色rgb识别[2]及双边滤波实现

![skin](/gallary/skin.png)

### References
* SLIC Superpixels Compared to State-of-the-art Superpixel Methods. TPAMI 2012
* Human Skin Colour Clustering for Face Detection. EUROCON 2003

