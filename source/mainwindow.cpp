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
#include <QTimerEvent>
#include <QPixmap>
#include <QJsonDocument>
#include "blueprints.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /* About box : Version and compliation time , link to GitHub */
    AboutBox = "<br><br><br><b>Version: 0.34</b><br><br>Built on ";
    AboutBox.append(__DATE__);
    AboutBox.append(", ");
    AboutBox.append(__TIME__);
    AboutBox.append("<br><br><a href='https://github.com/RORMasa/NodeBeamEditor'>https://github.com/RORMasa/NodeBeamEditor</a>");

    ui->setupUi(this);
    BeamProperties = new BeamDefaultsDialog;
    HubWheelProperties = new BeamDefaultsDialog;

    /* Create perspective views */
    for(int i=0; i<2; i++)
    {
        GLWidget * widget1 = new GLWidget;
        glWidgetViews[i] = widget1;
    }
    /* Create orthographic views */
    for(int i=0; i<4; i++)
    {
        GLWidgetOrtho * widget1 = new GLWidgetOrtho;
        glWidgetOViews[i] = widget1;
        glWidgetOViews[i]->WidgetID = i;
    }

    /* 3D perspective view */
    QVBoxLayout *vertikaali = new QVBoxLayout;
    QWidget * OpenGLView = new QWidget;
    vertikaali->addWidget(glWidgetViews[0]);
    vertikaali->addWidget(ui->spinBox);
    ui->spinBox->setMaximumWidth(100);
    ui->spinBox->setMinimumHeight(28);
    vertikaali->setMargin(1);
    OpenGLView->setLayout(vertikaali);

    /* Orthographic view */
    QVBoxLayout *vertikaali2 = new QVBoxLayout;
    QWidget * OpenGLViewO = new QWidget;
    vertikaali2->addWidget(glWidgetOViews[0]);
    vertikaali2->addWidget(ui->frame_5);
    ui->comboBox_3_views->setMaximumWidth(100);
    ui->comboBox_3_views->setMinimumHeight(28);
    vertikaali2->setMargin(1);
    OpenGLViewO->setLayout(vertikaali2);
    ui->doubleSpinBox->setValue(2.0);

    /* Quad view */
    QSplitter *splitterVertical = new QSplitter;
    splitterVertical->setOrientation(Qt::Vertical);

    QSplitter *splitter1 = new QSplitter;
    splitter1->addWidget(glWidgetOViews[1]);
    splitter1->addWidget(glWidgetOViews[2]);

    QSplitter *splitter2 = new QSplitter;
    splitter2->addWidget(glWidgetOViews[3]);
    splitter2->addWidget(glWidgetViews[1]);
    splitterVertical->addWidget(splitter1);
    splitterVertical->addWidget(splitter2);

    /* Views into tab widget */
    OpenGLViews = new QTabWidget;
    OpenGLViews->addTab(OpenGLView,tr("3D View"));
    OpenGLViews->addTab(OpenGLViewO, tr("Orthographic View"));
    OpenGLViews->addTab(splitterVertical,tr("Quad View"));
    //OpenGLViews->addTab(glWidget,tr("3D View"));
    //OpenGLViews->addTab(glWidgetO,tr("Orthographic View"));

    ui->horizontalLayout->addWidget(OpenGLViews);

    //Active working file
    CurrentNodeBeam = new NodeBeam;
    QObject::connect(CurrentNodeBeam,SIGNAL(LUA_log(QString)),this,SLOT(LUA_message(QString)));

    //Link nodebeam to glwidgets
    for(int i=0; i<2; i++)
    {
        glWidgetViews[i]->setNBPointer(CurrentNodeBeam);
    }
    for(int i=0; i<4; i++)
    {
        glWidgetOViews[i]->setNBPointer(CurrentNodeBeam);
    }

    BeamProperties->setNBPointer(&CurrentNodeBeam->BeamDefaults);
    BeamProperties->argumenttype = BeamProperties->BEAM_ARGUMENTS;

    HubWheelProperties->setNBPointer(&CurrentNodeBeam->HWArguments);
    HubWheelProperties->argumenttype = HubWheelProperties->HUBWHEEL_ARGUMENTS;

    EditorTitle = "NodeBEAM Editor";
    setWindowTitle(EditorTitle);

    //Settings for the application
    AppSettings = new settings;
    QVector <float> backgroundcolor;
    backgroundcolor.append((AppSettings->readsetting("bg_color_r").toInt())/255.0f);
    backgroundcolor.append((AppSettings->readsetting("bg_color_g").toInt())/255.0f);
    backgroundcolor.append((AppSettings->readsetting("bg_color_b").toInt())/255.0f);
    backgroundcolor.append(1.0f); //Alpha to 1

    QVector <float> gridcolor;
    gridcolor.append((AppSettings->readsetting("grid_color_r").toInt())/255.0f);
    gridcolor.append((AppSettings->readsetting("grid_color_g").toInt())/255.0f);
    gridcolor.append((AppSettings->readsetting("grid_color_b").toInt())/255.0f);
    gridcolor.append(1.0f); //Alpha to 1

    for(int i=0; i<2; i++)
    {
        glWidgetViews[i]->backgroundcolor = backgroundcolor;
        glWidgetViews[i]->gridcolor = gridcolor;
    }

    //Settings to orthogonal views
    for(int i=0; i<4; i++)
    {
        glWidgetOViews[i]->backgroundcolor = backgroundcolor;
        glWidgetOViews[i]->gridcolor = gridcolor;
    }

    //Connect signals and slots
    QObject::connect(AppSettings, SIGNAL(SettingsUpdated()), this, SLOT(SettingsUpdated()));

    for(int i=0; i<2; i++)
    {
        QObject::connect(glWidgetViews[i], SIGNAL(NodeBeamUpdated()), this, SLOT(MainNodeBeamUpdated()));
        QObject::connect(this, SIGNAL(ZoomChanged()), glWidgetViews[i], SLOT(setZoom()));
        QObject::connect(glWidgetViews[i], SIGNAL(JBEAM_AddBeamO()), this, SLOT(JBEAM_AddBeam()));
    }
    for(int i=0; i<4; i++)
    {
        QObject::connect(glWidgetOViews[i], SIGNAL(NodeBeamUpdated()), this, SLOT(MainNodeBeamUpdated()));
        QObject::connect(glWidgetOViews[i], SIGNAL(SelectionUpdated()), this, SLOT(UpdateSelection()));
        QObject::connect(glWidgetOViews[i], SIGNAL(JBEAM_AddNodeO()), this, SLOT(JBEAM_AddNode()));
        QObject::connect(glWidgetOViews[i], SIGNAL(JBEAM_UpdateO()), this, SLOT(JBEAM_Update()));
        QObject::connect(glWidgetOViews[i], SIGNAL(AdjustBlueprint(int)), this, SLOT(AdjustBlueprint(int)));
    }
    for(int i=0; i<2; i++)
    {
        for(int i2=0; i2<4; i2++)
        {
            QObject::connect(glWidgetOViews[i2], SIGNAL(NodeBeamUpdated()), glWidgetViews[i], SLOT(updateGL()));
            QObject::connect(glWidgetViews[i], SIGNAL(NodeBeamUpdated()), glWidgetOViews[i2], SLOT(updateGL()));
        }
    }
    for(int i=0; i<4; i++)
    {
        int i2[7] = {0,1,2,3,0,1,2};
        QObject::connect(glWidgetOViews[i], SIGNAL(NodeBeamUpdated()), glWidgetOViews[i2[i+1]], SLOT(updateGL()));
        QObject::connect(glWidgetOViews[i], SIGNAL(NodeBeamUpdated()), glWidgetOViews[i2[i+2]], SLOT(updateGL()));
        QObject::connect(glWidgetOViews[i], SIGNAL(NodeBeamUpdated()), glWidgetOViews[i2[i+3]], SLOT(updateGL()));
    }

    //JBEAM widget
    JBEAM_NodeCursor = -1;
    JBEAM_BeamCursor = -1;

    //Create JBEAM textbox with linenumbers
    JBEAMtextbox = new JBEAM_TextBox;
    ui->verticalLayout_9->addWidget(JBEAMtextbox);
    ui->verticalLayout_9->addWidget(ui->widget);
    JBEAMtextbox->setLineWrapMode(QPlainTextEdit::NoWrap);
    //Set smaller TAB width
    JBEAMtextbox->setTabStopWidth(15);

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
                   "   }\n"
                   "}\n";

    JBEAMtextbox->setPlainText(EmptyJbeamTextTemplate);

    //Connect JBEAM textbox signals
    QObject::connect(JBEAMtextbox, SIGNAL(textChanged ()), this, SLOT(on_JBEAM_textChanged()));

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
    for(int i=0; i<2 ;i++)
    {
        glWidgetViews[i]->setCursor(cursor);
    }
    for(int i=0; i<4 ;i++)
    {
        glWidgetOViews[i]->setCursor(cursor);
    }

    autosave.start(); //Start timer for autosave

    ui->treeWidget->header()->resizeSection(0,75); //Make node ID colum narrower in nodes tree widget

    //init StatusBar
    StatusBar_nodecount = new QLabel();
    StatusBar_nodecount->setToolTip("Node counter");
    StatusBar_mode = new QLabel();
    StatusBar_mode->setToolTip("Coordinate system");
    StatusBar_info = new QLabel();
    ui->statusBar->addWidget(StatusBar_nodecount);
    ui->statusBar->addWidget(StatusBar_mode, 1);
    ui->statusBar->addWidget(StatusBar_info);

    //Swap editor axises for RoR if necessary
    if(AppSettings->readsetting("editor_mode")=="2")
    {
        EditorMode = 1;
        CurrentNodeBeam->EditorMode = 1;
        ui->lineEdit_3->move(80,90);
        ui->lineEdit_4->move(80,50);
        ui->lineEdit_5->move(80,70);
        ui->lineEdit_movex->move(20,70);
        ui->lineEdit_movey->move(20,30);
        ui->lineEdit_movez->move(20,50);
        ui->lineEdit_rotatex->move(20,70);
        ui->lineEdit_rotatey->move(20,30);
        ui->lineEdit_rotatez->move(20,50);
        StatusBar_mode->setText("ROR");
    }
    else
    {
        EditorMode = 0;
        CurrentNodeBeam->EditorMode = 0;
        StatusBar_mode->setText("BeamNG");
    }

    //Update JBEAM textbox cursor locations
    JBEAM_UpdateCursors(JBEAMtextbox->toPlainText());

    //Change Quad view default views
    glWidgetOViews[1]->setViewFront();
    glWidgetOViews[2]->setViewLeft();

    //Accept drops
    this->setAcceptDrops(true);

    //Set font to script log
    ui->textBrowser->setFont(QFont("Courier"));

}

