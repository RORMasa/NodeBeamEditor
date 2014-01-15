#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"
#include "glwidgetOrtho.h"
#include <QtWidgets>
#include <QtOpenGL>
#include "NodeBeam.h"
#include <newprojectdialog.h>
#include "Generators/ChassisGenerator.h"
#include "Generators/tracksgenerator.h"
#include "inputdialog.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    glWidget = new GLWidget;    
    glWidgetO = new GLWidgetOrtho;
    BeamProperties = new BeamDefaultsDialog;
    HubWheelProperties = new BeamDefaultsDialog;

    OpenGLViews = new QTabWidget;
    OpenGLViews->addTab(glWidget,tr("3D View"));
    OpenGLViews->addTab(glWidgetO,tr("Orthographic View"));

    ui->horizontalLayout->addWidget(OpenGLViews);

    //Active working file
    CurrentNodeBeam = new NodeBeam;
    glWidget->setNBPointer(CurrentNodeBeam);
    glWidgetO->setNBPointer(CurrentNodeBeam);

    BeamProperties->setNBPointer(&CurrentNodeBeam->BeamDefaults);
    BeamProperties->argumenttype = BeamProperties->BEAM_ARGUMENTS;

    HubWheelProperties->setNBPointer(&CurrentNodeBeam->HWArguments);
    HubWheelProperties->argumenttype = HubWheelProperties->HUBWHEEL_ARGUMENTS;

    setWindowTitle(tr("BeamNG Editor - New NodeBeam"));

    //Settings for the application
    AppSettings = new settings;
    glWidget->backgroundcolor[0] = (AppSettings->readsetting("bg_color_r").toInt())/255.0f;
    glWidget->backgroundcolor[1] = (AppSettings->readsetting("bg_color_g").toInt())/255.0f;
    glWidget->backgroundcolor[2] = (AppSettings->readsetting("bg_color_b").toInt())/255.0f;
    glWidgetO->backgroundcolor[0] = (AppSettings->readsetting("bg_color_r").toInt())/255.0f;
    glWidgetO->backgroundcolor[1] = (AppSettings->readsetting("bg_color_g").toInt())/255.0f;
    glWidgetO->backgroundcolor[2] = (AppSettings->readsetting("bg_color_b").toInt())/255.0f;

    QObject::connect(glWidget, SIGNAL(NodeBeamUpdated()), this, SLOT(MainNodeBeamUpdated()));
    QObject::connect(glWidgetO, SIGNAL(NodeBeamUpdated()), this, SLOT(MainNodeBeamUpdated()));
    QObject::connect(glWidget, SIGNAL(NodeBeamUpdated()), glWidgetO, SLOT(updateGL()));
    QObject::connect(glWidgetO, SIGNAL(NodeBeamUpdated()), glWidget, SLOT(updateGL()));
    QObject::connect(this, SIGNAL(ZoomChanged()), glWidget, SLOT(setZoom()));
    QObject::connect(glWidgetO, SIGNAL(SelectionUpdated()), this, SLOT(UpdateSelection()));
    QObject::connect(AppSettings, SIGNAL(SettingsUpdated()), this, SLOT(SettingsUpdated()));


    CurrentBeamGroupi=0;
    RefreshLock=0;

    //Right click menu for beams list
    ui->treeWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->treeWidget_2, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu_Beams(const QPoint&)));

    //Install event filter
    qApp->installEventFilter(this);

    setFocusPolicy(Qt::StrongFocus);

    //Views combobox
    ui->comboBox_3_views->addItem("Top");
    ui->comboBox_3_views->addItem("Bottom");
    ui->comboBox_3_views->addItem("Front");
    ui->comboBox_3_views->addItem("Back");
    ui->comboBox_3_views->addItem("Right");
    ui->comboBox_3_views->addItem("Left");

    //Show axis arrows
    ui->checkBox_2->setChecked(1);

    //Change glwidget cursors to crosscursor
    QCursor cursor;
    cursor.setShape(Qt::CrossCursor);
    glWidget->setCursor(cursor);
    glWidgetO->setCursor(cursor);

    //Set slider range
    ui->horizontalSlider->setRange(0, 100);
    ui->horizontalSlider->setPageStep(2);
    ui->horizontalSlider->setValue(100);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete glWidget;
    delete glWidgetO;
    delete OpenGLViews;
    delete AppSettings;
    //delete CurrentNodeBeam;
    //delete BeamProperties;


}

void MainWindow::ShowContextMenu_Beams(const QPoint& position)
{
    QPoint globalPos = ui->treeWidget_2->mapToGlobal(position);

    QMenu BeamsMenu;
    BeamsMenu.addAction("Find beam");

    QAction* CurrentItem = BeamsMenu.exec(globalPos);
    if (CurrentItem)
    {
        if(CurrentItem->text() == "Find beam")
        {
            InputDialog inputdialog;
            inputdialog.FindBeam();
            inputdialog.exec();
            if(!inputdialog.ValuesOk)
            {
                QMessageBox msgBox;
                msgBox.setText("You must give two nodes that form the beam.");
                msgBox.exec();
            }
            else
            {
                for(int i = 0; i< CurrentNodeBeam->Beams.size(); i++)
                {
                    if(CurrentNodeBeam->Beams[i].Node1Name == inputdialog.valuesString[0])
                    {
                        if(CurrentNodeBeam->Beams[i].Node2Name == inputdialog.valuesString[1])
                        {
                            //Found it
                            qDebug() << "This beam exists";
                        }
                    }
                    else if(CurrentNodeBeam->Beams[i].Node2Name == inputdialog.valuesString[0])
                    {
                        if(CurrentNodeBeam->Beams[i].Node1Name == inputdialog.valuesString[1])
                        {
                            //Found it
                            qDebug() << "This beam exists";
                        }
                    }
                }

            }
        }

    }
}


//Start new file
void MainWindow::on_actionNew_triggered()
{
    delete CurrentNodeBeam;
    CurrentNodeBeam = new NodeBeam;
    glWidget->setNBPointer(CurrentNodeBeam);
    glWidgetO->setNBPointer(CurrentNodeBeam);
    glWidget->updateGL();
    glWidgetO->updateGL();
    ui->treeWidget->clear();
    ui->treeWidget_2->clear();

    NewProjectDialog Dialog;
    Dialog.exec();
}

/* File menu / Import RoR NB triggered */
void MainWindow::on_actionImport_Rigs_of_Rods_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        CurrentNodeBeam->ImportNBFile(fileName);

    glWidget->setNBPointer( CurrentNodeBeam);

    MainNodeBeamUpdated();
}

