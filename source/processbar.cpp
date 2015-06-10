#include "processbar.h"
#include "ui_processbar.h"

processBar::processBar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::processBar)
{
    ui->setupUi(this);
}

processBar::~processBar()
{
    delete ui;
}

void processBar::setLabel(QString text)
{
    ui->label->setText(text);
}

void processBar::setBarValue(int value)
{
    ui->progressBar->setValue(value);
}