MainWindow::~MainWindow()
{
    for(int i=0; i<4 ; i++)
    {
        delete glWidgetOViews[i];
    }

    for(int i=0; i<2 ; i++)
    {
        delete glWidgetViews[i];
    }

    delete JBEAMtextbox;
    delete AppSettings;
    delete CurrentNodeBeam;
    delete BeamProperties;
    delete ui;

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
    CurrentNodeBeam->clear();
    for(int i=0; i<2; i++)
    {
        glWidgetViews[i]->updateGL();
    }
    for(int i=0; i<4; i++)
    {
        glWidgetOViews[i]->updateGL();
    }
    ui->treeWidget->clear();
    ui->treeWidget_2->clear();
    ui->actionReload->setEnabled(false);

    JBEAMtextbox->setPlainText(EmptyJbeamTextTemplate);
    setWindowTitle(EditorTitle);
    //NewProjectDialog Dialog;
    //Dialog.exec();
}

/* File menu / Import RoR NB triggered */
void MainWindow::on_actionImport_Rigs_of_Rods_triggered()
{
    CurrentNodeBeam->JBEAM_temp.clear();
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        CurrentNodeBeam->ImportNBFile(fileName);

    MainNodeBeamUpdated();
    JBEAM_AddFromTemp();
}

/* File menu / Save as BeamNG JBEAM triggered */
void MainWindow::on_actionExport_to_BeamNG_triggered()
{
    QTime Timer;

    QString fileName = QFileDialog::getSaveFileName(this, "Export to JBeam",QDir::currentPath(),"BeamNG file (*.jbeam);;All files (*.*)",0,QFileDialog::DontUseNativeDialog);
    if (!fileName.isEmpty())
    {
        Timer.start();
        //CurrentNodeBeam->ExportBeamNG(fileName);

        if(CurrentNodeBeam->JBEAM_SaveAs(fileName,JBEAMtextbox->toPlainText()))
        {
            QStringList filepath = fileName.split('/');
            QString title = filepath.last() + " - " + EditorTitle;
            setWindowTitle(title);

            JBEAM_Filepath = fileName;
            ui->actionReload->setEnabled(true);
        }

        int result = Timer.elapsed();
        QString resultt = "File exporting finished in ";
        resultt.append(QString::number(result));
        resultt.append(" ms !");
        ui->statusBar->showMessage(resultt,10000);
    }
}

/* File menu / BeamNG quick save */
void MainWindow::on_actionSave_triggered()
{
    QString fileName = JBEAM_Filepath;
    if ((!fileName.isEmpty() && (ui->actionReload->isEnabled())))
    {
        if(CurrentNodeBeam->JBEAM_SaveAs(fileName,JBEAMtextbox->toPlainText()))
        {
            QString resultt = "JBEAM saved ";
            ui->statusBar->showMessage(resultt,10000);
        }
    }
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
            if(CurrentNodeBeam->Nodes.size()>ui->treeWidget->currentItem()->text(0).toInt())
            {
                int TempNodeID = ui->treeWidget->currentItem()->text(0).toInt();
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
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->updateGL();
        }
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->updateGL();
        }


        qDebug() << "Updating SELECTION";
        CurrentNodeBeam->SelectedNodes.clear();
        for(int i=0; i<ui->treeWidget->selectedItems().size(); i++)
        {
            if(ui->treeWidget->selectedItems()[i]->text(2) != "Group")
            {
                CurrentNodeBeam->SelectedNodes.append(ui->treeWidget->selectedItems()[i]->text(0).toInt());
                qDebug() << ui->treeWidget->selectedItems()[i]->text(0);
                qDebug() << CurrentNodeBeam->SelectedNodes[i];
            }
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
        if(CurrentNodeBeam->Beams[CurrentNodeBeam->ActiveBeam].draw == 1)
        {
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
        }

        //qDebug()<< "Active beam is "<< CurrentNodeBeam->ActiveBeam;
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->updateGL();
        }
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->updateGL();
        }
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
            if(CurrentNodeBeam->Beams[ui->treeWidget_2->selectedItems()[i]->text(4).toInt()].draw == 1)
            {
                CurrentNodeBeam->SelectedBeams.append(ui->treeWidget_2->selectedItems()[i]->text(4).toInt());
                qDebug() << CurrentNodeBeam->SelectedBeams[i];
            }
        }
    }
    if(CurrentNodeBeam->ActiveBeam < CurrentNodeBeam->Beams.size())
    {
        if(CurrentNodeBeam->Beams[CurrentNodeBeam->ActiveBeam].HasBeamDefs)
        {
            QString message = "Beam arguments are : ";
            message.append(CurrentNodeBeam->BeamDefaults[CurrentNodeBeam->Beams[CurrentNodeBeam->ActiveBeam].BeamDefsID].Name);
            ui->statusBar->showMessage(message,10000);
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
    for(int i=0; i<2; i++)
    {
        glWidgetViews[i]->updateGL();
    }
    for(int i=0; i<4; i++)
    {
        glWidgetOViews[i]->updateGL();
    }
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
        item2->setText(1, CurrentNodeBeam->NodeGroups[i2].NodeGroupName);
        item2->setText(2, "Group");
        item2->setText(3, QString::number(CurrentNodeBeam->NodeGroups[i2].NodeGroupID));
        //qDebug() << "lisätään node " << CurrentNodeBeam->NodeGroups[i2].NodeAmount;
        for(int i=0; i<CurrentNodeBeam->NodeGroups[i2].NodeAmount; i++)
        {
            item = new QTreeWidgetItem();
            item->setText(0, QString::number(CurrentNodeBeam->Nodes[i3].GlobalID));
            item->setText(1, CurrentNodeBeam->Nodes[i3].NodeName);
            item2->addChild(item);
            //qDebug() << CurrentNodeBeam->Nodes[i3].GlobalID << ", " << CurrentNodeBeam->Nodes[i3].NodeName;
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
    /*
     * Auto save disabled
    if(autosave.elapsed()>60000)
    {
        CurrentNodeBeam->SaveAs("autosave.beamproj");
        ui->statusBar->showMessage("Autosave: vehicle saved", 1000);
        autosave.restart();
    }
    */

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
    for(int i=0; i<2; i++)
    {
        glWidgetViews[i]->updateGL();
    }

}

void MainWindow::on_treeWidget_itemExpanded(QTreeWidgetItem *item)
{
    /*
    CurrentNodeBeam->NodeGroups[item->text(3).toInt()].draw=1;
    for(int i=0; i<2; i++)
    {
        glWidgetViews[i]->updateGL();
        qDebug() << "updating gl " << i;
    }
    */
}

/* Beams Tree Widget */
void MainWindow::on_treeWidget_2_itemCollapsed(QTreeWidgetItem *item)
{
    CurrentNodeBeam->BeamGroups[item->text(3).toInt()].draw=0;
    for(int i=0; i<2; i++)
    {
        glWidgetViews[i]->updateGL();
    }
}

void MainWindow::on_treeWidget_2_itemExpanded(QTreeWidgetItem *item)
{
    if(!RefreshLock)
    {
        if(item->text(3).toInt()<CurrentNodeBeam->BeamGroups.size()) CurrentNodeBeam->BeamGroups[item->text(3).toInt()].draw=1;
    }
    for(int i=0; i<2; i++)
    {
        glWidgetViews[i]->updateGL();
    }
}

//Add nodes button
void MainWindow::on_pushButton_6_clicked()
{



}
//Z coordinate
void MainWindow::on_lineEdit_zcoordinate_textChanged(const QString &arg1)
{
    bool ok=0;
    for(int i=0; i<4; i++)
    {
        glWidgetOViews[i]->Zcoordinate = ui->lineEdit_zcoordinate->text().toFloat(&ok);
    }
    if(!ok)
    {
        if(ui->lineEdit_zcoordinate->text()=="-");
        else
        {
            ui->lineEdit_zcoordinate->setText("0");
            for(int i=0; i<4; i++)
            {
                glWidgetOViews[i]->Zcoordinate=0;
            }
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
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->SnapToGrid = 1;
            glWidgetOViews[i]->updateGL();
        }
    }
    else
    {
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->SnapToGrid = 0;
            glWidgetOViews[i]->updateGL();
        }
    }
}
//Grid size
void MainWindow::on_spinBox_grids_valueChanged(const QString &arg1)
{
    for(int i=0; i<4; i++)
    {
        glWidgetOViews[i]->GridSize = ui->spinBox_grids->value()*0.01;
        glWidgetOViews[i]->updateGL();
    }
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
    for(int i=0; i<2; i++)
    {
        glWidgetViews[i]->ZoomFactor = ZoomF;
    }
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
    for(int i=0; i<2; i++)
    {
        glWidgetViews[i]->CurrentBeamGroup=ui->comboBox->currentIndex();
    }
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
    for(int i=0; i<4; i++)
    {
        glWidgetOViews[i]->CurrentNodeGroup=ui->comboBox_2->currentIndex();
    }
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
/* Show node names button */
void MainWindow::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked())
    {
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->ShowNodeNumbers = 1;
            glWidgetViews[i]->updateGL();
        }
    }
    else
    {
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->ShowNodeNumbers = 0;
            glWidgetViews[i]->updateGL();
        }
    }
}
/* Show node ID's button */
void MainWindow::on_checkBox_3_clicked()
{
    if(ui->checkBox_3->isChecked())
    {
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->ShowNodeNumbers1 = 1;
            glWidgetViews[i]->updateGL();
        }
    }
    else
    {
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->ShowNodeNumbers1 = 0;
            glWidgetViews[i]->updateGL();
        }
    }
}


