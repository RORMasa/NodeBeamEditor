#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QTextCursor>

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

    //Remove comments
    JBeamInputText = JBEAM_RemoveComments(JBeamInputText);

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

    int loc1;
    int loc2;

    QTextCursor  kursoori = ui->textEdit->textCursor();
    int muistiin = kursoori.position();

    QString TextBoxText = ui->textEdit->toPlainText();
    if(FindNodeContainer(TextBoxText, "a3", loc1, loc2))
    {
        QString texti = "diibadaaba";
        TextBoxText.replace(loc1,loc2-loc1,texti);

        ui->textEdit->setText(TextBoxText);
    }
    else qDebug() << "Node not found";

    //ui->textEdit->setTextCursor(kursoori);

}

/* Comment filter */
QByteArray MainWindow::JBEAM_RemoveComments(QByteArray JbeamText)
{
    QString JbeamTextSTR = JbeamText.constData();

    bool commentfound=0;
    int commentcheck = 0;
    for(int i=0; i<JbeamTextSTR.length(); i++)
    {
        qDebug() << "removing comments" << i;
        if(JbeamTextSTR[i] == '/') commentcheck++;
        else commentcheck = 0;
        if(commentcheck==2)
        {
            qDebug() << "removing commentrrs";
            commentcheck=0;
            commentfound=1;
            int i2;
            for(i2=i; i2<JbeamTextSTR.length();i2++)
            {
                if(JbeamTextSTR[i2]=='\n') break;
            }
            JbeamTextSTR.replace(i-1,i2-i+1,' ');
        }

    }
    if(commentfound)
    {
        JbeamText.clear();
        JbeamText.append(JbeamTextSTR);
        return JbeamText;
    }
    else return JbeamText;
}

/* This function will find, where the node and it's 3 coordinates are in the JBEAM string.
Results will be returned in NodeBegin and NodeEnd integer values.
Bool will be false, if node was not found.
*/

bool MainWindow::FindNodeContainer(QString JBEAM_box, QString nodename, int &NodeBegin, int &NodeEnd)
{
    int NodeFound = 0;
    int linenumber=0;

    QString temp = "";
    int i=0;
    for(i; i<JBEAM_box.length();i++)
    {
        if(JBEAM_box[i] == ' ');
        else if (JBEAM_box[i] == '\u0009');
        else if (JBEAM_box[i] == '\n')linenumber++;
        else
        {
            temp.append(JBEAM_box[i]);
            if(temp.indexOf("\"nodes\":") >= 0)
            {
                break;
            }
        }
    }
    int nodes_begin = i;

    //Find nodes section container END
    int listlevel = 0;
    for(i; i<JBEAM_box.length();i++)
    {
        if (JBEAM_box[i] == '\n')linenumber++;
        else if (JBEAM_box[i] == '[') listlevel++;
        else if (JBEAM_box[i] == ']')
        {
            listlevel--;
            if(listlevel == 0) break;
        }
    }

    int nodes_end = i;
    temp.clear();
    QString NodeLost = "[\"";
    NodeLost+=nodename;
    NodeLost+="\"";

    //Find node
    for(i=nodes_begin; i<nodes_end;i++)
    {
        if(JBEAM_box[i] == ' ');
        else if (JBEAM_box[i] == '\u0009');
        else if (JBEAM_box[i] == '\n')linenumber++;
        else
        {
            temp.append(JBEAM_box[i]);
            if(temp.indexOf(NodeLost) >= 0)
            {
                NodeFound++;
                break;
            }
        }
    }

    //Find node begin
    for(i; i>nodes_begin; i--)
    {
        if(JBEAM_box[i] == '[') break;
    }


    int node_begin = i;

    listlevel = 0;
    //Find node end
    for(i; i<nodes_end; i++)
    {
        if(JBEAM_box[i] == '[') listlevel++;
        else if(JBEAM_box[i] == ']')
        {
            listlevel--;
            if(listlevel == 0)
            {
                break;
            }
        }
    }

    int node_end = i;

    //Find the location of the last node coordinate.
    int grid_id = 0;
    bool valuefound = 0;
    int nodenamecalc = 0;
    for(i=node_begin+1; i<node_end+1; i++)
    {
        if(grid_id == 0)
        {
            if(JBEAM_box[i] == '"') nodenamecalc++;
            if(nodenamecalc==2)
            {
                valuefound=1;
            }
        }
        else if(grid_id > 0)
        {
            if(JBEAM_box[i].isDigit())
            {
                valuefound = 1;
            }
        }

        if(valuefound)
        {
            if((JBEAM_box[i] == ' ') || (JBEAM_box[i] == ',') || (JBEAM_box[i] == '\u0009') || (JBEAM_box[i] == '\n') || (JBEAM_box[i] == ']'))
            {
                valuefound=0;
                grid_id++;
            }
        }
        if(grid_id == 4)
        {
            NodeFound++;
            break;
        }
    }

    //int node_end_real = i-1;

    NodeBegin = node_begin+1;
    NodeEnd = i-1;

    if(NodeFound == 2)  return true;
    else return false;

    /* Test */
    /*
    temp.clear();
    temp = JBEAM_box.right(JBEAM_box.length()-node_begin-1);
    temp = temp.left(node_end_real - node_begin);

    qDebug() << temp;
    */

}
