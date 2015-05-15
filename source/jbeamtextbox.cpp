#include <QtWidgets>
#include <QtOpenGL>
#include <QTimerEvent>
#include <QPixmap>
#include <QJsonDocument>
#include <QLabel>

#include "mainwindow.h"

/* ==== JBEAM TEXT EDIT ==== */
JBEAM_TextBox::JBEAM_TextBox(QWidget *parent, NodeBeam *nb) : QPlainTextEdit(parent)
{
    LineNumbersA = new LineNumbers(this);
    setViewportMargins(26,0,0,0);

    //Connect linenumbers update request
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumbers(QRect,int)));
    connect(this, SIGNAL(textChanged()), this, SLOT(textchanged()));

    JBEAM_NodeCursor = -1;
    JBEAM_BeamCursor = -1;
    CurrentNodeBeam = nb;

    this->setLineWrapMode(QPlainTextEdit::NoWrap); //No automatic line change
    this->setTabStopWidth(15); //Set smaller TAB width

    EmptyJbeamTextTemplate = "{\n"
                   "    \"Vehicle\":{\n"
                   "        \"information\":{\n"
                   "            \"authors\":\"Your name\"      \n"
                   "        },\n"
                   "        \"nodes\":[\n"
                   "            [\"id\", \"posX\", \"posY\", \"posZ\"],\n\n"
                   "            //BNEnodes\n"
                   "   \n"
                   "        ],\n"
                   "        \"beams\":[\n"
                   "            [\"id1:\", \"id2:\"],\n\n"
                   "            //BNEbeams\n"
                   "   \n"
                   "   \n"
                   "        ],\n"
                   "        \"triangles\":[\n"
                   "            [\"id1:\", \"id2:\", \"id3:\"],\n\n"
                   "            //BNEtriangles\n"
                   "   \n"
                   "   \n"
                   "        ],\n"
                   "   }\n"
                   "}\n";

    ResetContents();
}

//Paint linenumbers
void JBEAM_TextBox::LineNumbersPaintEvent(QPaintEvent *event)
{
    QPainter painter(LineNumbersA);
    painter.fillRect(event->rect(), Qt::white);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::gray);
            painter.drawText(0, top, LineNumbersA->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void JBEAM_TextBox::updateLineNumbers(const QRect &rect, int dy)
{
    if (dy) LineNumbersA->scroll(0, dy);
    else LineNumbersA->update(0, rect.y(), LineNumbersA->width(), rect.height());
}

void JBEAM_TextBox::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    LineNumbersA->setGeometry(QRect(cr.left(), cr.top(), 26, cr.height()));
}

void JBEAM_TextBox::textchanged()
{
    this->JBEAM_UpdateCursors();
}

/* Place new nodes at current text cursor location */
void JBEAM_TextBox::PlaceNodeCursor()
{
    //Prepare new cursor
    int CurrentCursorPos = this->textCursor().position();
    //Remove old cursor
    QString JBEAM_text = this->toPlainText();
    JBEAM_text.replace("//BNEnodes\n", "          \n");
    this->setPlainText(JBEAM_text);

    QTextCursor cursor = this->textCursor();
    cursor.setPosition(CurrentCursorPos);
    cursor.insertText("\n\u0009\u0009\u0009//BNEnodes\n");
    this->setTextCursor(cursor);
}
/* Place new beams at current text cursor location */
void JBEAM_TextBox::PlaceBeamCursor()
{
    //Prepare new cursor
    int CurrentCursorPos = this->textCursor().position();
    //Remove old cursor
    QString JBEAM_text = this->toPlainText();
    JBEAM_text.replace("//BNEbeams\n", "          \n");
    this->setPlainText(JBEAM_text);

    QTextCursor cursor = this->textCursor();
    cursor.setPosition(CurrentCursorPos);
    cursor.insertText("\n\u0009\u0009\u0009//BNEbeams\n");
    this->setTextCursor(cursor);
}

