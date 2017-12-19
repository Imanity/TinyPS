#ifndef DIALOGHSL_H
#define DIALOGHSL_H

#include <QDialog>

namespace Ui {
class DialogHSL;
}

class DialogHSL : public QDialog {
    Q_OBJECT

public:
    explicit DialogHSL(QWidget *parent = 0);
    ~DialogHSL();

private:
    Ui::DialogHSL *ui;
};

#endif // DIALOGHSL_H
