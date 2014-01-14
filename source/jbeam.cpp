#include "jbeam.h"
#include <QString>
#include <QFile>
#include <QVector>
#include <QTextStream>
#include <QDebug>
#include <QStack>
#include <QApplication>


JBeam::JBeam(const QString &fileName)
{

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Failure";
    }
    else
    {

        QTextStream Stream(&file);
        in = &Stream;

        #ifndef QT_NO_CURSOR
            QApplication::setOverrideCursor(Qt::WaitCursor);
        #endif


        CurrentPos = &JBeamFile;
        CurrentPos->key = "ROOT_NODE";
        CurrentPos->parent = &JBeamFile;
        string = "";
        object_amount=0;
        CurrentSlot=0;
        UnsavedComments=0;

        OpenFile();
        #ifndef QT_NO_CURSOR
            QApplication::restoreOverrideCursor();
        #endif
        file.close();


    }
}

JBeam::~JBeam()
{
    CurrentPos = NULL;
    JBeamFile.Delete();
}

void JBeam::NewObject()
{
    if(CurrentPos->token == LIST)
    {
        //qDebug() << "New OBJECT";
        CurrentPos->NewLeftChild();
        CurrentPos = CurrentPos->childL;
        CurrentPos->objectparent=1;
        FindEndOfList();
        CurrentPos->token = KEY;
        object_amount++;
    }
    else
    {
        //qDebug() << "New OBJECT";
        CurrentPos->NewLeftChild();
        CurrentPos = CurrentPos->childL;
        CurrentPos->objectparent=1;
        CurrentPos->token = KEY;
        object_amount++;
    }

}
void JBeam::EndObject()
{
    FindParent();
    if(CurrentPos->token == VALUE)
    {
        CurrentPos->contenttype = OBJECT;
        CurrentPos->token = END;
    }
    //qDebug() << "END OBJECT " << CurrentPos->value << CurrentPos->token;
}

void JBeam::HandleString()
{
    if(CurrentPos->token == LIST)
    {
        CurrentPos->NewLeftChild();
        CurrentPos = CurrentPos->childL;
        FindEndOfList();
        object_amount++;
        CurrentPos->value = string;
        CurrentPos->token = LIST;
        CurrentPos->contenttype = LIST_STR;
        //qDebug() << "writing " << string << " " << CurrentPos->parent->value;
        SaveComments();
        FindParent();
    }
    else if(CurrentPos->token == VALUE)
    {
        //qDebug() << "value is " << string;
        CurrentPos->token = END;
        CurrentPos->value = string;
        CurrentPos->contenttype = STRING;
    }
    else if(CurrentPos->token == END)
    {
        //qDebug() << "same level";
        CurrentPos->NewRightChild();
        CurrentPos = CurrentPos->childR;
        CurrentPos->token = KEY;
        CurrentPos->key = string;
        object_amount++;
        SaveComments();
    }
}

void JBeam::NewList()
{
    CurrentPos->NewLeftChild();
    CurrentPos = CurrentPos->childL;
    FindEndOfList();
    CurrentPos->token = LIST;
    CurrentPos->contenttype = LIST_CONTAINER;
    CurrentPos->value = QString::number(object_amount);
    object_amount++;
    SaveComments();

    //qDebug() << "New list " << CurrentPos->value << " parent is " << CurrentPos->parent->value;

    //for debug

}

void JBeam::OpenFile()
{
qDebug()<<"starting to read file";
    while(!in->atEnd())
    {
        cha = in->read(1);

        if(cha[0] == '{')
        {
            //NEW OBJECT
            NewObject();
        }
        else if(cha[0] == '}')
        {
            //OBJECT ENDS
            EndObject();
        }
        else if(cha[0] == '"')
        {
            ReadString();
            HandleString();
        }
        else if(cha[0] == '[')
        {
            NewList();
        }
        else if(cha[0] == ']')
        {

            FindParent();

            //qDebug() << "end list " << CurrentPos->key << " " << CurrentPos->value;
            if(CurrentPos->token == VALUE)
            {
                CurrentPos->contenttype = ARRAY;
                CurrentPos->token = END;
            }
        }
        else if(cha[0] == ':')
        {
            if(CurrentPos->token == KEY)
            {
                //qDebug() << "Key is " << string;
                CurrentPos->key = string;
                CurrentPos->token = VALUE;
            }
            else qDebug() << "FAILURE";
        }
        else if(cha[0] == ' ')
        {

        }
        else if(cha[0] == ',')
        {

        }
        else if(cha[0] == '/')
        {
            ReadComment();
        }
        else
        {
            bool ok;
            cha.toInt(&ok);
            if(ok)
            {
                ReadNumber();
                HandleNumber();
            }
            else if(cha[0]=='-')
            {
                cha = in->read(1);
                cha.toInt(&ok);
                if(ok)
                {
                    ReadNumber();

                    string.insert(0,"-");
                    HandleNumber();
                }
            }
            else
            {
                int boolc = CheckBoolean();
                if(CurrentPos->token == VALUE)
                {
                    if(boolc>0)
                    {
                        if(boolc==1)
                        {
                            CurrentPos->contenttype = BOOLVAL;
                            CurrentPos->value = "true";
                            CurrentPos->token = END;
                        }
                        else if(boolc==2)
                        {
                            CurrentPos->contenttype = BOOLVAL;
                            CurrentPos->value = "false";
                            CurrentPos->token = END;
                        }
                    }
                }
            }
        }
    }
}