/* Find node and beam cursors from JBEAM string in JBEAM textbox */
void JBEAM_TextBox::JBEAM_UpdateCursors()
{
    QString JBEAM_text = this->toPlainText();
    JBEAM_NodeCursor = JBEAM_text.indexOf("//BNEnodes");
    if(JBEAM_NodeCursor>=0)
    {
        for(int i=JBEAM_NodeCursor; i>0;i--)
        {
            if(JBEAM_text.at(i) == '\n')
            {
                JBEAM_NodeCursor = i;
                break;
            }
            else JBEAM_NodeCursor = -1;
        }
    }
    //qDebug() << "Node cursor: " << JBEAM_NodeCursor;
    JBEAM_BeamCursor = JBEAM_text.indexOf("//BNEbeams");
    if(JBEAM_BeamCursor>=0)
    {
        for(int i=JBEAM_BeamCursor; i>0;i--)
        {
            if(JBEAM_text.at(i) == '\n')
            {
                JBEAM_BeamCursor = i;
                break;
            }
            else JBEAM_BeamCursor = -1;
        }
    }
    //qDebug() << "Beam cursor: " << JBEAM_BeamCursor;
}

/* Update contents */
//Update all nodes
void JBEAM_TextBox::JBEAM_UpdateAllNodes()
{

    int pos1; //Position containers for parsing the textbox
    int pos2, pos3;

    QString TextBoxText = this->toPlainText();
    for(int i=0; i<CurrentNodeBeam->Nodes.size();i++)
    {
        QString NodeName = CurrentNodeBeam->Nodes.at(i).NodeName;

        //Find node from textbox by nodename
        if(FindNodeContainer(&TextBoxText, NodeName, pos1, pos2, 0, pos3))
        {
            QString nodeline = '"' + NodeName + '"';
            nodeline+= ", ";
            nodeline+= QString::number(CurrentNodeBeam->Nodes[i].locX,'f',5);
            nodeline+= ", ";
            nodeline+= QString::number(CurrentNodeBeam->Nodes[i].locY,'f',5);
            nodeline+= ", ";
            nodeline+= QString::number(CurrentNodeBeam->Nodes[i].locZ,'f',5);

            TextBoxText.replace(pos1,pos2-pos1+1,nodeline);
        }
        else qDebug() << "Node not found";

    }
    this->setPlainText(TextBoxText);
}
//Update selected nodes
void JBEAM_TextBox::JBEAM_UpdateSelectedNodes()
{
    int pos1; //Position containers for parsing the textbox
    int pos2, pos3;

    QString TextBoxText = this->toPlainText();
    for(int i=0; i<CurrentNodeBeam->SelectedNodes.size();i++)
    {
        QString NodeName = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i]].NodeName;

        //Find node from textbox by nodename
        if(FindNodeContainer(&TextBoxText, NodeName, pos1, pos2, 0, pos3))
        {
            QString nodeline = '"' + NodeName + '"';
            nodeline+= ", ";
            nodeline+= QString::number(CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i]].locX,'f',5);
            nodeline+= ", ";
            nodeline+= QString::number(CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i]].locY,'f',5);
            nodeline+= ", ";
            nodeline+= QString::number(CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i]].locZ,'f',5);

            TextBoxText.replace(pos1,pos2-pos1+1,nodeline);
        }
        else qDebug() << "Node not found";

    }
    this->setPlainText(TextBoxText);
}

