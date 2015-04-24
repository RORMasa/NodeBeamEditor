#include "NodeBeam.h"
#include <QDir>
#include <QDebug>
#include <QApplication>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

void NodeBeam::Load_ListTypes()
{
    qDebug() << "Loading JBEAM List types";

    QDir jbeamfolder;
    jbeamfolder.current();
    jbeamfolder.cd(tr("jbeam"));

    QStringList filters;
    filters << "*.json" << "*.JSON";
    jbeamfolder.setNameFilters(filters);

    QStringList files = jbeamfolder.entryList();
    //qDebug() << files;

    for(int i2=0; i2<files.size();i2++)
    {
        QString filename = jbeamfolder.path() + "/";
        filename+=files.at(i2);
        //qDebug() << filename;
        if (!filename.isEmpty())
        {
            QFile file(filename);
            if (!file.open(QFile::ReadOnly | QFile::Text));
            else
            {
                //Read file
                QTextStream in(&file);
                #ifndef TESTING
                #ifndef QT_NO_CURSOR
                    QApplication::setOverrideCursor(Qt::WaitCursor);
                #endif
                #endif
                QString FileContents = in.readAll();
                #ifndef TESTING
                #ifndef QT_NO_CURSOR
                    QApplication::restoreOverrideCursor();
                #endif
                #endif
                file.close();

                //Parse file
                if(FileContents.size()>0)
                {
                    QByteArray FileContentsBytearray;
                    FileContentsBytearray = FileContents.toUtf8();

                    QJsonParseError ParseError;
                    QJsonDocument JbeamPart = QJsonDocument::fromJson(FileContentsBytearray, &ParseError);

                    //Error messages
                    //qDebug() << "JSON parsing result: " << ", " << ParseError.errorString();

                    //Convert Json document to json object
                    QJsonObject JbeamPartObject = JbeamPart.object();

                    JBEAM_ListType ListType;
                    int validated = 0;
                    bool ok=0;

                    //Parse JSON object
                    if(!JbeamPartObject.isEmpty())
                    {
                        if(JbeamPartObject.contains("keyword"))
                        {
                            QJsonValue value = JbeamPartObject.value("keyword");
                            if(value.isString()) validated++;
                            ListType.keyword = value.toString();
                        }
                        if(JbeamPartObject.contains("drawtype"))
                        {
                            QJsonValue value = JbeamPartObject.value("drawtype").toString();
                            if(value.isString())
                            {
                                QString drawtype = value.toString();
                                if(drawtype == "none")
                                {
                                    ListType.drawtype = ListType.NONE;
                                    validated++;
                                }
                                else if(drawtype == "beam")
                                {
                                    ListType.drawtype = ListType.BEAM;
                                    validated++;
                                }
                                else if(drawtype == "wheel")
                                {
                                    ListType.drawtype = ListType.WHEEL;
                                    validated++;
                                }
                                else if(drawtype == "triangle")
                                {
                                    ListType.drawtype = ListType.TRIANGLE;
                                    validated++;
                                }
                            }

                        }
                        if(JbeamPartObject.contains("nodeamount"))
                        {
                            ListType.nodeamount = JbeamPartObject.value("nodeamount").toInt(0);
                        }
                        if(JbeamPartObject.contains("tableheader"))
                        {
                            ListType.JBEAM_tableheader = JbeamPartObject.value("tableheader").toString();
                            if(JbeamPartObject.value("tableheader").isString()) validated++;
                        }
                        if(JbeamPartObject.contains("template"))
                        {
                            ListType.JBEAM_template = JbeamPartObject.value("template").toString();
                            if(JbeamPartObject.value("template").isString())
                            {
                                QStringList template_placeholders = ListType.JBEAM_template.split(",");
                                for(int i3=0; i3<template_placeholders.size(); i3++)
                                {
                                    if(template_placeholders.at(i3).contains("%"))
                                    {
                                        ListType.node_positions.append(i3);
                                    }
                                }
                                if(ListType.node_positions.size() == ListType.nodeamount)
                                {
                                    validated++;
                                }

                            }
                        }
                        if(JbeamPartObject.contains("nodenames"))
                        {
                            QJsonArray names = JbeamPartObject.value("nodenames").toArray();
                            for(int n=0; n<names.size();n++)
                            {
                                if(names.at(n).isString())
                                {
                                    ListType.nodenames.append(names.at(n).toString());
                                }
                            }
                            if(ListType.nodenames.size() == ListType.nodeamount) validated++;
                        }
                        if(JbeamPartObject.contains("draworder"))
                        {
                            QJsonArray drawo = JbeamPartObject.value("draworder").toArray();
                            for(int n=0; n<drawo.size();n++)
                            {
                                if(drawo.at(n).isDouble())
                                {
                                    ListType.draworder.append(drawo.at(n).toInt(n));
                                }
                            }
                            if(ListType.drawtype==3)
                            {
                                if(ListType.draworder.size() == 3) validated++;;
                            }
                            else if(ListType.drawtype==2)
                            {
                                if(ListType.draworder.size() == 2) validated++;;
                            }
                            else if(ListType.drawtype==1)
                            {
                                if(ListType.draworder.size() == 2) validated++;;
                            }
                            else if(ListType.drawtype==0) validated++;
                        }

                    }
                    if(validated == 6)
                    {
                        ListTypes.append(ListType);
                        qDebug() << "[OK] Parsing JBEAM component " << ListType.keyword;
                    }
                    else
                    {
                        qDebug() << filename << " JSON parsing result: " << ", " << ParseError.errorString();
                        qDebug() << "[FAIL] Parsing JBEAM component " << ListType.keyword;
                    }
                }
            }
        }
    }

}
JBEAM_ListType::JBEAM_ListType()
{

}

JBEAM_ListType::~JBEAM_ListType()
{
    this->contaier.clear();
    this->draworder.clear();
    this->node_positions.clear();
    this->nodenames.clear();
}

bool JBEAM_ListType::Add(QVector <int> item)
{
    if(item.size() == this->nodeamount)
    {
        this->contaier.append(item);
        return true;
    }
    else return false;
}

//Node is removed, remove all instances that connect to removed node.
void JBEAM_ListType::NodeRemoved(int nodeid)
{
    for(int i=0; i<this->contaier.size();i++)
    {
        for(int i2=0; i2<contaier.at(i).size(); i2++)
        {
            if(contaier.at(i).at(i2) == nodeid)
            {
                contaier.removeAt(i);
                i--;
                qDebug() << "List type container removed " << i;
                break;
            }
        }
    }

    //Lower global ID's
    for(int i=0; i<this->contaier.size();i++)
    {
        for(int i2=0; i2<contaier.at(i).size(); i2++)
        {
            if(contaier.at(i).at(i2) >= nodeid)
            {
                contaier[i][i2] = contaier[i][i2]-1;
            }
        }
    }

}