/* File menu / Export to BeamNG triggered */
void MainWindow::on_actionExport_to_BeamNG_triggered()
{
//    QTime Timer;

//    QString fileName = QFileDialog::getSaveFileName(this, "Export to JBeam",QDir::currentPath(),"BeamNG file (*.jbeam);;All files (*.*)",0,QFileDialog::DontUseNativeDialog);
//    if (!fileName.isEmpty())
//    {
//        Timer.start();
//        CurrentNodeBeam->ExportBeamNG(fileName);

//        int result = Timer.elapsed();
//        QString resultt = "File exporting finished in ";
//        resultt.append(QString::number(result));
//        resultt.append(" ms !");
//        ui->statusBar->showMessage(resultt);
//    }

    CurrentNodeBeam->WriteInJBeamTree("pickup_fueltank");


//tosi vanha    QFileDialog SaveWindow;
//    SaveWindow.setDirectory(QDir::currentPath());
//    SaveWindow.setDefaultSuffix("jbeam");
//    SaveWindow.setAcceptMode(QFileDialog::AcceptSave);
//    SaveWindow.setViewMode(QFileDialog::Detail);
//    SaveWindow.exec();

}

/* File menu / Export to RoR triggered */
void MainWindow::on_actionExport_to_Rigs_of_Rods_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export to Rigs of Rods",QDir::currentPath(),"RoR file (*.truck);;All files (*.*)",0,QFileDialog::DontUseNativeDialog);
    if (!fileName.isEmpty())
        CurrentNodeBeam->ExportRoR(fileName);
}

//Nodes in tree widget to boxes below.
void MainWindow::on_treeWidget_itemSelectionChanged()
{
    if(ui->treeWidget->currentItem()!=NULL)
    {
        if(ui->treeWidget->currentItem()->text(2) == "Group")
        {

        }
        else
        {
            if(CurrentNodeBeam->Nodes.size()>ui->treeWidget->currentItem()->text(1).toInt())
            {
                int TempNodeID = ui->treeWidget->currentItem()->text(1).toInt();
                ui->lineEdit_2->setText(CurrentNodeBeam->Nodes[TempNodeID].Properties);
                ui->lineEdit_3->setText(QString::number(CurrentNodeBeam->Nodes[TempNodeID].locX));
                ui->lineEdit_4->setText(QString::number(CurrentNodeBeam->Nodes[TempNodeID].locY));
                ui->lineEdit_5->setText(QString::number(CurrentNodeBeam->Nodes[TempNodeID].locZ));
                ui->lineEdit_nodename->setText(CurrentNodeBeam->Nodes[TempNodeID].NodeName);
                ui->label_6nodeID->setText(QString::number(CurrentNodeBeam->Nodes[TempNodeID].GlobalID));
                CurrentNodeBeam->ActiveNode = TempNodeID;
                ui->pushButton_5->setEnabled(1);
            }
        }
    }

    if(!RefreshLock)
    {
        glWidget->updateGL();
        glWidgetO->updateGL();


        //qDebug() << "Updating SELECTION";
        CurrentNodeBeam->SelectedNodes.clear();
        for(int i=0; i<ui->treeWidget->selectedItems().size(); i++)
        {
            CurrentNodeBeam->SelectedNodes.append(ui->treeWidget->selectedItems()[i]->text(1).toInt());
            //qDebug() << ui->treeWidget->selectedItems()[i]->text(1);
            //qDebug() << CurrentNodeBeam->SelectedNodes[i];
        }
    }


}
//Beams tree widget
void MainWindow::on_treeWidget_2_itemSelectionChanged()
{
    if(!RefreshLock)
    {
    if(ui->treeWidget_2->currentItem()->text(2) == "Group")
    {

    }
    else
    {
        if(CurrentNodeBeam->Beams.size()>ui->treeWidget_2->currentItem()->text(4).toInt())
        {
        ui->lineEdit_6->setText(CurrentNodeBeam->Beams[ui->treeWidget_2->currentItem()->text(4).toInt()].Node1Name);
        ui->lineEdit_7->setText(CurrentNodeBeam->Beams[ui->treeWidget_2->currentItem()->text(4).toInt()].Node2Name);
        ui->lineEdit_8->setText(CurrentNodeBeam->Beams[ui->treeWidget_2->currentItem()->text(4).toInt()].Properties);
        CurrentNodeBeam->ActiveBeam = ui->treeWidget_2->currentItem()->text(4).toInt();

        //For testing purposes, line where's beam information...
        QString BeamIDs;
        BeamIDs.clear();
        BeamIDs.append(QString::number(CurrentNodeBeam->Beams[ui->treeWidget_2->currentItem()->text(4).toInt()].Node1GlobalID));
        BeamIDs.append(", ");
        BeamIDs.append(QString::number(CurrentNodeBeam->Beams[ui->treeWidget_2->currentItem()->text(4).toInt()].Node2GlobalID));
        BeamIDs.append(", ");
        if(CurrentNodeBeam->Beams[ui->treeWidget_2->currentItem()->text(4).toInt()].Node1GlobalID < CurrentNodeBeam->Nodes.size())
        {
            BeamIDs.append(CurrentNodeBeam->Nodes[CurrentNodeBeam->Beams[ui->treeWidget_2->currentItem()->text(4).toInt()].Node1GlobalID].NodeName);
        }
        else
        {
            BeamIDs.append(CurrentNodeBeam->Beams[ui->treeWidget_2->currentItem()->text(4).toInt()].Node1Name);
        }
        BeamIDs.append(", ");
        if(CurrentNodeBeam->Beams[ui->treeWidget_2->currentItem()->text(4).toInt()].Node2GlobalID < CurrentNodeBeam->Nodes.size())
        {
            BeamIDs.append(CurrentNodeBeam->Nodes[CurrentNodeBeam->Beams[ui->treeWidget_2->currentItem()->text(4).toInt()].Node2GlobalID].NodeName);
        }
        else
        {
            BeamIDs.append(CurrentNodeBeam->Beams[ui->treeWidget_2->currentItem()->text(4).toInt()].Node2Name);
        }

        ui->label_9->setText(BeamIDs);
        //qDebug()<< "Active beam is "<< CurrentNodeBeam->ActiveBeam;
        glWidget->updateGL();
        glWidgetO->updateGL();
        }
    }

    CurrentNodeBeam->SelectedBeams.clear();
    for(int i=0; i<ui->treeWidget_2->selectedItems().size(); i++)
    {
        if(ui->treeWidget_2->selectedItems()[i]->text(2) == "Group")
        {

        }
        else
        {

            CurrentNodeBeam->SelectedBeams.append(ui->treeWidget_2->selectedItems()[i]->text(4).toInt());
            qDebug() << CurrentNodeBeam->SelectedBeams[i];
        }
    }
    if(CurrentNodeBeam->ActiveBeam < CurrentNodeBeam->Beams.size())
    {
        if(CurrentNodeBeam->Beams[CurrentNodeBeam->ActiveBeam].HasBeamDefs)
        {
            QString message = "Beam arguments are : ";
            message.append(CurrentNodeBeam->BeamDefaults[CurrentNodeBeam->Beams[CurrentNodeBeam->ActiveBeam].BeamDefsID].Name);
            ui->statusBar->showMessage(message);
        }
    }

    }

}


//Apply node changes
void MainWindow::on_pushButton_5_clicked()
{
    int TempNodeID = ui->label_6nodeID->text().toInt();
    CurrentNodeBeam->Nodes[TempNodeID].locX = ui->lineEdit_3->text().toFloat();
    CurrentNodeBeam->Nodes[TempNodeID].locY = ui->lineEdit_4->text().toFloat();
    CurrentNodeBeam->Nodes[TempNodeID].locZ = ui->lineEdit_5->text().toFloat();
    CurrentNodeBeam->Nodes[TempNodeID].Properties = ui->lineEdit_2->text();
    CurrentNodeBeam->Nodes[TempNodeID].NodeName = ui->lineEdit_nodename->text();
    glWidget->updateGL();
    glWidgetO->updateGL();
    MainNodeBeamUpdated();

}

