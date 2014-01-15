#include <QString>
#include <QVector>
#include <QApplication>
#include <qmath.h>

#include <QGLWidget>
#include <QMatrix4x4>
#include <QVector3D>
#include <QFile>
#include <QXmlStreamWriter>

#include <jbeam.h>
#include "NodeBeam.h"

//NB Editoria varten
static const qreal node_size = 0.10;
static const int num_divisions = 32;


NodeBeam::NodeBeam()
{
    VehicleName = "Newvehicle";
    VehicleInGameName = "New vehicle";
    VehicleAuthors.resize(1);
    VehicleAuthors[0] = "Editor";
    green=0;
    blue=0;
    ActiveNodeGroup=-1;
    ActiveBeamGroup=-1;

    DefaultPrefixCalculator=0;

}

NodeBeam::~NodeBeam(void)
{

}

/* Rigs of rods importer */
void NodeBeam::ImportNBFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//        QMessageBox::warning(this, tr("Application"),
//                             tr("Cannot read file %1:\n%2.")
//                             .arg(fileName)
//                             .arg(file.errorString()));
//        return;
    }

    QTextStream in(&file);

    #ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif

    int ParsingMode = 0;

    QString CurrentLine = in.readLine();
    VehicleName = CurrentLine;

    while(!in.atEnd())
    {
        CurrentLine = in.readLine();

        if(CurrentLine == "globals")
        {
            ParsingMode = 4;
        }

        else if(CurrentLine == "description")
        {
            ParsingMode = 5;
        }

        else if(CurrentLine == "end_description")
        {
            ParsingMode = 6;
        }

        else if(CurrentLine == "engine")
        {
            ParsingMode = 7;
        }

        else if(CurrentLine == "engoption")
        {
            ParsingMode = 8;
        }

        else if(CurrentLine == "torquecurve")
        {
            ParsingMode = 9;
        }
        else if(CurrentLine == "minimass")
        {
            ParsingMode = 10;
        }
        else if(CurrentLine == "brakes")
        {
            ParsingMode = 11;
        }
        else if(CurrentLine == "enable_advanced_deformation")
        {
            ParsingMode = 12;
        }
        else if(CurrentLine == "managedmaterials")
        {
            ParsingMode = 13;
        }
        else if(CurrentLine == "cameras")
        {
            ParsingMode = 14;
        }
        else if(CurrentLine == "cinecam")
        {
            ParsingMode = 15;
        }

        //NODES
        else if(CurrentLine == "nodes")
        {
            ParsingMode = 1;
        }
        else if(CurrentLine == "beams")
        {
            ParsingMode = 2;

        }
        else if(CurrentLine == "nodes2")
        {
            ParsingMode = 3;
        }
        else if (CurrentLine.length() == 0);
        else
        {
            bool IsComment=1;
            for(int i5=0; i5<CurrentLine.length(); i5++)
            {
                if(CurrentLine[i5] == ' ');
                else if(CurrentLine[i5] == ';')
                {
                    if(CurrentLine.indexOf(";grp:") != -1)
                    {
                        IsComment=0;
                    }
                    break;
                }
                else
                {
                    IsComment=0;
                    break;
                }
            }
            if(IsComment==0) ParseLine(CurrentLine, ParsingMode);
        }

    }
    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
    #endif

    file.close();

}

/* Rigs of Rods import, parse line
Takes as arguments the line, and integer that represents
the section, nodes, beams, etc. */
void NodeBeam::ParseLine(QString line, int ParsingMode)
{

    QString temp;
    if(ParsingMode==1)
    {
        //ParsingNodes
        qDebug() << "Parsing nodes ...";
        if(line.indexOf(";grp:",0)==0)
        {
            NodeGroups.resize(NodeGroups.size()+1);
            NodeGroups[NodeGroups.size()-1].NodeGroupID = NodeGroups.size()-1;
            NodeGroups[NodeGroups.size()-1].NodeGroupName = line.right(line.length()-5);
            NodeGroups[NodeGroups.size()-1].draw=1;
            qDebug()<<"New node group added: " << NodeGroups[NodeGroups.size()-1].NodeGroupName << ", " << NodeGroups[NodeGroups.size()-1].NodeGroupID;
        }
        else if(line.indexOf("set_node_defaults")!=-1);
        else
        {
            grid_index = 0;
            temp = "";
            for(int index=0; index<line.length(); index++)
            {
                if(line[index]== QChar(';'))
                {
                    //Comment starts
                    break;
                }

                else if(line[index] == QChar(' '));
                else if(line[index] == QChar(','))
                {
                    if (grid_index == 0)
                    {
                        //New nodes system
                        Nodes.resize(Nodes.size()+1);
                        Nodes[(Nodes.size()-1)].GlobalID = (Nodes.size()-1);
                        Nodes[(Nodes.size()-1)].NodeName = temp;

                        Nodes[(Nodes.size()-1)].RoRNodeType = 0;

                        //If node group exists, add in that
                        if(NodeGroups.size() > 0)
                        {
                            Nodes[(Nodes.size()-1)].GroupID = NodeGroups.size()-1;
                            NodeGroups[NodeGroups.size()-1].NodeAmount++;
                            qDebug()<<"Adding node to group :" << Nodes[(Nodes.size()-1)].GroupID << "Nodes in group: "<< NodeGroups[NodeGroups.size()-1].NodeAmount;
                        }
                        //else create empty group.
                        else
                        {
                            qDebug()<<"Creating 1.st node group.";
                            NodeGroups.resize(NodeGroups.size()+1);
                            NodeGroups[NodeGroups.size()-1].NodeGroupName = "Node Group 1.";
                            NodeGroups[NodeGroups.size()-1].NodeGroupID = 0;
                            Nodes[(Nodes.size()-1)].GroupID = 0;
                            NodeGroups[NodeGroups.size()-1].draw=1;
                            NodeGroups[NodeGroups.size()-1].NodeAmount++;
                        }

                        //Setting node color for 3D picking feature
                        Nodes[(Nodes.size()-1)].green=green;
                        Nodes[(Nodes.size()-1)].blue=blue;

                        if(green<255) green++;
                        else
                        {
                            green=0;
                            blue++;
                        }

                        temp = "";

                        grid_index++;

                    }
                    else if(grid_index == 1)
                    {
                        Nodes[(Nodes.size()-1)].locX = temp.toFloat();

                        temp = "";
                        grid_index++;
                    }
                    else if(grid_index == 2)
                    {
                        Nodes[(Nodes.size()-1)].locY = temp.toFloat();

                        temp = "";
                        grid_index++;
                    }
                    else if(grid_index == 3)
                    {
                        Nodes[(Nodes.size()-1)].locZ = temp.toFloat();

                        temp = "";
                        grid_index++;
                    }

                }
                else
                {
                    temp.append(line[index]);
                }



            }
            if(grid_index<4)
            {
                Nodes[(Nodes.size()-1)].locZ = temp.toFloat();

                temp = "";
            }
            else
            {

                Nodes[(Nodes.size()-1)].Properties.append(temp);

                temp = "";
            }
            grid_index=0;
            qDebug() << Nodes[(Nodes.size()-1)].GlobalID << ", " << Nodes[(Nodes.size()-1)].NodeName << ", " << Nodes[(Nodes.size()-1)].locX << ", " << Nodes[(Nodes.size()-1)].locY << ", " << Nodes[(Nodes.size()-1)].locZ << ", " << Nodes[(Nodes.size()-1)].Properties;

        }
    }
    else if(ParsingMode == 2)
    {
        //ParsingBeams
        qDebug() << "Parsing beams ...";   

        if(line.indexOf(";grp:",0)==0)
        {
            BeamGroups.resize(BeamGroups.size()+1);
            BeamGroups[BeamGroups.size()-1].BeamGroupID = BeamGroups.size()-1;
            BeamGroups[BeamGroups.size()-1].BeamGroupName = line.right(line.length()-5);
            BeamGroups[BeamGroups.size()-1].draw=1;
            qDebug()<<"New beam group added: " << BeamGroups[BeamGroups.size()-1].BeamGroupName << ", " << BeamGroups[BeamGroups.size()-1].BeamGroupID;
        }
        else if(line.indexOf("set_beam_defaults")!= -1)
        {
            qDebug() << "Parsing Beam defaults";
        }
        else
        {
            qDebug() << "Parsing Beam";



            grid_index = 0;
            temp = "";
            for(int index=0; index<line.length(); index++)
            {
                if(line[index]== QChar(';'))
                {
                    //Comment starts
                    break;
                }
                else if(line[index] == QChar(' '));
                else if(line[index] == QChar(','))
                {
                    if (grid_index == 0)
                    {
                        Beams.resize(Beams.size()+1);
                        Beams[Beams.size()-1].Node1Name = temp;

                        //If beam group exists, add in that
                        if(BeamGroups.size() > 0)
                        {
                            Beams[(Beams.size()-1)].BeamGroupID = BeamGroups.size()-1;
                            BeamGroups[BeamGroups.size()-1].BeamAmount++;
                            qDebug()<<"Adding beam to group :" << Beams[(Beams.size()-1)].BeamGroupID << "Beams in group: "<< BeamGroups[BeamGroups.size()-1].BeamAmount;
                        }
                        //else create empty group.
                        else
                        {
                            qDebug()<<"Creating 1.st beam group.";
                            BeamGroups.resize(BeamGroups.size()+1);
                            BeamGroups[BeamGroups.size()-1].BeamGroupName = "Beam Group 1.";
                            BeamGroups[BeamGroups.size()-1].BeamGroupID = 0;
                            Beams[(Beams.size()-1)].BeamGroupID = 0;
                            BeamGroups[BeamGroups.size()-1].draw=1;
                            BeamGroups[BeamGroups.size()-1].BeamAmount++;
                        }


                        temp = "";
                        grid_index++;

                    }
                    else if(grid_index == 1)
                    {
                        Beams[Beams.size()-1].Node2Name = temp;
                        temp = "";
                        grid_index++;
                    }

                }
                else
                {
                    temp.append(line[index]);
                }



            }

            Beams[Beams.size()-1].Properties.append(temp);

            temp = "";
            grid_index=0;

            bool Node1Found = 0;
            bool Node2Found = 0;
            for(int luku=0; luku<Nodes.size(); luku++)
            {
                if(Nodes[luku].NodeName == Beams[Beams.size()-1].Node1Name)
                {
                    Beams[Beams.size()-1].Node1GlobalID = Nodes[luku].GlobalID;
                    Node1Found=1;
                }
                if(Nodes[luku].NodeName == Beams[Beams.size()-1].Node2Name)
                {
                    Beams[Beams.size()-1].Node2GlobalID = Nodes[luku].GlobalID;
                    Node2Found=1;
                }
            }
            if(Node1Found == 0)
            {
                Beams[Beams.size()-1].Node1GlobalID = 99999;
            }
            if(Node2Found == 0)
            {
                Beams[Beams.size()-1].Node2GlobalID = 99999;
            }
            qDebug() << Beams[Beams.size()-1].Node1GlobalID << ", " << Beams[Beams.size()-1].Node2GlobalID << ", " << Beams[Beams.size()-1].Node1Name << ", " << Beams[Beams.size()-1].Node2Name << ", " << Beams[Beams.size()-1].Properties;
        }
    }
    if(ParsingMode==3)
    {

    }

}

