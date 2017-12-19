#ifndef DIALOGBUTTERWORTH_H
#define DIALOGBUTTERWORTH_H

#include <QDialog>

namespace Ui {
class DialogButterworth;
}

class DialogButterworth : public QDialog
{
    Q_OBJECT

public:
    explicit DialogButterworth(QWidget *parent = 0);
    ~DialogButterworth();

private:
    Ui::DialogButterworth *ui;

public slots:
    void updateD0Val(int val);
    void updateNVal(int val);
    void btnSlot();

signals:
    void submitVal(int d0, int n);
};

#endif // DIALOGBUTTERWORTH_H
