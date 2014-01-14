#include "inputdialog.h"
#include "ui_inputdialog.h"
#include <QMessageBox>
#include <QDebug>

InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);
    ui->lineEdit->hide();
    ui->lineEdit_2->hide();
    ui->lineEdit_3->hide();
    ui->lineEdit_4->hide();
    ui->lineEdit_5->hide();

    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();

    /* Dialog mode */
    DialogMode=0;

}

InputDialog::~InputDialog()
{
        qDebug()<<"Destroying inputdialog ...";
    delete ui;
}
void InputDialog::MoveNodes()
{

    ui->lineEdit->show();
    ui->lineEdit_2->show();
    ui->lineEdit_3->show();

    ui->label->show();
    ui->label_2->show();
    ui->label_3->show();

    ui->label_6->setText("Moving nodes");

    ui->label->setText("X distance in meters");
    ui->label_2->setText("Y distance in meters");
    ui->label_3->setText("Z distance in meters");

    /*Dialog is handling information about node movement*/
    DialogMode=1;
    ValuesOk=0;
        qDebug()<<"Setting up inputdialog";
}

void InputDialog::ScaleNodes()
{

    ui->lineEdit->show();
    ui->lineEdit_2->show();
    ui->lineEdit_3->show();

    ui->label->show();
    ui->label_2->show();
    ui->label_3->show();

    ui->label_6->setText("Scaling nodes");

    ui->label->setText("X factor");
    ui->label_2->setText("Y factor");
    ui->label_3->setText("Z factor");

    /*Dialog is handling information about node movement*/
    DialogMode=2;
    ValuesOk=0;
    qDebug()<<"Setting up inputdialog";
}

void InputDialog::RotateNodes()
{

    ui->lineEdit->show();
    ui->lineEdit_2->show();
    ui->lineEdit_3->show();

    ui->label->show();
    ui->label_2->show();
    ui->label_3->show();

    ui->label_6->setText("Rotating nodes");

    ui->label->setText("Angle X in degrees");
    ui->label_2->setText("Angle Y in degrees");
    ui->label_3->setText("Angle Z in degrees");

    /*Dialog is handling information about node rotationt*/
    DialogMode=4;
    ValuesOk=0;
        qDebug()<<"Setting up inputdialog";
}

void InputDialog::FindBeam()
{
    ui->lineEdit->show();
    ui->lineEdit_2->show();

    ui->label->show();
    ui->label_2->show();

    ui->label_6->setText("Find beam");

    ui->label->setText("Node 1");
    ui->label_2->setText("Node 2");

    /*Dialog is handling information abut beam nodes*/
    DialogMode=3;
    ValuesOk=0;
    qDebug()<<"Setting up inputdialog";
}

void InputDialog::on_buttonBox_accepted()
{
    qDebug()<<"Collecting information";
    if(DialogMode==1)
    {
        if(ui->lineEdit->text()=="") ui->lineEdit->setText("0");
        if(ui->lineEdit_2->text()=="") ui->lineEdit_2->setText("0");
        if(ui->lineEdit_3->text()=="") ui->lineEdit_3->setText("0");
        bool ok;
        valuesFloat.resize(3);
        valuesFloat[0] = ui->lineEdit->text().toFloat(&ok);
        if(ok)
        {
            valuesFloat[1] = ui->lineEdit_2->text().toFloat(&ok);
            if(ok)
            {
                valuesFloat[2] = ui->lineEdit_3->text().toFloat(&ok);
                if(ok)
                {
                    ValuesOk=1;
                }
            }
        }
    }
    else if(DialogMode==2)
    {
        if(ui->lineEdit->text()=="") ui->lineEdit->setText("1.0");
        if(ui->lineEdit_2->text()=="") ui->lineEdit_2->setText("1.0");
        if(ui->lineEdit_3->text()=="") ui->lineEdit_3->setText("1.0");
        bool ok;
        valuesFloat.resize(3);
        valuesFloat[0] = ui->lineEdit->text().toFloat(&ok);
        if(ok)
        {
            valuesFloat[1] = ui->lineEdit_2->text().toFloat(&ok);
            if(ok)
            {
                valuesFloat[2] = ui->lineEdit_3->text().toFloat(&ok);
                if(ok)
                {
                    ValuesOk=1;
                }
            }
        }
    }
    else if(DialogMode==3)
    {
        ValuesOk=1;
        if(ui->lineEdit->text()=="") ValuesOk=0;
        if(ui->lineEdit_2->text()=="") ValuesOk=0;
        valuesString.resize(2);
        valuesString[0] = ui->lineEdit->text();
        valuesString[1] = ui->lineEdit_2->text();
    }
    else if(DialogMode==4)
    {
        if(ui->lineEdit->text()=="") ui->lineEdit->setText("0");
        if(ui->lineEdit_2->text()=="") ui->lineEdit_2->setText("0");
        if(ui->lineEdit_3->text()=="") ui->lineEdit_3->setText("0");
        bool ok;
        valuesFloat.resize(3);
        valuesFloat[0] = ui->lineEdit->text().toFloat(&ok);
        if(ok)
        {
            valuesFloat[1] = ui->lineEdit_2->text().toFloat(&ok);
            if(ok)
            {
                valuesFloat[2] = ui->lineEdit_3->text().toFloat(&ok);
                if(ok)
                {
                    ValuesOk=1;
                }
            }
        }
    }
}