/* Import nodes and beams from Wavefront OBJ file */
void NodeBeam::ImportWavefrontOBJ(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//        QMessageBox::warning(this, tr("Application"),
//                             tr("Cannot read file %1:\n%2.")
//                             .arg(fileName)
//                             .arg(file.errorString()));
//        return;
    }

    QTextStream in(&file);

    #ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif

    int ParsingMode = 0;

    QString CurrentLine;
    QString TempStr = "";
    int NodeCounter = 0;

    while(!in.atEnd())
    {
        grid_index = 0;
        CurrentLine = in.readLine();
        if(CurrentLine[0]=='v')
        {
            qDebug() << "node found";

            Nodes.resize(Nodes.size()+1);
            Nodes[(Nodes.size()-1)].GlobalID = (Nodes.size()-1);
            Nodes[(Nodes.size()-1)].NodeName = QString::number(NodeCounter);

            NodeCounter++;

            //If node group exists, add in that
            if(NodeGroups.size() > 0)
            {
                Nodes[(Nodes.size()-1)].GroupID = NodeGroups.size()-1;
                NodeGroups[NodeGroups.size()-1].NodeAmount++;
                qDebug()<<"Adding node to group :" << Nodes[(Nodes.size()-1)].GroupID << "Nodes in group: "<< NodeGroups[NodeGroups.size()-1].NodeAmount;
            }
            //else create empty group.
            else
            {
                qDebug()<<"Creating 1.st node group.";
                NodeGroups.resize(NodeGroups.size()+1);
                NodeGroups[NodeGroups.size()-1].NodeGroupName = "Node Group 1.";
                NodeGroups[NodeGroups.size()-1].NodeGroupID = 0;
                Nodes[(Nodes.size()-1)].GroupID = 0;
                NodeGroups[NodeGroups.size()-1].draw=1;
                NodeGroups[NodeGroups.size()-1].NodeAmount++;
            }

            //Setting node color for 3D picking feature
            Nodes[(Nodes.size()-1)].green=green;
            Nodes[(Nodes.size()-1)].blue=blue;

            if(green<255) green++;
            else
            {
                green=0;
                blue++;
            }

            /* Now parsing the rest of the line for node coordinates */
            for(int i=1; i<CurrentLine.length();i++)
            {
                if(CurrentLine[i]==' ')
                {
                    if(grid_index==1)
                    {
                        qDebug()<<"Node x location:" << TempStr;
                        Nodes[(Nodes.size()-1)].locX = TempStr.toFloat();

                    }
                    else if(grid_index==2)
                    {
                        qDebug()<<"Node y location:" << TempStr;
                        Nodes[(Nodes.size()-1)].locY = TempStr.toFloat();
                    }
                    grid_index++;
                    TempStr="";
                }
                else
                {
                    TempStr.append(CurrentLine[i]);
                }
            }
            //save Z
            qDebug()<<"Node z location:" << TempStr;
            Nodes[(Nodes.size()-1)].locZ = TempStr.toFloat();
            TempStr="";

        }
        else if(CurrentLine[0]=='f')
        {
            /* OBJ file faces will be converted to beams */

            Beams.resize(Beams.size()+1);

//            //If a beam group exists, add in that
            if(BeamGroups.size() > 0)
            {
                Beams[(Beams.size()-1)].BeamGroupID = BeamGroups.size()-1;
                qDebug()<<"Adding beam to group :" << Beams[(Beams.size()-1)].BeamGroupID << "Beams in group: "<< BeamGroups[BeamGroups.size()-1].BeamAmount;
            }
//            //else create empty group.
            else
            {
                qDebug()<<"Creating 1.st beam group.";
                BeamGroups.resize(BeamGroups.size()+1);
                BeamGroups[BeamGroups.size()-1].BeamGroupName = "Beam Group 1.";
                BeamGroups[BeamGroups.size()-1].BeamGroupID = 0;
                Beams[(Beams.size()-1)].BeamGroupID = 0;
                BeamGroups[BeamGroups.size()-1].draw=1;
            }
            int even=0;
            for(int i=1; i<CurrentLine.length();i++)
            {

                if(CurrentLine[i]==' ')
                {
                    if(grid_index>=1)
                    {
                        if(even==0)
                        {
                            BeamGroups[BeamGroups.size()-1].BeamAmount++;
                            qDebug()<<"Beams n1:" << TempStr;
                            qDebug()<<"Beams n1R:" << (Nodes.size()+ TempStr.toInt() - NodeCounter -1);
                            Beams[(Beams.size()-1)].Node1GlobalID = (Nodes.size()+ TempStr.toInt() - NodeCounter -1);
                            Beams[(Beams.size()-1)].Node1Name = QString::number( (Nodes.size()+ TempStr.toInt() - NodeCounter -1));
                            even++;
                        }
                        else if(even==1)
                        {
                            Beams[(Beams.size()-1)].Node2GlobalID = (Nodes.size() + TempStr.toInt() - NodeCounter - 1);
                            Beams[(Beams.size()-1)].Node2Name = QString::number( (Nodes.size()+ TempStr.toInt() - NodeCounter -1));
                            qDebug()<<"Beams n2:" << TempStr;
                            qDebug()<<"Beams n2R:" << (Nodes.size()+ TempStr.toInt() - NodeCounter -1);
                            even++;
                        }
                        else
                        {
                            //joohoo
                        }

                    }
                    grid_index++;
                    TempStr="";
                }
                else
                {
                    TempStr.append(CurrentLine[i]);
                }

            }

            if(even==1)
            {
                Beams[(Beams.size()-1)].Node2GlobalID = (Nodes.size() + TempStr.toInt() - NodeCounter - 1);
                Beams[(Beams.size()-1)].Node2Name = QString::number( (Nodes.size()+ TempStr.toInt() - NodeCounter -1));
                qDebug()<<"Beams n2:" << TempStr;
                qDebug()<<"Beams n2R:" << (Nodes.size()+ TempStr.toInt() - NodeCounter -1);
            }
            else if(even > 1)
            {

            }

        }





    }
    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
    #endif

    file.close();
}


/* BeamNG import: parsing number values */
void NodeBeam::ImportBeamNG_NumberValue(QString TempStr, int ParsingMode)
{
    /*
    Parsing modes
    nodes : 0

    */
    if(ParsingMode==0)
    {
        if(grid_index == 0)
        {
            qDebug()<< "Node locX: " << TempStr;
            Nodes[(Nodes.size()-1)].locZ = TempStr.toFloat();
            grid_index++;

        }
        else if(grid_index == 1)
        {
            qDebug()<< "Node locY: " << TempStr;
            Nodes[(Nodes.size()-1)].locX = TempStr.toFloat();
            grid_index++;
        }
        else if(grid_index == 2)
        {
            qDebug()<< "Node locZ: " << TempStr;
            Nodes[(Nodes.size()-1)].locY = TempStr.toFloat();
            grid_index=0;
        }
    }
    else
    {
        qDebug()<< "Saving number value: " << TempStr;
    }

}

void NodeBeam::ImportJSON(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        // fail
    }

    QTextStream in(&file);
    #ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif

    QString CurrentLine;
    QString TempStr;


    while(!in.atEnd())
    {
        CurrentLine = in.readLine();
        for(int i=0; i<CurrentLine.length(); i++)
        {
            if(CurrentLine[i]== '{')
            {
                //new object
            }
            else if(CurrentLine[i]== '}')
            {
                //close object
            }
            else if(CurrentLine[i]== '"')
            {
                //string
            }
            else if(CurrentLine[i]== '[')
            {
                //array begin
            }
            else if(CurrentLine[i]== ']')
            {
                //array end
            }
            else if(CurrentLine[i]== '/')
            {
                //comment
            }
            else if(CurrentLine[i]== ',')
            {
                //comma
            }
            else if(CurrentLine[i]== ':')
            {
                //separator in object
            }
            else
            {

            }
        }
    }

    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
    #endif
    file.close();

}