//Signal from GLWidget's, to refresh GUI
void MainWindow::MainNodeBeamUpdated()
{
    int i3 = 0;
    qDebug() << "Refreshing tree widgets";
    ui->treeWidget->clear();
    for(int i2=0; i2<CurrentNodeBeam->NodeGroups.size(); i2++)
    {

        item2 = new QTreeWidgetItem();
        item2->setText(0, CurrentNodeBeam->NodeGroups[i2].NodeGroupName);
        item2->setText(2, "Group");
        item2->setText(3, QString::number(CurrentNodeBeam->NodeGroups[i2].NodeGroupID));
        for(int i=0; i<CurrentNodeBeam->NodeGroups[i2].NodeAmount; i++)
        {
            item = new QTreeWidgetItem();
            item->setText(1, QString::number(CurrentNodeBeam->Nodes[i3].GlobalID));
            item->setText(0, CurrentNodeBeam->Nodes[i3].NodeName);
            item2->addChild(item);
            qDebug() << CurrentNodeBeam->Nodes[i3].GlobalID << ", " << CurrentNodeBeam->Nodes[i3].NodeName;
            i3++;

        }
        ui->treeWidget->addTopLevelItem(item2);

    }

    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item =  ui->treeWidget->topLevelItem ( i );
        if(CurrentNodeBeam->NodeGroups[i].draw==1) item->setExpanded(1);
    }

    /* Refresh beams tree widget */
    i3=0;
    ui->treeWidget_2->clear();
    for(int i2=0; i2<CurrentNodeBeam->BeamGroups.size(); i2++)
    {

        /* Type 1000 for beam Groups */
        item2 = new QTreeWidgetItem(1000);
        item2->setText(0, CurrentNodeBeam->BeamGroups[i2].BeamGroupName);
        item2->setText(2, "Group");
        item2->setText(3, QString::number(CurrentNodeBeam->BeamGroups[i2].BeamGroupID));
        for(int i=0; i<CurrentNodeBeam->BeamGroups[i2].BeamAmount; i++)
        {
            /* Type 1100 for beams */
            item = new QTreeWidgetItem(1100);
            QString CurrentLine = CurrentNodeBeam->Beams[i3].Node1Name;
            CurrentLine.append(", ");
            CurrentLine.append(CurrentNodeBeam->Beams[i3].Node2Name);
            item->setText(0,CurrentLine);
            item->setText(3, QString::number(CurrentNodeBeam->Beams[i3].BeamGroupID));
            item->setText(4,QString::number(i3));
            item->setText(5, "Beam");
            item2->addChild(item);
            i3++;
        }
        ui->treeWidget_2->addTopLevelItem(item2);

    }

    for (int i = 0; i < ui->treeWidget_2->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item =  ui->treeWidget_2->topLevelItem ( i );
        if(CurrentNodeBeam->BeamGroups[i].draw==1) item->setExpanded(1);
    }

    /* Update node groups in Add nodes box */
    ui->comboBox_2->clear();
    for(int i=0; i<CurrentNodeBeam->NodeGroups.size(); i++)
    {
        ui->comboBox_2->addItem(CurrentNodeBeam->NodeGroups[i].NodeGroupName);
    }
    ui->comboBox_2->setCurrentIndex(CurrentNodeGroupi);

    /* Update beam groups in Add beams box */
    ui->comboBox->clear();
    for(int i=0; i<CurrentNodeBeam->BeamGroups.size(); i++)
    {
        ui->comboBox->addItem(CurrentNodeBeam->BeamGroups[i].BeamGroupName);
    }
    ui->comboBox->setCurrentIndex(CurrentBeamGroupi);

    /* Hubwheels list update */
    QListWidgetItem * item;
    ui->listWidget->clear();
    for(int i=0; i<CurrentNodeBeam->Hubwheels.size(); i++)
    {
        item = new QListWidgetItem;
        item->setText(CurrentNodeBeam->Hubwheels[i].name);
        ui->listWidget->addItem(item);
    }


}

/* Create new node group */
void MainWindow::on_pushButton_13_clicked()
{
    bool ok;
         QString text = QInputDialog::getText(this, tr("New Nodegroup"),
                                              tr("New nodegroup name:"), QLineEdit::Normal,
                                              "New Group", &ok);
         if (ok && !text.isEmpty())
         {
             qDebug()<<"Creating new node group.";
             CurrentNodeBeam->NodeGroups.resize(CurrentNodeBeam->NodeGroups.size()+1);
             CurrentNodeBeam->NodeGroups[CurrentNodeBeam->NodeGroups.size()-1].NodeGroupName = text;
             CurrentNodeBeam->NodeGroups[CurrentNodeBeam->NodeGroups.size()-1].NodeGroupID = CurrentNodeBeam->NodeGroups.size()-1;
             CurrentNodeBeam->NodeGroups[CurrentNodeBeam->NodeGroups.size()-1].draw=1;
             MainNodeBeamUpdated();
         }
}

/* Create new beam group */
void MainWindow::on_pushButton_16_clicked()
{
    bool ok;
         QString text = QInputDialog::getText(this, tr("New Beamgroup"),
                                              tr("New beamgroup name:"), QLineEdit::Normal,
                                              "New Group", &ok);
         if (ok && !text.isEmpty())
         {
             QStringList items;
             items << tr("At the end of the beams list") << tr("Before Current beam");
             QString item = QInputDialog::getItem(this, tr("Creating a new beam group."),
                                                  tr("Where to create a new group:"), items, 0, false, &ok);
             if (ok && !item.isEmpty())
             {
                 if(item == "At the end of the beams list")
                 {
                     qDebug()<<"Creating a new beam group.";
                     CurrentNodeBeam->BeamGroups.resize(CurrentNodeBeam->BeamGroups.size()+1);
                     CurrentNodeBeam->BeamGroups[CurrentNodeBeam->BeamGroups.size()-1].BeamGroupName = text;
                     CurrentNodeBeam->BeamGroups[CurrentNodeBeam->BeamGroups.size()-1].BeamGroupID = CurrentNodeBeam->BeamGroups.size()-1;
                     CurrentNodeBeam->BeamGroups[CurrentNodeBeam->BeamGroups.size()-1].draw=1;
                     MainNodeBeamUpdated();
                 }
                 else
                 {
                    int TempBeamIndex = ui->treeWidget_2->currentItem()->text(4).toInt();
                    for(int i=TempBeamIndex; i<CurrentNodeBeam->Beams.size();i++)
                    {
                        CurrentNodeBeam->Beams[i].BeamGroupID++;
                        qDebug()<<"Increasing number to " << CurrentNodeBeam->Beams[i].BeamGroupID;
                    }

                    int TempBeamGroupIndex = CurrentNodeBeam->Beams[TempBeamIndex].BeamGroupID+1;
                    CurrentNodeBeam->TempBeamGroup.BeamGroupName = text;
                    CurrentNodeBeam->TempBeamGroup.BeamGroupID = TempBeamGroupIndex-1;
                    CurrentNodeBeam->TempBeamGroup.draw=1;
                    CurrentNodeBeam->BeamGroups.insert(TempBeamGroupIndex-1,CurrentNodeBeam->TempBeamGroup);

                    for(int i=TempBeamGroupIndex; i< CurrentNodeBeam->BeamGroups.size();i++)
                    {
                        CurrentNodeBeam->BeamGroups[i].BeamGroupID++;
                    }
                    for(int i=0; i< CurrentNodeBeam->BeamGroups.size();i++)
                    {
                        CurrentNodeBeam->BeamGroups[i].BeamAmount=0;
                    }
                    for(int i = 0; i<CurrentNodeBeam->Beams.size();i++)
                    {
                        qDebug() << "Current beam " <<CurrentNodeBeam->Beams[i].BeamGroupID << ", " << CurrentNodeBeam->Beams[i].Node1Name << ", " << CurrentNodeBeam->Beams[i].Node2Name;
                        for(int i2=0; i2<CurrentNodeBeam->BeamGroups.size();i2++)
                        {
                            qDebug()<< CurrentNodeBeam->BeamGroups[i2].BeamGroupID << " and " << CurrentNodeBeam->Beams[i].BeamGroupID ;
                            if(CurrentNodeBeam->Beams[i].BeamGroupID == CurrentNodeBeam->BeamGroups[i2].BeamGroupID)
                            {
                                CurrentNodeBeam->BeamGroups[i2].BeamAmount++;
                                break;
                            }
                        }
                    }

                    MainNodeBeamUpdated();

                 }
             }
         }

}

