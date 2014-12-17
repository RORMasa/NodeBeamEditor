#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::FixCommas_NextChar(QString &sample, QChar &nextchar, int &nextchar_i)
{
    //Find the next character other than comma, space, or line change
    for(int i=nextchar_i; i<sample.length();i++)
    {
        if(sample[i] == ' ');
        else if(sample[i] == '\n');
        else if(sample[i] == '	');
        else
        {
            nextchar = sample[i];
            nextchar_i = i;
            break;
        }
    }
}

void MainWindow::FixCommas_PrevChar(QString &sample, QChar &prevchar, int &prevchar_i)
{
    //Find the previous character other than space, or line change
    for(int i=prevchar_i; i>0;i--)
    {
        qDebug() << "checking p " << sample[i];
        if(sample[i] == ' ');
        else if(sample[i] == '\n');
        else if(sample[i] == '	');
        else
        {
            prevchar = sample[i];
            prevchar_i = i;
            //qDebug() << "NEXT char is " << nextchar;
            break;
        }
    }
}

QByteArray MainWindow::FixCommas(QByteArray JbeamText)
{
    QString temp1;
    bool checking = 0;

    QString JbeamTextSTR = JbeamText.constData();

    for(int i=0; i<JbeamTextSTR.length(); i++)
    {
        //qDebug() << "tarkisus " << JbeamTextSTR[i];
        if((JbeamTextSTR[i] == ']')||(JbeamTextSTR[i] == '}'))
        {
            QChar nextchar = 'ö';
            int nextchar_i = i+1;
            FixCommas_NextChar(JbeamTextSTR, nextchar, nextchar_i);
            qDebug() << JbeamTextSTR[nextchar_i];
            qDebug() << "next char is " << nextchar;
            if((nextchar == '[')||(nextchar == '{'))
            {
                JbeamTextSTR.insert(i+1,',');
                i++;
                qDebug() << "Inserting letter";
            }
            else if(nextchar == '"')
            {
                JbeamTextSTR.insert(i+1,',');
                i++;
                qDebug() << "Inserting letter";
            }
        }

    }

    for(int i=0; i<JbeamTextSTR.length(); i++)
    {
        //qDebug() << "tarkisus " << JbeamTextSTR[i];
        if((JbeamTextSTR[i] == ']')||(JbeamTextSTR[i] == '}'))
        {
            QChar prevchar = 'ö';
            int prevchar_i=0;
            if (i>0) prevchar_i = i-1;
            qDebug() << "aloituspiste " << JbeamTextSTR[i-1]<<JbeamTextSTR[prevchar_i]<<JbeamTextSTR[i+1];
            FixCommas_PrevChar(JbeamTextSTR, prevchar, prevchar_i);
            if(prevchar == ',')
            {
                int comma_i = prevchar_i;
                prevchar_i--;

                FixCommas_PrevChar(JbeamTextSTR, prevchar, prevchar_i);
                qDebug() << JbeamTextSTR[prevchar_i];
                qDebug() << "prev char is " << prevchar;
                /*
                if((prevchar == ']') || (prevchar == '}'))
                {
                    JbeamTextSTR.replace(comma_i, 1, " ");
                    qDebug() << "Removing letter";
                }
                */
                JbeamTextSTR.replace(comma_i, 1, " ");

            }
        }

    }

    JbeamText.clear();
    JbeamText.append(JbeamTextSTR);
    ui->textBrowser->setText(JbeamText);
    return JbeamText;
}

void MainWindow::on_pushButton_clicked()
{


    //Text from text browser to QByteArray
    QByteArray JBeamInputText;
    JBeamInputText.append(ui->textEdit->toPlainText());

    //Fix commas to make valid JSON
    JBeamInputText = FixCommas(JBeamInputText);

    //Parse error
    QJsonParseError virhe;

    //Parse textbox text to JSON
    QJsonDocument Jbeam = QJsonDocument::fromJson(JBeamInputText, &virhe);

    //Error messages
    qDebug() << "JSON parsing result: " << ", " << virhe.errorString();

    QJsonObject JbeamObject = Jbeam.object();

    qDebug() << "Object is empty:" << Jbeam.object().isEmpty();

    if(!JbeamObject.isEmpty())
    {
        for(int i=0; i<JbeamObject.keys().length(); i++)
        {
            qDebug() << "Parsing slot " << JbeamObject.keys()[i];
            QJsonObject JbeamObject_slot;
            QJsonValue slot = JbeamObject.value(JbeamObject.keys()[i]);

            if(slot.isObject())
            {
                JbeamObject_slot = slot.toObject();
                qDebug() << "Keys in slot: " << JbeamObject_slot.keys();

                //Get nodes from slot
                QJsonArray Jbeam_nodes;
                QJsonObject::iterator slot_iterator = JbeamObject_slot.find("nodes");
                if (slot_iterator != JbeamObject_slot.end())
                {
                    Jbeam_nodes = JbeamObject_slot.value("nodes").toArray();
                    qDebug() << "nodes found " << Jbeam_nodes.count();
                }


            }



        }
    }
    else qDebug() << "JBEAM object not found.";

    QJsonArray JBeamArray = Jbeam.array();
    qDebug() << JBeamArray.count();

    QJsonValue arvo;



}