/* BeamNG import */
void NodeBeam::ImportBeamNG(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
    // file open failed
    }

    QTextStream in(&file);

    #ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif

    QString CurrentLine;
    QString TempStr;

    int ParsingLevel=0;
    QVector <bool> KeywordContent;
    KeywordContent.resize(10);
    bool ParsingList = 0;
    int ListLevel = 0;
    bool ParsingNodes=0;
    bool ParsingBeams=0;
    bool SaveValue=0;
    bool ValueSaved=0;
    bool SaveNumberValue=0;

    bool ParsingNodeGroup=0;
    bool ParsingBeamGroup=0;
    int ListBeginSkip = 0;
    int CommentCheck = 0;

    while(!in.atEnd())
    {
        CommentCheck = 0;
        CurrentLine = in.readLine();
        qDebug()<<CurrentLine;
            for(int i=0; i<CurrentLine.length(); i++)
            {
                if(SaveValue==1)
                {
                    if(CurrentLine[i]=='"')
                    {
                        SaveValue=0;
                        //save value
                        if(KeywordContent[ParsingLevel-1]==0)
                        {
                            qDebug()<<"Saving keyword: " << TempStr;
                            if(QString::compare(TempStr,"nodes",Qt::CaseInsensitive) == 0)
                            {
                                ParsingNodes=1;
                                ParsingBeams=0;
                                qDebug()<< "Parsing Nodes" << TempStr;
                                TempStr="";
                                ListBeginSkip=0;
                                grid_index=0;
                            }
                            else if(QString::compare(TempStr,"beams",Qt::CaseInsensitive) == 0)
                            {
                                ParsingNodes=0;
                                ParsingBeams=1;
                                qDebug()<<"Parsing Beams" << TempStr;
                                TempStr="";
                                ListBeginSkip=0;
                                grid_index=0;
                            }
                            else if(QString::compare(TempStr,"group",Qt::CaseInsensitive) == 0)
                            {
                                if(ParsingNodes)
                                {
                                    ParsingNodeGroup=1;
                                    qDebug()<<"Parsing Node Group" << TempStr;
                                    TempStr="";
                                    grid_index=0;
                                }
                            }
//                            else if()
//                            {
//                                if(ParsingBeams)
//                                {
//                                    ParsingBeamGroup=1;
//                                    qDebug()<<"Parsing Beam DeformGroup" << TempStr;
//                                    TempStr="";
//                                    grid_index=0;
//                                }
//                            }
                            else
                            {
                                if(ParsingLevel == 2)
                                {
                                    ParsingNodes=0;
                                    ParsingBeams=0;
                                }
                            }

                            TempStr="";
                        }
                        else
                        {
                            ValueSaved=1;

                            if(ParsingNodes)
                            {
                                if(ListLevel == 2)
                                {
                                    if(ListBeginSkip == 4)
                                    {
                                        qDebug()<<"Saving nodename: " << TempStr;

                                        Nodes.resize(Nodes.size()+1);
                                        Nodes[(Nodes.size()-1)].GlobalID = (Nodes.size()-1);
                                        Nodes[(Nodes.size()-1)].NodeName = TempStr;

                                        //If node group exists, add in that
                                        if(NodeGroups.size() > 0)
                                        {
                                            Nodes[(Nodes.size()-1)].GroupID = NodeGroups.size()-1;
                                            NodeGroups[NodeGroups.size()-1].NodeAmount++;
                                            qDebug()<<"Adding node to group :" << Nodes[(Nodes.size()-1)].GroupID << "Nodes in group: "<< NodeGroups[NodeGroups.size()-1].NodeAmount;
                                        }
                                        //else create empty group.
                                        else
                                        {
                                            qDebug()<<"Creating 1.st node group.";
                                            NodeGroups.resize(NodeGroups.size()+1);
                                            NodeGroups[NodeGroups.size()-1].NodeGroupName = "Node Group 1.";
                                            NodeGroups[NodeGroups.size()-1].NodeGroupID = 0;
                                            Nodes[(Nodes.size()-1)].GroupID = 0;
                                            NodeGroups[NodeGroups.size()-1].draw=1;
                                            NodeGroups[NodeGroups.size()-1].NodeAmount++;
                                        }

                                        //Setting node color for 3D picking feature
                                        Nodes[(Nodes.size()-1)].green=green;
                                        Nodes[(Nodes.size()-1)].blue=blue;

                                        if(green<255) green++;
                                        else
                                        {
                                            green=0;
                                            blue++;
                                        }


                                    }
                                    else
                                    {
                                        //Have to skip 4 text values before the list begins
                                        //in nodes section
                                        ListBeginSkip++;
                                    }
                                }
                                if(ParsingNodeGroup && ParsingLevel==3)
                                {
                                    qDebug()<<"Name of new group: " << TempStr;
                                    NodeGroups.resize(NodeGroups.size()+1);
                                    NodeGroups[NodeGroups.size()-1].NodeGroupID = NodeGroups.size()-1;
                                    NodeGroups[NodeGroups.size()-1].NodeGroupName = TempStr;
                                    NodeGroups[NodeGroups.size()-1].draw=1;
                                    qDebug()<<"New node group added: " << NodeGroups[NodeGroups.size()-1].NodeGroupName << ", " << NodeGroups[NodeGroups.size()-1].NodeGroupID;
                                    ParsingNodeGroup=0;
                                }
                            }
                            else if(ParsingBeams)
                            {
                                if(ListLevel == 2)
                                {
                                    if(ListBeginSkip == 2)
                                    {
                                        if(grid_index==0)
                                        {
                                            qDebug()<<"Saving beam 1: " << TempStr;

                                            Beams.resize(Beams.size()+1);
                                            Beams[Beams.size()-1].Node1Name = TempStr;

                                            //If beam group exists, add in that
                                            if(BeamGroups.size() > 0)
                                            {
                                                Beams[(Beams.size()-1)].BeamGroupID = BeamGroups.size()-1;
                                                BeamGroups[BeamGroups.size()-1].BeamAmount++;
                                                qDebug()<<"Adding beam to group :" << Beams[(Beams.size()-1)].BeamGroupID << "Beams in group: "<< BeamGroups[BeamGroups.size()-1].BeamAmount;
                                            }
                                            //else create empty group.
                                            else
                                            {
                                                qDebug()<<"Creating 1.st beam group.";
                                                BeamGroups.resize(BeamGroups.size()+1);
                                                BeamGroups[BeamGroups.size()-1].BeamGroupName = "Beam Group 1.";
                                                BeamGroups[BeamGroups.size()-1].BeamGroupID = 0;
                                                Beams[(Beams.size()-1)].BeamGroupID = 0;
                                                BeamGroups[BeamGroups.size()-1].draw=1;
                                                BeamGroups[BeamGroups.size()-1].BeamAmount++;
                                            }


                                            grid_index++;
                                        }
                                        else if(grid_index==1)
                                        {
                                            qDebug()<<"Saving beam 2: " << TempStr;
                                            Beams[Beams.size()-1].Node2Name = TempStr;
                                            grid_index=0;

                                            for(int luku=0; luku<Nodes.size(); luku++)
                                            {
                                                if(Nodes[luku].NodeName == Beams[Beams.size()-1].Node1Name)
                                                {
                                                    Beams[Beams.size()-1].Node1GlobalID = Nodes[luku].GlobalID;
                                                }
                                                if(Nodes[luku].NodeName == Beams[Beams.size()-1].Node2Name)
                                                {
                                                    Beams[Beams.size()-1].Node2GlobalID = Nodes[luku].GlobalID;
                                                }
                                            }

                                        }
                                    }
                                    else ListBeginSkip++;
                                }
                            }
                            else
                            {
                                qDebug()<<"Saving textvalue: " << TempStr;
                            }
                        }
                    }
                    else
                    {
                        TempStr.append(CurrentLine[i]);
                        //
                    }
                }
                else
                {
                    if(CurrentLine[i] == '{')
                    {
                        ParsingLevel++;
                        KeywordContent[ParsingLevel-1]=0;
                        qDebug()<<"Parsing level ++" << ParsingLevel;
                    }
                    else if(CurrentLine[i] == '}')
                    {
                        ParsingLevel=ParsingLevel-1;
                        if(ParsingLevel!=0) KeywordContent[ParsingLevel]=0;
                        qDebug()<<"Parsing level --" << ParsingLevel;
                    }
                    else if(CurrentLine[i]=='"')
                    {
                        SaveValue=1;
                        SaveNumberValue=0;
                        TempStr="";
                    }
                    else if(CurrentLine[i] == ':')
                    {
                        KeywordContent[ParsingLevel-1] = 1;
                        SaveNumberValue=1;
                        //qDebug()<<"Content";
                    }
                    else if((CurrentLine[i] == ',') && (ParsingList == 0))
                    {
                        KeywordContent[ParsingLevel-1] = 0;
                        //qDebug()<<"Keyword";
                        //Next value . . .

                        if(ValueSaved==0)
                        {
                            //Dot comes and text has not been saved means
                            //the value is a number and must be saved now.
                            qDebug()<< "Saving number value: " << TempStr;
                            SaveNumberValue=0;
                        }
                        ValueSaved=1;
                    }
                    else if((CurrentLine[i] == ',') && (ParsingList == 1))
                    {
                        if(ValueSaved==0)
                        {
                            if(ListLevel== 2)
                            {
                                if(ParsingNodes) ImportBeamNG_NumberValue(TempStr, 0);
                            }
                            TempStr="";
                        }
                        ValueSaved=1;
                        SaveNumberValue=1;
                        TempStr="";
                    }
                    else if((CurrentLine[i] == '/') && (ParsingList == 1))
                    {
                        if(CommentCheck>0)
                        {
                            QString subString = CurrentLine.mid((i+1),5);
                            // if a comment inside list has 5 first letters: "BNEG:", make a new group.
                            if(subString == "BNEG:")
                            {
                                QString BNGroupName = CurrentLine.mid((i+6),(CurrentLine.length()-(i+5)));
                                qDebug()<< "Make a group";
                                if(ParsingNodes)
                                {
                                    NodeGroups.resize(NodeGroups.size()+1);
                                    NodeGroups[NodeGroups.size()-1].NodeGroupName = BNGroupName;
                                    NodeGroups[NodeGroups.size()-1].NodeGroupID = NodeGroups.size()-1;
                                    NodeGroups[NodeGroups.size()-1].draw=1;
                                    NodeGroups[NodeGroups.size()-1].NodeAmount=0;
                                }
                                else if(ParsingBeams)
                                {
                                    BeamGroups.resize(BeamGroups.size()+1);
                                    BeamGroups[BeamGroups.size()-1].BeamGroupName = BNGroupName;
                                    BeamGroups[BeamGroups.size()-1].BeamGroupID = BeamGroups.size()-1;
                                    BeamGroups[BeamGroups.size()-1].draw=1;
                                }
                            }

                        }
                        CommentCheck++;
                    }
                    else if(CurrentLine[i] == '[')
                    {
                        if(ParsingList == 0)
                        {
                            //List begins
                            ParsingList = 1;
                            ListLevel++;
                            qDebug()<<"List begins" << ListLevel;
                            SaveNumberValue=0;
                        }
                        else{
                            ListLevel++;
                            qDebug()<<"List level++" << ListLevel;
                        }

                        SaveNumberValue = 1;
                        TempStr="";

                    }
                    else if(CurrentLine[i] == ']')
                    {
                        ListLevel=ListLevel-1;
                        if(ListLevel==0)
                        {
                            ParsingList=0;
                            KeywordContent[ParsingLevel-1]=0;
                        }
                        if(ValueSaved==0)
                        {
                            if(ListLevel== 1)
                            {
                                if(ParsingNodes) ImportBeamNG_NumberValue(TempStr, 0);
                            }
                            TempStr="";
                        }
                        ValueSaved=1;
                        qDebug()<<"ListLevel --" << ListLevel;

                    }
                    else if(CurrentLine[i] == ' ');
                    else
                    {
                        if(SaveNumberValue == 1)
                        {
                            TempStr.append(CurrentLine[i]);
                            ValueSaved=0;
                        }

                    }

                }
            }
    }

    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
    #endif
    file.close();

}

