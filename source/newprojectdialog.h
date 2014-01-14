#pragma once

#include <QDialog>

namespace Ui {
class NewProjectDialog;
}

class NewProjectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewProjectDialog(QWidget *parent = 0);
    ~NewProjectDialog();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::NewProjectDialog *ui;
};

