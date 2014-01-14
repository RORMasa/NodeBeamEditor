#pragma once

#include <QDialog>

namespace Ui {
class InputDialog;
}

class InputDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit InputDialog(QWidget *parent = 0);
    ~InputDialog();

    void MoveNodes();
    void ScaleNodes();
    void RotateNodes();
    void FindBeam();

    int DialogMode;

    QVector <QString> valuesString;
    QVector <int> valuesInteger;
    QVector <float> valuesFloat;

    bool ValuesOk;
    
private slots:

    void on_buttonBox_accepted();

private:
    Ui::InputDialog *ui;
};