/* Import BeamNG file triggered */
void MainWindow::on_actionImport_BeamNG_triggered()
{

    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        //CurrentNodeBeam->ReadJBeamTree(fileName);
        //CurrentNodeBeam->ImportBeamNG(fileName);

        /*New JBEAM input system trough textbox */

        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox msgBox;
            msgBox.setText("Error opening file.");
            msgBox.exec();
        }
        else
        {
            QTextStream in(&file);

            #ifndef QT_NO_CURSOR
                QApplication::setOverrideCursor(Qt::WaitCursor);
            #endif

            QString FileContents = in.readAll();


            #ifndef QT_NO_CURSOR
                QApplication::restoreOverrideCursor();
            #endif
            file.close();

            //Put file contents in JBEAM TextBox
            JBEAMtextbox->setPlainText(FileContents);
            JBEAM_ParseTextEdit();

            QStringList filepath = fileName.split('/');
            QString title = filepath.last() + " - " + EditorTitle;
            setWindowTitle(title);

            JBEAM_Filepath = fileName;
            ui->actionReload->setEnabled(true);

        }
    }

    MainNodeBeamUpdated();

}

//Reload JBEAM currently under construction
void MainWindow::on_actionReload_triggered()
{
    if (!JBEAM_Filepath.isEmpty())
    {
        //CurrentNodeBeam->ReadJBeamTree(fileName);
        //CurrentNodeBeam->ImportBeamNG(fileName);

        /*New JBEAM input system trough textbox */

        QFile file(JBEAM_Filepath);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox msgBox;
            msgBox.setText("Error opening file.");
            msgBox.exec();
        }
        else
        {
            QTextStream in(&file);

            #ifndef QT_NO_CURSOR
                QApplication::setOverrideCursor(Qt::WaitCursor);
            #endif

            QString FileContents = in.readAll();


            #ifndef QT_NO_CURSOR
                QApplication::restoreOverrideCursor();
            #endif
            file.close();

            //Put file contents in JBEAM TextBox
            JBEAMtextbox->setPlainText(FileContents);
            JBEAM_ParseTextEdit();
        }
    }

    MainNodeBeamUpdated();
}

/* Import nodes and beams from Wavefront OBJ file */
void MainWindow::on_actionImport_OBJ_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        if(CurrentNodeBeam->ImportWavefrontOBJ(fileName))
        {
            //Move contents from temp to JBEAM widget
            for(int i=0; i< CurrentNodeBeam->TempNodes.size();i++)
            {
                CurrentNodeBeam->TempNode = CurrentNodeBeam->TempNodes[i];
                JBEAM_AddNode();
            }
            for(int i=0; i< CurrentNodeBeam->TempBeams.size();i++)
            {
                CurrentNodeBeam->TempBeam = CurrentNodeBeam->TempBeams[i];
                JBEAM_AddBeam();
            }
            JBEAM_ParseTextEdit(); //Visualize JBEAM widget
            MainNodeBeamUpdated();
        }
    }
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

/* Parse JBEAM widget */
void MainWindow::on_pushButton_DeleteNode_clicked()
{
    //Text from text browser to QByteArray
    QByteArray JBeamInputText;
    JBeamInputText.append(JBEAMtextbox->toPlainText());


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
    QString fileName = QFileDialog::getSaveFileName(this, "Save As",QDir::currentPath(),"Editor file (*.beamproj);;All files (*.*)",0,QFileDialog::DontUseNativeDialog);
    if (!fileName.isEmpty())
        CurrentNodeBeam->SaveAs(fileName);
}

/* Add nodes */
void MainWindow::on_toolButton_clicked()
{
    ButtsUp(0);
    if(glWidgetOViews[0]->AddingNodes==0)
    {
        if(OpenGLViews->currentIndex()==0) OpenGLViews->setCurrentIndex(1);
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->AddingNodes=1;
        }
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(0);
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->AddingNodes=0;
        }
    }

}

/* Move nodes */
void MainWindow::on_toolButton_2_clicked()
{
    ButtsUp(1);
    if(ui->toolButton_2->isChecked())
    {
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->MovingNodes = 1;
        }
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->MovingNodes = 1;
            glWidgetViews[i]->updateGL();
        }
        ui->stackedWidget->setCurrentIndex(2);
    }
    else
    {
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->MovingNodes = 0;
        }
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->MovingNodes = 0;
        }
        ui->stackedWidget->setCurrentIndex(0);
    }
}

/* Move by typing distance, button clicked from top toolbar*/
void MainWindow::on_toolButton_21_clicked()
{
    bool NumberTest = 1;
    float MoveDistX;
    float MoveDistY;
    float MoveDistZ;
    if(ui->lineEdit_movex->text() == "") MoveDistX = 0;
    else MoveDistX = ui->lineEdit_movex->text().toFloat(&NumberTest);
    if(NumberTest)
    {
        if(ui->lineEdit_movey->text() == "") MoveDistY = 0;
        else MoveDistY = ui->lineEdit_movey->text().toFloat(&NumberTest);
        if(NumberTest)
        {
            if(ui->lineEdit_movez->text() == "") MoveDistZ = 0;
            else MoveDistZ = ui->lineEdit_movez->text().toFloat(&NumberTest);
        }
    }
    if(!NumberTest)
    {
        QMessageBox msgBox;
        msgBox.setText("The moving distance must be a number.");
        msgBox.exec();
    }
    else
    {
        for(int i2=0; i2<CurrentNodeBeam->SelectedNodes.size();i2++)
        {
            CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locX = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locX + MoveDistX;
            CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locY = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locY + MoveDistY;
            CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locZ = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locZ + MoveDistZ;

        }
    }
    for(int i=0; i<2; i++)
    {
        glWidgetViews[i]->updateGL();
    }
    for(int i=0; i<4; i++)
    {
        glWidgetOViews[i]->updateGL();
    }
}

/* Move - reset values in text boxes */
void MainWindow::on_toolButton_22_clicked()
{
    ui->lineEdit_movex->clear();
    ui->lineEdit_movey->clear();
    ui->lineEdit_movez->clear();
}

/* Scale by typing scaling factor, button clicked from top toolbar*/
void MainWindow::on_toolButton_28_clicked()
{
    bool NumberTest = 1;
    float ScaleFactorX;
    float ScaleFactorY;
    float ScaleFactorZ;
    if(ui->lineEdit_scalex->text() == "") ScaleFactorX = 1;
    else ScaleFactorX = ui->lineEdit_scalex->text().toFloat(&NumberTest);
    if(NumberTest)
    {
        if(ui->lineEdit_scaley->text() == "") ScaleFactorY = 1;
        else ScaleFactorY = ui->lineEdit_scaley->text().toFloat(&NumberTest);
        if(NumberTest)
        {
            if(ui->lineEdit_scalez->text() == "") ScaleFactorZ = 1;
            else ScaleFactorZ = ui->lineEdit_scalez->text().toFloat(&NumberTest);
        }
    }
    if(!NumberTest)
    {
        QMessageBox msgBox;
        msgBox.setText("The scaling factor must be a number.");
        msgBox.exec();
    }
    else
    {
        for(int i2=0; i2<CurrentNodeBeam->SelectedNodes.size();i2++)
        {
            CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locX = ScaleFactorX*(CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locX);
            CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locY = ScaleFactorY*(CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locY);
            CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locZ = ScaleFactorZ*(CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locZ);

        }
    }
    for(int i=0; i<2; i++)
    {
        glWidgetViews[i]->updateGL();
    }
    for(int i=0; i<4; i++)
    {
        glWidgetOViews[i]->updateGL();
    }
}