/* Export function to BeamNG JBEAM format */
void NodeBeam::ExportBeamNG(const QString &fileName)
{
    /* Try and open a file for output */
    QString suffixi = fileName.right(6);
    QString FileName1 = fileName;
    if(QString::compare(suffixi,".jbeam",Qt::CaseInsensitive) != 0) FileName1.append(".jbeam");

    QFile outputFile(FileName1);
    outputFile.open(QIODevice::WriteOnly);

    /* Check it opened OK */
    if(!outputFile.isOpen()){
        qDebug() << "- Error, unable to open" << fileName << "for output";

    }
    else
    {
        /* Point a QTextStream object at the file */
        QTextStream outStream(&outputFile);

        /* JBeam Export begins */
        outStream << "{\r\n";
        outStream <<'"' << VehicleName << '"' << ": {\r\n";

        /* Writing information*/
        outStream <<"   "<<'"'<<"information"<<'"'<<":{";
        outStream << "\r\n";
        outStream <<"       "<<'"'<<"authors"<<'"'<<":";
        if(VehicleAuthors.size()<2) outStream<<'"' << VehicleAuthors[0] << '"' << ",\r\n";

        outStream <<"       "<<'"'<<"name"<<'"'<<":";
        outStream<<'"'<<VehicleInGameName<<'"'<<",\r\n";
        outStream <<"   }\r\n";

        /**/
        outStream <<"   "<<'"'<<"slotType"<<'"'<<" : "<<'"'<<"main"<<'"'<<",\r\n";
        outStream <<"   "<<'"'<<"slots"<<'"'<<" : [\r\n";
        outStream <<"       ["<<'"'<<"type"<<'"'<<", "<< '"'<<"default"<<'"'<<", "<<'"'<<"description"<<'"'<<"]\r\n";

        outStream <<"       ],\r\n";
        outStream <<"   "<<'"'<<"dragCoef"<<'"'<<" : 10,\r\n";
        outStream <<"   "<<'"'<<"scalebeamSpring"<<'"'<<" : 1.0,\r\n";
        outStream <<"   "<<'"'<<"scalebeamDamp"<<'"'<<" : 1.0,\r\n";
        outStream <<"   "<<'"'<<"scalebeamDeform"<<'"'<<" : 1.0,\r\n";
        outStream <<"   "<<'"'<<"scalebeamStrength"<<'"'<<" : 1.0,\r\n";


        /*Nodes export*/

        outStream <<"   "<<'"'<<"nodes"<<'"'<<" : [\r\n";
        outStream <<"       ["<<'"'<<"id"<<'"'<<", "<< '"'<<"posX"<<'"'<<", "<<'"'<<"posY"<<'"'<<", "<<'"'<<"posZ"<<'"'<<"],\r\n";
//        for(int i=0; i<Nodes.size();i++)
//        {
//            outStream <<"       ["<<'"'<<Nodes[i].NodeName<<'"'<<", "<<Nodes[i].locZ<<", "<<Nodes[i].locX<<", "<<Nodes[i].locY<<"],\r\n";
//        }
        int i3=0;
        for(int i=0; i<NodeGroups.size();i++)
        {
            if(BeamGroups[i].draw) outStream << "       //BNEG:"<< NodeGroups[i].NodeGroupName << "\r\n";
            for(int i2=0; i2<NodeGroups[i].NodeAmount; i2++)
            {
                if(NodeGroups[i].draw)
                {
                    outStream <<"       ["<<'"'<<Nodes[i3].NodeName<<'"'<<", "<<Nodes[i3].locX<<", "<<Nodes[i3].locY<<", "<<Nodes[i3].locZ<<"],\r\n";
                }
                i3++;
            }
        }
        outStream <<"]\r\n";
        /*Beams export*/

        outStream <<"   "<<'"'<<"beams"<<'"'<<" : [\r\n";
        outStream <<"       ["<<'"'<<"id1:"<<'"'<<", "<< '"'<<"id2:"<<'"'<<"],\r\n";

//        for(int i=0; i<Beams.size();i++)
//        {
//            outStream <<"       ["<<'"'<<Beams[i].Node1Name<<'"'<<", "<<'"'<<Beams[i].Node2Name<<'"'<<"],\r\n";
//        }

        i3=0;
        int lastbeamdefs = -1;
        for(int i=0; i<BeamGroups.size();i++)
        {
            if(BeamGroups[i].draw) outStream << "       //BNEG:"<< BeamGroups[i].BeamGroupName << "\r\n";
            for(int i2=0; i2<BeamGroups[i].BeamAmount; i2++)
            {
                if(BeamGroups[i].draw)
                {
                    if(Beams[i3].HasBeamDefs)
                    {
                        if(lastbeamdefs != Beams[i3].BeamDefsID)
                        {
                            for(int n5=0; n5<BeamDefaults[Beams[i3].BeamDefsID].KeyValuePairs.size();n5++)
                            {
                                bool ok;
                                outStream << "       {" << '"' << BeamDefaults[Beams[i3].BeamDefsID].KeyValuePairs[n5].name << '"';
                                float value = BeamDefaults[Beams[i3].BeamDefsID].KeyValuePairs[n5].value.toFloat(&ok);
                                if(ok) outStream << ":" << BeamDefaults[Beams[i3].BeamDefsID].KeyValuePairs[n5].value <<"},\r\n";
                                else outStream << ":" << '"' << BeamDefaults[Beams[i3].BeamDefsID].KeyValuePairs[n5].value<< '"'<<"},\r\n";
                            }
                            lastbeamdefs = Beams[i3].BeamDefsID;
                        }
                    }
                    outStream <<"       ["<<'"'<<Beams[i3].Node1Name<<'"'<<", "<<'"'<<Beams[i3].Node2Name<<'"'<<"],\r\n";
                }
                i3++;
            }
        }

        outStream <<"]\r\n";

        outStream <<"   "<<'"'<<"hubwheels"<<'"'<<" : [\r\n";
        if(Hubwheels.size()>0)
        {
            for(int i=0; i<Hubwheels.size();i++)
            {
                for(int i2=0; i<HWArguments[Hubwheels[i].arguments].KeyValuePairs.size();i2++)
                {
                    bool ok;
                    outStream << "       {" << '"' << HWArguments[Hubwheels[i].arguments].KeyValuePairs[i2].name << '"';
                    float value = HWArguments[Hubwheels[i].arguments].KeyValuePairs[i2].value.toFloat(&ok);
                    if(ok) outStream << ":" << HWArguments[Hubwheels[i].arguments].KeyValuePairs[i2].value <<"},\r\n";
                    else outStream << ":" << '"' << HWArguments[Hubwheels[i].arguments].KeyValuePairs[i2].value<< '"'<<"},\r\n";
                }
                outStream <<"       ["<<'"'<<Hubwheels[i].name<<'"'<<", "<<'"'<<  Hubwheels[i].group  <<'"'<<", "<<'"'<<  Hubwheels[i].node1   <<'"'<<", "<<'"'<<   Hubwheels[i].node2    <<'"'<<", "<<'"'<<    Hubwheels[i].nodeArm <<'"'<<", "<<'"'<<    Hubwheels[i].nodeS <<'"'<<", "<<'"'<<    Hubwheels[i].wheelDir<<'"'<<"],\r\n";
            }


        outStream <<"]\r\n";

        }

        outStream <<"]\r\n";
        outStream <<"\r\n}\r\n";
        outStream <<"}\r\n";

//        else
//        {
//            for(int i=0; i<VehicleAuthors.size();i++)
//            {
//                outStream<<'"' << VehicleAuthors[i] << '"';
//            }
//        }
        /* Close the file */
        outputFile.close();
    }
}

/* Export function to Rigs of Rods format */
void NodeBeam::ExportRoR(const QString &fileName)
{
    /* Try and open a file for output */
    QString suffixi = fileName.right(6);
    QString FileName1 = fileName;
    if(QString::compare(suffixi,".truck",Qt::CaseInsensitive) != 0) FileName1.append(".truck");

    QFile outputFile(FileName1);
    outputFile.open(QIODevice::WriteOnly);

    /* Check it opened OK */
    if(!outputFile.isOpen()){
        qDebug() << "- Error, unable to open" << fileName << "for output";

    }
    else
    {
        /* Point a QTextStream object at the file */
        QTextStream outStream(&outputFile);

        /* Title section */
        outStream <<VehicleName << "\r\n\r\n";

        /* Globals */
        outStream <<"globals" << "\r\n";
        outStream <<";dry mass, cargo mass, material\r\n";
        outStream <<"0.0,  0.0,     tracks/semi\r\n\r\n";

        /*Nodes export*/

        outStream <<"nodes\r\n";
        int i3=0;
        for(int i=0; i<NodeGroups.size();i++)
        {
            if(NodeGroups[i].draw) outStream << ";grp:"<< NodeGroups[i].NodeGroupName << "\r\n";
            for(int i2=0; i2<NodeGroups[i].NodeAmount; i2++)
            {
                if(NodeGroups[i].draw)
                {
                    outStream << i3 <<", "<< Nodes[i3].locX <<", "<<Nodes[i3].locY<<", "<<Nodes[i3].locZ<<", l 50 \r\n";
                }
                i3++;
            }
        }
        outStream <<"\r\n";

        /*Beams export*/

        outStream <<"beams\r\n";

        i3=0;
        for(int i=0; i<BeamGroups.size();i++)
        {
            if(BeamGroups[i].draw) outStream << ";grp:"<< BeamGroups[i].BeamGroupName << "\r\n";
            for(int i2=0; i2<BeamGroups[i].BeamAmount; i2++)
            {
                if(BeamGroups[i].draw)
                {
                    outStream <<Beams[i3].Node1GlobalID<<", "<<Beams[i3].Node2GlobalID<<", v\r\n";
                }
                i3++;
            }
        }
        outStream <<"\r\ncameras\r\n;center, back, left\r\n0,1,2\r\n";
        outStream <<"\r\ncinecam\r\n;  x,   y,   z, <---------8 bindings--------->, spring, damping\r\n0.66, 2.0, 1.8, 0, 1, 2, 3, 4, 5, 6, 7, 8000.0,   800.0\r\n";
        outStream <<"\r\n";
        outStream <<"end";

        /* Close the file */
        outputFile.close();
    }
}

/* Add node, input values as argument */
void NodeBeam::AddNodeT(float locx, float locy, float locz, int NodeGroupID, QString NodeName)
{
    TempNode.locX = locx;
    TempNode.locY = locy;
    TempNode.locZ = locz;

    if(NodeName != "")
    {
        /* If a name was given as input parameter, check that node with this name does not exsist */
//        for(int i=0; i< Nodes.size(); i++)
//        {
//            if(Nodes[i].NodeName == NodeName)
//            {
//                NodeName.append("_1");
//            }
//        }
        TempNode.NodeName = NewNodeName(CurrentNamePrefix);
    }
    else
    {
        /* Naming the node, different letter for each group */
        QString NodeName = "";
        NodeName.append(char(NodeGroupID+97));
        if(NodeGroups.size()>0) NodeName.append(QString::number(NodeGroups[NodeGroupID].NodeAmount));
        else NodeName.append(QString::number(0));
//        NodeName=GiveName(0,"",NodeGroupID);
        TempNode.NodeName=NodeName;
    }
    AddNode(NodeGroupID);

}

/* Add node to last group */
int NodeBeam::AddNode()
{
    int NodeGroupID = NodeGroups.size();
    if(NodeGroups.size()>0) NodeGroupID--;
    return AddNode(NodeGroupID);
}

/* Add node, takes input values from TempNode */
int NodeBeam::AddNode(int NodeGroupID)
{
    TempNode.green = green;
    TempNode.blue = blue;

    //If node group exists, add in that
    if(NodeGroups.size() > 0)
    {
        /* Calculating the node Global ID
        First go trough all node groups before this group
        and sum nodes from them */

        int activenode = 0;
        for(int i5=0; i5<NodeGroupID ; i5++)
        {
            activenode = activenode + NodeGroups[i5].NodeAmount;
        }
        activenode = activenode + NodeGroups[NodeGroupID].NodeAmount;

        TempNode.GlobalID = activenode;
        TempNode.GroupID = NodeGroupID;
        NodeGroups[NodeGroupID].NodeAmount++;

        /* Inserting the node from TempNode to Nodes vector */
        Nodes.insert(activenode, TempNode);

        /* If the node was not last node of the vector,
        increment every node GlobalID after the new node */

        for(int i5=activenode+1;i5<Nodes.size();i5++)
        {
            Nodes[i5].GlobalID++;
        }
        for(int i5=0; i5<Beams.size();i5++)
        {
            if(Beams[i5].Node1GlobalID >= activenode) Beams[i5].Node1GlobalID++;

            if(Beams[i5].Node2GlobalID >= activenode) Beams[i5].Node2GlobalID++;

        }
    }
    //else create empty group.
    else
    {
        qDebug()<<"Creating 1.st node group.";
        NodeGroups.resize(NodeGroups.size()+1);
        NodeGroups[NodeGroups.size()-1].NodeGroupName = "Node Group 1.";
        NodeGroups[NodeGroups.size()-1].NodeGroupID = 0;
        NodeGroups[NodeGroups.size()-1].draw=1;
        NodeGroups[NodeGroups.size()-1].NodeAmount++;

        int activenode=0;
        TempNode.GlobalID = activenode;
        TempNode.GroupID = 0;
        Nodes.insert(activenode, TempNode);


    }

    if(green<255) green++;
    else
    {
        green=0;
        blue++;
    }

    return TempNode.GlobalID;
}

/* Delete node */
void NodeBeam::DeleteNode(int NodeGlobalID)
{
    qDebug()<< "Deleting node " << NodeGlobalID;

    QVector<int> DeleteQueue;
    DeleteQueue.clear();
    //Delete all beams connected to this node
    for(int i=0; i<Beams.size(); i++)
    {
        qDebug()<<"Should I delete beam " << Beams[i].Node1GlobalID << ", " << Beams[i].Node2GlobalID << ", BeamID: " << i;
        if(Beams[i].Node1GlobalID == NodeGlobalID)
        {
            qDebug()<<"Deleting beam connceted to node" << Beams[i].Node1GlobalID;
            DeleteQueue.append(i);
        }

        else if(Beams[i].Node2GlobalID == NodeGlobalID)
        {
            qDebug()<<"Deleting beam connceted to node" << Beams[i].Node2GlobalID;
            DeleteQueue.append(i);
        }
    }

    for(int i=0; i< DeleteQueue.size(); i++)
    {

        DeleteBeam(DeleteQueue[i]-i);
    }


    for(int i = 0; i<Beams.size(); i++)
    {
        if(Beams[i].Node1GlobalID >= NodeGlobalID)
        {
            Beams[i].Node1GlobalID--;

        }
        if(Beams[i].Node2GlobalID >= NodeGlobalID)
        {
            Beams[i].Node2GlobalID--;

        }

    }

    //Delete node
    NodeGroups[Nodes[NodeGlobalID].GroupID].NodeAmount--;
    Nodes.remove(NodeGlobalID);

    for(int i=NodeGlobalID; i<Nodes.size(); i++)
    {
        Nodes[i].GlobalID--;
        //qDebug() << "Node global ID lowered from " << (Nodes[i].GlobalID+1) << " to " <<  Nodes[i].GlobalID  ;
    }


}

