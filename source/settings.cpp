#include "settings.h"
#include "ui_settings.h"
#include <QSettings>
#include <QFile>
#include <QFileDialog>
#include <QColorDialog>
#include <QListWidget>
#include <QListWidgetItem>

settings::settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);

    QString path = qApp->applicationDirPath() + "/editor.ini";
    appsett = new QSettings (path, QSettings::IniFormat);
    ui->lineEdit->setText(appsett->value("author").toString());
    ui->lineEdit_2->setText(appsett->value("beamnglocation").toString());
    ui->lineEdit_3->setText(appsett->value("currentvehicle").toString());
    ui->lineEdit_4->setText(appsett->value("currentlevel").toString());
    ui->lineEdit_5->setText(appsett->value("rorlocation").toString());
    ui->lineEdit_6->setText(appsett->value("rorveh").toString());
    ui->lineEdit_7->setText(appsett->value("rormap").toString());



    //EditorMode
    if(appsett->value("editor_mode") == "1")
    {
        ui->radioButton->setChecked(1);
        ui->radioButton_2->setChecked(0);
    }
    else if(appsett->value("editor_mode") == "2")
    {
        ui->radioButton->setChecked(0);
        ui->radioButton_2->setChecked(1);
    }


    //Color buttons
    QString stylesheet;
    bg_color.setRgb(appsett->value("bg_color_r").toInt(),appsett->value("bg_color_g").toInt(),appsett->value("bg_color_b").toInt());
    grid_color.setRgb(appsett->value("grid_color_r").toInt(),appsett->value("grid_color_g").toInt(),appsett->value("grid_color_b").toInt());
    stylesheet = QString("background-color: %1").arg(bg_color.name());
    ui->pushButton_2->setStyleSheet(stylesheet);
    stylesheet = QString("background-color: %1").arg(grid_color.name());
    ui->pushButton_3->setStyleSheet(stylesheet);

    QVector <QString> pages;
    pages.resize(3);
    pages[0] = "General";
    pages[1] = "Testing";
    pages[2] = "UI";

    for(int i=0; i<pages.size(); i++)
    {
        QListWidgetItem *item5 = new QListWidgetItem;
        item5->setText(pages[i]);
        ui->listWidget->addItem(item5);

    }
}

settings::~settings()
{
    delete ui;
}

QString settings::readsetting(QString key1)
{
    QString setting;
    setting = appsett->value(key1).toString();
    return setting;
}

void settings::SaveSettings()
{
    appsett->setValue("author", ui->lineEdit->text());
    appsett->setValue("beamnglocation", ui->lineEdit_2->text());
    appsett->setValue("currentvehicle", ui->lineEdit_3->text());
    appsett->setValue("currentlevel", ui->lineEdit_4->text());
    appsett->setValue("bg_color_r", bg_color.red());
    appsett->setValue("bg_color_g", bg_color.green());
    appsett->setValue("bg_color_b", bg_color.blue());
    appsett->setValue("grid_color_r", grid_color.red());
    appsett->setValue("grid_color_g", grid_color.green());
    appsett->setValue("grid_color_b", grid_color.blue());
    if(ui->radioButton->isChecked())
    {
        appsett->setValue("editor_mode", "1");
    }
    else if(ui->radioButton_2->isChecked())
    {
        appsett->setValue("editor_mode", "2");
    }

    appsett->setValue("rorlocation", ui->lineEdit_5->text());
    appsett->setValue("rorveh", ui->lineEdit_6->text());
    appsett->setValue("rormap", ui->lineEdit_7->text());
}

void settings::on_buttonBox_accepted()
{
    SaveSettings();
    emit SettingsUpdated();
}

/* Locate BeamNG exe */
void settings::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        ui->lineEdit_2->setText(fileName);
    }
}

//Background color chooser
void settings::on_pushButton_2_clicked()
{
    QString stylesheet;
    bg_color = QColorDialog::getColor(bg_color,0,"Choose background color",0);
    if(bg_color.isValid())
    {
        stylesheet = QString("background-color: %1").arg(bg_color.name());
        ui->pushButton_2->setStyleSheet(stylesheet);

    }
}

void settings::on_listWidget_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}


void settings::showEvent(QShowEvent *)
{

}

/* Change grid color */
void settings::on_pushButton_3_clicked()
{
    QString stylesheet;
    grid_color = QColorDialog::getColor(grid_color,0,"Choose grid color",0);
    if(grid_color.isValid())
    {
        stylesheet = QString("background-color: %1").arg(grid_color.name());
        ui->pushButton_3->setStyleSheet(stylesheet);

    }
}

/* Locate RoR exe */
void settings::on_pushButton_4_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        ui->lineEdit_5->setText(fileName);
    }
}
