#ifndef PROCESSBAR_H
#define PROCESSBAR_H

#include <QDialog>

namespace Ui {
class processBar;
}

class processBar : public QDialog
{
    Q_OBJECT

public:
    explicit processBar(QWidget *parent = 0);
    ~processBar();

    void setLabel(QString text);
    void setBarValue(int value);

private:
    Ui::processBar *ui;
};

#endif // PROCESSBAR_H