/* Add beam */
void NodeBeam::AddBeam(int Node1ID, int Node2ID, int BeamGroup)
{

    TempBeam.Node1GlobalID = Node1ID;
    TempBeam.Node2GlobalID = Node2ID;
    TempBeam.Node1Name = Nodes[Node1ID].NodeName;
    TempBeam.Node2Name = Nodes[Node2ID].NodeName;
    TempBeam.HasBeamDefs=0;

    //Adding to group
    //If beam group exists, add in that
    if(BeamGroups.size() > 0)
    {
        //Calculate location
        int BeamNumber = 0;
        for(int i=0; i<BeamGroup;i++)
        {
            BeamNumber = BeamNumber + BeamGroups[i].BeamAmount;
        }

        TempBeam.BeamGroupID = BeamGroup;
        Beams.insert(BeamNumber,TempBeam);
        BeamGroups[BeamGroup].BeamAmount++;
        qDebug()<<"Adding beam to group :" << Beams[BeamNumber].BeamGroupID << "Beams in group: "<< BeamGroups[BeamGroup].BeamAmount;

    }
    //else create empty group.
    else
    {
        qDebug()<<"Creating 1.st beam group.";
        BeamGroups.resize(BeamGroups.size()+1);
        BeamGroups[BeamGroups.size()-1].BeamGroupName = "Beam Group 1.";
        BeamGroups[BeamGroups.size()-1].BeamGroupID = 0;
        BeamGroups[BeamGroups.size()-1].draw=1;
        BeamGroups[BeamGroups.size()-1].BeamAmount++;
        TempBeam.BeamGroupID = 0;
        Beams.insert(0,TempBeam);
    }

}

/* Add beam from TempBeam */
void NodeBeam::AddBeamT()
{

    //Adding to group
    //If beam group exists, add in that
    if(BeamGroups.size() > 0)
    {
        int BeamGroup = TempBeam.BeamGroupID;
        //Calculate location
        int BeamNumber = 0;
        for(int i=0; i<BeamGroup;i++)
        {
            BeamNumber = BeamNumber + BeamGroups[i].BeamAmount;
        }

        Beams.insert(BeamNumber,TempBeam);
        BeamGroups[BeamGroup].BeamAmount++;
        qDebug()<<"Adding beam to group :" << Beams[BeamNumber].BeamGroupID << "Beams in group: "<< BeamGroups[BeamGroup].BeamAmount;

    }
    //else create empty group.
    else
    {
        qDebug()<<"Creating 1.st beam group.";
        BeamGroups.resize(BeamGroups.size()+1);
        BeamGroups[BeamGroups.size()-1].BeamGroupName = "Beam Group 1.";
        BeamGroups[BeamGroups.size()-1].BeamGroupID = 0;
        BeamGroups[BeamGroups.size()-1].draw=1;
        BeamGroups[BeamGroups.size()-1].BeamAmount++;
        TempBeam.BeamGroupID = 0;
        Beams.insert(0,TempBeam);
    }

}

/* Add empty beam group at the end of the list  */
int NodeBeam::NewBeamGroup(QString groupname)
{
    if(groupname.length() == 0)
    {
        groupname = "New beam Group 1.";
    }
    int GroupID = BeamGroups.size();
    BeamGroups.resize(GroupID+1);
    BeamGroups[BeamGroups.size()-1].BeamGroupName = groupname;
    BeamGroups[BeamGroups.size()-1].BeamGroupID = GroupID;
    BeamGroups[BeamGroups.size()-1].draw=1;

    return GroupID;
}

/* Delete Beam */
void NodeBeam::DeleteBeam(int BeamIndex)
{
    BeamGroups[Beams[BeamIndex].BeamGroupID].BeamAmount--;
    //qDebug()<<"Beams size before delete : " << Beams.size();
    Beams.remove(BeamIndex);
    //qDebug()<<"Deleted beam: " << BeamIndex << ", beams size: " << Beams.size();
}

void NodeBeam::DuplicateNodes()
{
    QVector <int> Copied;
    int selected;
    for(int i=0; i<SelectedNodes.size(); i++)
    {
        selected = SelectedNodes[i];
        TempNode.GroupID = Nodes[selected].GroupID;
        TempNode.locX = Nodes[selected].locX;
        TempNode.locY = Nodes[selected].locY;
        TempNode.locZ = Nodes[selected].locZ;
        QString nameprefix = Nodes[selected].NodeName.at(0);
        TempNode.NodeName = NewNodeName(nameprefix);
        TempNode.Properties = Nodes[selected].Properties;
        TempNode.RoRNodeType = Nodes[selected].RoRNodeType;
        Copied.append(AddNode(TempNode.GroupID));
    }

    //List of copied nodes
    SelectedNodes2 = Copied;
    QVector <Beam> TempBeams;

    for(int i=0; i<SelectedNodes.size(); i++)
    {
        for(int i2=0; i2<Beams.size(); i2++)
        {
            if(Beams[i2].Node1GlobalID == SelectedNodes[i])
            {
                for(int i3=0; i3<Copied.size(); i3++)
                {
                    if(Beams[i2].Node2GlobalID == SelectedNodes[i3])
                    {
                        TempBeam.BeamDefsID = Beams[i2].BeamDefsID;
                        TempBeam.BeamGroupID = Beams[i2].BeamGroupID;
                        TempBeam.HasBeamDefs = Beams[i2].HasBeamDefs;
                        TempBeam.Node1GlobalID = Copied[i];
                        TempBeam.Node2GlobalID = Copied[i3];
                        TempBeam.Node1Name = Nodes[TempBeam.Node1GlobalID].NodeName;
                        TempBeam.Node2Name = Nodes[TempBeam.Node2GlobalID].NodeName;
                        TempBeam.Properties = TempBeam.Properties;
                        TempBeams.append(TempBeam);
                    }
                }
            }

        }
    }

    for(int i=0; i<TempBeams.size(); i++)
    {
        TempBeam = TempBeams[i];
        AddBeamT();
    }
}

void NodeBeam::ExtrudeNodes()
{
    if(SelectedNodes.size()>0)
    {
        QVector <Beam> TempBeams;

        //Copying selected nodes
        DuplicateNodes();

        //Add beams between the extruded part and exsisting part

        ActiveBeamGroup = NewBeamGroup("Extruded");
        for(int i=0; i<SelectedNodes.size(); i++)
        {
            AddBeam(SelectedNodes[i], SelectedNodes2[i], ActiveBeamGroup);
        }

        for(int i=0; i<SelectedNodes.size(); i++)
        {
            for(int i2=0; i2<Beams.size(); i2++)
            {
                if(Beams[i2].Node1GlobalID == SelectedNodes[i])
                {
                    for(int i3=0; i3<SelectedNodes.size(); i3++)
                    {
                        if(Beams[i2].Node2GlobalID == SelectedNodes[i3])
                        {
                            TempBeam.BeamDefsID = Beams[i2].BeamDefsID;
                            TempBeam.BeamGroupID = Beams[i2].BeamGroupID;
                            TempBeam.HasBeamDefs = Beams[i2].HasBeamDefs;
                            TempBeam.Node1GlobalID = SelectedNodes[i];
                            TempBeam.Node2GlobalID = SelectedNodes2[i3];
                            TempBeam.Node1Name = Nodes[TempBeam.Node1GlobalID].NodeName;
                            TempBeam.Node2Name = Nodes[TempBeam.Node2GlobalID].NodeName;
                            TempBeam.Properties = TempBeam.Properties;
                            TempBeams.append(TempBeam);


                            TempBeam.BeamDefsID = Beams[i2].BeamDefsID;
                            TempBeam.BeamGroupID = Beams[i2].BeamGroupID;
                            TempBeam.HasBeamDefs = Beams[i2].HasBeamDefs;
                            TempBeam.Node1GlobalID = SelectedNodes[i3];
                            TempBeam.Node2GlobalID = SelectedNodes2[i];
                            TempBeam.Node1Name = Nodes[TempBeam.Node1GlobalID].NodeName;
                            TempBeam.Node2Name = Nodes[TempBeam.Node2GlobalID].NodeName;
                            TempBeam.Properties = TempBeam.Properties;
                            TempBeams.append(TempBeam);
                        }
                    }
                }

            }
        }

        for(int i=0; i<TempBeams.size(); i++)
        {
            TempBeam = TempBeams[i];
            AddBeamT();
        }
    }

}