void MainWindow::on_treeWidget_itemCollapsed(QTreeWidgetItem *item)
{
    CurrentNodeBeam->NodeGroups[item->text(3).toInt()].draw=0;
    glWidget->updateGL();

}

void MainWindow::on_treeWidget_itemExpanded(QTreeWidgetItem *item)
{
    CurrentNodeBeam->NodeGroups[item->text(3).toInt()].draw=1;
    glWidget->updateGL();
}

/* Beams Tree Widget */
void MainWindow::on_treeWidget_2_itemCollapsed(QTreeWidgetItem *item)
{
    CurrentNodeBeam->BeamGroups[item->text(3).toInt()].draw=0;
    glWidget->updateGL();
}

void MainWindow::on_treeWidget_2_itemExpanded(QTreeWidgetItem *item)
{
    if(!RefreshLock)
    {
        if(item->text(3).toInt()<CurrentNodeBeam->BeamGroups.size()) CurrentNodeBeam->BeamGroups[item->text(3).toInt()].draw=1;
    }
    glWidget->updateGL();
}

//Add nodes button
void MainWindow::on_pushButton_6_clicked()
{



}
//Z coordinate
void MainWindow::on_lineEdit_zcoordinate_textChanged(const QString &arg1)
{
    bool ok=0;
    glWidgetO->Zcoordinate= ui->lineEdit_zcoordinate->text().toFloat(&ok);
    if(!ok)
    {
        if(ui->lineEdit_zcoordinate->text()=="-");
        else
        {
            ui->lineEdit_zcoordinate->setText("0");
            glWidgetO->Zcoordinate=0;
            QMessageBox msgBox;
            msgBox.setText("The Z coordinate must be a number.");
            msgBox.exec();
        }
    }
}
//Snap to grid checkbox
void MainWindow::on_checkBox_gridsnap_stateChanged(int arg1)
{
    if(ui->checkBox_gridsnap->isChecked())
    {
        glWidgetO->SnapToGrid = 1;
        glWidgetO->updateGL();
    }
    else
    {
        glWidgetO->SnapToGrid = 0;
        glWidgetO->updateGL();
    }
}
//Grid size
void MainWindow::on_spinBox_grids_valueChanged(const QString &arg1)
{
    glWidgetO->GridSize = ui->spinBox_grids->value()*0.01;
    glWidgetO->updateGL();
}

/* Choosing beam group to add beams in */
void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{

}

/* Zoom the 3D view */
void MainWindow::on_spinBox_valueChanged(int arg1)
{
    float ZoomF = 1.0f;
    ZoomF = ZoomF/(arg1*0.01f);
    glWidget->ZoomFactor = ZoomF;
    emit ZoomChanged();

}

/* Change beam group */
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->toolButton_12->setEnabled(1);
}

void MainWindow::on_toolButton_12_clicked()
{
    CurrentBeamGroupi=ui->comboBox->currentIndex();
    glWidget->CurrentBeamGroup=ui->comboBox->currentIndex();
    ui->toolButton_12->setEnabled(0);
}


/* Enable node group apply button, when combobox index has changed */
void MainWindow::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    ui->toolButton_13->setEnabled(1);
}


void MainWindow::on_pushButton_19_clicked()
{

}
/* Apply node group change button */
void MainWindow::on_toolButton_13_clicked()
{
    CurrentNodeGroupi=ui->comboBox_2->currentIndex();
    ui->toolButton_13->setEnabled(0);
    glWidgetO->CurrentNodeGroup=ui->comboBox_2->currentIndex();
}

/* Toggle rectangle selection */
void MainWindow::on_pushButton_9_clicked()
{




}

/* Hide all / Show all nodes */
void MainWindow::on_pushButton_20_clicked()
{
    if(ui->pushButton_20->text() == "Hide all")
    {
        ui->treeWidget->collapseAll();
        ui->pushButton_20->setText("Show all");
    }
    else
    {
        //QT's Expand all function for some reason causes terrible performance issues
        //so using a loop instead that is fast
        for(int i=0; i<CurrentNodeBeam->NodeGroups.size();i++)
        {
            ui->treeWidget->topLevelItem(i)->setExpanded(1);
        }
        ui->pushButton_20->setText("Hide all");
    }
}

/* Show / hide all beams */
void MainWindow::on_pushButton_21_hidebeams_clicked()
{
    if(ui->pushButton_21_hidebeams->text() == "Hide all")
    {
        RefreshLock=1;
        ui->treeWidget_2->collapseAll();
        RefreshLock=0;
        ui->pushButton_21_hidebeams->setText("Show all");
    }
    else
    {
        RefreshLock=1;
        //ui->treeWidget_2->expandAll();

        ui->pushButton_21_hidebeams->setText("Hide all");
        for(int i=0; i<CurrentNodeBeam->BeamGroups.size();i++)
        {
            ui->treeWidget_2->topLevelItem(i)->setExpanded(1);
            CurrentNodeBeam->BeamGroups[i].draw=1;
        }
        RefreshLock=0;
    }
}


void MainWindow::on_pushButton_22_shownodenumbers_clicked()
{

}
/* Show node numbers button */
void MainWindow::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked())
    {
        glWidget->ShowNodeNumbers = 1;
    }
    else
    {
        glWidget->ShowNodeNumbers = 0;
    }
}

/* Import BeamNG file triggered */
void MainWindow::on_actionImport_BeamNG_triggered()
{

    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        CurrentNodeBeam->ReadJBeamTree(fileName);
        //CurrentNodeBeam->ImportBeamNG(fileName);

    MainNodeBeamUpdated();

}

