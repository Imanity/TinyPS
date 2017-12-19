#ifndef DIALOGSLIC_H
#define DIALOGSLIC_H

#include <QDialog>

namespace Ui {
class DialogSlic;
}

class DialogSlic : public QDialog {
    Q_OBJECT

public:
    explicit DialogSlic(QWidget *parent = 0);
    ~DialogSlic();

private:
    Ui::DialogSlic *ui;

public slots:
    void kvalSlot(int val);
    void mvalSlot(int val);
    void nvalSlot(int val);
    void btnSlot();

signals:
    void submitVal(int k, int m, int n);
};

#endif // DIALOGSLIC_H
