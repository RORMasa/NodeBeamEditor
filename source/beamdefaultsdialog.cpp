#include "beamdefaultsdialog.h"
#include "ui_beamdefaultsdialog.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QColor>
#include <QColorDialog>
#include <QRegExp>

BeamDefaultsDialog::BeamDefaultsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BeamDefaultsDialog)
{
    ui->setupUi(this);
    lastactiveindex = 0;

    RGB_color.resize(3);

    firstload = 0;
    selecting = 0;
    selected = -1;

}

BeamDefaultsDialog::~BeamDefaultsDialog()
{
    delete ui;
}

void BeamDefaultsDialog::on_pushButton_clicked()
{
    NewProperties();
}

/* Load all arguments from the folder */
void BeamDefaultsDialog::LoadAll()
{
    SetFilePath();

    QStringList suffixi("*.txt");

    QDir dir(filepath);
    QStringList files = dir.entryList(suffixi);

    for(int i=0; i< files.size(); i++)
    {
        QString filename = filepath;
        filename.append(files[i]);
        LoadPropertiesFromFile(filename);
    }
    RefreshGUI();

}

void BeamDefaultsDialog::SetFilePath()
{
    if(argumenttype == BEAM_ARGUMENTS)
    {
        filepath = "arguments/beams/";
    }
    else if (argumenttype == HUBWHEEL_ARGUMENTS)
    {
        filepath = "arguments/wheels/";
    }
    else if (argumenttype == NODE_ARGUMENTS)
    {
        filepath = "arguments/nodes/";
    }
}

void BeamDefaultsDialog::NewProperties()
{
    LoadPropertiesFromFile("arguments/beams/default.txt");
    RefreshGUI();
}

void BeamDefaultsDialog::RefreshGUI()
{

    ui->comboBox->clear();
    for(int i=0; i<(*NBPointer).size();i++)
    {
        ui->comboBox->addItem((*NBPointer)[i].Name);

    }

    if((*NBPointer).size()>0)
    {
        ui->comboBox->setCurrentIndex(lastactiveindex);
        ui->lineEdit->setText((*NBPointer)[ui->comboBox->currentIndex()].Name);

        if((*NBPointer)[lastactiveindex].RGB_Color.size() > 1)
        {
            RGB_color[0] = (*NBPointer)[lastactiveindex].RGB_Color[0];
            RGB_color[1] = (*NBPointer)[lastactiveindex].RGB_Color[1];
            RGB_color[2] = (*NBPointer)[lastactiveindex].RGB_Color[2];
            QColor color;
            color.setRgb(RGB_color[0],RGB_color[1],RGB_color[2]);
            QString stylesheet;
            stylesheet = QString("background-color: %1").arg(color.name());
            ui->pushButton_4->setStyleSheet(stylesheet);
        }

        QStringList names;
        names << "Name" << "Value";
        ui->tableWidget->clear();
        ui->tableWidget->setColumnCount(2);
        ui->tableWidget->setColumnWidth(0,180);
        ui->tableWidget->setColumnWidth(1,150);
        ui->tableWidget->setRowCount(50);
        ui->tableWidget->setHorizontalHeaderLabels(names);
        for(int i=0; i<(*NBPointer)[lastactiveindex].KeyValuePairs.size();i++)
        {
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem((*NBPointer)[lastactiveindex].KeyValuePairs[i].name));
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem((*NBPointer)[lastactiveindex].KeyValuePairs[i].value));

        }
    }
}

void BeamDefaultsDialog::setNBPointer(QVector<BeamDefs> *NBPointer1)
{
    NBPointer = NBPointer1;
}

void BeamDefaultsDialog::on_pushButton_4_clicked()
{
    QColor color;
    color.setRgb(RGB_color[0],RGB_color[1],RGB_color[2]);
    QString stylesheet;
    color = QColorDialog::getColor(color,0,"Choose beam color",0);
    if(color.isValid())
    {
        stylesheet = QString("background-color: %1").arg(color.name());
        ui->pushButton_4->setStyleSheet(stylesheet);

        RGB_color[0] = color.red();
        RGB_color[1] = color.green();
        RGB_color[2] = color.blue();

    }
}