void JBeam::HandleNumber()
{
    if(CurrentPos->token == VALUE)
    {
        //qDebug()<< "writing to " << CurrentPos->key << " , " << string;
        CurrentPos->value = string;
        CurrentPos->contenttype = NUMBER;
        CurrentPos->token = END;

    }
    else if(CurrentPos->token == LIST)
    {
        CurrentPos->NewLeftChild();
        CurrentPos = CurrentPos->childL;
        FindEndOfList();
        object_amount++;
        CurrentPos->value = string;
        CurrentPos->token = LIST;
        CurrentPos->contenttype = LIST_NUMBER;
        SaveComments();
        FindParent();
    }
}

int JBeam::CheckBoolean()
{
    int result = 0;
    if(cha[0] == 't')
    {
        cha.append(in->read(3));
        int x = QString::compare(cha, "true", Qt::CaseInsensitive);
        if(x==0) result = 1;
    }
    else if(cha[0] == 'f')
    {
        cha.append(in->read(4));
        int x = QString::compare(cha, "false", Qt::CaseInsensitive);
        if(x==0) result = 2;
    }
    return result;
}

void JBeam::ReadString()
{
    bool ok=1;
    string.clear();
    while(ok)
    {
        cha = in->read(1);
        if(cha[0] == '"')
        {
            ok = !ok;
            break;
        }
        string.append(cha);
    }
    //qDebug() << string;
}

void JBeam::ReadComment()
{
    cha = in->read(1);
    if(cha[0] == '/')
    {
        QString comment;
        UnsavedComments=1;
        comment = in->readLine();
        comments.append(comment);
    }
}

void JBeam::ReadNumber()
{
    bool ok = 1;
    bool ok2 = 1;
    QString numbert;
    numbert.append(cha);
    while(ok)
    {
        cha = in->read(1);
        cha.toInt(&ok2);
        if(cha[0] != '.')
        {
             if(!ok2) break;

        }
        numbert.append(cha);
    }
    in->seek(in->pos()-1);
    string = numbert;
    //qDebug () << "Number is "<< numbert.toFloat();
}

void JBeam::FindParent()
{
    bool ok=1;
    while(ok)
    {
        if(CurrentPos == CurrentPos->parent->childR)
        {
            CurrentPos = CurrentPos->parent;
        }
        else
        {
            CurrentPos = CurrentPos->parent;
            ok = 0;
            break;
        }
    }
}

void JBeam::PrintTree()
{
    CurrentPos = &JBeamFile;
    qDebug() << "tree size is " << object_amount;
    for(int i=0; i< object_amount; i++)
    {
        GoToNext();
        if(CurrentPos->iterated == 1)
        {
            i--;
        }
        else
        {
            qDebug() << CurrentPos->key << " , " << CurrentPos->value;
        }
    }
}