/* Open saved vehicle project */
void NodeBeam::OpenProject(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//        QMessageBox::warning(this, tr("Application"),
//                             tr("Cannot read file %1:\n%2.")
//                             .arg(fileName)
//                             .arg(file.errorString()));
//        return;
    }

    //file.open(QFile::ReadOnly);

    QXmlStreamReader StreamReader(&file);

    while (!StreamReader.atEnd() && !StreamReader.hasError())
    {

        QXmlStreamReader::TokenType token = StreamReader.readNext();

        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement) {

            if(StreamReader.name() == "nodes") {
                //Nodes.append(XML_nodes(StreamReader));
                qDebug()<< "nodes section found";

            }
            else if(StreamReader.name() == "node") {
                qDebug() << "node found";
                while(!(StreamReader.tokenType() == QXmlStreamReader::EndElement &&
                           StreamReader.name() == "node")) {
                       if(StreamReader.tokenType() == QXmlStreamReader::StartElement)
                       {
                           if(StreamReader.name() == "NodeID") {
                               TempNode.GlobalID = StreamReader.readElementText().toInt();
                           }
                           if(StreamReader.name() == "name") {
                               TempNode.NodeName = StreamReader.readElementText();
                           }
                           if(StreamReader.name() == "locx") {
                               TempNode.locX = StreamReader.readElementText().toFloat();
                           }
                           if(StreamReader.name() == "locy") {
                               TempNode.locY = StreamReader.readElementText().toFloat();
                           }
                           if(StreamReader.name() == "locz") {
                               TempNode.locZ = StreamReader.readElementText().toFloat();
                           }
                           if(StreamReader.name() == "group") {
                               TempNode.GroupID = StreamReader.readElementText().toInt();
                           }
                           if(StreamReader.name() == "properties") {
                               TempNode.Properties = StreamReader.readElementText();
                           }
                           if(StreamReader.name() == "ror") {
                               TempNode.RoRNodeType = StreamReader.readElementText().toInt();
                           }
                       }
                       /* ...and next... */
                       StreamReader.readNext();
                }
                TempNode.green = green;
                green++;

                TempNode.blue = blue;
                blue++;

                Nodes.append(TempNode);
                qDebug()<< Nodes.size();
            }
            else if(StreamReader.name() == "beams") {
                qDebug() << "beams found";
            }
            else if(StreamReader.name() == "beam") {
                qDebug() << "beam found";
                while(!(StreamReader.tokenType() == QXmlStreamReader::EndElement &&
                           StreamReader.name() == "beam")) {
                       if(StreamReader.tokenType() == QXmlStreamReader::StartElement)
                       {
                           if(StreamReader.name() == "beamdefsID") {
                               TempBeam.BeamDefsID = StreamReader.readElementText().toInt();
                           }
                           if(StreamReader.name() == "beamgroup") {
                               TempBeam.BeamGroupID = StreamReader.readElementText().toInt();
                           }
                           if(StreamReader.name() == "hasbeamdefs") {
                               TempBeam.HasBeamDefs = StreamReader.readElementText().toInt();
                           }
                           if(StreamReader.name() == "N1ID") {
                               TempBeam.Node1GlobalID = StreamReader.readElementText().toInt();
                           }
                           if(StreamReader.name() == "N1N") {
                               TempBeam.Node1Name = StreamReader.readElementText();
                           }
                           if(StreamReader.name() == "N2ID") {
                               TempBeam.Node2GlobalID = StreamReader.readElementText().toInt();
                           }
                           if(StreamReader.name() == "N2N") {
                               TempBeam.Node2Name = StreamReader.readElementText();
                           }
                           if(StreamReader.name() == "properties") {
                               TempBeam.Properties = StreamReader.readElementText();
                           }
                       }
                       /* ...and next... */
                       StreamReader.readNext();
                }
                Beams.append(TempBeam);
                qDebug()<< Beams.size();
            }
            else if(StreamReader.name() == "node_group") {
                qDebug() << "node group found";
                while(!(StreamReader.tokenType() == QXmlStreamReader::EndElement &&
                           StreamReader.name() == "node_group")) {
                       if(StreamReader.tokenType() == QXmlStreamReader::StartElement)
                       {
                           if(StreamReader.name() == "draw") {
                               TempNodeGroup.draw = StreamReader.readElementText().toInt();
                           }
                           if(StreamReader.name() == "NodeAmount") {
                               TempNodeGroup.NodeAmount = StreamReader.readElementText().toInt();
                           }
                           if(StreamReader.name() == "GroupID") {
                               TempNodeGroup.NodeGroupID = StreamReader.readElementText().toInt();
                           }
                           if(StreamReader.name() == "GroupName") {
                               TempNodeGroup.NodeGroupName = StreamReader.readElementText();
                           }
                       }
                       /* ...and next... */
                       StreamReader.readNext();
                }
                NodeGroups.append(TempNodeGroup);
                qDebug()<< NodeGroups.size();
            }
            else if(StreamReader.name() == "beam_group") {
                qDebug() << "beam group found";
                while(!(StreamReader.tokenType() == QXmlStreamReader::EndElement &&
                           StreamReader.name() == "beam_group")) {
                       if(StreamReader.tokenType() == QXmlStreamReader::StartElement)
                       {
                           if(StreamReader.name() == "BeamAmount") {
                               TempBeamGroup.BeamAmount = StreamReader.readElementText().toInt();
                           }
                           if(StreamReader.name() == "BeamGroupID") {
                               TempBeamGroup.BeamGroupID = StreamReader.readElementText().toInt();
                           }
                           if(StreamReader.name() == "GroupName") {
                               TempBeamGroup.BeamGroupName = StreamReader.readElementText();
                           }
                           if(StreamReader.name() == "draw") {
                               TempBeamGroup.draw = StreamReader.readElementText().toInt();
                           }
                       }
                       /* ...and next... */
                       StreamReader.readNext();
                }
                BeamGroups.append(TempBeamGroup);
                qDebug()<< BeamGroups.size();
            }
            else if(StreamReader.name() == "beam_arguments") {
                qDebug() << "beam arguments found";
                TempBeamDefaults.KeyValuePairs.clear();
                while(!(StreamReader.tokenType() == QXmlStreamReader::EndElement &&
                           StreamReader.name() == "beam_arguments")) {
                       if(StreamReader.tokenType() == QXmlStreamReader::StartElement)
                       {
                           if(StreamReader.name() == "ID") {
                               TempBeamDefaults.ID_BeamDefs = StreamReader.readElementText().toInt();
                               qDebug() << "ID is " <<TempBeamDefaults.ID_BeamDefs;
                           }
                           if(StreamReader.name() == "name") {
                               TempBeamDefaults.Name = StreamReader.readElementText();
                               qDebug() << "name is " << TempBeamDefaults.Name;
                           }
                           if(StreamReader.name() == "red") {
                               if(TempBeamDefaults.RGB_Color.size()<3) TempBeamDefaults.RGB_Color.resize(3);
                               TempBeamDefaults.RGB_Color[0] = StreamReader.readElementText().toInt();
                               qDebug() << "red: " << TempBeamDefaults.RGB_Color[0];
                           }
                           if(StreamReader.name() == "green") {
                               if(TempBeamDefaults.RGB_Color.size()<3) TempBeamDefaults.RGB_Color.resize(3);
                               TempBeamDefaults.RGB_Color[1] = StreamReader.readElementText().toInt();
                               qDebug() << "green " << TempBeamDefaults.RGB_Color[1];
                           }
                           if(StreamReader.name() == "blue") {
                               if(TempBeamDefaults.RGB_Color.size()<3) TempBeamDefaults.RGB_Color.resize(3);
                               TempBeamDefaults.RGB_Color[2] = StreamReader.readElementText().toInt();
                               qDebug() << "blue " << TempBeamDefaults.RGB_Color[2];
                           }
                           if(StreamReader.name() == "argument") {
                               int size = TempBeamDefaults.KeyValuePairs.size();
                               TempBeamDefaults.KeyValuePairs.resize(size+1);

                               while(!(StreamReader.tokenType() == QXmlStreamReader::EndElement &&
                                          StreamReader.name() == "argument")) {
                                      if(StreamReader.tokenType() == QXmlStreamReader::StartElement)
                                      {
                                          if(StreamReader.name() == "name") {
                                              TempBeamDefaults.KeyValuePairs[size].name = StreamReader.readElementText();
                                          }
                                          if(StreamReader.name() == "value") {
                                              TempBeamDefaults.KeyValuePairs[size].value = StreamReader.readElementText();
                                          }
                                      }
                                      /* ...and next... */
                                      StreamReader.readNext();
                               }
                           }
                       }
                       /* ...and next... */
                       StreamReader.readNext();
                }
                BeamDefaults.append(TempBeamDefaults);
            }
            else if(StreamReader.name() == "hubwheels") {
                            qDebug() << "hubwheels found";
                            while(!(StreamReader.tokenType() == QXmlStreamReader::EndElement &&
                                       StreamReader.name() == "hubwheels")) {
                                   if(StreamReader.tokenType() == QXmlStreamReader::StartElement)
                                   {
                                       if(StreamReader.name() == "N1ide") {
                                           TempHubwheel.node1id = StreamReader.readElementText().toInt();
                                       }
                                       else if(StreamReader.name() == "N2ide") {
                                           TempHubwheel.node2id = StreamReader.readElementText().toInt();
                                       }
                                       else if(StreamReader.name() == "NAide") {
                                           TempHubwheel.nodeArmid = StreamReader.readElementText().toInt();
                                       }
                                       else if(StreamReader.name() == "NSide") {
                                           TempHubwheel.nodeSid = StreamReader.readElementText().toInt();
                                       }
                                       else if(StreamReader.name() == "N1Name") {
                                           TempHubwheel.node1 = StreamReader.readElementText();
                                       }
                                       else if(StreamReader.name() == "N2Name") {
                                           TempHubwheel.node2 = StreamReader.readElementText();
                                       }
                                       else if(StreamReader.name() == "NAName") {
                                           TempHubwheel.nodeArm = StreamReader.readElementText();
                                       }
                                       else if(StreamReader.name() == "NSName") {
                                           TempHubwheel.nodeS = StreamReader.readElementText();
                                       }
                                       else if(StreamReader.name() == "HWName") {
                                           TempHubwheel.name = StreamReader.readElementText();
                                       }
                                       else if(StreamReader.name() == "HWgroup") {
                                           TempHubwheel.group = StreamReader.readElementText();
                                       }
                                   }
                                   /* ...and next... */
                                   StreamReader.readNext();
                            }
                            AddHubWheel();
                        }
            else if(StreamReader.name() == "activenode")
            {
                ActiveNode = StreamReader.readElementText().toInt();
            }
            else if(StreamReader.name() == "activebeam")
            {
                ActiveBeam = StreamReader.readElementText().toInt();
            }
            else if(StreamReader.name() == "VehicleName")
            {
                VehicleName = StreamReader.readElementText();
            }
            else if(StreamReader.name() == "VehicleInGameName")
            {
                VehicleInGameName = StreamReader.readElementText();
            }

        }
        // do processing
    }
    if (StreamReader.hasError())
    {
        qDebug() << "error";
        qDebug() << StreamReader.errorString();
            // do error handling
    }

    file.close();

}

int NodeBeam::MoveToGroup(int NodeGlobalID, int GroupID)
{
    int nodeindex;
    if(GroupID < Nodes[NodeGlobalID].GroupID)
    {
        TempNode.blue = Nodes[NodeGlobalID].blue;
        TempNode.green = Nodes[NodeGlobalID].green;
        TempNode.GlobalID = Nodes[NodeGlobalID].GlobalID;
        TempNode.GroupID = Nodes[NodeGlobalID].GroupID;
        TempNode.locX = Nodes[NodeGlobalID].locX;
        TempNode.locY = Nodes[NodeGlobalID].locY;
        TempNode.locZ = Nodes[NodeGlobalID].locZ;
        TempNode.NodeName = Nodes[NodeGlobalID].NodeName;
        TempNode.Properties = Nodes[NodeGlobalID].Properties;
        TempNode.RoRNodeType = Nodes[NodeGlobalID].RoRNodeType;
        qDebug()<< TempNode.NodeName;

        Nodes.remove(NodeGlobalID);

        NodeGroups[TempNode.GroupID].NodeAmount = NodeGroups[TempNode.GroupID].NodeAmount - 1;
        NodeGroups[GroupID].NodeAmount++;

        nodeindex = 0;
        for(int i=0; i<(GroupID+1); i++)
        {
            nodeindex = nodeindex + NodeGroups[i].NodeAmount;
        }
        TempNode.GlobalID = nodeindex-1;
        TempNode.GroupID = GroupID;
        for(int i=(nodeindex-1);i<NodeGlobalID;i++)
        {
            Nodes[i].GlobalID++;
        }

        for(int i=0;i<Beams.size();i++)
        {
            if(Beams[i].Node1GlobalID<NodeGlobalID)
            {
                if(Beams[i].Node1GlobalID>= nodeindex-1)
                {
                    Beams[i].Node1GlobalID++;
                }
            }
            else if(Beams[i].Node1GlobalID==NodeGlobalID)
            {
                Beams[i].Node1GlobalID=nodeindex-1;
            }
            if(Beams[i].Node2GlobalID<NodeGlobalID)
            {
                if(Beams[i].Node2GlobalID>= nodeindex-1)
                {
                    Beams[i].Node2GlobalID++;
                }
            }
            else if(Beams[i].Node2GlobalID==NodeGlobalID)
            {
                Beams[i].Node2GlobalID=nodeindex-1;
            }
        }
        Nodes.insert((nodeindex-1),TempNode);


    }
    else if(GroupID == Nodes[NodeGlobalID].GroupID);
    else
    {
        TempNode.blue = Nodes[NodeGlobalID].blue;
        TempNode.green = Nodes[NodeGlobalID].green;
        TempNode.GlobalID = Nodes[NodeGlobalID].GlobalID;
        TempNode.GroupID = Nodes[NodeGlobalID].GroupID;
        TempNode.locX = Nodes[NodeGlobalID].locX;
        TempNode.locY = Nodes[NodeGlobalID].locY;
        TempNode.locZ = Nodes[NodeGlobalID].locZ;
        TempNode.NodeName = Nodes[NodeGlobalID].NodeName;
        TempNode.Properties = Nodes[NodeGlobalID].Properties;
        TempNode.RoRNodeType = Nodes[NodeGlobalID].RoRNodeType;

        Nodes.remove(NodeGlobalID);

        NodeGroups[TempNode.GroupID].NodeAmount = NodeGroups[TempNode.GroupID].NodeAmount - 1;
        NodeGroups[GroupID].NodeAmount++;

        nodeindex = 0;
        for(int i=0; i<(GroupID+1); i++)
        {
            nodeindex = nodeindex + NodeGroups[i].NodeAmount;
        }
        qDebug()<< "nodeindex " << nodeindex;

        TempNode.GroupID = GroupID;
        for(int i=NodeGlobalID;i<(nodeindex-1);i++)
        {
            Nodes[i].GlobalID--;
            qDebug() << "Node global id to " << Nodes[i].GlobalID;
        }
        TempNode.GlobalID = nodeindex-1;
        Nodes.insert((nodeindex-1),TempNode);

        for(int i=0;i<Beams.size();i++)
        {
            if(Beams[i].Node1GlobalID>NodeGlobalID)
            {
                if(Beams[i].Node1GlobalID<= nodeindex-1)
                {
                    Beams[i].Node1GlobalID--;
                }
            }
            else if(Beams[i].Node1GlobalID==NodeGlobalID)
            {
                Beams[i].Node1GlobalID=nodeindex-1;
            }
            if(Beams[i].Node2GlobalID>NodeGlobalID)
            {
                if(Beams[i].Node2GlobalID<= nodeindex-1)
                {
                    Beams[i].Node2GlobalID--;
                }
            }
            else if(Beams[i].Node2GlobalID==NodeGlobalID)
            {
                Beams[i].Node2GlobalID=nodeindex-1;
            }
        }

    }

    return nodeindex-1;
}