/* Apply changes */
void BeamDefaultsDialog::on_pushButton_5_clicked()
{
    if((*NBPointer).size()>0)
    {
        /* Apply name changes*/
        (*NBPointer)[ui->comboBox->currentIndex()].Name = ui->lineEdit->text();

        /* Apply color changes */
        (*NBPointer)[ui->comboBox->currentIndex()].RGB_Color[0] = RGB_color[0];
        (*NBPointer)[ui->comboBox->currentIndex()].RGB_Color[1] = RGB_color[1];
        (*NBPointer)[ui->comboBox->currentIndex()].RGB_Color[2] = RGB_color[2];

        /* Apply beam argument changes */
        for(int i=0; i<ui->tableWidget->rowCount();i++)
        {
            if(ui->tableWidget->item(i,0)!= 0)
            {

                bool found = 0;
                for(int i2=0; i2<(*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs.size();i2++)
                {

                    if(ui->tableWidget->item(i,0)->text() == (*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs[i2].name)
                    {
                        (*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs[i2].value = ui->tableWidget->item(i,1)->text();
                        found=1;
                    }
                }

                if(found == 0)
                {
                    //Value doesn't exist, adding it.
                    int vectorsize = (*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs.size();

                    (*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs.resize(vectorsize+1);
                    (*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs[vectorsize].name = ui->tableWidget->item(i,0)->text();
                    (*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs[vectorsize].value = ui->tableWidget->item(i,1)->text();
                }
            }
        }
        for(int i=0; i<(*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs.size();i++)
        {
            bool found = 0;
            for(int i2=0; i2<ui->tableWidget->rowCount(); i2++)
            {
                if(ui->tableWidget->item(i2,0)!= 0)
                {
                    if(ui->tableWidget->item(i2, 0)->text() == (*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs[i].name)
                    {
                       found = 1;
                    }
                }
            }
            if(found == 0)
            {
                (*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs.remove(i);
                i = i - 1;
            }
        }

        RefreshGUI();
    }
}

void BeamDefaultsDialog::on_comboBox_activated(int index)
{
    ui->lineEdit->setText((*NBPointer)[ui->comboBox->currentIndex()].Name);
    lastactiveindex = ui->comboBox->currentIndex();

    RefreshGUI();

}

void BeamDefaultsDialog::LoadPropertiesFromFile(const QString &fileName)
{
    qDebug()<< "loading file";
    QFile file(fileName);
    if(!file.exists())
    {
        qDebug() << "file does not exist";
    }
    else
    {
    if (!file.open(QFile::ReadOnly | QFile::Text)) {

    }

    QTextStream in(&file);

    #ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif

    QString CurrentLine;
    QString temp;

    /* Name */
    (*NBPointer).resize((*NBPointer).size()+1);
    (*NBPointer)[(*NBPointer).size()-1].Name = in.readLine();

    /* RGB color should be on 2nd line */
    QString color1 = in.readLine();
    QRegExp regexp("(\\,)");
    QStringList colorrgba = color1.split(regexp);
    (*NBPointer)[(*NBPointer).size()-1].RGB_Color.resize(3);
    (*NBPointer)[(*NBPointer).size()-1].RGB_Color[0]= colorrgba[0].toInt();
    (*NBPointer)[(*NBPointer).size()-1].RGB_Color[1]= colorrgba[1].toInt();
    (*NBPointer)[(*NBPointer).size()-1].RGB_Color[2]= colorrgba[2].toInt();

    /* arguments itself */
    while(!in.atEnd())
    {
        int currindex = (*NBPointer)[(*NBPointer).size()-1].KeyValuePairs.size();
        (*NBPointer)[(*NBPointer).size()-1].KeyValuePairs.resize(currindex+1);
        CurrentLine = in.readLine();
        temp="";
        int i;
        for(i=0; i<CurrentLine.length();i++)
        {
            if(CurrentLine[i]=='=')
            {
                break;
            }
            else
            {
                temp.append(CurrentLine[i]);
            }
        }
        (*NBPointer)[(*NBPointer).size()-1].KeyValuePairs[currindex].name = temp;
        temp = "";
        i++;
        for(i; i<CurrentLine.length();i++)
        {
                temp.append(CurrentLine[i]);
        }
        (*NBPointer)[(*NBPointer).size()-1].KeyValuePairs[currindex].value = temp;

    }
    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
    #endif
    }
    file.close();


}

void BeamDefaultsDialog::on_buttonBox_accepted()
{
    if((*NBPointer).size()>0)
    {
        /* Apply name changes*/
        (*NBPointer)[ui->comboBox->currentIndex()].Name = ui->lineEdit->text();

        /* Apply color changes */
        (*NBPointer)[ui->comboBox->currentIndex()].RGB_Color[0] = RGB_color[0];
        (*NBPointer)[ui->comboBox->currentIndex()].RGB_Color[1] = RGB_color[1];
        (*NBPointer)[ui->comboBox->currentIndex()].RGB_Color[2] = RGB_color[2];

        /* Apply beam argument changes */
        for(int i=0; i<ui->tableWidget->rowCount();i++)
        {
            if(ui->tableWidget->item(i,0)!= 0)
            {

                bool found = 0;
                for(int i2=0; i2<(*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs.size();i2++)
                {

                    if(ui->tableWidget->item(i,0)->text() == (*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs[i2].name)
                    {
                        (*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs[i2].value = ui->tableWidget->item(i,1)->text();
                        found=1;
                    }
                }

                if(found == 0)
                {
                    //Value doesn't exist, adding it.
                    int vectorsize = (*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs.size();

                    (*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs.resize(vectorsize+1);
                    (*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs[vectorsize].name = ui->tableWidget->item(i,0)->text();
                    (*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs[vectorsize].value = ui->tableWidget->item(i,1)->text();
                }
            }
        }
        for(int i=0; i<(*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs.size();i++)
        {
            bool found = 0;
            for(int i2=0; i2<ui->tableWidget->rowCount(); i2++)
            {
                if(ui->tableWidget->item(i2,0)!= 0)
                {
                    if(ui->tableWidget->item(i2, 0)->text() == (*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs[i].name)
                    {
                       found = 1;
                    }
                }
            }
            if(found == 0)
            {
                (*NBPointer)[ui->comboBox->currentIndex()].KeyValuePairs.remove(i);
                i = i - 1;
            }
        }

        RefreshGUI();

        if(selecting) selected = ui->comboBox->currentIndex();
    }
}