void JBeam::WriteFile(const QString &fileName)
{
    /* Try and open a file for output */
    QString suffixi = fileName.right(6);
    QString FileName1 = fileName;
    if(QString::compare(suffixi,".jbeam",Qt::CaseInsensitive) != 0) FileName1.append(".jbeam");

    QFile outputFile(FileName1);
    outputFile.open(QIODevice::WriteOnly);

    /* Check it opened OK */
    if(!outputFile.isOpen()){
        //qDebug() << "- Error, unable to open" << fileName << "for output";

    }
    else
    {
        /* Point a QTextStream object at the file */
        QTextStream outStream(&outputFile);

        CurrentPos = &JBeamFile;

        //qDebug() << "saving file ";
        QStack <QString> pino;
        CurrentPos = &JBeamFile;
        int indentation=0;
        QString indent;

        outStream << "{\r\n";
        pino.push("}");

        for(int i=0; i< object_amount; i++)
        {
            int direction = GoToNext();
            if(direction == 3)
            {
                outStream  << pino.pop();
                indentation = indentation - 5;
                indent = WriteFileIndent(indentation);
                if(CurrentPos->childR != NULL) outStream << ", ";
            }
//            if(CurrentPos->iterated == 1)
//            {
//                i--;
//            }
            else
            {
                if(CurrentPos->token == LIST)
                {
                    if((CurrentPos->parent->token != LIST) && (CurrentPos == CurrentPos->parent->childR))
                    {
                        outStream << "\r\n";
                        outStream << indent << "}";
                        indentation = indentation - 5;
                        indent = WriteFileIndent(indentation);
                    }
                    if(CurrentPos->contenttype == LIST_STR)
                    {
                        outStream << '"' << CurrentPos->value << '"';
                        if(CurrentPos->childR != NULL) outStream << ", ";
                    }
                    else if(CurrentPos->contenttype == LIST_NUMBER)
                    {
                        outStream << CurrentPos->value;
                        if(CurrentPos->childR != NULL) outStream << ", ";
                    }
                    else if(CurrentPos->contenttype == LIST_CONTAINER)
                    {
                        outStream << "\r\n";
                        indentation = indentation + 5;
                        indent = WriteFileIndent(indentation);
                        if(CurrentPos->parent->contenttype != ARRAY)
                        {
                            outStream << indent << "[";
                            pino.push("]");
                        }
                        else
                        {
                            outStream << indent << "";
                            pino.push("");
                        }
                    }
                }
                else
                {
                    outStream << "\r\n";
                    if(CurrentPos->parent->token == LIST)
                    {
                        indentation = indentation + 5;
                        indent = WriteFileIndent(indentation);
                        outStream << indent << "{";
                        outStream << "\r\n";
                    }
                    if(CurrentPos->contenttype == OBJECT)
                    {
                        indentation = indentation + 5;
                        indent = WriteFileIndent(indentation);
                        outStream << indent << '"' << CurrentPos->key << '"' << ": {";
                        pino.push("}");
                    }
                    else if(CurrentPos->contenttype == NUMBER)
                    {
                        outStream << indent << '"' << CurrentPos->key << '"' << " : " << CurrentPos->value;
                        if(CurrentPos->childR != NULL) outStream << ", ";
                    }
                    else if(CurrentPos->contenttype == STRING)
                    {
                        outStream << indent << '"' << CurrentPos->key << '"' << " : " << '"' << CurrentPos->value << '"';
                        if(CurrentPos->childR != NULL) outStream << ", ";
                    }
                    else if(CurrentPos->contenttype == BOOLVAL)
                    {
                        outStream << indent << '"' << CurrentPos->key << '"' << " : " << CurrentPos->value;
                        if(CurrentPos->childR != NULL) outStream << ", ";
                    }
                    else if(CurrentPos->contenttype == ARRAY)
                    {
                        indentation = indentation + 5;
                        indent = WriteFileIndent(indentation);
                        outStream << indent << '"' << CurrentPos->key << '"' << ": [";
                        pino.push("]");
                    }

                    if(CurrentPos->actualparent->token == LIST)
                    {
                        if(CurrentPos->childR == NULL)
                        {
                            outStream << "\r\n";
                            outStream << indent << "}";
                            indentation = indentation - 5;
                            indent = WriteFileIndent(indentation);
                            outStream << "\r\n";
                        }
                    }
                }
            }
        }
        while(pino.size()>0)
        {
            outStream << "\r\n";
            outStream << indent << pino.pop();
            indentation = indentation - 5;
            indent = WriteFileIndent(indentation);
        }
        outputFile.close();
    }
}

QString JBeam::WriteFileIndent(int indentsize)
{
    QString indent;
    for(int i=0; i<indentsize; i++)
    {
        indent.append(" ");
    }
    return indent;
}

int JBeam::GoToNext()
{
    int dir = 0;
    CurrentPos->iterated=1;
    if((CurrentPos->childL != NULL) && (CurrentPos->childL->iterated==0))
    {
        CurrentPos = CurrentPos->childL;
        dir=1;
    }
    else if((CurrentPos->childR != NULL) && (CurrentPos->childR->iterated==0))
    {
        CurrentPos = CurrentPos->childR;
        dir=2;
    }
    else if(CurrentPos->parent != CurrentPos)
    {
        CurrentPos = CurrentPos->actualparent;
        dir=3;
    }
    return dir;
}

int JBeam::GoToNextUntilStop()
{
    int dir;
    dir = GoToNext();
    if(CurrentPos->stophere == 1)
    {
        CurrentPos->stophere=0;
        dir = 4;
    }
    return dir;
}