/* Scale - reset values in text boxes */
void MainWindow::on_toolButton_29_clicked()
{
    ui->lineEdit_scalex->clear();
    ui->lineEdit_scaley->clear();
    ui->lineEdit_scalez->clear();
}

/* Scale nodes */
void MainWindow::on_toolButton_3_clicked()
{
    ButtsUp(2);
    if(ui->toolButton_3->isChecked())
    {
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->ScalingNodes = 1;
        }
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->ScalingNodes = 1;
            glWidgetViews[i]->updateGL();
        }

        ui->statusBar->showMessage("Press X, Y or Z to choose lock scaling axis");
        ui->stackedWidget->setCurrentIndex(3);
    }

    else
    {
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->ScalingNodes = 0;
        }
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->ScalingNodes = 0;
            glWidgetViews[i]->updateGL();
        }
        ui->statusBar->clearMessage();
        ui->stackedWidget->setCurrentIndex(0);
    }

}

/* Rotate nodes */
void MainWindow::on_toolButton_4_clicked()
{
    ButtsUp(3);
    if(ui->toolButton_4->isChecked())
    {
        enum viewmodes {VIEW_TOP, VIEW_RIGHT, VIEW_LEFT, VIEW_BACK, VIEW_FRONT, VIEW_BOTTOM};
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i];
            if((glWidgetOViews[i]->CurrentViewMode == VIEW_TOP) || (glWidgetOViews[i]->CurrentViewMode == VIEW_BOTTOM))
            {
                glWidgetOViews[i]->RotatingNodes = 3;
            }
            else if((glWidgetOViews[i]->CurrentViewMode == VIEW_FRONT) || (glWidgetOViews[i]->CurrentViewMode == VIEW_BACK))
            {
                glWidgetOViews[i]->RotatingNodes = 2;
            }
            else if((glWidgetOViews[i]->CurrentViewMode == VIEW_RIGHT) || (glWidgetOViews[i]->CurrentViewMode == VIEW_LEFT))
            {
                glWidgetOViews[i]->RotatingNodes = 1;
            }
        }

        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->RotatingNodes = 1;
            glWidgetViews[i]->updateGL();
        }
        ui->statusBar->showMessage("Hold CTRL to rotate in 10 degree steps.",5000);
        ui->stackedWidget->setCurrentIndex(4);
    }

    else
    {
        for(int i=0; i<4 ;i++)
        {
            glWidgetOViews[i]->RotatingNodes = 0;
        }
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->RotatingNodes = 0;
            glWidgetViews[i]->updateGL();
        }
        ui->statusBar->clearMessage();
        ui->stackedWidget->setCurrentIndex(0);    
    }
}

/* Rotate buttons */
void MainWindow::on_toolButton_26_clicked()
{
    bool NumberTest = 1;
    float RotateAngX;
    float RotateAngY;
    float RotateAngZ;
    if(ui->lineEdit_rotatex->text() == "") RotateAngX = 0;
    else RotateAngX = ui->lineEdit_rotatex->text().toFloat(&NumberTest);
    if(NumberTest)
    {
        if(ui->lineEdit_rotatey->text() == "") RotateAngY = 0;
        else RotateAngY = ui->lineEdit_rotatey->text().toFloat(&NumberTest);
        if(NumberTest)
        {
            if(ui->lineEdit_rotatez->text() == "") RotateAngZ = 0;
            else RotateAngZ = ui->lineEdit_rotatez->text().toFloat(&NumberTest);
        }
    }
    if(!NumberTest)
    {
        QMessageBox msgBox;
        msgBox.setText("The rotating angle must be in degrees.");
        msgBox.exec();
    }
    else
    {
        float a1, b1, a2, b2;

        if(RotateAngX!=0)
        {
            //X-Rotation
            //Calculating rotation matrix
            // cos a1 | -sin b1
            // sin a2 |  cos b2
            a1 = qCos(qDegreesToRadians(RotateAngX));
            b1 = qSin(qDegreesToRadians(RotateAngX));
            a2 = qSin(qDegreesToRadians(RotateAngX));
            b2 = qCos(qDegreesToRadians(RotateAngX));

            for(int i2=0; i2<CurrentNodeBeam->SelectedNodes.size();i2++)
            {
                float XCoordinate = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locY;
                float YCoordinate = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locZ;
                CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locY = a1*XCoordinate - b1*YCoordinate;
                CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locZ = a2*XCoordinate + b2*YCoordinate;
            }
        }
        if(RotateAngY!=0)
        {
            //Y-Rotation
            //Calculating rotation matrix
            // cos a1 | -sin b1
            // sin a2 |  cos b2
            a1 = qCos(qDegreesToRadians(RotateAngY));
            b1 = qSin(qDegreesToRadians(RotateAngY));
            a2 = qSin(qDegreesToRadians(RotateAngY));
            b2 = qCos(qDegreesToRadians(RotateAngY));

            for(int i2=0; i2<CurrentNodeBeam->SelectedNodes.size();i2++)
            {
                float XCoordinate = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locX;
                float YCoordinate = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locZ;
                CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locX = a1*XCoordinate - b1*YCoordinate;
                CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locZ = a2*XCoordinate + b2*YCoordinate;
            }
        }
        if(RotateAngZ!=0)
        {
            //Z-Rotation
            //Calculating rotation matrix
            // cos a1 | -sin b1
            // sin a2 |  cos b2
            a1 = qCos(qDegreesToRadians(RotateAngZ));
            b1 = qSin(qDegreesToRadians(RotateAngZ));
            a2 = qSin(qDegreesToRadians(RotateAngZ));
            b2 = qCos(qDegreesToRadians(RotateAngZ));

            for(int i2=0; i2<CurrentNodeBeam->SelectedNodes.size();i2++)
            {
                float XCoordinate = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locX;
                float YCoordinate = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locY;
                CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locX = a1*XCoordinate - b1*YCoordinate;
                CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i2]].locY = a2*XCoordinate + b2*YCoordinate;
            }
        }
    }
    for(int i=0; i<2; i++)
    {
        glWidgetViews[i]->updateGL();
    }
    for(int i=0; i<4 ;i++)
    {
        glWidgetOViews[i]->updateGL();
    }
}
//Rotate - clear text boxes
void MainWindow::on_toolButton_25_clicked()
{
    ui->lineEdit_rotatex->clear();
    ui->lineEdit_rotatey->clear();
    ui->lineEdit_rotatez->clear();
}

