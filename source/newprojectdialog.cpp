#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"
#include <QDesktopServices>

NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

void NewProjectDialog::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.guidgenerator.com/"));
}