/* Import nodes and beams from Wavefront OBJ file */
void MainWindow::on_actionImport_OBJ_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        CurrentNodeBeam->ImportWavefrontOBJ(fileName);

    MainNodeBeamUpdated();
}

/* Chassis generator menu button triggered */
void MainWindow::on_actionChassis_Generator_triggered()
{
    ChassisGenerator ChGen;
    ChGen.NBPointer = CurrentNodeBeam;
    ChGen.exec();
    MainNodeBeamUpdated();

}

/* Move selected nodes to group */
void MainWindow::on_pushButton_14_clicked()
{
    bool ok;
    int NodeGroupID;
    QStringList items;
    for(int i=0; i<CurrentNodeBeam->NodeGroups.size();i++)
    {
        items << CurrentNodeBeam->NodeGroups[i].NodeGroupName;
    }

    QString item = QInputDialog::getItem(this, tr("Moving node to group"),
                                             tr("Nove node to group:"), items, 0, false, &ok);
    if (ok && !item.isEmpty())
    {
        for(int i=0; i<CurrentNodeBeam->NodeGroups.size();i++)
        {
            if(item == CurrentNodeBeam->NodeGroups[i].NodeGroupName)
            {
                NodeGroupID = i;
            }
        }
    }
    int newindex;
    for(int i=0; i<CurrentNodeBeam->SelectedNodes.size();i++)
    {
        newindex = CurrentNodeBeam->MoveToGroup(CurrentNodeBeam->SelectedNodes[i],NodeGroupID);
        qDebug() << "Moving node " << CurrentNodeBeam->SelectedNodes[i] << " to group " << NodeGroupID;
        for(int i2=0; i2<CurrentNodeBeam->SelectedNodes.size();i2++)
        {
            if(newindex >= CurrentNodeBeam->SelectedNodes[i2])
            {
                CurrentNodeBeam->SelectedNodes[i2]--;
            }
        }

    }
    MainNodeBeamUpdated();
}

/* Generate tracks for tracked vehicles */
void MainWindow::on_actionTracks_triggered()
{
    TracksGenerator trackgen;
    trackgen.exec();
}

/* Delete beams */
void MainWindow::on_pushButton_3_clicked()
{


}

/* Delete nodes */
void MainWindow::on_pushButton_DeleteNode_clicked()
{

}

/* File->Exit */
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

/* Choose arguments for selected beams */
void MainWindow::on_pushButton_21_clicked()
{
    if(BeamProperties->firstload == 0)
    {
        BeamProperties->LoadAll();
        BeamProperties->firstload = 1;
    }

    bool ok;
    QStringList items;
    for(int i=0; i<CurrentNodeBeam->BeamDefaults.size();i++)
    {
        items.append(CurrentNodeBeam->BeamDefaults[i].Name);
    }
    QString item = QInputDialog::getItem(this, tr("Choose arguments for selected beams"),
                                         tr("Arguments:"), items, 0, false, &ok);
    int index = 0;
    if (ok && !item.isEmpty())
    {
        for(int i=0; i<CurrentNodeBeam->BeamDefaults.size();i++)
        {
            if(item == CurrentNodeBeam->BeamDefaults[i].Name)
            {
                index = i;
                break;
            }
        }
        for(int i=0; i<CurrentNodeBeam->SelectedBeams.size();i++)
        {
            CurrentNodeBeam->Beams[CurrentNodeBeam->SelectedBeams[i]].HasBeamDefs = 1;
            CurrentNodeBeam->Beams[CurrentNodeBeam->SelectedBeams[i]].BeamDefsID = index;
        }
    }
}

/* Open saved vehicle project from XML file */
void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) CurrentNodeBeam->OpenProject(fileName);

    MainNodeBeamUpdated();
}

/* Save current vehicle project to XML file */
void MainWindow::on_actionSave_As_triggered()
{   
    QString fileName = QFileDialog::getSaveFileName(this, "Save As",QDir::currentPath(),"BeamNG vehicle editor file (*.beamproj);;All files (*.*)",0,QFileDialog::DontUseNativeDialog);
    if (!fileName.isEmpty())
        CurrentNodeBeam->SaveAs(fileName);
}

/* Add nodes */
void MainWindow::on_toolButton_clicked()
{
    ButtsUp(0);
    if(glWidgetO->AddingNodes==0)
    {
        if(OpenGLViews->currentIndex()!=1) OpenGLViews->setCurrentIndex(1);
        glWidgetO->AddingNodes=1;
    }
    else glWidgetO->AddingNodes=0;
}

/* Move nodes */
void MainWindow::on_toolButton_2_clicked()
{
    ButtsUp(1);
    if(ui->toolButton_2->isChecked())
    {
        glWidgetO->MovingNodes = 1;
        glWidget->MovingNodes=1;
    }
    else
    {
        glWidgetO->MovingNodes = 0;
        glWidget->MovingNodes=0;
    }


}

/* Scale nodes */
void MainWindow::on_toolButton_3_clicked()
{
    ButtsUp(2);
    if(ui->toolButton_3->isChecked())
    {
        glWidgetO->ScalingNodes = 1;
        ui->statusBar->showMessage("Press X, Y or Z to choose lock scaling axis");
    }

    else
    {
        glWidgetO->ScalingNodes = 0;
        ui->statusBar->clearMessage();
    }

}

/* Rotate nodes */
void MainWindow::on_toolButton_4_clicked()
{
    ButtsUp(3);
    if(ui->toolButton_4->isChecked())
    {
        glWidgetO->RotatingNodes = 1;
        ui->statusBar->showMessage("Press X, Y or Z to choose lock rotating axis");
    }

    else
    {
        glWidgetO->RotatingNodes = 0;
        ui->statusBar->clearMessage();
    }
}