/* Key press events */
void MainWindow::keyPressEvent(QKeyEvent * eventti)
{
    /* Add nodes */
    if(eventti->key() == Qt::Key_N)
    {
        if(!ui->toolButton->isChecked())
        {
            ui->toolButton->setChecked(1);
        }
        else
        {
            ui->toolButton->setChecked(0);
        }
        on_toolButton_clicked();
    }
    /* Add beams single */
    if(eventti->key() == Qt::Key_V)
    {
        if(!ui->toolButton_8->isChecked())
        {
            ui->toolButton_8->setChecked(1);
        }
        else
        {
            ui->toolButton_8->setChecked(0);
        }
        on_toolButton_8_clicked();
    }
    /* Add beams cont */
    if(eventti->key() == Qt::Key_B)
    {
        if(!ui->toolButton_9->isChecked())
        {
            ui->toolButton_9->setChecked(1);
        }
        else
        {
            ui->toolButton_9->setChecked(0);
        }
        on_toolButton_9_clicked();
    }
    /* Move */
    else if(eventti->key() == Qt::Key_M)
    {
        if(!ui->toolButton_2->isChecked())
        {
            ui->toolButton_2->setChecked(1);
        }
        else
        {
            ui->toolButton_2->setChecked(0);
        }
        on_toolButton_2_clicked();
    }
    /* Scale */
    else if(eventti->key() == Qt::Key_S)
    {
        if(!ui->toolButton_3->isChecked())
        {
            ui->toolButton_3->setChecked(1);
        }
        else
        {
            ui->toolButton_3->setChecked(0);
        }
        on_toolButton_3_clicked();
    }
    /* Rotate */
    else if(eventti->key() == Qt::Key_R)
    {
        if(!ui->toolButton_4->isChecked())
        {
            ui->toolButton_4->setChecked(1);
        }
        else
        {
            ui->toolButton_4->setChecked(0);
        }
        on_toolButton_4_clicked();
    }
    /* Duplicate */
    else if(eventti->key() == Qt::Key_D)
    {
        on_toolButton_5_clicked();
    }
    /* RectSelect */
    else if(eventti->key() == Qt::Key_W)
    {
        if(!ui->toolButton_6->isChecked())
        {
            ui->toolButton_6->setChecked(1);
        }
        else
        {
            ui->toolButton_6->setChecked(0);
        }
        on_toolButton_6_clicked();
    }
    /* Extrude */
    else if(eventti->key() == Qt::Key_E)
    {
        on_toolButton_14_clicked();
    }
    /* Snap to grid */
    else if(eventti->key() == Qt::Key_G)
    {
        if(!ui->checkBox_gridsnap->isChecked())
        {
            ui->checkBox_gridsnap->setChecked(1);
        }
        else
        {
            ui->checkBox_gridsnap->setChecked(0);
        }
        on_checkBox_gridsnap_stateChanged(1);
    }
    /* Choose X-axis */
    else if(eventti->key() == Qt::Key_X)
    {
        if(glWidgetOViews[0]->ScalingNodes>0)
        {
            if(EditorMode == 1)
            {
                //RoR axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->ScalingNodes=3;
                }
                ui->statusBar->showMessage("Scaling X",10000);
            }
            else
            {
                //BeamNG axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->ScalingNodes=2;
                }
                ui->statusBar->showMessage("Scaling X",10000);
            }
        }
        else if(glWidgetOViews[0]->MovingNodes>0)
        {
            if(EditorMode == 1)
            {
                //RoR axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->MovingNodes=3;
                }
                for(int i=0; i<2; i++)
                {
                    glWidgetViews[i]->MovingNodes=3;
                }
                ui->statusBar->showMessage("Moving X",10000);
            }
            else
            {
                //BeamNG axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->MovingNodes=2;
                }
                for(int i=0; i<2; i++)
                {
                    glWidgetViews[i]->MovingNodes=2;
                }
                ui->statusBar->showMessage("Moving X",10000);
            }
        }
        else if(glWidgetOViews[0]->RotatingNodes>0)
        {
            if(EditorMode == 1)
            {
                //RoR axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->RotatingNodes=2;
                }
                ui->statusBar->showMessage("Rotating X",10000);
            }
            else
            {
                //BeamNG axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->RotatingNodes=1;
                }
                ui->statusBar->showMessage("Rotating X",10000);
            }
        }
    }
    /* Choose Y-axis */
    else if(eventti->key() == Qt::Key_Y)
    {
        if(glWidgetOViews[0]->ScalingNodes>0)
        {
            if(EditorMode == 1)
            {
                //RoR axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->ScalingNodes=4;
                }
                ui->statusBar->showMessage("Scaling Y");
            }
            else
            {
                //BeamNG axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->ScalingNodes=3;
                }
                ui->statusBar->showMessage("Scaling Y");
            }
        }
        else if(glWidgetOViews[0]->MovingNodes>0)
        {
            if(EditorMode == 1)
            {
                //RoR axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->MovingNodes=4;
                }
                for(int i=0; i<2; i++)
                {
                    glWidgetViews[i]->MovingNodes=4;
                }
                ui->statusBar->showMessage("Moving Y");
            }
            else
            {
                //BeamNG axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->MovingNodes=3;
                }
                for(int i=0; i<2; i++)
                {
                    glWidgetViews[i]->MovingNodes=3;
                }
                ui->statusBar->showMessage("Moving Y");
            }
        }
        else if(glWidgetOViews[0]->RotatingNodes>0)
        {
            if(EditorMode == 1)
            {
                //RoR axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->RotatingNodes=3;
                }
                ui->statusBar->showMessage("Rotating Y");
            }
            else
            {
                //BeamNG axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->RotatingNodes=2;
                }
                ui->statusBar->showMessage("Rotating Y");
            }
        }
    }
    /* Choose Z-axis */
    else if(eventti->key() == Qt::Key_Z)
    {
        if(glWidgetOViews[0]->ScalingNodes>0)
        {
            if(EditorMode == 1)
            {
                //RoR axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->ScalingNodes=2;
                }
                ui->statusBar->showMessage("Scaling Z");
            }
            else
            {
                //BeamNG axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->ScalingNodes=4;
                }
                ui->statusBar->showMessage("Scaling Z");
            }
        }
        else if(glWidgetOViews[0]->MovingNodes>0)
        {
            if(EditorMode == 1)
            {
                //RoR axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->MovingNodes=2;
                }
                for(int i=0; i<2; i++)
                {
                    glWidgetViews[i]->MovingNodes=2;
                }
                ui->statusBar->showMessage("Moving Z");
            }
            else
            {
                //BeamNG axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->MovingNodes=4;
                }
                for(int i=0; i<2; i++)
                {
                    glWidgetViews[i]->MovingNodes=4;
                }
                ui->statusBar->showMessage("Moving Z");
            }
        }
        else if(glWidgetOViews[0]->RotatingNodes>0)
        {
            if(EditorMode == 1)
            {
                //RoR axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->RotatingNodes=1;
                }
                ui->statusBar->showMessage("Rotating Z");
            }
            else
            {
                //BeamNG axises
                for(int i=0; i<4 ;i++)
                {
                    glWidgetOViews[i]->RotatingNodes=3;
                }
                ui->statusBar->showMessage("Rotating Z");
            }
        }
    }
    /* Add to rectangle selection */
    else if(eventti->key() == Qt::Key_Shift)
    {
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->Select_AddToSelection=1;
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *eventti)
{
    if(eventti->key() == Qt::Key_Shift)
    {
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->Select_AddToSelection=0;
        }
    }
    /* Move/Scale/Rotate by step */
    else if(eventti->key() == Qt::Key_Control)
    {
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->ManipulateByStep=0;
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
    CurrentNodeBeam->SelectedNodes.clear();
    JBEAM_DeleteNodes();
    JBEAM_DeleteBeams();
    CurrentNodeBeam->JBEAM_temp.clear();
    /*
    if(ui->treeWidget->currentItem()->text(2) == "Group")
    {
        CurrentNodeBeam->DeleteNodeGroup(ui->treeWidget->currentItem()->text(3).toInt());
    }*/
    MainNodeBeamUpdated();
}

/* Rectangle select */
void MainWindow::on_toolButton_6_clicked()
{
    ButtsUp(5);
    if(ui->toolButton_6->isChecked())
    {
        //if(OpenGLViews->currentIndex()!=1) OpenGLViews->setCurrentIndex(1);
        for(int i=0; i<4 ;i++)
        {
            glWidgetOViews[i]->RectSelect=1;
        }
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->RectSelect=1;
        }
    }
    else
    {
        for(int i=0; i<4 ;i++)
        {
            glWidgetOViews[i]->RectSelect=0;
        }
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->RectSelect=0;
        }
    }

}

/* Turn all tools off except buttoni */
void MainWindow::ButtsUp(int buttoni)
{
    if(buttoni != 0)
    {
        ui->toolButton->setChecked(0);
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->AddingNodes=0;
        }
    }
    if(buttoni != 1)
    {
        ui->toolButton_2->setChecked(0);
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->MovingNodes=0;
        }
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->MovingNodes=0;
        }
    }
    if(buttoni != 2)
    {
        ui->toolButton_3->setChecked(0);
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->ScalingNodes=0;
        }
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->ScalingNodes=0;
        }
        ui->statusBar->clearMessage();
    }
    if(buttoni != 3)
    {
        ui->toolButton_4->setChecked(0);
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->RotatingNodes=0;
        }
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->RotatingNodes=0;
        }
        ui->statusBar->clearMessage();
    }
    if(buttoni != 5)
    {
        ui->toolButton_6->setChecked(0);
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->RectSelect=0;
        }
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->RectSelect=0;
        }
    }
    if(buttoni != 6)
    {
        ui->toolButton_19->setChecked(0);
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->AddingWheels=0;
        }
    }

}

/* Get's called when rectangle selection is used and the selected
 nodes must be highlighted in the tree widget */
void MainWindow::UpdateSelection()
{
    CurrentNodeBeam->SelectedNodes.clear();
    CurrentNodeBeam->SelectedNodes = CurrentNodeBeam->SelectedNodes2;


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

/* This system has performance issues.
        int i3=0;
        int selectionsize = CurrentNodeBeam->SelectedNodes2.size();
        QTreeWidgetItemIterator iterator(ui->treeWidget);

        qDebug()<< "Updating treewidget selected items";
        ui->treeWidget->setUpdatesEnabled(false);
        ui->treeWidget->clearSelection();
        RefreshLock=1;

        QTime myTimer;
        myTimer.start();

        if(selectionsize>0)
        {
            while(*iterator)
            {
                if((*iterator)->text(2) != "Group")
                {
                    if((*iterator)->text(0).toInt() == CurrentNodeBeam->SelectedNodes2[i3])
                    {
                        (*iterator)->setSelected(1);
                        //(*iterator)->setCheckState(0,Qt::Checked);
                        i3++;
                        if(i3==selectionsize) break;
                    }
                    ui->statusBar->showMessage(QString::number(myTimer.elapsed()),10000);

                }
                ++iterator;
            }
        }
        ui->treeWidget->setUpdatesEnabled(true);
        RefreshLock=0;
        qDebug() << "iterating took " << myTimer.elapsed() << " ms";
*/


}

/* Duplicate nodes */
void MainWindow::on_toolButton_5_clicked()
{
    CurrentNodeBeam->JBEAM_temp.clear();

    CurrentNodeBeam->DuplicateNodes();
    MainNodeBeamUpdated();
    UpdateSelection();

    JBEAM_AddFromTemp();

    /*
    for(int i=0; i< CurrentNodeBeam->SelectedNodes2.size();i++)
    {
        CurrentNodeBeam->TempNode = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes2[i]];
        JBEAM_AddNode();
    }
    for(int i=0; i< CurrentNodeBeam->TempBeams.size();i++)
    {
        CurrentNodeBeam->TempBeam = CurrentNodeBeam->TempBeams[i];
        JBEAM_AddBeam();
    }
    */
}