bool JBeam::SetStopPointHere()
{
    bool ok = 0;
    if(CurrentPos->childL != NULL)
    {
        CurrentPos->stophere=1;
        ok = 1;
    }
    return ok;
}

int JBeam::GoToNextUntilStop_List()
{
    int dir;
    dir = GoToNext();
    if(CurrentPos->stophere == 1)
    {
        CurrentPos->stophere=0;
        GoToNext();
        if(CurrentPos->stophere == 1)
        {
            CurrentPos->stophere=0;
        }
        dir = 4;
    }
    return dir;
}

bool JBeam::SetStopPointHere_List()
{
    bool ok = 0;
    if(CurrentPos->childL != NULL)
    {
        CurrentPos->stophere=1;
        ok = 1;

        CurrentPos=CurrentPos->childL;
        CurrentPos->stophere=1;
    }
    return ok;
}

void JBeam::PrintTree3()
{
    QStack <QString> pino;
    CurrentPos = &JBeamFile;
    qDebug() << "tree size is " << object_amount;
    for(int i=0; i< object_amount; i++)
    {
        int direction = GoToNext();
        if(direction == 3)
        {
            qDebug() << pino.pop();
        }
        if(CurrentPos->iterated == 1)
        {
            i--;
        }
        else
        {
            if(CurrentPos->token == LIST)
            {
                if((CurrentPos->parent->token != LIST) && (CurrentPos == CurrentPos->parent->childR))
                {
                    qDebug() << "LISTA ALKAA TAAS PRKL";
                }
                if(CurrentPos->contenttype == LIST_STR)
                {
                    qDebug() << CurrentPos->value;
                }
                else if(CurrentPos->contenttype == LIST_NUMBER)
                {
                    qDebug() << CurrentPos->value;
                }
                else if(CurrentPos->contenttype == LIST_CONTAINER)
                {
                    qDebug() << "[";
                    pino.push("]");
                }
            }
            else
            {
                if(CurrentPos->parent->token == LIST) qDebug() << "LISTASSA";
                if(CurrentPos->contenttype == OBJECT)
                {
                    qDebug() << CurrentPos->key << ": {";
                    pino.push("},");
                }
                else if(CurrentPos->contenttype == NUMBER)
                {
                    qDebug() << CurrentPos->key << " : " << CurrentPos->value;
                                    }
                else if(CurrentPos->contenttype == STRING)
                {
                    qDebug() << CurrentPos->key << " : " << CurrentPos->value;
                }
                else if(CurrentPos->contenttype == ARRAY)
                {
                    qDebug() << CurrentPos->key << ": ";
                    //pino.push("]");
                }
            }

        }
    }
    while(pino.size()>0)
    {
        qDebug() << pino.pop();
    }

}

void JBeam::FindEndOfList()
{
    while(CurrentPos->childR != NULL)
    {
        CurrentPos = CurrentPos->childR;
    }
}

bool JBeam::FindObjectByKey(QString key)
{
    bool ok=1;
    for(int i=0; i<object_amount;i++)
    {
        if(CurrentPos->iterated)
        {
            i--;
        }
        qDebug()<<"is it "<<CurrentPos->key;
        if(CurrentPos->key == key) break;
        GoToNext();
    }
}

void JBeam::InsertObject()
{
    newobject = new jbeam_object;
    newobject->parent = CurrentPos->parent;
    newobject->childR = CurrentPos;
    CurrentPos->parent->childR = newobject;
    CurrentPos->parent = newobject;
    CurrentPos = newobject;
}

void JBeam::CurrPosReset()
{
    CurrentPos = &JBeamFile;
}

int JBeam::GoToNextRight()
{
    int dir = 0;
    if(CurrentPos->childR != NULL)
    {
        CurrentPos = CurrentPos->childR;
        dir=1;
    }
    return dir;
}

void JBeam::ResetIterated()
{
    CurrPosReset();
    CurrentPos->ResetIterated();
}

//BeamNG slot
void JBeam::ResetSlot()
{

}

bool JBeam::GoToNextSlot()
{
    int ok;
    CurrPosReset();
    ResetIterated();
    GoToNext();
    for(int i=0; i<CurrentSlot; i++)
    {
        GoToNextRight();
    }
    if(CurrentPos->childR!=NULL)
    {
        CurrentSlot++;
    }
    else ok=0;
    return ok;
}

void JBeam::SaveComments()
{
    if(UnsavedComments)
    {
        CurrentPos->comments = comments;
        comments.clear();
        UnsavedComments=0;
    }
}