/* Key press events */
void MainWindow::keyPressEvent(QKeyEvent * eventti)
{
    /* Move */
    if(eventti->key() == Qt::Key_G)
    {
        qDebug() << "G";
        InputDialog testi;
        testi.MoveNodes();
        testi.exec();
        if(!testi.ValuesOk)
        {
            QMessageBox msgBox;
            msgBox.setText("The moving distance must be a number.");
            msgBox.exec();
        }
        else
        {
            for(int i2=0; i2<CurrentNodeBeam->SelectedNodes.size();i2++)
            {
                CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locX = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locX + testi.valuesFloat[0];
                CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locY = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locY + testi.valuesFloat[1];
                CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locZ = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locZ + testi.valuesFloat[2];

            }
        }
    }
    /* Scale */
    else if(eventti->key() == Qt::Key_S)
    {
        /* Set up the scale dialog and execute it */
        InputDialog ScaleInput;
        ScaleInput.ScaleNodes();
        ScaleInput.exec();

        /* If values given by user are not ok, show error message
         * else do the scaling. */
        if(!ScaleInput.ValuesOk)
        {
            QMessageBox msgBox;
            msgBox.setText("The scaling factor must be a number.");
            msgBox.exec();
        }
        else
        {
            for(int i2=0; i2<CurrentNodeBeam->SelectedNodes.size();i2++)
            {
                CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locX = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locX * ScaleInput.valuesFloat[0];
                CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locY = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locY * ScaleInput.valuesFloat[1];
                CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locZ = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locZ * ScaleInput.valuesFloat[2];

            }
        }
    }
    /* Choose X-axis */
    else if(eventti->key() == Qt::Key_X)
    {
        if(glWidgetO->ScalingNodes>0)
        {
            glWidgetO->ScalingNodes=2;
            ui->statusBar->showMessage("Scaling X");
        }
        else if(glWidgetO->MovingNodes>0)
        {
            glWidgetO->MovingNodes=2;
            glWidget->MovingNodes=2;
            ui->statusBar->showMessage("Moving X");
        }
        else if(glWidgetO->RotatingNodes>0)
        {
            glWidgetO->RotatingNodes=1;
            ui->statusBar->showMessage("Rotating X");
        }
    }
    /* Choose Y-axis */
    else if(eventti->key() == Qt::Key_Y)
    {
        if(glWidgetO->ScalingNodes>0)
        {
            glWidgetO->ScalingNodes=3;
            ui->statusBar->showMessage("Scaling Y");
        }
        else if(glWidgetO->MovingNodes>0)
        {
            glWidgetO->MovingNodes=3;
            glWidget->MovingNodes=3;
            ui->statusBar->showMessage("Moving Y");
        }
        else if(glWidgetO->RotatingNodes>0)
        {
            glWidgetO->RotatingNodes=2;
            ui->statusBar->showMessage("Rotating Y");
        }
    }
    /* Choose Z-axis */
    else if(eventti->key() == Qt::Key_Z)
    {
        if(glWidgetO->ScalingNodes>0)
        {
            glWidgetO->ScalingNodes=4;
            ui->statusBar->showMessage("Scaling Z");
        }
        else if(glWidgetO->MovingNodes>0)
        {
            glWidgetO->MovingNodes=4;
            glWidget->MovingNodes=4;
            ui->statusBar->showMessage("Moving Z");
        }
        else if(glWidgetO->RotatingNodes>0)
        {
            glWidgetO->RotatingNodes=3;
            ui->statusBar->showMessage("Rotating Z");
        }
    }
    /* Rotate */
    else if(eventti->key() == Qt::Key_R)
    {
        InputDialog testi;
        testi.RotateNodes();
        testi.exec();
        if(!testi.ValuesOk)
        {
            QMessageBox msgBox;
            msgBox.setText("The rotating angle must be in degrees.");
            msgBox.exec();
        }
        else
        {
            float a1, b1, a2, b2;

            if(testi.valuesFloat[0]!=0)
            {
                //X-Rotation
                //Calculating rotation matrix
                // cos a1 | -sin b1
                // sin a2 |  cos b2
                a1 = qCos(qDegreesToRadians(testi.valuesFloat[0]));
                b1 = qSin(qDegreesToRadians(testi.valuesFloat[0]));
                a2 = qSin(qDegreesToRadians(testi.valuesFloat[0]));
                b2 = qCos(qDegreesToRadians(testi.valuesFloat[0]));

                for(int i2=0; i2<CurrentNodeBeam->SelectedNodes.size();i2++)
                {
                    float XCoordinate = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locY;
                    float YCoordinate = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locZ;
                    CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locY = a1*XCoordinate - b1*YCoordinate;
                    CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locZ = a2*XCoordinate + b2*YCoordinate;
                }
            }
            if(testi.valuesFloat[1]!=0)
            {
                //Y-Rotation
                //Calculating rotation matrix
                // cos a1 | -sin b1
                // sin a2 |  cos b2
                a1 = qCos(qDegreesToRadians(testi.valuesFloat[1]));
                b1 = qSin(qDegreesToRadians(testi.valuesFloat[1]));
                a2 = qSin(qDegreesToRadians(testi.valuesFloat[1]));
                b2 = qCos(qDegreesToRadians(testi.valuesFloat[1]));

                for(int i2=0; i2<CurrentNodeBeam->SelectedNodes.size();i2++)
                {
                    float XCoordinate = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locX;
                    float YCoordinate = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locZ;
                    CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locX = a1*XCoordinate - b1*YCoordinate;
                    CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locZ = a2*XCoordinate + b2*YCoordinate;
                }
            }
            if(testi.valuesFloat[2]!=0)
            {
                //Z-Rotation
                //Calculating rotation matrix
                // cos a1 | -sin b1
                // sin a2 |  cos b2
                a1 = qCos(qDegreesToRadians(testi.valuesFloat[2]));
                b1 = qSin(qDegreesToRadians(testi.valuesFloat[2]));
                a2 = qSin(qDegreesToRadians(testi.valuesFloat[2]));
                b2 = qCos(qDegreesToRadians(testi.valuesFloat[2]));

                for(int i2=0; i2<CurrentNodeBeam->SelectedNodes.size();i2++)
                {
                    float XCoordinate = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locX;
                    float YCoordinate = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locY;
                    CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locX = a1*XCoordinate - b1*YCoordinate;
                    CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locY = a2*XCoordinate + b2*YCoordinate;
                }
            }
        }
    }
}

/* Delete nodes */
void MainWindow::on_toolButton_7_clicked()
{
    for(int i=0; i<CurrentNodeBeam->SelectedNodes.size(); i++)
    {
        qDebug() << "Deleting Node " << CurrentNodeBeam->SelectedNodes[i];
        CurrentNodeBeam->DeleteNode(CurrentNodeBeam->SelectedNodes[i]);
        for(int i2=i; i2<CurrentNodeBeam->SelectedNodes.size(); i2++)
        {
            CurrentNodeBeam->SelectedNodes[i2] = CurrentNodeBeam->SelectedNodes[i2]-1;
        }
    }
    MainNodeBeamUpdated();
}

/* Rectangle select */
void MainWindow::on_toolButton_6_clicked()
{
    ButtsUp(5);
    if(ui->toolButton_6->isChecked())
    {
        if(OpenGLViews->currentIndex()!=1) OpenGLViews->setCurrentIndex(1);
        glWidgetO->RectSelect=1;
    }
    else glWidgetO->RectSelect=0;

}

/* Turn all tools off except buttoni */
void MainWindow::ButtsUp(int buttoni)
{
    if(buttoni != 0)
    {
        ui->toolButton->setChecked(0);
        glWidgetO->AddingNodes=0;
    }
    if(buttoni != 1)
    {
        ui->toolButton_2->setChecked(0);
        glWidgetO->MovingNodes = 0;
        glWidget->MovingNodes=0;
    }
    if(buttoni != 2)
    {
        ui->toolButton_3->setChecked(0);
        glWidgetO->ScalingNodes = 0;
        ui->statusBar->clearMessage();
    }
    if(buttoni != 3)
    {
        ui->toolButton_4->setChecked(0);
        glWidgetO->RotatingNodes = 0;
        ui->statusBar->clearMessage();
    }
    if(buttoni != 5)
    {
        ui->toolButton_6->setChecked(0);
        glWidgetO->RectSelect=0;
    }
    if(buttoni != 6)
    {
        ui->toolButton_19->setChecked(0);
        glWidget->AddingWheels=0;
    }

}