void MainWindow::on_pushButton_17_clicked()
{

}

/* Add single beam */
void MainWindow::on_toolButton_8_clicked()
{
    if(ui->toolButton_8->isChecked())
    {
        if(OpenGLViews->currentIndex()==1) OpenGLViews->setCurrentIndex(0);
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->NodePicking = 1;
            glWidgetViews[i]->AddingBeamsSingle = 1;
        }
    }
    else
    {
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->NodePicking = 0;
            glWidgetViews[i]->AddingBeamsSingle = 0;
        }
    }
    if(ui->toolButton_9->isChecked())
    {
        ui->toolButton_9->setChecked(0);
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->AddingBeamsCont=0;
        }
    }
}

/* Add continuously */
void MainWindow::on_toolButton_9_clicked()
{
    if(ui->toolButton_9->isChecked())
    {
        if(OpenGLViews->currentIndex()!=0) OpenGLViews->setCurrentIndex(0);
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->NodePicking=1;
            glWidgetViews[i]->AddingBeamsCont=1;
        }
    }
    else
    {
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->NodePicking=0;
            glWidgetViews[i]->AddingBeamsCont=0;
        }
    }
    if(ui->toolButton_8->isChecked())
    {
        ui->toolButton_8->setChecked(0);
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->AddingBeamsSingle=0;
        }
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
    CurrentNodeBeam->JBEAM_temp.clear();
    for(int i=0; i<CurrentNodeBeam->SelectedBeams.size(); i++)
    {
        qDebug() << "Deleting Beam " << CurrentNodeBeam->SelectedBeams[i];
        CurrentNodeBeam->DeleteBeam(CurrentNodeBeam->SelectedBeams[i]);
        for(int i2=i; i2<CurrentNodeBeam->SelectedBeams.size(); i2++)
        {
            CurrentNodeBeam->SelectedBeams[i2] = CurrentNodeBeam->SelectedBeams[i2]-1;
        }
    }
    JBEAM_DeleteBeams();
    MainNodeBeamUpdated();
}

/* Extrude node beam from selected nodes */
void MainWindow::on_toolButton_14_clicked()
{
    CurrentNodeBeam->JBEAM_temp.clear();

    CurrentNodeBeam->ExtrudeNodes();
    MainNodeBeamUpdated();
    UpdateSelection();
    JBEAM_AddFromTemp();
    /*
    for(int i=0; i< CurrentNodeBeam->SelectedNodes2.size();i++)
    {
        CurrentNodeBeam->TempNode = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes2[i]];
        JBEAM_AddNode();
    }

    for(int i2=0; i2< CurrentNodeBeam->TempBeamsL.size(); i2++)
    {
        for(int i=0; i< CurrentNodeBeam->TempBeamsL.at(i2).size();i++)
        {
            CurrentNodeBeam->TempBeam = CurrentNodeBeam->TempBeamsL[i2][i];
            JBEAM_AddBeam();
        }
    }
    */



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
        glWidgetOViews[0]->setViewTop();
        glWidgetOViews[0]->textureid = 0;
    }
    else if(index == 1)
    {
        //bottom
        glWidgetOViews[0]->setViewBottom();
        glWidgetOViews[0]->textureid = 1;
    }
    else if(index == 2)
    {
        //front
        glWidgetOViews[0]->setViewFront();
        glWidgetOViews[0]->textureid = 2;
    }
    else if(index == 3)
    {
        //back
        glWidgetOViews[0]->setViewBack();
        glWidgetOViews[0]->textureid = 3;
    }
    else if(index == 4)
    {
        //right
        glWidgetOViews[0]->setViewRight();
        glWidgetOViews[0]->textureid = 4;
    }
    else if(index == 5)
    {
        //left
        glWidgetOViews[0]->setViewLeft();
        glWidgetOViews[0]->textureid = 5;
    }

    glWidgetOViews[0]->updateGL();


}

/* show arrows */
void MainWindow::on_checkBox_2_clicked()
{
    if(ui->checkBox_2->isChecked())
    {
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->ShowArrows = 1;
        }
        for(int i=0; i<4 ;i++)
        {
            glWidgetOViews[i]->ShowArrows=1;
        }
    }
    else
    {
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->ShowArrows = 0;
        }
        for(int i=0; i<4 ;i++)
        {
            glWidgetOViews[i]->ShowArrows=0;
        }
    }
}

//load blueprint
void MainWindow::on_toolButton_17_clicked()
{
    /*
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
    */

}

//change blueprint scaling
void MainWindow::on_lineEdit_bluep_scale_textEdited(const QString &arg1)
{
    /*
    glWidgetO->blueprint_scale[glWidgetO->textureid] = arg1.toFloat();
    glWidgetO->updateGL();*/
}

void MainWindow::on_lineEdit_9_bpoffY_textEdited(const QString &arg1)
{
    /*
    glWidgetO->blueprint_offY[glWidgetO->textureid] = arg1.toFloat();
    glWidgetO->updateGL();
    */
}

void MainWindow::on_lineEdit_9_bpoffX_textEdited(const QString &arg1)
{
    /*
    glWidgetO->blueprint_offX[glWidgetO->textureid] = arg1.toFloat();
    glWidgetO->updateGL();
    */
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
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->AddingWheels=1;
            glWidgetViews[i]->NodePicking=1;
        }
    }
    else
    {
        for(int i=0; i<2; i++)
        {
            glWidgetViews[i]->AddingWheels=0;
            glWidgetViews[i]->NodePicking=0;
            glWidgetViews[i]->TextOverlay.clear();
        }
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
    QVector <float> backgroundcolor;
    backgroundcolor.append((AppSettings->readsetting("bg_color_r").toInt())/255.0f);
    backgroundcolor.append((AppSettings->readsetting("bg_color_b").toInt())/255.0f);
    backgroundcolor.append((AppSettings->readsetting("bg_color_g").toInt())/255.0f);
    backgroundcolor.append(1.0f);

    QVector <float> gridcolor;
    gridcolor.append((AppSettings->readsetting("grid_color_r").toInt())/255.0f);
    gridcolor.append((AppSettings->readsetting("grid_color_b").toInt())/255.0f);
    gridcolor.append((AppSettings->readsetting("grid_color_g").toInt())/255.0f);
    gridcolor.append(1.0f);

    for(int i=0; i<2; i++)
    {
        glWidgetViews[i]->backgroundcolor = backgroundcolor;
        glWidgetViews[i]->gridcolor = gridcolor;
    }
    for(int i=0; i<4; i++)
    {
        glWidgetOViews[i]->backgroundcolor = backgroundcolor;
        glWidgetOViews[i]->gridcolor = gridcolor;
    }
    CurrentNodeBeam->VehicleAuthors[0] = AppSettings->readsetting("author");
}

/* Blueprints opacity adjust */
void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    /*
    glWidgetO->blueprint_opa[glWidgetO->textureid] = ui->horizontalSlider->value()/100.0f;
    glWidgetO->updateGL();
    */
}

//Set scale by measuring distance
void MainWindow::on_toolButton_20_clicked()
{
    if(ui->toolButton_20->isChecked())
    {
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->MeasuringDistance=1;
            glWidgetOViews[i]->SetScaleByDistance=1;
        }
    }
    else
    {
        for(int i=0; i<4; i++)
        {
            glWidgetOViews[i]->MeasuringDistance=0;
            glWidgetOViews[i]->SetScaleByDistance=0;
        }
    }

}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox* about = new QMessageBox;
    about->setText(AboutBox);
    about->setWindowTitle("About NodeBEAM Editor");
    about->setStyleSheet("QMessageBox {background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(201, 201, 201, 255), stop:1 rgba(227, 227, 227, 255));}");
    about->setIconPixmap(QPixmap(":/images/images/logo.png"));
    about->setWindowIcon(QPixmap(":/icons/icons/windowicon.png"));
    about->show();
}

/* Change orthographic view zoom */
void MainWindow::on_doubleSpinBox_editingFinished()
{

}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{

    for(int i=0; i<4; i++)
    {
        glWidgetOViews[i]->ViewHeight = arg1;
        glWidgetOViews[i]->resizeGL(glWidgetOViews[i]->width(), glWidgetOViews[i]->height());
        glWidgetOViews[i]->updateGL();
    }

}