/* Find containers */
/* This function will find, where the node and it's 3 coordinates are in the JBEAM string.
Results will be returned in NodeBegin and NodeEnd integer values.
Bool will be false, if node was not found.*/
bool JBEAM_TextBox::FindNodeContainer(QString *JBEAM_box, QString nodename, int &NodeBegin, int &NodeEnd, bool FindComma, int &RealNodeEnd)
{
    int NodeFound = 0;
    int linenumber=0;

    QString temp = "";
    int i;
    for(i=0; i<JBEAM_box->length();i++)
    {
        if(JBEAM_box->at(i) == ' ');
        else if (JBEAM_box->at(i) == '\u0009');
        else if (JBEAM_box->at(i) == '\n')linenumber++;
        else
        {
            temp.append(JBEAM_box->at(i));
            if(temp.indexOf("\"nodes\":") >= 0)
            {
                break;
            }
        }
    }
    int nodes_begin = i;

    //Find nodes section container END
    int listlevel = 0;
    for(i; i<JBEAM_box->length();i++)
    {
        if (JBEAM_box->at(i) == '\n')linenumber++;
        else if (JBEAM_box->at(i) == '[') listlevel++;
        else if (JBEAM_box->at(i) == ']')
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
        if(JBEAM_box->at(i) == ' ');
        else if (JBEAM_box->at(i) == '\u0009');
        else if (JBEAM_box->at(i) == '\n')linenumber++;
        else
        {
            temp.append(JBEAM_box->at(i));
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
        if(JBEAM_box->at(i) == '[') break;
    }


    int node_begin = i;

    listlevel = 0;
    //Find node end
    for(i; i<nodes_end; i++)
    {
        if(JBEAM_box->at(i) == '[') listlevel++;
        else if(JBEAM_box->at(i) == ']')
        {
            listlevel--;
            if(listlevel == 0)
            {
                break;
            }
        }
    }

    int node_end = i;
    RealNodeEnd = i;

    //Find comma after the node if needed, to be able ro remove it
    //when deleting nodes
    if(FindComma)
    {
        for(i=node_end+1; i<nodes_end; i++)
        {
            if(JBEAM_box->at(i) == ' ');
            else if (JBEAM_box->at(i) == '\u0009');
            else if (JBEAM_box->at(i) == '\n');
            else if (JBEAM_box->at(i) == ',')
            {
                RealNodeEnd = i;
                break;
            }
            else
            {
                break;
            }
        }
    }

    //Find the location of the last node coordinate.
    int grid_id = 0;
    bool valuefound = 0;
    int nodenamecalc = 0;
    for(i=node_begin+1; i<node_end+1; i++)
    {
        if(grid_id == 0)
        {
            if(JBEAM_box->at(i) == '"') nodenamecalc++;
            if(nodenamecalc==2)
            {
                valuefound=1;
            }
        }
        else if(grid_id > 0)
        {
            if(JBEAM_box->at(i).isDigit())
            {
                valuefound = 1;
            }
        }

        if(valuefound)
        {
            if((JBEAM_box->at(i) == ' ') || (JBEAM_box->at(i) == ',') || (JBEAM_box->at(i) == '\u0009') || (JBEAM_box->at(i) == '\n') || (JBEAM_box->at(i) == ']'))
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
    temp = JBEAM_box->right(JBEAM_box->length()-node_begin-1);
    temp = temp.left(node_end_real - node_begin);

    qDebug() << temp;
    */
}

bool JBEAM_TextBox::FindBeamContainer(QString *JBEAM_box, QString beam, int &Begin, int &End, bool FindComma,int &RealEnd)
{
    int BeamFound = 0;

    QString temp = "";
    int i=0;
    for(i; i<JBEAM_box->length();i++)
    {
        if(JBEAM_box->at(i) == ' ');
        else if (JBEAM_box->at(i) == '\u0009');
        else if (JBEAM_box->at(i) == '\n');
        else
        {
            temp.append(JBEAM_box->at(i));
            if(temp.indexOf("\"beams\":") >= 0)
            {
                break;
            }
        }
    }
    int beams_begin = i;

    //Find beams section container END
    int listlevel = 0;
    for(i; i<JBEAM_box->length();i++)
    {
        if (JBEAM_box->at(i) == '\n');
        else if (JBEAM_box->at(i) == '[') listlevel++;
        else if (JBEAM_box->at(i) == ']')
        {
            listlevel--;
            if(listlevel == 0) break;
        }
    }

    int beams_end = i;
    temp.clear();
    QString BeamLost = "[";
    BeamLost+=beam;

    //Find beam
    for(i=beams_begin; i<beams_end;i++)
    {
        if(JBEAM_box->at(i) == ' ');
        else if (JBEAM_box->at(i) == '\u0009');
        else if (JBEAM_box->at(i) == '\n');
        else if (JBEAM_box->at(i) == ',');
        else
        {
            temp.append(JBEAM_box->at(i));
            if(temp.indexOf(BeamLost) >= 0)
            {
                BeamFound++;
                break;
            }
        }
    }

    //Find beam begin
    for(i; i>beams_begin; i--)
    {
        if(JBEAM_box->at(i) == '[') break;
    }


    int beam_begin = i;

    listlevel = 0;
    //Find beam end
    for(i; i<beams_end; i++)
    {
        if(JBEAM_box->at(i) == '[') listlevel++;
        else if(JBEAM_box->at(i) == ']')
        {
            listlevel--;
            if(listlevel == 0)
            {
                break;
            }
        }
    }

    int beam_end = i;
    RealEnd = i;

    //Find comma after the beam if needed, to be able ro remove it
    //when deleting beams
    if(FindComma)
    {
        for(i=beam_end+1; i<beams_end; i++)
        {
            if(JBEAM_box->at(i) == ' ');
            else if (JBEAM_box->at(i) == '\u0009');
            else if (JBEAM_box->at(i) == '\n');
            else if (JBEAM_box->at(i) == ',')
            {
                RealEnd = i;
                break;
            }
            else
            {
                break;
            }
        }
    }

    //Still missing finding pos2

    Begin = beam_begin;
    RealEnd = beam_end;

    if(BeamFound>0) return true;
    else return false;
}

bool JBEAM_TextBox::JBEAM_FindOtherContainer(QString JBEAM_box, QString listtype, int &Begin, int &End)
{
    QString pattern = "\"" + listtype + "\"\\s*:\\s*\\[";
    QRegExp re_container;
    re_container.setPattern(pattern);

    Begin = JBEAM_box.indexOf(re_container);
    int i=Begin;
    for(i; i<JBEAM_box.length(); i++)
    {
        if(JBEAM_box.at(i) == '[') break;
    }
    Begin = i;
    int level = 1;
    i++;
    qDebug() << JBEAM_box.at(i);
    for(i; i<JBEAM_box.length(); i++)
    {
        if(JBEAM_box.at(i) == '[') level++;
        else if(JBEAM_box.at(i) == ']') level--;
        if(level < 1)
        {
            qDebug() << "stop" << i;
            break;
        }
    }
    End = i;

    return true;
}


/* Add node at text cursor position */
void JBEAM_TextBox::JBEAM_AddNode()
{
    int node_id = CurrentNodeBeam->TempNode.GlobalID;
    QString nodeline = "           [";
    nodeline+= '"' + CurrentNodeBeam->TempNode.NodeName + '"';
    nodeline+= ", ";
    nodeline+= QString::number(CurrentNodeBeam->TempNode.locX,'f',5);
    nodeline+= ", ";
    nodeline+= QString::number(CurrentNodeBeam->TempNode.locY,'f',5);
    nodeline+= ", ";
    nodeline+= QString::number(CurrentNodeBeam->TempNode.locZ,'f',5);
    nodeline.append("],\n");
    QTextCursor textcursor = this->textCursor();
    if(JBEAM_NodeCursor >= 0)
    {
        textcursor.setPosition(JBEAM_NodeCursor);
    }

    for(int i=0; i<CurrentNodeBeam->TempNode.comments.size();i++)
    {
        JBEAM_AddComment(JBEAM_NodeCursor, CurrentNodeBeam->TempNode.comments.ReadComment(i));
    }
    textcursor.insertText(nodeline);
    this->setTextCursor(textcursor);
}

/* Add beam, in JBEAM edit widget at text cursor position */
void JBEAM_TextBox::JBEAM_AddBeam()
{
    QString beamline = "           [";
    beamline+= '"' + CurrentNodeBeam->TempBeam.Node1Name + '"';
    beamline+= ", ";
    beamline+= '"' + CurrentNodeBeam->TempBeam.Node2Name + '"';
    beamline+=("],\n");
    QTextCursor textcursor = this->textCursor();
    if(JBEAM_BeamCursor >= 0)
    {
        textcursor.setPosition(JBEAM_BeamCursor);
    }

    for(int i=0; i<CurrentNodeBeam->TempBeam.comments.size();i++)
    {
        JBEAM_AddComment(JBEAM_BeamCursor, CurrentNodeBeam->TempBeam.comments.ReadComment(i));
    }

    textcursor.insertText(beamline);
    this->setTextCursor(textcursor);
}


int JBEAM_TextBox::JBEAM_AddComment(int CursorPos, QString Comment)
{
    QString commentline = "\u0009\u0009\u0009//";
    commentline+= Comment;
    commentline+= "\n";
    QTextCursor textcursor = this->textCursor();
    if(CursorPos >= 0)
    {
        textcursor.setPosition(CursorPos);
    }
    textcursor.insertText(commentline);
    this->setTextCursor(textcursor);

    return commentline.length();
}

/* Add other part, in JBEAM edit widget at text cursor position */
void JBEAM_TextBox::JBEAM_AddArrayItem(int ListType_id)
{
    QVector <int> pickednodes = CurrentNodeBeam->ListTypes.at(ListType_id).contaier.last();
    QString line = "           [";
    line+= CurrentNodeBeam->ListTypes.at(ListType_id).JBEAM_template;
    line+=("],\n");

    for(int i=0; i<CurrentNodeBeam->ListTypes.at(ListType_id).nodeamount;i++)
    {
        line = line.arg(CurrentNodeBeam->Nodes.at(pickednodes.at(i)).NodeName);
    }

    QTextCursor textcursor = this->textCursor();

    textcursor.insertText(line);
    this->setTextCursor(textcursor);
}


//Write picked node in JBEAM widget
void JBEAM_TextBox::PrintNodePicked(int node_id)
{
    QString line = "\"%1\", ";
    line = line.arg(CurrentNodeBeam->Nodes.at(node_id).NodeName);

    QTextCursor textcursor = this->textCursor();

    textcursor.insertText(line);
    this->setTextCursor(textcursor);
}


void JBEAM_TextBox::JBEAM_DeleteNodes()
{
    int pos1; //Position containers for parsing the textbox
    int pos2;
    int pos3;

    QString TextBoxText = this->toPlainText();
    for(int i=0; i<CurrentNodeBeam->JBEAM_temp.delete_nodes.size();i++)
    {
        QString NodeName = CurrentNodeBeam->JBEAM_temp.delete_nodes.at(i).NodeName;

        //Find node from textbox by nodename
        if(FindNodeContainer(&TextBoxText, NodeName, pos1, pos2, true, pos3))
        {
            QString nodeline = "";

            TextBoxText.replace(pos1-1,pos3-pos1+2,nodeline);
        }
        //else qDebug() << "Node not found";

    }
    this->setPlainText(TextBoxText);
}

//Delete beams
void JBEAM_TextBox::JBEAM_DeleteBeams()
{
    int pos1; //Position containers for parsing the textbox
    int pos2;
    int pos3;

    QString TextBoxText = this->toPlainText();
    for(int i=0; i<CurrentNodeBeam->JBEAM_temp.delete_beams.size();i++)
    {
        QString Beam = "\"" + CurrentNodeBeam->JBEAM_temp.delete_beams.at(i).Node1Name + "\"";
        Beam += "\"" + CurrentNodeBeam->JBEAM_temp.delete_beams.at(i).Node2Name + "\"";

        //Find node from textbox by nodename
        if(FindBeamContainer(&TextBoxText, Beam, pos1, pos2, true, pos3))
        {
            QString beamline = "";

            TextBoxText.replace(pos1,pos3-pos1+2,beamline);
        }
        //else qDebug() << "Beam not found";

    }
    this->setPlainText(TextBoxText);
}

/* Add beams and nodes from temp */
void JBEAM_TextBox::JBEAM_AddFromTemp()
{
    //Add all nodes from TEMP
    for(int i=0; i<CurrentNodeBeam->JBEAM_temp.nodes.size();i++)
    {
        for(int i2=0; i2<CurrentNodeBeam->JBEAM_temp.nodes.at(i).size();i2++)
        {
            CurrentNodeBeam->TempNode = CurrentNodeBeam->JBEAM_temp.nodes.at(i).at(i2);
            JBEAM_AddNode();
        }
    }

    //Add all beams from TEMP
    for(int i=0; i<CurrentNodeBeam->JBEAM_temp.beams.size();i++)
    {
        for(int i2=0; i2<CurrentNodeBeam->JBEAM_temp.beams.at(i).size();i2++)
        {
            CurrentNodeBeam->TempBeam = CurrentNodeBeam->JBEAM_temp.beams.at(i).at(i2);
            JBEAM_AddBeam();
        }
    }
    CurrentNodeBeam->JBEAM_temp.clear();
}

void JBEAM_TextBox::ResetContents()
{
    FileName = "untitled";
    //Default empty file template
    this->setPlainText(EmptyJbeamTextTemplate);
}

void JBEAM_TextBox::SetFilePath(QString FileName)
{
    this->FullFilePath = FileName;
    int i=0;
    for(i=FileName.length()-1;i>0;i--)
    {
        if(FileName.at(i) == '/') break;
    }
    this->FileName = FileName.right(FileName.length()-i-1);
    qDebug() << "Tiedostonimi on " << this->FileName;
}