/* Save current vehicle project */
void NodeBeam::SaveAs(const QString &fileName)
{
    /* Try and open a file for output */
    QString suffixi = fileName.right(9);
    QString FileName1 = fileName;
    if(QString::compare(suffixi,".beamproj",Qt::CaseInsensitive) != 0) FileName1.append(".beamproj");

    QFile outputFile(FileName1);
    outputFile.open(QFile::WriteOnly);

    QXmlStreamWriter StreamWriter(&outputFile);
    StreamWriter.setAutoFormattingIndent(3);
    StreamWriter.setAutoFormatting(true);

    StreamWriter.writeStartDocument();

    StreamWriter.writeStartElement("BeamNG-Editor");

    if(Nodes.size()>0)
    {
        StreamWriter.writeStartElement("nodes");
        for(int i=0; i<Nodes.size(); i++)
        {
            StreamWriter.writeStartElement("node");
            StreamWriter.writeTextElement("NodeID",QString::number(Nodes[i].GlobalID));
            StreamWriter.writeTextElement("name",Nodes[i].NodeName);
            StreamWriter.writeTextElement("locx",QString::number(Nodes[i].locX));
            StreamWriter.writeTextElement("locy",QString::number(Nodes[i].locY));
            StreamWriter.writeTextElement("locz",QString::number(Nodes[i].locZ));
            StreamWriter.writeTextElement("group",QString::number(Nodes[i].GroupID));
            StreamWriter.writeTextElement("properties",Nodes[i].Properties);
            StreamWriter.writeTextElement("ror",QString::number(Nodes[i].RoRNodeType));
            StreamWriter.writeEndElement();
        }
        StreamWriter.writeEndElement();

        StreamWriter.writeStartElement("node_groups");

        for(int i=0; i<NodeGroups.size(); i++)
        {
            StreamWriter.writeStartElement("node_group");
            StreamWriter.writeTextElement("draw",QString::number(NodeGroups[i].draw));
            StreamWriter.writeTextElement("NodeAmount",QString::number(NodeGroups[i].NodeAmount));
            StreamWriter.writeTextElement("GroupID",QString::number(NodeGroups[i].NodeGroupID));
            StreamWriter.writeTextElement("GroupName",NodeGroups[i].NodeGroupName);
            StreamWriter.writeEndElement();
        }

        StreamWriter.writeEndElement();

    }
    if(Beams.size()>0)
    {
        StreamWriter.writeStartElement("beams");
        for(int i=0; i<Beams.size(); i++)
        {
            StreamWriter.writeStartElement("beam");
            StreamWriter.writeTextElement("beamdefsID",QString::number(Beams[i].BeamDefsID));
            StreamWriter.writeTextElement("beamgroup",QString::number(Beams[i].BeamGroupID));
            StreamWriter.writeTextElement("hasbeamdefs",QString::number(Beams[i].HasBeamDefs));
            StreamWriter.writeTextElement("N1ID",QString::number(Beams[i].Node1GlobalID));
            StreamWriter.writeTextElement("N1N",Beams[i].Node1Name);
            StreamWriter.writeTextElement("N2ID",QString::number(Beams[i].Node2GlobalID));
            StreamWriter.writeTextElement("N2N",Beams[i].Node2Name);
            StreamWriter.writeTextElement("properties",Beams[i].Properties);
            StreamWriter.writeEndElement();
        }
        StreamWriter.writeEndElement();

        StreamWriter.writeStartElement("beam_groups");

        for(int i=0; i<BeamGroups.size(); i++)
        {
            StreamWriter.writeStartElement("beam_group");
            StreamWriter.writeTextElement("BeamAmount", QString::number(BeamGroups[i].BeamAmount));
            StreamWriter.writeTextElement("BeamGroupID", QString::number(BeamGroups[i].BeamGroupID));
            StreamWriter.writeTextElement("GroupName",BeamGroups[i].BeamGroupName);
            StreamWriter.writeTextElement("draw", QString::number(BeamGroups[i].draw));
            StreamWriter.writeEndElement();
        }

        StreamWriter.writeEndElement();

    }

    for(int i=0; i<BeamDefaults.size(); i++)
    {
        StreamWriter.writeStartElement("beam_arguments");
        StreamWriter.writeTextElement("ID",QString::number(BeamDefaults[i].ID_BeamDefs));
        StreamWriter.writeTextElement("name",BeamDefaults[i].Name);
        StreamWriter.writeTextElement("red",QString::number(BeamDefaults[i].RGB_Color[0]));
        StreamWriter.writeTextElement("green",QString::number(BeamDefaults[i].RGB_Color[1]));
        StreamWriter.writeTextElement("blue",QString::number(BeamDefaults[i].RGB_Color[2]));
        for(int i2=0; i2<BeamDefaults[i].KeyValuePairs.size();i2++)
        {
            StreamWriter.writeStartElement("argument");
            StreamWriter.writeTextElement("name",BeamDefaults[i].KeyValuePairs[i2].name);
            StreamWriter.writeTextElement("value",BeamDefaults[i].KeyValuePairs[i2].value);
            StreamWriter.writeEndElement();
        }
        StreamWriter.writeEndElement();

    }

    for(int i=0; i<Hubwheels.size(); i++)
    {
    StreamWriter.writeStartElement("hubwheels");
    StreamWriter.writeTextElement("N1ide",QString::number(Hubwheels[i].node1id));
    StreamWriter.writeTextElement("N2ide",QString::number(Hubwheels[i].node2id));
    StreamWriter.writeTextElement("NAide",QString::number(Hubwheels[i].nodeArmid));
    StreamWriter.writeTextElement("NSide",QString::number(Hubwheels[i].nodeSid));
    StreamWriter.writeTextElement("N1Name",Hubwheels[i].node1);
    StreamWriter.writeTextElement("N2Name",Hubwheels[i].node2);
    StreamWriter.writeTextElement("NAName",Hubwheels[i].nodeArm);
    StreamWriter.writeTextElement("NSName",Hubwheels[i].nodeS);
    StreamWriter.writeTextElement("HWName",Hubwheels[i].name);
    StreamWriter.writeTextElement("HWgroup",Hubwheels[i].group);
    StreamWriter.writeEndElement();
    }

    StreamWriter.writeTextElement("activenode", QString::number(ActiveNode));
    StreamWriter.writeTextElement("activebeam", QString::number(ActiveBeam));
    StreamWriter.writeTextElement("VehicleName", VehicleName);
    StreamWriter.writeTextElement("VehicleInGameName", VehicleInGameName);

    StreamWriter.writeEndElement();
    StreamWriter.writeEndDocument();

    outputFile.close();


}

/* Select nodes by their location */
//Selects all nodes inside the box that is between point 0 and 1
void NodeBeam::SelectNodesLoc(float x0, float y0, float z0, float x1, float y1, float z1)
{
    SelectedNodes2.clear();
    for(int i=0; i<Nodes.size(); i++)
    {
        if((Nodes[i].locX>= x0) && (Nodes[i].locX<= x1))
        {
            if((Nodes[i].locY>= y0) && (Nodes[i].locY<= y1))
            {
                if((Nodes[i].locZ>= z0) && (Nodes[i].locZ<= z1))
                {
                    SelectedNodes2.append(Nodes[i].GlobalID);
                }
            }
        }
    }
    qDebug()<< SelectedNodes2;

}

void NodeBeam::ReadJBeamTree(QString fileName)
{
    //JBeam jbeamtree("test3.jbeam");
    JBeam jbeamtree(fileName);
    jbeamtree.CurrPosReset();
    jbeamtree.GoToNext();
    bool ok=1;

    //Loop trough every slot in the tree
    while(ok)
    {
        ok = jbeamtree.GoToNextSlot();

        /* Finding content from the slot */
        qDebug()<< "  ";
        qDebug() << "Slot is"<< jbeamtree.CurrentPos->key;

        jbeamtree.SetStopPointHere();
        while(jbeamtree.GoToNextUntilStop()!=4)
        {
            if(jbeamtree.CurrentPos->key == "nodes")
            {
                qDebug()<<"NODES FOUND!!!";
                jbeamtree.SetStopPointHere_List();
                while(jbeamtree.GoToNextUntilStop_List()!=4)
                {
                    if(jbeamtree.CurrentPos->contenttype==jbeamtree.LIST_CONTAINER)
                    {
                        jbeamtree.SetStopPointHere();
                        int indexi=0;
                        bool notfail = 0;
                        while(jbeamtree.GoToNextUntilStop()!=4)
                        {
                            if(indexi==0) TempNode.NodeName = jbeamtree.CurrentPos->value;
                            else if(indexi==1) TempNode.locX = jbeamtree.CurrentPos->value.toFloat(&notfail);
                            else if(indexi==2) TempNode.locY = jbeamtree.CurrentPos->value.toFloat(&notfail);
                            else if(indexi==3) TempNode.locZ = jbeamtree.CurrentPos->value.toFloat(&notfail);
                            else if(indexi>3) qDebug()<< "argument is " << jbeamtree.CurrentPos->key << " : "<< jbeamtree.CurrentPos->value;
                            indexi++;
                        }
                        if(notfail==1) AddNode();
                    }
                    else
                    {
                        qDebug()<< "argument is " << jbeamtree.CurrentPos->key << " : "<< jbeamtree.CurrentPos->value;
                    }
                }

            }
            else if(jbeamtree.CurrentPos->key == "beams")
            {
                qDebug()<<"BEAMS FOUND!!!";
                NewBeamArguments();
                bool Arguments=1;
                jbeamtree.SetStopPointHere_List();
                while(jbeamtree.GoToNextUntilStop_List()!=4)
                {
                    if(jbeamtree.CurrentPos->contenttype==jbeamtree.LIST_CONTAINER)
                    {
                        jbeamtree.SetStopPointHere();
                        int indexi=0;
                        while(jbeamtree.GoToNextUntilStop()!=4)
                        {
                            if(indexi==0)
                            {
                                TempBeam.Node1Name = jbeamtree.CurrentPos->value;
                                TempBeam.Node1GlobalID = FindBeamNodeByName(jbeamtree.CurrentPos->value);
                            }
                            else if(indexi==1)
                            {
                                TempBeam.Node2Name = jbeamtree.CurrentPos->value;
                                TempBeam.Node2GlobalID = FindBeamNodeByName(jbeamtree.CurrentPos->value);
                            }
                            else if(indexi>3) qDebug()<< "argument is " << jbeamtree.CurrentPos->key << " : "<< jbeamtree.CurrentPos->value;
                            indexi++;
                        }
                        if(!((TempBeam.Node1GlobalID == 99999) && (TempBeam.Node2GlobalID == 99999)))
                        {
                            TempBeam.BeamDefsID = BeamDefaults[BeamDefaults.size()-1].ID_BeamDefs;
                            TempBeam.HasBeamDefs=1;
                            AddBeamT();
                            Arguments=1;
                        }
                    }
                    else
                    {
                        if(Arguments == 1)
                        {
                            NewBeamArgumentsFromPrevious();
                            Arguments=0;
                        }
                        qDebug()<< "argument is " << jbeamtree.CurrentPos->key << " : "<< jbeamtree.CurrentPos->value;
                        ModifyBeamArgument(jbeamtree.CurrentPos->key, jbeamtree.CurrentPos->value, BeamDefaults.size()-1);
                    }
                }
            }

        }
    }

}