/* Create duplicating array */
void MainWindow::on_toolButton_16_clicked()
{
    if(ui->toolButton_16->isEnabled())
    {
        ui->stackedWidget->setCurrentIndex(5);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}


/* Launch Rigs of Rods with command line parameters */
void MainWindow::on_toolButton_27_clicked()
{
    QString program = AppSettings->readsetting("rorlocation");
    QProcess *myProcess = new QProcess(this);
    QString arguments = "-map ";
    arguments.append(AppSettings->readsetting("rormap"));
    arguments.append(" -truck ");
    arguments.append(AppSettings->readsetting("rorveh"));
    myProcess->setNativeArguments(arguments);
    myProcess->start(program);
}

void MainWindow::on_actionRigs_of_Rods_Wiki_triggered()
{
    QString wiki = "http://www.rigsofrods.com/wiki/pages/Main_Page";
    QDesktopServices::openUrl(QUrl(wiki));
}

void MainWindow::on_actionBeamNG_Wiki_triggered()
{
    QString wiki = "http://wiki.beamng.com/";
    QDesktopServices::openUrl(QUrl(wiki));
}

/* Run Lua script */
void MainWindow::on_actionRun_triggered()
{
    QDir luafolder;
    luafolder.current();
    luafolder.cd(tr("lua"));

    CurrentNodeBeam->JBEAM_temp.clear();   
    QString fileName = QFileDialog::getOpenFileName(this, tr("Run Lua script"),
                                              luafolder.path(),
                                              tr("Lua script (*.lua *.*)"));
    if (!fileName.isEmpty())
    {
        CurrentNodeBeam->RunLUAScript(fileName);
        LastScript=fileName;
    }
    JBEAM_AddFromTemp();
    MainNodeBeamUpdated();
    JBEAM_ParseTextEdit();
}

/* Run Lua script again */
void MainWindow::on_actionRun_again_triggered()
{
    if(LastScript.size()>0)
    {
        CurrentNodeBeam->JBEAM_temp.clear();
        CurrentNodeBeam->RunLUAScript(LastScript);
        JBEAM_AddFromTemp();
        MainNodeBeamUpdated();
        JBEAM_ParseTextEdit();
    }
}

/* Parse JBEAM widget refresh button clicked */
void MainWindow::on_pushButton_3_clicked()
{
    JBEAM_UpdateAllNodes();
    JBEAM_ParseTextEdit();
}

/* Parse JBEAM widget */
void MainWindow::JBEAM_ParseTextEdit()
{
    JBEAM_UpdateCursors(JBEAMtextbox->toPlainText());

    QByteArray textbox;
    textbox.append(JBEAMtextbox->toPlainText());
    QJsonParseError ParseError;
    ParseError = CurrentNodeBeam->ParseJBEAM_TextEdit(textbox);
    if(ParseError.offset>0)
    {
        QString errormsg = "JBEAM read error: ";
        errormsg+= ParseError.errorString();
        errormsg+= ", on line: " + QString::number(CurrentNodeBeam->JbeamErrorLine);
        StatusBar_info->setText(errormsg);

        //Move cursor near error position
        QTextCursor cursor = JBEAMtextbox->textCursor();
        cursor.setPosition(ParseError.offset);
        JBEAMtextbox->setTextCursor(cursor);
    }
    else StatusBar_info->clear();
    MainNodeBeamUpdated();
}

/* Toggle add nodes in the JBEAM edit widget at text cursor position on/off */
void MainWindow::on_checkBox_5_clicked()
{

}

/* Add node in JBEAM edit widget at text cursor position */
void MainWindow::JBEAM_AddNode()
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
    QTextCursor textcursor = JBEAMtextbox->textCursor();
    if(JBEAM_NodeCursor >= 0)
    {
        textcursor.setPosition(JBEAM_NodeCursor);
    }

    for(int i=0; i<CurrentNodeBeam->TempNode.comments.size();i++)
    {
        JBEAM_AddComment(JBEAM_NodeCursor, CurrentNodeBeam->TempNode.comments.ReadComment(i));
    }
    textcursor.insertText(nodeline);
    JBEAMtextbox->setTextCursor(textcursor);
}

/* Add beam, in JBEAM edit widget at text cursor position */
void MainWindow::JBEAM_AddBeam()
{
    QString beamline = "           [";
    beamline+= '"' + CurrentNodeBeam->TempBeam.Node1Name + '"';
    beamline+= ", ";
    beamline+= '"' + CurrentNodeBeam->TempBeam.Node2Name + '"';
    beamline+=("],\n");
    QTextCursor textcursor = JBEAMtextbox->textCursor();
    if(JBEAM_BeamCursor >= 0)
    {
        textcursor.setPosition(JBEAM_BeamCursor);
    }

    for(int i=0; i<CurrentNodeBeam->TempBeam.comments.size();i++)
    {
        JBEAM_AddComment(JBEAM_BeamCursor, CurrentNodeBeam->TempBeam.comments.ReadComment(i));
    }

    textcursor.insertText(beamline);
    JBEAMtextbox->setTextCursor(textcursor);
}

int MainWindow::JBEAM_AddComment(int CursorPos, QString Comment)
{
    QString commentline = "\u0009\u0009\u0009//";
    commentline+= Comment;
    commentline+= "\n";
    QTextCursor textcursor = JBEAMtextbox->textCursor();
    if(CursorPos >= 0)
    {
        textcursor.setPosition(CursorPos);
    }
    textcursor.insertText(commentline);
    JBEAMtextbox->setTextCursor(textcursor);

    return commentline.length();
}

