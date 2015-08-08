#include <QtWidgets>
#include <QtOpenGL>
#include <QTimerEvent>
#include <QPixmap>
#include <QJsonDocument>
#include <QLabel>
#include <QStringBuilder>

#include "mainwindow.h"
#include "jbeamtextbox.h"

/* ==== JBEAM TEXT EDIT ==== */
JBEAM_TextBox::JBEAM_TextBox(QWidget *parent, NodeBeam *nb) : QPlainTextEdit(parent)
{
    LineNumbersA = new LineNumbers(this);
    setViewportMargins(26,0,0,0);

    //Connect linenumbers update request
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumbers(QRect,int)));
    connect(this, SIGNAL(textChanged()), this, SLOT(textchanged()));

    synhigh = new Highlighter(this->document());

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

    QFont fontti;
    fontti.setFamily(tr("Verdana"));
    this->setFont(fontti);

    for(int i=0; i<CurrentNodeBeam->ListTypes.size();i++)
    {
        this->JBEAM_curorNames.append(CurrentNodeBeam->ListTypes.at(i).keyword);
    }
}

/* Line numbers */
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

    JBEAM_cursorLocs.clear();
    for(int i=0; i<JBEAM_curorNames.size(); i++)
    {
        QString search = "//BNE" + JBEAM_curorNames.at(i);
        int tempcursor = JBEAM_text.indexOf(search);
        if(tempcursor>-1)
        {
            for(int i=tempcursor; i>0;i--)
            {
                if(JBEAM_text.at(i) == '\n')
                {
                    tempcursor = i;
                    break;
                }
                else tempcursor = -1;
            }
        }
        this->JBEAM_cursorLocs.append(tempcursor);
    }
}

/* Update contents */
//Update all nodes
void JBEAM_TextBox::JBEAM_UpdateAllNodes(bool *updated)
{
    int pos1, length; //Position containers for parsing the textbox
    int nodesBegin, nodesEnd = 0;

    QString TextBoxText = this->toPlainText();

    while(FindNodesSection(&TextBoxText, nodesEnd, nodesBegin, nodesEnd))
    {
        QString nodesText = TextBoxText.left(nodesEnd);
        QString otherText = TextBoxText.right(TextBoxText.length()-nodesEnd);

        for(int i=0; i<CurrentNodeBeam->Nodes.size();++i)
        {
            if(!(*(updated + i)))
            {
                QString NodeName = CurrentNodeBeam->Nodes.at(i).NodeName;

                //Find node from textbox by nodename
                if(FindNodeContainer2(&nodesText, NodeName, nodesBegin, pos1, length))
                {
                    QString nodeline("\"" + NodeName + "\", ");
                    nodeline+= QString::number(CurrentNodeBeam->Nodes.at(i).locX,'f',5);
                    nodeline+= ", " + QString::number(CurrentNodeBeam->Nodes.at(i).locY,'f',5);
                    nodeline+= ", " + QString::number(CurrentNodeBeam->Nodes.at(i).locZ,'f',5);
                    nodesText.replace(pos1,length,nodeline);
                    *(updated + i) = true;
                }
            }
        }
        TextBoxText = nodesText + otherText;
    }
    this->setPlainText(TextBoxText);
}

/* Update currently selected node information into the text box */
void JBEAM_TextBox::JBEAM_UpdateSelectedNodes()
{
    int pos1, length; //Position containers for parsing the textbox
    bool updated[CurrentNodeBeam->Nodes.size()] = { false };
    int nodesBegin, nodesEnd = 0;

    QString TextBoxText = this->toPlainText();
    FindNodesSection(&TextBoxText,0, nodesBegin, nodesEnd);
    QString nodesText = TextBoxText.left(nodesEnd);
    QString otherText = TextBoxText.right(TextBoxText.length()-nodesEnd);

    for(int i=0; i<CurrentNodeBeam->SelectedNodes.size();++i)
    {
        if(!updated[i])
        {
            QString NodeName = CurrentNodeBeam->Nodes.at(CurrentNodeBeam->SelectedNodes.at(i)).NodeName;

            //Find node from textbox by nodename
            if(FindNodeContainer2(&nodesText, NodeName, nodesBegin, pos1, length))
            {
                QString nodeline("\"" + NodeName + "\", ");
                nodeline+= QString::number(CurrentNodeBeam->Nodes.at(CurrentNodeBeam->SelectedNodes.at(i)).locX,'f',5);
                nodeline+= ", " + QString::number(CurrentNodeBeam->Nodes.at(CurrentNodeBeam->SelectedNodes.at(i)).locY,'f',5);
                nodeline+= ", " + QString::number(CurrentNodeBeam->Nodes.at(CurrentNodeBeam->SelectedNodes.at(i)).locZ,'f',5);
                nodesText.replace(pos1,length,nodeline);
                updated[i] = true;
            }
        }
    }

    TextBoxText = nodesText + otherText;
    this->setPlainText(TextBoxText);
}