/* Write in specified SLOT in JBeamTree */
void NodeBeam::WriteInJBeamTree(QString slotname)
{
    //First loading the editable jbeam
    JBeam jbeamtree("test3.jbeam");
    jbeamtree.CurrPosReset();
    jbeamtree.GoToNext();
    bool ok=1;

    //Find the correct slot
    while(ok)
    {
        ok = jbeamtree.GoToNextSlot();

        /* Finding content from the slot */
        qDebug()<< "  ";
        qDebug() << "Slot is"<< jbeamtree.CurrentPos->key;

        if(jbeamtree.CurrentPos->key == slotname)
        {
            qDebug() << "this is the slot;";
            jbeamtree.SetStopPointHere();
            while(jbeamtree.GoToNextUntilStop()!=4)
            {
                if(jbeamtree.CurrentPos->key == "nodes")
                {

                }
            }
        }


    }
    jbeamtree.WriteFile("testi.jbeam");
}

/* When node name is known, but node global id is not known, this function finds it */
int NodeBeam::FindNodeByName(QString nodename)
{
    int result = -1;
    for(int i=0; i<Nodes.size(); i++)
    {
        if(Nodes[i].NodeName == nodename)
        {
            result = i;
        }
    }
    return result;
}

int NodeBeam::FindBeamNodeByName(QString nodename)
{
    int nodenumber = FindNodeByName(nodename);
    if(nodenumber == -1) nodenumber = 99999;
    return nodenumber;
}

void NodeBeam::NewBeamArguments()
{
    QString name = "properties_";
    name.append(QString::number(BeamDefaults.size()));
    BeamDefaults.resize(BeamDefaults.size()+1);
    BeamDefaults[BeamDefaults.size()-1].ID_BeamDefs = BeamDefaults.size()-1;
    BeamDefaults[BeamDefaults.size()-1].Name = name;
    BeamDefaults[BeamDefaults.size()-1].RGB_Color.resize(3);
    BeamDefaults[BeamDefaults.size()-1].RGB_Color[0] = BeamDefaults.size()*5;
    BeamDefaults[BeamDefaults.size()-1].RGB_Color[1] = BeamDefaults.size()*5;
    BeamDefaults[BeamDefaults.size()-1].RGB_Color[2] = BeamDefaults.size()*5;
}

void NodeBeam::NewBeamArgumentsFromPrevious()
{
    qDebug()<<"huhuu";
    KeyValue Argument;
    NewBeamArguments();
    if(BeamDefaults.size()>1)
    {
        int PreviousSet = BeamDefaults.size()-2;
        for(int i=0; i<BeamDefaults[PreviousSet].KeyValuePairs.size();i++)
        {
            qDebug()<<i;
            Argument.name = BeamDefaults[PreviousSet].KeyValuePairs[i].name;
            Argument.value = BeamDefaults[PreviousSet].KeyValuePairs[i].value;
            BeamDefaults[BeamDefaults.size()-1].KeyValuePairs.append(Argument);
            if(i>200) break;
        }
    }
}

void NodeBeam::AddBeamArgument(QString key, QString value)
{
    KeyValue Argument;
    Argument.name = key;
    Argument.value = value;

    if(!(BeamDefaults.size()>0)) NewBeamArguments();

    BeamDefaults[BeamDefaults.size()-1].KeyValuePairs.append(Argument);
}

void NodeBeam::ModifyBeamArgument(QString key, QString value, int BeamdefsSet)
{
    bool modified = 0;
    for(int i=0; i<BeamDefaults[BeamdefsSet].KeyValuePairs.size();i++)
    {
        if(BeamDefaults[BeamdefsSet].KeyValuePairs[i].name == key)
        {
            BeamDefaults[BeamdefsSet].KeyValuePairs[i].value = value;
            modified=1;
        }
    }
    if(modified == 0)
    {
        KeyValue Argument;
        Argument.name = key;
        Argument.value = value;

        if(!(BeamDefaults.size()>0)) NewBeamArguments();

        BeamDefaults[BeamDefaults.size()-1].KeyValuePairs.append(Argument);
    }
}

QString NodeBeam::NewNodeName(QString prefix)
{
    QString name = "";
    int prefixnumber = CheckPrefixExistance(prefix);
    if(prefixnumber > -1)
    {
        bool doubles = 1;
        while(doubles)
        {
            name = NodeNamePrefixes[prefixnumber];
            name.append(QString::number(NodeNameCalculators[prefixnumber]));
            NodeNameCalculators[prefixnumber]++;

            if(CheckDoubles(name)==0) doubles=0;
        }
    }
    else
    {
        NewNodeNamePrefix(prefix);
        prefixnumber=NodeNamePrefixes.size()-1;

        bool doubles = 1;
        while(doubles)
        {
            name = NodeNamePrefixes[prefixnumber];
            name.append(QString::number(NodeNameCalculators[prefixnumber]));
            NodeNameCalculators[prefixnumber]++;

            if(CheckDoubles(name)==0) doubles=0;
        }
    }

    return name;
}

void NodeBeam::NewNodeNamePrefix()
{
    QString prefix = "";
    prefix.append(char(DefaultPrefixCalculator+97));
    DefaultPrefixCalculator++;

    NodeNamePrefixes.resize(NodeNamePrefixes.size()+1);
    NodeNameCalculators.resize(NodeNameCalculators.size()+1);
    NodeNamePrefixes[NodeNamePrefixes.size()-1] = prefix;
    NodeNameCalculators[NodeNameCalculators.size()-1] = 0;
}

bool NodeBeam::NewNodeNamePrefix(QString prefix)
{

    NodeNamePrefixes.resize(NodeNamePrefixes.size()+1);
    NodeNameCalculators.resize(NodeNameCalculators.size()+1);
    NodeNamePrefixes[NodeNamePrefixes.size()-1] = prefix;
    NodeNameCalculators[NodeNameCalculators.size()-1] = 0;

}

bool NodeBeam::CheckDoubles(QString NameSuggestion)
{
    bool used = 0;
    for(int i=0; i<Nodes.size();i++)
    {
        if(Nodes[i].NodeName == NameSuggestion) used = 1;
    }
    return used;
}

int NodeBeam::CheckPrefixExistance(QString prefix)
{
    int exists=-1;
    for(int i=0; i<NodeNamePrefixes.size();i++)
    {
        if(NodeNamePrefixes[i]==prefix) exists = i;
    }
    return exists;

}

/* HubWheels */
void NodeBeam::AddHubWheel()
{
    Hubwheels.resize(Hubwheels.size()+1);
    Hubwheels[Hubwheels.size()-1].name = NewNodeName("HubW");
    Hubwheels[Hubwheels.size()-1].group = Hubwheels[Hubwheels.size()-1].name;

    Hubwheels[Hubwheels.size()-1].node1id = TempHubwheel.node1id;
    Hubwheels[Hubwheels.size()-1].node1 = Nodes[TempHubwheel.node1id].NodeName;

    Hubwheels[Hubwheels.size()-1].node2id = TempHubwheel.node2id;
    Hubwheels[Hubwheels.size()-1].node2 = Nodes[TempHubwheel.node2id].NodeName;

    Hubwheels[Hubwheels.size()-1].nodeSid = TempHubwheel.nodeSid;
    Hubwheels[Hubwheels.size()-1].nodeS = Nodes[TempHubwheel.nodeSid].NodeName;

    Hubwheels[Hubwheels.size()-1].nodeArmid = TempHubwheel.nodeArmid;
    Hubwheels[Hubwheels.size()-1].nodeArm = Nodes[TempHubwheel.nodeArmid].NodeName;

    Hubwheels[Hubwheels.size()-1].wheelDir = 1;   
    Hubwheels[Hubwheels.size()-1].radius = 0.35;
    Hubwheels[Hubwheels.size()-1].arguments = 0;
    qDebug() << "Added hubwheel";
    qDebug() << "node1" << Hubwheels[Hubwheels.size()-1].node1;
    qDebug() << "node2" <<  Hubwheels[Hubwheels.size()-1].node2;
    qDebug() << "nodeArm" <<  Hubwheels[Hubwheels.size()-1].nodeArm;
    qDebug() << "nodeS" <<  Hubwheels[Hubwheels.size()-1].nodeS;
    qDebug() << "wheelname" <<  Hubwheels[Hubwheels.size()-1].name;

}

//For openGL view, precalculated rotation of the wheel
void NodeBeam::CalcHubWheelRotation(int wheelid)
{
    //Calculate hubwheel rotation
    double pii = 2*qAsin(1);
    qDebug() << "pii is " << pii;

    int hnode1 = Hubwheels[wheelid].node1id;
    int hnode2 = Hubwheels[wheelid].node2id;

    QVector3D wheelvec; //Vector from wheel node1 to wheel node2
    wheelvec.setX(Nodes[hnode2].locX-Nodes[hnode1].locX);
    wheelvec.setY(Nodes[hnode2].locY-Nodes[hnode1].locY);
    wheelvec.setZ(Nodes[hnode2].locZ-Nodes[hnode1].locZ);

    Hubwheels[wheelid].RotationX = (360/(2*pii))*qAsin(wheelvec.z()/calculate_length(wheelvec.y(), wheelvec.z()));
    qDebug() << "Rotation X is " << Hubwheels[wheelid].RotationX;
    double length_zplane = calculate_length(wheelvec.y(),wheelvec.z());
    double wheelnodes_dist = calculate_length(wheelvec.x(),length_zplane);
    double theta = (360/(2*pii))*qAcos(length_zplane/wheelnodes_dist);
    Hubwheels[wheelid].RotationZ = theta;

    qDebug() << "theta is " << theta;

    if(wheelvec.y()<0) Hubwheels[wheelid].RotationX= 180-Hubwheels[wheelid].RotationX;
    if(wheelvec.x()>0) Hubwheels[wheelid].RotationZ= -Hubwheels[wheelid].RotationZ;

    //Checking if it's NaN
    if(Hubwheels[wheelid].RotationX != Hubwheels[wheelid].RotationX) Hubwheels[wheelid].RotationX = 0;

    //Calculating width
    Hubwheels[wheelid].width = wheelnodes_dist;
}

//Calculate length
double NodeBeam::calculate_length(double x, double y)
{
    double length = sqrt((x*x)+(y*y));
    return length;
}