/* Update selected nodes in JBEAM edit widget */
void MainWindow::JBEAM_UpdateSelectedNodes()
{

    int pos1; //Position containers for parsing the textbox
    int pos2, pos3;

    QString TextBoxText = JBEAMtextbox->toPlainText();
    for(int i=0; i<CurrentNodeBeam->SelectedNodes.size();i++)
    {
        QString NodeName = CurrentNodeBeam->Nodes[CurrentNodeBeam->SelectedNodes[i]].NodeName;

        //Find node from textbox by nodename
        if(FindNodeContainer(TextBoxText, NodeName, pos1, pos2, 0, pos3))
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
    JBEAMtextbox->setPlainText(TextBoxText);
}

/* Update all nodes */
void MainWindow::JBEAM_UpdateAllNodes()
{
    int pos1; //Position containers for parsing the textbox
    int pos2, pos3;

    QString TextBoxText = JBEAMtextbox->toPlainText();
    for(int i=0; i<CurrentNodeBeam->Nodes.size();i++)
    {
        QString NodeName = CurrentNodeBeam->Nodes[i].NodeName;

        //Find node from textbox by nodename
        if(FindNodeContainer(TextBoxText, NodeName, pos1, pos2, 0, pos3))
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
    JBEAMtextbox->setPlainText(TextBoxText);
}

/* This function will find, where the node and it's 3 coordinates are in the JBEAM string.
Results will be returned in NodeBegin and NodeEnd integer values.
Bool will be false, if node was not found.*/
bool MainWindow::FindNodeContainer(QString JBEAM_box, QString nodename, int &NodeBegin, int &NodeEnd, bool FindComma,int &RealNodeEnd)
{
    int NodeFound = 0;
    int linenumber=0;

    QString temp = "";
    int i=0;
    for(i; i<JBEAM_box.length();i++)
    {
        if(JBEAM_box[i] == ' ');
        else if (JBEAM_box.at(i) == '\u0009');
        else if (JBEAM_box.at(i) == '\n')linenumber++;
        else
        {
            temp.append(JBEAM_box.at(i));
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
        if (JBEAM_box.at(i) == '\n')linenumber++;
        else if (JBEAM_box.at(i) == '[') listlevel++;
        else if (JBEAM_box.at(i) == ']')
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
        if(JBEAM_box.at(i) == ' ');
        else if (JBEAM_box.at(i) == '\u0009');
        else if (JBEAM_box.at(i) == '\n')linenumber++;
        else
        {
            temp.append(JBEAM_box.at(i));
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
        if(JBEAM_box.at(i) == '[') break;
    }


    int node_begin = i;

    listlevel = 0;
    //Find node end
    for(i; i<nodes_end; i++)
    {
        if(JBEAM_box.at(i) == '[') listlevel++;
        else if(JBEAM_box.at(i) == ']')
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
            if(JBEAM_box.at(i) == ' ');
            else if (JBEAM_box.at(i) == '\u0009');
            else if (JBEAM_box.at(i) == '\n');
            else if (JBEAM_box.at(i) == ',')
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
            if(JBEAM_box.at(i) == '"') nodenamecalc++;
            if(nodenamecalc==2)
            {
                valuefound=1;
            }
        }
        else if(grid_id > 0)
        {
            if(JBEAM_box.at(i).isDigit())
            {
                valuefound = 1;
            }
        }

        if(valuefound)
        {
            if((JBEAM_box.at(i) == ' ') || (JBEAM_box.at(i) == ',') || (JBEAM_box.at(i) == '\u0009') || (JBEAM_box.at(i) == '\n') || (JBEAM_box.at(i) == ']'))
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

bool MainWindow::FindBeamContainer(QString JBEAM_box, QString beam, int &Begin, int &End, bool FindComma,int &RealEnd)
{
    int BeamFound = 0;

    QString temp = "";
    int i=0;
    for(i; i<JBEAM_box.length();i++)
    {
        if(JBEAM_box[i] == ' ');
        else if (JBEAM_box.at(i) == '\u0009');
        else if (JBEAM_box.at(i) == '\n');
        else
        {
            temp.append(JBEAM_box.at(i));
            if(temp.indexOf("\"beams\":") >= 0)
            {
                break;
            }
        }
    }
    int beams_begin = i;

    //Find beams section container END
    int listlevel = 0;
    for(i; i<JBEAM_box.length();i++)
    {
        if (JBEAM_box.at(i) == '\n');
        else if (JBEAM_box.at(i) == '[') listlevel++;
        else if (JBEAM_box.at(i) == ']')
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
        if(JBEAM_box.at(i) == ' ');
        else if (JBEAM_box.at(i) == '\u0009');
        else if (JBEAM_box.at(i) == '\n');
        else if (JBEAM_box.at(i) == ',');
        else
        {
            temp.append(JBEAM_box.at(i));
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
        if(JBEAM_box.at(i) == '[') break;
    }


    int beam_begin = i;

    listlevel = 0;
    //Find beam end
    for(i; i<beams_end; i++)
    {
        if(JBEAM_box.at(i) == '[') listlevel++;
        else if(JBEAM_box.at(i) == ']')
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
            if(JBEAM_box.at(i) == ' ');
            else if (JBEAM_box.at(i) == '\u0009');
            else if (JBEAM_box.at(i) == '\n');
            else if (JBEAM_box.at(i) == ',')
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

void MainWindow::JBEAM_DeleteNodes()
{
    int pos1; //Position containers for parsing the textbox
    int pos2;
    int pos3;

    QString TextBoxText = JBEAMtextbox->toPlainText();
    for(int i=0; i<CurrentNodeBeam->JBEAM_temp.delete_nodes.size();i++)
    {
        QString NodeName = CurrentNodeBeam->JBEAM_temp.delete_nodes.at(i).NodeName;

        //Find node from textbox by nodename
        if(FindNodeContainer(TextBoxText, NodeName, pos1, pos2, true, pos3))
        {
            QString nodeline = "";

            TextBoxText.replace(pos1-1,pos3-pos1+2,nodeline);
        }
        else qDebug() << "Node not found";

    }
    JBEAMtextbox->setPlainText(TextBoxText);
}

//Delete beams
void MainWindow::JBEAM_DeleteBeams()
{
    int pos1; //Position containers for parsing the textbox
    int pos2;
    int pos3;

    QString TextBoxText = JBEAMtextbox->toPlainText();
    for(int i=0; i<CurrentNodeBeam->JBEAM_temp.delete_beams.size();i++)
    {
        QString Beam = "\"" + CurrentNodeBeam->JBEAM_temp.delete_beams.at(i).Node1Name + "\"";
        Beam += "\"" + CurrentNodeBeam->JBEAM_temp.delete_beams.at(i).Node2Name + "\"";

        //Find node from textbox by nodename
        if(FindBeamContainer(TextBoxText, Beam, pos1, pos2, true, pos3))
        {
            QString beamline = "";

            TextBoxText.replace(pos1,pos3-pos1+2,beamline);
        }
        else qDebug() << "Beam not found";

    }
    JBEAMtextbox->setPlainText(TextBoxText);
    CurrentNodeBeam->JBEAM_temp.clear();
}

/* Place new nodes at current text cursor location */
void MainWindow::on_pushButton_SetNodeCursor_clicked()
{
    //Prepare new cursor
    int CurrentCursorPos = JBEAMtextbox->textCursor().position();
    //Remove old cursor
    QString JBEAM_text = JBEAMtextbox->toPlainText();
    JBEAM_text.replace("//BNEnodes\n", "          \n");
    JBEAMtextbox->setPlainText(JBEAM_text);

    QTextCursor cursor = JBEAMtextbox->textCursor();
    cursor.setPosition(CurrentCursorPos);
    cursor.insertText("\n\u0009\u0009\u0009//BNEnodes\n");
    JBEAMtextbox->setTextCursor(cursor);

}
/* Place new beams at current text cursor location */
void MainWindow::on_pushButton_SetBeamCursor_clicked()
{
    //Prepare new cursor
    int CurrentCursorPos = JBEAMtextbox->textCursor().position();
    //Remove old cursor
    QString JBEAM_text = JBEAMtextbox->toPlainText();
    JBEAM_text.replace("//BNEbeams\n", "          \n");
    JBEAMtextbox->setPlainText(JBEAM_text);

    QTextCursor cursor = JBEAMtextbox->textCursor();
    cursor.setPosition(CurrentCursorPos);
    cursor.insertText("\n\u0009\u0009\u0009//BNEbeams\n");
    JBEAMtextbox->setTextCursor(cursor);

}

/* Find node and beam cursors from JBEAM string in JBEAM textbox */
void MainWindow::JBEAM_UpdateCursors(QString JBEAM_box)
{
    QString JBEAM_texti = JBEAMtextbox->toPlainText();
    JBEAM_NodeCursor = JBEAM_box.indexOf("//BNEnodes");
    if(JBEAM_NodeCursor>=0)
    {
        for(int i=JBEAM_NodeCursor; i>0;i--)
        {
            if(JBEAM_texti[i] == '\n')
            {
                JBEAM_NodeCursor = i;
                break;
            }
            else JBEAM_NodeCursor = -1;
        }
    }
    //qDebug() << "Node cursor: " << JBEAM_NodeCursor;
    JBEAM_BeamCursor = JBEAM_box.indexOf("//BNEbeams");
    if(JBEAM_BeamCursor>=0)
    {
        for(int i=JBEAM_BeamCursor; i>0;i--)
        {
            if(JBEAM_texti[i] == '\n')
            {
                JBEAM_BeamCursor = i;
                break;
            }
            else JBEAM_BeamCursor = -1;
        }
    }
    //qDebug() << "Beam cursor: " << JBEAM_BeamCursor;
}

/* JBEAM text edit box Text changed event */
void MainWindow::on_JBEAM_textChanged()
{
    JBEAM_UpdateCursors(JBEAMtextbox->toPlainText());
}



/* Signal from OpenGL view to update JBEAM */
void MainWindow::JBEAM_Update()
{
    /*
    QVector <int> tempselected = CurrentNodeBeam->SelectedNodes;
    JBEAM_ParseTextEdit();
    CurrentNodeBeam->SelectedNodes = tempselected;
    */
}

/* Open blueprint adjustment widget for ortographic view */
void MainWindow::AdjustBlueprint(int WidgetID)
{
    blueprints bps;
    bps.glwidgetortho = glWidgetOViews[WidgetID];
    QObject::connect(&bps,SIGNAL(updateGL()),glWidgetOViews[0],SLOT(updateGL()));
    bps.exec();
}

/* Add beams and nodes from temp */
void MainWindow::JBEAM_AddFromTemp()
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

/* Open files dragged and dropped on the window */
void MainWindow::dropEvent(QDropEvent * dropevent)
{
    qDebug() << "tiedostoa pukkaa " << dropevent->mimeData()->urls();
    QList <QUrl> urls = dropevent->mimeData()->urls();
    if(urls.size()>0)
    {
        QString url = urls.at(0).url();
        QString fileName = url;
        fileName.replace("file:///","");

        QStringList suffix = url.split(".");
        QString suf = "jbeam";
        QString suf2 = "lua";
        if(suf.compare(suffix.last(),Qt::CaseInsensitive) == 0)
        {
            //Open JBEAM
            if (!fileName.isEmpty())
            {
                /* JBEAM input system trough textbox */

                QFile file(fileName);
                if (!file.open(QFile::ReadOnly | QFile::Text)) {
                    QMessageBox msgBox;
                    msgBox.setText("Error opening file.");
                    msgBox.exec();
                }
                else
                {
                    QTextStream in(&file);

                    #ifndef QT_NO_CURSOR
                        QApplication::setOverrideCursor(Qt::WaitCursor);
                    #endif

                    QString FileContents = in.readAll();


                    #ifndef QT_NO_CURSOR
                        QApplication::restoreOverrideCursor();
                    #endif
                    file.close();

                    //Put file contents in JBEAM TextBox
                    JBEAMtextbox->setPlainText(FileContents);
                    JBEAM_ParseTextEdit();

                    QStringList filepath = fileName.split('/');
                    QString title = filepath.last() + " - " + EditorTitle;
                    setWindowTitle(title);

                }
            }

            MainNodeBeamUpdated();
        }
        else if(suf2.compare(suffix.last(),Qt::CaseInsensitive) == 0)
        {
            //Run Lua script
            CurrentNodeBeam->JBEAM_temp.clear();
            if (!fileName.isEmpty())
            {
                CurrentNodeBeam->RunLUAScript(fileName);
                LastScript=fileName;
            }
            JBEAM_AddFromTemp();
            MainNodeBeamUpdated();
            JBEAM_ParseTextEdit();
        }
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    //User drags a file on the widget
    QList <QUrl> urls = event->mimeData()->urls();
    if(urls.size()>0)
    {
        QString url = urls.at(0).url();
        QStringList suffix = url.split(".");
        QString suf = "jbeam";
        QString suf2 = "lua";
        if(suf.compare(suffix.last(),Qt::CaseInsensitive) == 0)
        {
            event->acceptProposedAction();
        }
        else if(suf2.compare(suffix.last(),Qt::CaseInsensitive) == 0)
        {
            event->acceptProposedAction();
        }
    }
}

void MainWindow::LUA_message(QString msg)
{
    ui->textBrowser->append(msg);
}

/* ==== JBEAM TEXT EDIT ==== */
JBEAM_TextBox::JBEAM_TextBox(QWidget *parent) : QPlainTextEdit(parent)
{
    LineNumbersA = new LineNumbers(this);
    setViewportMargins(26,0,0,0);
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumbers(QRect,int)));
}

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