/* Get's called when rectangle selection is used and the selected
 nodes must be highlighted in the tree widget */
void MainWindow::UpdateSelection()
{
    qDebug()<< "Updating treewidget selected items";
    ui->treeWidget->clearSelection();
//    for(int i3=0; i3<CurrentNodeBeam->SelectedNodes2.size(); i3++)
//    {
//        QTreeWidgetItemIterator iterator(ui->treeWidget);
//        qDebug()<< "hmmdssss";
//        while(*iterator)
//        {
//            if((*iterator)->text(2) != "Group")
//            {
//                if((*iterator)->text(1).toInt() == CurrentNodeBeam->SelectedNodes2[i3])
//                {
//                    (*iterator)->setSelected(1);
//                    //(*iterator)->setCheckState(0,Qt::Checked);
//                }

//            }
//        ++iterator;
//        }
//    }

    int i3=0;
    int selectionsize = CurrentNodeBeam->SelectedNodes2.size();
    QTreeWidgetItemIterator iterator(ui->treeWidget);

    QTime myTimer;
    myTimer.start();

    ui->treeWidget->setUpdatesEnabled(false);

    if(selectionsize>0)
    {
        while(*iterator)
        {
            if((*iterator)->text(2) != "Group")
            {
                if((*iterator)->text(1).toInt() == CurrentNodeBeam->SelectedNodes2[i3])
                {
                    (*iterator)->setSelected(1);
                    //(*iterator)->setCheckState(0,Qt::Checked);
                    i3++;
                    if(i3==selectionsize) break;
                }
                ui->statusBar->showMessage(QString::number(myTimer.elapsed()));

            }
            ++iterator;
        }
    }
    ui->treeWidget->setUpdatesEnabled(true);
    qDebug() << "iterating took " << myTimer.elapsed() << " ms";


}

/* Duplicate nodes */
void MainWindow::on_toolButton_5_clicked()
{
    CurrentNodeBeam->DuplicateNodes();
    MainNodeBeamUpdated();
    UpdateSelection();
}



void MainWindow::on_pushButton_17_clicked()
{

}

/* Add single beam */
void MainWindow::on_toolButton_8_clicked()
{
    if(ui->toolButton_8->isChecked())
    {
        if(OpenGLViews->currentIndex()!=0) OpenGLViews->setCurrentIndex(0);
        glWidget->NodePicking = 1;
        glWidget->AddingBeamsSingle=1;
    }
    else
    {
        glWidget->NodePicking = 0;
        glWidget->AddingBeamsSingle=0;
    }
    if(ui->toolButton_9->isChecked())
    {
        ui->toolButton_9->setChecked(0);
        glWidget->AddingBeamsCont=0;
    }
}

/* Add continuously */
void MainWindow::on_toolButton_9_clicked()
{
    if(ui->toolButton_9->isChecked())
    {
        if(OpenGLViews->currentIndex()!=0) OpenGLViews->setCurrentIndex(0);
        glWidget->NodePicking = 1;
        glWidget->AddingBeamsCont=1;
    }
    else
    {
        glWidget->NodePicking = 0;
        glWidget->AddingBeamsCont=0;
    }
    if(ui->toolButton_8->isChecked())
    {
        ui->toolButton_8->setChecked(0);
        glWidget->AddingBeamsSingle=0;
    }

}

/* Open beam arguments editor */
void MainWindow::on_toolButton_11_clicked()
{
    if(BeamProperties->firstload == 0)
    {
        BeamProperties->LoadAll();
        BeamProperties->firstload = 1;
    }
    BeamProperties->exec();
}

/* Delete beams */
void MainWindow::on_toolButton_10_clicked()
{
    for(int i=0; i<CurrentNodeBeam->SelectedBeams.size(); i++)
    {
        qDebug() << "Deleting Beam " << CurrentNodeBeam->SelectedBeams[i];
        CurrentNodeBeam->DeleteBeam(CurrentNodeBeam->SelectedBeams[i]);
        for(int i2=i; i2<CurrentNodeBeam->SelectedBeams.size(); i2++)
        {
            CurrentNodeBeam->SelectedBeams[i2] = CurrentNodeBeam->SelectedBeams[i2]-1;
        }
    }
    MainNodeBeamUpdated();
}

/* Extrude node beam from selected nodes */
void MainWindow::on_toolButton_14_clicked()
{
    CurrentNodeBeam->ExtrudeNodes();
    MainNodeBeamUpdated();
    UpdateSelection();

}

void MainWindow::on_lineEdit_editingFinished()
{

}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    CurrentNodeBeam->CurrentNamePrefix = ui->lineEdit->text();
}

//Change orthographic view direction
void MainWindow::on_comboBox_3_views_activated(int index)
{
    if(index == 0)
    {
        //top
        glWidgetO->setViewTop();
        ui->label_11_currView->setText("TOP VIEW");
        glWidgetO->textureid = 0;
        ui->horizontalSlider->setValue(glWidgetO->blueprint_opa[0]*100);
    }
    else if(index == 1)
    {
        //bottom
        glWidgetO->setViewBottom();
        ui->label_11_currView->setText("BOTTOM VIEW");
        glWidgetO->textureid = 1;
        ui->horizontalSlider->setValue(glWidgetO->blueprint_opa[1]*100);
    }
    else if(index == 2)
    {
        //front
        glWidgetO->setViewFront();
        ui->label_11_currView->setText("FRONT VIEW");
        glWidgetO->textureid = 2;
        ui->horizontalSlider->setValue(glWidgetO->blueprint_opa[2]*100);
    }
    else if(index == 3)
    {
        //back
        glWidgetO->setViewBack();
        ui->label_11_currView->setText("BACK VIEW");
        glWidgetO->textureid = 3;
        ui->horizontalSlider->setValue(glWidgetO->blueprint_opa[3]*100);
    }
    else if(index == 4)
    {
        //right
        glWidgetO->setViewRight();
        ui->label_11_currView->setText("RIGHT VIEW");
        glWidgetO->textureid = 4;
        ui->horizontalSlider->setValue(glWidgetO->blueprint_opa[4]*100);
    }
    else if(index == 5)
    {
        //left
        glWidgetO->setViewLeft();
        ui->label_11_currView->setText("LEFT VIEW");
        glWidgetO->textureid = 5;
        ui->horizontalSlider->setValue(glWidgetO->blueprint_opa[5]*100);
    }

    ui->lineEdit_bluep_scale->setText(QString::number(glWidgetO->blueprint_scale[glWidgetO->textureid]));
    QString texti = glWidgetO->blueprint_file[glWidgetO->textureid];
    for(int i = texti.length(); i>0;i--)
    {
        if(texti[i] == '/')
        {
            texti = texti.remove(0,i+1);
            break;
        }
    }
    ui->label_10_bluepname->setText(texti);
    glWidgetO->updateGL();
}

/* show arrows */
void MainWindow::on_checkBox_2_clicked()
{
    if(ui->checkBox_2->isChecked())
    {
        glWidget->ShowArrows = 1;
        glWidgetO->ShowArrows = 1;
    }
    else
    {
        glWidget->ShowArrows = 0;
        glWidgetO->ShowArrows = 0;
    }
}