/* Find containers */
/* This function will find, where the node and it's 3 coordinates are in the JBEAM string.
Results will be returned in NodeBegin and NodeEnd integer values.
Bool will be false, if node was not found.*/
//old, not in use
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

/* Find the nodes section */
bool JBEAM_TextBox::FindNodesSection(QString *JBEAM_box, int offset, int &nodesBegin, int &nodesEnd)
{
    QRegExp re_nodes("\\\"nodes\\\"\\s*:\\s*\\[");
    int i = re_nodes.indexIn(*JBEAM_box, offset);
    if(i <0) return false;

    nodesBegin = i;

    int level = -1;
    for(;i<JBEAM_box->length();++i)
    {
        if(JBEAM_box->at(i) == '[') ++level;
        else if(JBEAM_box->at(i) == ']')
        {
             --level;
             if(level<0) break;
        }
    }
    nodesEnd = i;
    return true;
}

/* Find a node from the nodes section */
bool JBEAM_TextBox::FindNodeContainer2(QString *JBEAM_box, QString nodename, int &nodesBegin, int &pos1, int &pos2)
{
    QString re = "\\[\\s*\\\"" + nodename + "\\\"\\s*,?\\s*-?\\d*.\\d*\\s*,?\\s*-?\\d*.\\d*\\s*,?\\s*-?\\d*.\\d*";
    QRegExp lost(re);
    pos1 = lost.indexIn(*JBEAM_box,nodesBegin) + 1;
    if(pos1>=1)
    {
        pos2 = lost.matchedLength()-1;
        return true;
    }
    return false;
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
    QRegExp re_container(pattern);

    Begin = JBEAM_box.indexOf(re_container);
    int i=Begin;
    for(; i<JBEAM_box.length(); ++i)
    {
        if(JBEAM_box.at(i) == '[') break;
    }
    Begin = i;
    int level = 1;
    ++i;
    for(;i<JBEAM_box.length();++i)
    {
        if(JBEAM_box.at(i) == '[') ++level;
        else if(JBEAM_box.at(i) == ']')
        {
             --level;
             if(level<1) break;
        }
    }
    End = i;
    return true;
}

/* Add node at text cursor position */
void JBEAM_TextBox::JBEAM_AddNode()
{
    int node_id = CurrentNodeBeam->TempNode.GlobalID;
    QString nodeline;
    this->str_addIndent(&nodeline, 3);
    nodeline+= "[\"" + CurrentNodeBeam->TempNode.NodeName + '"';
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
    QString beamline;
    this->str_addIndent(&beamline, 3);
    beamline+= "[\"" + CurrentNodeBeam->TempBeam.Node1Name + '"';
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

/* Add other part, in JBEAM edit widget at text cursor position */
void JBEAM_TextBox::JBEAM_AddArrayItem(int ListType_id)
{
    QVector <int> pickednodes = CurrentNodeBeam->ListTypes.at(ListType_id).contaier.last();
    QString line;
    this->str_addIndent(&line,3);
    line+= "[" + CurrentNodeBeam->ListTypes.at(ListType_id).JBEAM_template;
    line+=("],\n");

    for(int i=0; i<CurrentNodeBeam->ListTypes.at(ListType_id).nodeamount;i++)
    {
        line = line.arg(CurrentNodeBeam->Nodes.at(pickednodes.at(i)).NodeName);
    }

    QTextCursor textcursor = this->textCursor();

    if(JBEAM_cursorLocs.at(ListType_id)>-1) textcursor.setPosition(JBEAM_cursorLocs.at(ListType_id));

    textcursor.insertText(line);
    this->setTextCursor(textcursor);
}


/* Write the name of currently picked node at textcursor position */
void JBEAM_TextBox::PrintNodePicked(int node_id)
{
    QString line = "\"%1\", ";
    line = line.arg(CurrentNodeBeam->Nodes.at(node_id).NodeName);

    QTextCursor textcursor = this->textCursor();

    textcursor.insertText(line);
    this->setTextCursor(textcursor);
}

/* Delete nodes that are in temp delete queue */
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

/* Delete beams that are in temp delete queue */
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
    }
    this->setPlainText(TextBoxText);
}

/* Add beams and nodes from temp queue */
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

/* Reset textbox contents */
void JBEAM_TextBox::ResetContents()
{
    FileName = "untitled";
    //Default empty file template
    this->setPlainText(EmptyJbeamTextTemplate);
}

/* Set the filepath of current text file */
void JBEAM_TextBox::SetFilePath(QString FileName)
{
    this->FullFilePath = FileName;
    int i=0;
    for(i=FileName.length()-1;i>0;i--)
    {
        if(FileName.at(i) == '/') break;
    }
    this->FileName = FileName.right(FileName.length()-i-1);
    //qDebug() << "Tiedostonimi on " << this->FileName;
}

/* Cut nodelines */
QStringList JBEAM_TextBox::JBEAM_CutNodeLines()
{
    QStringList nodelines;

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
            nodelines.append(TextBoxText.mid(pos1-1,pos3-pos1+2));
            TextBoxText.replace(pos1-1,pos3-pos1+2,nodeline);
        }
    }
    this->setPlainText(TextBoxText);

    return nodelines;
}

