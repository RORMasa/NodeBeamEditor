#include "blueprints.h"
#include "ui_blueprints.h"
#include <QDebug>
#include <QFileDialog>


blueprints::blueprints(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::blueprints)
{
    ui->setupUi(this);

    //Scaling slider range and default value
    ui->dial_2->setRange(1,750);
    ui->dial_2->setValue(312);

    //Offset X and Y slider range and default value
    ui->dial_3->setRange(-500,500);
    ui->dial_4->setRange(-500,500);

    //Opaciy slider range
    ui->dial->setRange(0,100);
    ui->dial->setValue(100);
}

blueprints::~blueprints()
{
    delete ui;
}

/* Open blueprint image */
void blueprints::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        glwidgetortho->LoadBlueprint(fileName);
        for(int i = fileName.length(); i>0;i--)
        {
            if(fileName[i] == '/')
            {
                fileName = fileName.remove(0,i+1);
                break;
            }
        }
        ui->label_8->setText(fileName);
    }
}

/* Scale blueprint - SLIDER */
void blueprints::on_dial_2_sliderMoved(int position)
{
    glwidgetortho->blueprint_scale[glwidgetortho->textureid] = position*0.016f;
    glwidgetortho->updateGL();
    ui->lineEdit_2->setText(QString::number(position*0.016f));
}
/* Scale blueprint - line edit */
void blueprints::on_lineEdit_2_textEdited(const QString &arg1)
{
    bool ok;
    arg1.toFloat(&ok);
    if(ok)
    {
        glwidgetortho->blueprint_scale[glwidgetortho->textureid] = arg1.toFloat(&ok);
        glwidgetortho->updateGL();
    }
}

/* Change offset in X direction */
void blueprints::on_dial_3_sliderMoved(int position)
{
    glwidgetortho->blueprint_offX[glwidgetortho->textureid] = (float)position/200;
    glwidgetortho->updateGL();
    ui->lineEdit_3->setText(QString::number((float)position/200));
}

void blueprints::on_lineEdit_3_textEdited(const QString &arg1)
{
    bool ok;
    arg1.toFloat(&ok);
    if(ok)
    {
        glwidgetortho->blueprint_offX[glwidgetortho->textureid] = arg1.toFloat(&ok);
        glwidgetortho->updateGL();
    }
}

/* Change offset in Y direction */
void blueprints::on_dial_4_sliderMoved(int position)
{
    glwidgetortho->blueprint_offY[glwidgetortho->textureid] = (float)position/200;
    glwidgetortho->updateGL();
    ui->lineEdit_4->setText(QString::number((float)position/200));
}

void blueprints::on_lineEdit_4_textEdited(const QString &arg1)
{
    bool ok;
    arg1.toFloat(&ok);
    if(ok)
    {
        glwidgetortho->blueprint_offY[glwidgetortho->textureid] = arg1.toFloat(&ok);
        glwidgetortho->updateGL();
    }
}

/* Opacity edited in slider */
void blueprints::on_dial_sliderMoved(int position)
{
    glwidgetortho->blueprint_opa[glwidgetortho->textureid] = (float)position/100;
    glwidgetortho->updateGL();
    ui->lineEdit->setText(QString::number(position));
}
/* Opacity edited in text box */
void blueprints::on_lineEdit_textEdited(const QString &arg1)
{
    bool ok;
    int opacity = arg1.toFloat(&ok);
    if((opacity >= 0) && (opacity <= 100));
    else ok = 0;

    if(ok)
    {
        glwidgetortho->blueprint_opa[glwidgetortho->textureid] = (float)opacity/100;
        glwidgetortho->updateGL();
    }
}