//load blueprint
void MainWindow::on_toolButton_17_clicked()
{
    if(OpenGLViews->currentIndex()!=1) OpenGLViews->setCurrentIndex(1);
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        glWidgetO->LoadBlueprint(fileName);
        for(int i = fileName.length(); i>0;i--)
        {
            if(fileName[i] == '/')
            {
                fileName = fileName.remove(0,i+1);
                break;
            }
        }
        ui->label_10_bluepname->setText(fileName);
    }


}

//change blueprint scaling
void MainWindow::on_lineEdit_bluep_scale_textEdited(const QString &arg1)
{
    glWidgetO->blueprint_scale[glWidgetO->textureid] = arg1.toFloat();
    glWidgetO->updateGL();
}

void MainWindow::on_lineEdit_9_bpoffY_textEdited(const QString &arg1)
{
    glWidgetO->blueprint_offY[glWidgetO->textureid] = arg1.toFloat();
    glWidgetO->updateGL();
}

void MainWindow::on_lineEdit_9_bpoffX_textEdited(const QString &arg1)
{
    glWidgetO->blueprint_offX[glWidgetO->textureid] = arg1.toFloat();
    glWidgetO->updateGL();
}

//Launch beamng
void MainWindow::on_toolButton_18_clicked()
{
    QString program = AppSettings->readsetting("beamnglocation");
    QProcess *myProcess = new QProcess(this);
    QString arguments = "-level ";
    arguments.append(AppSettings->readsetting("currentlevel"));
    arguments.append(" -vehicle ");
    arguments.append(AppSettings->readsetting("currentvehicle"));
    myProcess->setNativeArguments(arguments);
    myProcess->start(program);

}

void MainWindow::on_actionSettings_triggered()
{
    AppSettings->exec();
}

/* Add hubwheels */
void MainWindow::on_toolButton_19_clicked()
{
    ButtsUp(6);
    if(ui->toolButton_19->isChecked())
    {
        if(OpenGLViews->currentIndex()!=0) OpenGLViews->setCurrentIndex(0);
        glWidget->AddingWheels=1;
        glWidget->NodePicking=1;

    }
    else
    {
        glWidget->NodePicking=0;
        glWidget->AddingWheels=0;
        glWidget->TextOverlay="";
    }
}

/* Update hubwheel to boxes below */
void MainWindow::on_listWidget_activated(const QModelIndex &index)
{
    for(int i=0; i<CurrentNodeBeam->Hubwheels.size(); i++)
    {
        if(CurrentNodeBeam->Hubwheels[i].name == ui->listWidget->currentItem()->text())
        {
            ui->lineEdit_hubwheel->setText(CurrentNodeBeam->Hubwheels[i].name);
            ui->lineEdit_hubwheel_2->setText(CurrentNodeBeam->Hubwheels[i].node1);
            ui->lineEdit_hubwheel_3->setText(CurrentNodeBeam->Hubwheels[i].node2);
            ui->lineEdit_hubwheel_4->setText(CurrentNodeBeam->Hubwheels[i].nodeArm);
            ui->lineEdit_hubwheel_5->setText(CurrentNodeBeam->Hubwheels[i].nodeS);
            ui->lineEdit_hubwheel_6->setText(QString::number(CurrentNodeBeam->Hubwheels[i].wheelDir));
        }
    }

}

void MainWindow::on_pushButton_clicked()
{
    for(int i=0; i<CurrentNodeBeam->Hubwheels.size(); i++)
    {
        if(CurrentNodeBeam->Hubwheels[i].name == ui->listWidget->currentItem()->text())
        {
            CurrentNodeBeam->Hubwheels[i].name = ui->lineEdit_hubwheel->text();
            CurrentNodeBeam->Hubwheels[i].node1id = CurrentNodeBeam->FindBeamNodeByName(ui->lineEdit_hubwheel_2->text());
            CurrentNodeBeam->Hubwheels[i].node1 = ui->lineEdit_hubwheel_2->text();
            CurrentNodeBeam->Hubwheels[i].node2id = CurrentNodeBeam->FindBeamNodeByName(ui->lineEdit_hubwheel_3->text());
            CurrentNodeBeam->Hubwheels[i].node2 = ui->lineEdit_hubwheel_3->text();
            CurrentNodeBeam->Hubwheels[i].nodeArmid = CurrentNodeBeam->FindBeamNodeByName(ui->lineEdit_hubwheel_4->text());
            CurrentNodeBeam->Hubwheels[i].nodeArm = ui->lineEdit_hubwheel_4->text();
            CurrentNodeBeam->Hubwheels[i].nodeSid = CurrentNodeBeam->FindBeamNodeByName(ui->lineEdit_hubwheel_5->text());
            CurrentNodeBeam->Hubwheels[i].nodeS = ui->lineEdit_hubwheel_5->text();
            CurrentNodeBeam->Hubwheels[i].wheelDir = ui->lineEdit_hubwheel_6->text().toInt();

        }
    }
 }

//Set hubwheel properties
void MainWindow::on_pushButton_2_clicked()
{
    HubWheelProperties->setWindowTitle("Hubwheel Arguments");
    if(HubWheelProperties->firstload == 0)
    {
        HubWheelProperties->LoadAll();
        HubWheelProperties->firstload = 1;
    }

    HubWheelProperties->selecting=1;
    HubWheelProperties->exec();
    if(HubWheelProperties->selected > -1)
    {
        //Something has been selected
        if((ui->listWidget->currentRow()>-1) && (ui->listWidget->currentRow()<CurrentNodeBeam->Hubwheels.size()))
        {
            CurrentNodeBeam->Hubwheels[ui->listWidget->currentRow()].arguments = HubWheelProperties->selected;
        }
    }

    HubWheelProperties->selecting = 0;
    HubWheelProperties->selected = -1;
}

void MainWindow::SettingsUpdated()
{
    glWidget->backgroundcolor[0] = (AppSettings->readsetting("bg_color_r").toInt())/255.0f;
    glWidget->backgroundcolor[1] = (AppSettings->readsetting("bg_color_g").toInt())/255.0f;
    glWidget->backgroundcolor[2] = (AppSettings->readsetting("bg_color_b").toInt())/255.0f;
    glWidgetO->backgroundcolor[0] = (AppSettings->readsetting("bg_color_r").toInt())/255.0f;
    glWidgetO->backgroundcolor[1] = (AppSettings->readsetting("bg_color_g").toInt())/255.0f;
    glWidgetO->backgroundcolor[2] = (AppSettings->readsetting("bg_color_b").toInt())/255.0f;
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    glWidgetO->blueprint_opa[glWidgetO->textureid] = ui->horizontalSlider->value()/100.0f;
    glWidgetO->updateGL();
}

//Set scale by measuring distance
void MainWindow::on_toolButton_20_clicked()
{
    if(ui->toolButton_20->isChecked())
    {
        glWidgetO->MeasuringDistance=1;
        glWidgetO->SetScaleByDistance=1;
    }
    else
    {
        glWidgetO->MeasuringDistance=0;
        glWidgetO->SetScaleByDistance=0;
    }

}