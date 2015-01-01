#ifndef BLUEPRINTS_H
#define BLUEPRINTS_H

#include <QDialog>
#include <glwidgetOrtho.h>

namespace Ui {
class blueprints;
}

class blueprints : public QDialog
{
    Q_OBJECT

public:
    explicit blueprints(QWidget *parent = 0);
    ~blueprints();

    GLWidgetOrtho * glwidgetortho;

signals:
    void updateGL();

private slots:
    void on_dial_3_sliderMoved(int position);

    void on_pushButton_clicked();

    void on_dial_2_sliderMoved(int position);

    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_lineEdit_3_textEdited(const QString &arg1);

    void on_dial_4_sliderMoved(int position);

    void on_lineEdit_4_textEdited(const QString &arg1);

    void on_lineEdit_textEdited(const QString &arg1);

    void on_dial_sliderMoved(int position);

private:
    Ui::blueprints *ui;
};

#endif // BLUEPRINTS_H