/* Paste text at cursor position */
void JBEAM_TextBox::JBEAM_PasteNodeLine(QString nodeStr)
{
    QString nodeline;
    this->str_addIndent(&nodeline, 3);
    nodeline += nodeStr + "\n";

    QTextCursor textcursor = this->textCursor();
    if(JBEAM_NodeCursor >= 0)
    {
        textcursor.setPosition(JBEAM_NodeCursor);
    }
    textcursor.insertText(nodeline);
    this->setTextCursor(textcursor);
}


//===============================================
//----- JBEAM Pretty printing
//===============================================

// Add indentation
void JBEAM_TextBox::str_addIndent(QString * text, int indent)
{
    for(int i=0; i<indent; ++i)
    {
        text->append("    ");
    }
}
// Add string value
void JBEAM_TextBox::str_addValueStr(QString * text, QString key, QString value, int indent)
{
    this->str_addIndent(text, indent);
    *text+= "\"" + key + "\":\"" + value + "\",\n";
}
// Begin new object
void JBEAM_TextBox::str_beginObject(QString * text, QString key, int indent)
{
    this->str_addIndent(text, indent);
    *text+= "\"" + key + "\":{\n";
}
// End object
void JBEAM_TextBox::str_endObject(QString * text, int indent)
{
    this->str_addIndent(text, indent);
    *text+= "},\n";
}
// Begin list
void JBEAM_TextBox::str_beginList(QString * text, QString key, int indent)
{
    this->str_addIndent(text, indent);
    *text+= "\"" + key + "\":[\n";
}
// End list
void JBEAM_TextBox::str_endList(QString * text, int indent)
{
    this->str_addIndent(text, indent);
    *text+= "],\n";
}
// Add comment
int JBEAM_TextBox::JBEAM_AddComment(int CursorPos, QString Comment)
{
    QString commentline;
    this->str_addIndent(&commentline, 3);
    commentline+= "//" + Comment;
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



//===============================================
//----- SYNTAX HIGHLIGHTER
//===============================================

Highlighter::Highlighter(QTextDocument * document) :
    QSyntaxHighlighter(document)
{

    QTextCharFormat strformat;
    strformat.setForeground(QColor(40,40,255));

    HighlightingRule temprule;
    temprule.pattern = QRegExp("\"[^\\\"]*\"");
    temprule.format = strformat;
    rules.append(temprule);


    strformat.setForeground(QColor(220,40,40));
    strformat.setFontWeight(QFont::Bold);

    temprule.pattern = QRegExp("\\[");
    temprule.format = strformat;
    rules.append(temprule);
    temprule.pattern = QRegExp("\\]");
    rules.append(temprule);

    QStringList keywords;
    /*
    keywords << "\"nodes\"" << "\"beams\"" << "\"triangles\"" << "\"information\""
             << "\"authors\"" << "\"hubWheels\"" << "\"pressureWheels\"" << "\"refNodes\""
             << "\"slots\"" << "\"slotType\"" << "\"name\"" << "\"slots\"" << "\"flexbodies\""
             << "\"scalebeamSpring\"" << "\"scalebeamDamp\"" << "\"scalebeamDeform\""
             << "\"scalebeamStrength\"" << "\"breakGroup\"" << "\"deformGroup\"" << "\"disableMeshBreaking\""
             << "\"beamDeform\"" << "\"beamStrength\""<< "\"beamSpring\"" << "\"beamDamp\"" << "\"beamPrecompression\"";*/

    keywords << "\\\"[^\\\"]*\\\"\\s*:";

    strformat.setForeground(QColor(200,40,40));
    strformat.setFontWeight(QFont::Bold);

    foreach(const QString keyword, keywords)
    {
        temprule.pattern = QRegExp(keyword);
        temprule.pattern.setMinimal(true);
        temprule.format = strformat;
        rules.append(temprule);
    }

    QTextCharFormat commentformat;
    commentformat.setForeground(QColor(0,140,0));
    commentformat.setFontItalic(1);

    //Comment highlight
    temprule.pattern = QRegExp("\/\/.*");
    temprule.format = commentformat;
    rules.append(temprule);

    QStringList BNEkeywords;
    BNEkeywords << "//BNE.*";
    strformat.setFontWeight(QFont::Bold);
    strformat.setForeground(QColor(40,100,40));
    strformat.setBackground(QColor(220,220,220));

    foreach(const QString keyword, BNEkeywords)
    {
        temprule.pattern = QRegExp(keyword);
        temprule.format = strformat;
        rules.append(temprule);
    }


}

Highlighter::~Highlighter()
{



}

void Highlighter::highlightBlock(const QString &text)
{
    foreach(const HighlightingRule &rule, rules)
    {
        QRegExp regex = rule.pattern;
        int index = regex.indexIn(text);
        while(index >= 0)
        {
            setFormat(index,regex.matchedLength(),rule.format);
            index = regex.indexIn(text,index+regex.matchedLength());
        }
    }
}
