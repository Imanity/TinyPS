#ifndef DIALOGFILTER_H
#define DIALOGFILTER_H

#include <QDialog>

namespace Ui {
class DialogFilter;
}

class DialogFilter : public QDialog {
    Q_OBJECT

public:
    explicit DialogFilter(QWidget *parent = 0);
    ~DialogFilter();

private:
    Ui::DialogFilter *ui;

public slots:
    void applyBtnSlot();
    void updateSlider(int val);

signals:
    void sendSize(int);
};

#endif // DIALOGFILTER_H
