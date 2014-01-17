/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave_As;
    QAction *actionExport_to_BeamNG;
    QAction *actionExport_to_Rigs_of_Rods;
    QAction *actionExit;
    QAction *actionImport_Rigs_of_Rods;
    QAction *actionImport_BeamNG;
    QAction *actionAbout;
    QAction *actionEdit_details;
    QAction *actionChassis_Generator;
    QAction *actionTracks;
    QAction *actionImport_OBJ;
    QAction *actionSettings;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_8;
    QFrame *frame_3;
    QVBoxLayout *verticalLayout;
    QTabWidget *EditorTab;
    QWidget *tab;
    QComboBox *comboBox_2;
    QFrame *frame_2;
    QLineEdit *lineEdit_zcoordinate;
    QSpinBox *spinBox_grids;
    QLabel *label_zcoordinate;
    QLabel *label_7grids;
    QLineEdit *lineEdit;
    QLabel *label_zcoordinate_2;
    QToolButton *toolButton;
    QToolButton *toolButton_2;
    QToolButton *toolButton_3;
    QToolButton *toolButton_4;
    QToolButton *toolButton_5;
    QToolButton *toolButton_6;
    QToolButton *toolButton_7;
    QToolButton *toolButton_13;
    QCheckBox *checkBox;
    QToolButton *toolButton_14;
    QToolButton *toolButton_15;
    QToolButton *toolButton_16;
    QCheckBox *checkBox_gridsnap;
    QCheckBox *checkBox_2;
    QWidget *tab_2;
    QComboBox *comboBox;
    QToolButton *toolButton_8;
    QToolButton *toolButton_9;
    QToolButton *toolButton_10;
    QToolButton *toolButton_11;
    QToolButton *toolButton_12;
    QWidget *tab_4;
    QToolButton *toolButton_19;
    QWidget *tab_3;
    QToolButton *toolButton_17;
    QLabel *label_10_bluepname;
    QLabel *label_11_currView;
    QLineEdit *lineEdit_bluep_scale;
    QLabel *label_10;
    QLineEdit *lineEdit_9_bpoffX;
    QLineEdit *lineEdit_9_bpoffY;
    QLabel *label_11_bpox;
    QLabel *label_11_bpoy;
    QToolButton *toolButton_18;
    QSlider *horizontalSlider;
    QToolButton *toolButton_20;
    QWidget *tab_6;
    QHBoxLayout *horizontalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab_nodes;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_6;
    QTreeWidget *treeWidget;
    QFrame *frame_4;
    QLabel *label_5nodename;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_2;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label;
    QPushButton *pushButton_5;
    QLabel *label_6nodeID;
    QLineEdit *lineEdit_nodename;
    QLabel *label_7;
    QPushButton *pushButton_13;
    QPushButton *pushButton_14;
    QPushButton *pushButton_20;
    QWidget *tab_beams;
    QVBoxLayout *verticalLayout_7;
    QTreeWidget *treeWidget_2;
    QGroupBox *groupBox;
    QLineEdit *lineEdit_6;
    QLineEdit *lineEdit_7;
    QLineEdit *lineEdit_8;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_8;
    QPushButton *pushButton_15;
    QPushButton *pushButton_16;
    QPushButton *pushButton_17;
    QPushButton *pushButton_21_hidebeams;
    QLabel *label_9;
    QPushButton *pushButton_21;
    QWidget *tab_wheels;
    QVBoxLayout *verticalLayout_4;
    QListWidget *listWidget;
    QFrame *frame_6;
    QLineEdit *lineEdit_hubwheel;
    QLineEdit *lineEdit_hubwheel_2;
    QLineEdit *lineEdit_hubwheel_3;
    QLineEdit *lineEdit_hubwheel_4;
    QLineEdit *lineEdit_hubwheel_5;
    QLineEdit *lineEdit_hubwheel_6;
    QLabel *label_hubwheel;
    QLabel *label_hubwheel_2;
    QLabel *label_hubwheel_3;
    QLabel *label_hubwheel_4;
    QLabel *label_hubwheel_5;
    QLabel *label_hubwheel_6;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout;
    QFrame *frame_5;
    QSpinBox *spinBox;
    QComboBox *comboBox_3_views;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuGenerate;
    QMenu *menuHelp;
    QMenu *menuNobeBeam;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::NonModal);
        MainWindow->resize(1159, 771);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setWindowOpacity(1);
        MainWindow->setStyleSheet(QStringLiteral(""));
        MainWindow->setDockNestingEnabled(false);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        QIcon icon;
        icon.addFile(QStringLiteral("../icons/document-new-7.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QStringLiteral("../icons/NewFile.png"), QSize(), QIcon::Normal, QIcon::On);
        actionNew->setIcon(icon);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon1;
        icon1.addFile(QStringLiteral("../icons/document-open-7.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon1);
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        QIcon icon2;
        icon2.addFile(QStringLiteral("../icons/document-save-5.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_As->setIcon(icon2);
        actionExport_to_BeamNG = new QAction(MainWindow);
        actionExport_to_BeamNG->setObjectName(QStringLiteral("actionExport_to_BeamNG"));
        actionExport_to_Rigs_of_Rods = new QAction(MainWindow);
        actionExport_to_Rigs_of_Rods->setObjectName(QStringLiteral("actionExport_to_Rigs_of_Rods"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionImport_Rigs_of_Rods = new QAction(MainWindow);
        actionImport_Rigs_of_Rods->setObjectName(QStringLiteral("actionImport_Rigs_of_Rods"));
        actionImport_BeamNG = new QAction(MainWindow);
        actionImport_BeamNG->setObjectName(QStringLiteral("actionImport_BeamNG"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionEdit_details = new QAction(MainWindow);
        actionEdit_details->setObjectName(QStringLiteral("actionEdit_details"));
        actionChassis_Generator = new QAction(MainWindow);
        actionChassis_Generator->setObjectName(QStringLiteral("actionChassis_Generator"));
        actionTracks = new QAction(MainWindow);
        actionTracks->setObjectName(QStringLiteral("actionTracks"));
        actionImport_OBJ = new QAction(MainWindow);
        actionImport_OBJ->setObjectName(QStringLiteral("actionImport_OBJ"));
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName(QStringLiteral("actionSettings"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setStyleSheet(QLatin1String("QWidget#centralWidget {\n"
"	background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(121, 121, 121, 255), stop:1 rgba(207, 207, 207, 255));\n"
"\n"
"}\n"
"QToolButton {\n"
"	background-color: #e2e2e2;\n"
"     border-style: outset;\n"
"     border-width: 0px;\n"
"     font:  12px;\n"
"	\n"
"	color: rgb(0, 0, 0);\n"
"     padding: 6px;\n"
"}\n"
"QToolButton:checked {\n"
"  background-color:rgb(176, 176, 255);\n"
"\n"
"     border-style:solid;\n"
"     border-width: 0px;\n"
"     border-radius: 0px;\n"
"     border-color:rgb(255, 255, 0);\n"
"     font:  12px;\n"
"}\n"
"QToolButton:hover {\n"
"  background-color:rgb(197, 208, 255);\n"
"\n"
"     border-style:solid;\n"
"     border-width: 0px;\n"
"     border-radius: 0px;\n"
"     border-color:rgb(255, 255, 0);\n"
"     font:  12px;\n"
"}\n"
"QPushButton {\n"
"	background-color: rgb(68, 68, 68);\n"
"     border-style: outset;\n"
"     border-width: 2px;\n"
"     border-radius: 10px;\n"
"     border-color: orange;\n"
"     font:  12px;\n"
""
                        "	\n"
"	color: rgb(255, 255, 255);\n"
"     padding: 6px;\n"
"}\n"
" QPushButton:checked {\n"
"     background-color: rgb(255, 119, 16);\n"
"     border-style: inset;\n"
" }\n"
" QPushButton:pressed {\n"
"     background-color: rgb(205, 109, 11);\n"
"     border-style: inset;\n"
" }\n"
"QPushButton:Disabled {\n"
"	background-color: rgb(68, 68, 68);\n"
"     border-style: outset;\n"
"     border-width: 2px;\n"
"     border-radius: 10px;\n"
"     border-color: grey;\n"
"     font:  12px;\n"
"	\n"
"	color: grey;\n"
"     padding: 6px;\n"
"}\n"
"QWidget#tab {\n"
"	background-color:  #e2e2e2;\n"
"}\n"
"QWidget#tab_2 {\n"
"	background-color:  #e2e2e2;\n"
"}\n"
"QWidget#tab_3 {\n"
"	background-color:  #e2e2e2;\n"
"}\n"
"QWidget#tab_4 {\n"
"	background-color:  #e2e2e2;\n"
"}\n"
"QWidget#tab_5 {\n"
"	background-color:  #e2e2e2;\n"
"}\n"
"QWidget#tab_nodes {\n"
"	background-color:  #e2e2e2;\n"
"}\n"
"QWidget#tab_beams {\n"
"	background-color:  #e2e2e2;\n"
"}\n"
"QWidget#tab_wheels {\n"
"	background-color:  #e2e2e2;\n"
"}"
                        "\n"
" QTabWidget::pane { /* The tab widget frame */\n"
"     border-top: 2px solid #e2e2e2;\n"
"\n"
" }\n"
"QTabWidget::tab-bar {\n"
"     left: 1px; /* move to the right by 5px */\n"
" }\n"
" QTabBar::tab {\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,\n"
"                                 stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
"     border: 2px solid #C4C4C3;\n"
"     border-bottom-color: #C2C7CB; /* same as the pane color */\n"
"     border-top-left-radius: 4px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 2px;\n"
" }\n"
"\n"
" QTabBar::tab:selected, QTabBar::tab:hover {\n"
"    /* background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #fafafa, stop: 0.4 #f4f4f4,\n"
"                                 stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);*/\n"
"	background:  #e2e2e2;\n"
"\n"
" }\n"
"\n"
" QTabBar::tab:selected {\n"
"     border-color:"
                        " #9B9B9B;\n"
"     border-bottom-color: #e2e2e2; /* same as pane color */\n"
" }\n"
"\n"
" QTabBar::tab:!selected {\n"
"     margin-top: 2px; /* make non-selected tabs look smaller */\n"
" }\n"
"\n"
" QToolBox::tab {\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,\n"
"                                 stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
"     border-radius: 5px;\n"
"     color: darkgray;\n"
" }\n"
"\n"
" QToolBox::tab:selected { /* italicize selected tabs */\n"
"     font: Bold;\n"
"     color: Grey;\n"
" }\n"
""));
        verticalLayout_8 = new QVBoxLayout(centralWidget);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(-1, 2, -1, -1);
        frame_3 = new QFrame(centralWidget);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame_3->sizePolicy().hasHeightForWidth());
        frame_3->setSizePolicy(sizePolicy1);
        frame_3->setMinimumSize(QSize(215, 131));
        frame_3->setStyleSheet(QStringLiteral(""));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame_3);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        EditorTab = new QTabWidget(frame_3);
        EditorTab->setObjectName(QStringLiteral("EditorTab"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(EditorTab->sizePolicy().hasHeightForWidth());
        EditorTab->setSizePolicy(sizePolicy2);
        EditorTab->setMinimumSize(QSize(200, 0));
        EditorTab->setStyleSheet(QLatin1String("QGroupBox#groupBox {\n"
"	\n"
"	border-color: rgb(223, 223, 223);\n"
"\n"
"}"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        comboBox_2 = new QComboBox(tab);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));
        comboBox_2->setGeometry(QRect(730, 60, 131, 28));
        frame_2 = new QFrame(tab);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(930, 10, 161, 81));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        lineEdit_zcoordinate = new QLineEdit(frame_2);
        lineEdit_zcoordinate->setObjectName(QStringLiteral("lineEdit_zcoordinate"));
        lineEdit_zcoordinate->setGeometry(QRect(10, 40, 61, 20));
        spinBox_grids = new QSpinBox(frame_2);
        spinBox_grids->setObjectName(QStringLiteral("spinBox_grids"));
        spinBox_grids->setGeometry(QRect(10, 60, 61, 22));
        spinBox_grids->setMinimum(1);
        spinBox_grids->setMaximum(100);
        spinBox_grids->setSingleStep(5);
        spinBox_grids->setValue(25);
        label_zcoordinate = new QLabel(frame_2);
        label_zcoordinate->setObjectName(QStringLiteral("label_zcoordinate"));
        label_zcoordinate->setGeometry(QRect(80, 40, 81, 21));
        label_7grids = new QLabel(frame_2);
        label_7grids->setObjectName(QStringLiteral("label_7grids"));
        label_7grids->setGeometry(QRect(80, 60, 61, 21));
        lineEdit = new QLineEdit(frame_2);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(10, 20, 61, 20));
        label_zcoordinate_2 = new QLabel(frame_2);
        label_zcoordinate_2->setObjectName(QStringLiteral("label_zcoordinate_2"));
        label_zcoordinate_2->setGeometry(QRect(80, 20, 81, 21));
        toolButton = new QToolButton(tab);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        toolButton->setGeometry(QRect(0, 0, 71, 101));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/icons/NewNode.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon3);
        toolButton->setIconSize(QSize(60, 60));
        toolButton->setCheckable(true);
        toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_2 = new QToolButton(tab);
        toolButton_2->setObjectName(QStringLiteral("toolButton_2"));
        toolButton_2->setGeometry(QRect(70, 0, 71, 101));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/icons/MoveNode.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_2->setIcon(icon4);
        toolButton_2->setIconSize(QSize(60, 60));
        toolButton_2->setCheckable(true);
        toolButton_2->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_3 = new QToolButton(tab);
        toolButton_3->setObjectName(QStringLiteral("toolButton_3"));
        toolButton_3->setGeometry(QRect(140, 0, 71, 101));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/icons/ScaleNode.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_3->setIcon(icon5);
        toolButton_3->setIconSize(QSize(60, 60));
        toolButton_3->setCheckable(true);
        toolButton_3->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_4 = new QToolButton(tab);
        toolButton_4->setObjectName(QStringLiteral("toolButton_4"));
        toolButton_4->setGeometry(QRect(210, 0, 71, 101));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icons/icons/RotateNode.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_4->setIcon(icon6);
        toolButton_4->setIconSize(QSize(60, 60));
        toolButton_4->setCheckable(true);
        toolButton_4->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_5 = new QToolButton(tab);
        toolButton_5->setObjectName(QStringLiteral("toolButton_5"));
        toolButton_5->setGeometry(QRect(280, 0, 71, 101));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icons/icons/DuplicateNode.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_5->setIcon(icon7);
        toolButton_5->setIconSize(QSize(60, 60));
        toolButton_5->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_6 = new QToolButton(tab);
        toolButton_6->setObjectName(QStringLiteral("toolButton_6"));
        toolButton_6->setGeometry(QRect(350, 0, 81, 101));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/icons/icons/RectSelNode.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_6->setIcon(icon8);
        toolButton_6->setIconSize(QSize(60, 60));
        toolButton_6->setCheckable(true);
        toolButton_6->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_7 = new QToolButton(tab);
        toolButton_7->setObjectName(QStringLiteral("toolButton_7"));
        toolButton_7->setGeometry(QRect(430, 0, 71, 101));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/icons/icons/DeleteNode.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_7->setIcon(icon9);
        toolButton_7->setIconSize(QSize(60, 60));
        toolButton_7->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_13 = new QToolButton(tab);
        toolButton_13->setObjectName(QStringLiteral("toolButton_13"));
        toolButton_13->setGeometry(QRect(870, 60, 51, 31));
        checkBox = new QCheckBox(tab);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(730, 10, 141, 20));
        toolButton_14 = new QToolButton(tab);
        toolButton_14->setObjectName(QStringLiteral("toolButton_14"));
        toolButton_14->setGeometry(QRect(510, 30, 61, 31));
        toolButton_15 = new QToolButton(tab);
        toolButton_15->setObjectName(QStringLiteral("toolButton_15"));
        toolButton_15->setGeometry(QRect(510, 60, 61, 31));
        toolButton_16 = new QToolButton(tab);
        toolButton_16->setObjectName(QStringLiteral("toolButton_16"));
        toolButton_16->setGeometry(QRect(510, 0, 61, 31));
        checkBox_gridsnap = new QCheckBox(tab);
        checkBox_gridsnap->setObjectName(QStringLiteral("checkBox_gridsnap"));
        checkBox_gridsnap->setGeometry(QRect(730, 30, 101, 21));
        checkBox_2 = new QCheckBox(tab);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setGeometry(QRect(590, 10, 131, 20));
        EditorTab->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        comboBox = new QComboBox(tab_2);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(310, 40, 141, 28));
        toolButton_8 = new QToolButton(tab_2);
        toolButton_8->setObjectName(QStringLiteral("toolButton_8"));
        toolButton_8->setGeometry(QRect(0, 0, 71, 101));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/icons/icons/BeamsAddS.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_8->setIcon(icon10);
        toolButton_8->setIconSize(QSize(60, 60));
        toolButton_8->setCheckable(true);
        toolButton_8->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_9 = new QToolButton(tab_2);
        toolButton_9->setObjectName(QStringLiteral("toolButton_9"));
        toolButton_9->setGeometry(QRect(70, 0, 71, 101));
        toolButton_9->setIcon(icon10);
        toolButton_9->setIconSize(QSize(60, 60));
        toolButton_9->setCheckable(true);
        toolButton_9->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_10 = new QToolButton(tab_2);
        toolButton_10->setObjectName(QStringLiteral("toolButton_10"));
        toolButton_10->setGeometry(QRect(140, 0, 71, 101));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/icons/icons/BeamsDelete.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_10->setIcon(icon11);
        toolButton_10->setIconSize(QSize(60, 60));
        toolButton_10->setCheckable(false);
        toolButton_10->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_11 = new QToolButton(tab_2);
        toolButton_11->setObjectName(QStringLiteral("toolButton_11"));
        toolButton_11->setGeometry(QRect(210, 0, 71, 101));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/icons/icons/BeamsArg.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_11->setIcon(icon12);
        toolButton_11->setIconSize(QSize(60, 60));
        toolButton_11->setCheckable(false);
        toolButton_11->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_12 = new QToolButton(tab_2);
        toolButton_12->setObjectName(QStringLiteral("toolButton_12"));
        toolButton_12->setEnabled(false);
        toolButton_12->setGeometry(QRect(460, 40, 51, 31));
        EditorTab->addTab(tab_2, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        toolButton_19 = new QToolButton(tab_4);
        toolButton_19->setObjectName(QStringLiteral("toolButton_19"));
        toolButton_19->setGeometry(QRect(0, 0, 101, 101));
        toolButton_19->setCheckable(true);
        EditorTab->addTab(tab_4, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        toolButton_17 = new QToolButton(tab_3);
        toolButton_17->setObjectName(QStringLiteral("toolButton_17"));
        toolButton_17->setGeometry(QRect(10, 60, 101, 31));
        label_10_bluepname = new QLabel(tab_3);
        label_10_bluepname->setObjectName(QStringLiteral("label_10_bluepname"));
        label_10_bluepname->setGeometry(QRect(120, 60, 211, 31));
        label_11_currView = new QLabel(tab_3);
        label_11_currView->setObjectName(QStringLiteral("label_11_currView"));
        label_11_currView->setGeometry(QRect(10, 0, 121, 31));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_11_currView->setFont(font);
        label_11_currView->setAlignment(Qt::AlignCenter);
        lineEdit_bluep_scale = new QLineEdit(tab_3);
        lineEdit_bluep_scale->setObjectName(QStringLiteral("lineEdit_bluep_scale"));
        lineEdit_bluep_scale->setGeometry(QRect(60, 35, 51, 22));
        label_10 = new QLabel(tab_3);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(20, 30, 41, 31));
        lineEdit_9_bpoffX = new QLineEdit(tab_3);
        lineEdit_9_bpoffX->setObjectName(QStringLiteral("lineEdit_9_bpoffX"));
        lineEdit_9_bpoffX->setGeometry(QRect(160, 35, 61, 22));
        lineEdit_9_bpoffY = new QLineEdit(tab_3);
        lineEdit_9_bpoffY->setObjectName(QStringLiteral("lineEdit_9_bpoffY"));
        lineEdit_9_bpoffY->setGeometry(QRect(160, 10, 61, 22));
        label_11_bpox = new QLabel(tab_3);
        label_11_bpox->setObjectName(QStringLiteral("label_11_bpox"));
        label_11_bpox->setGeometry(QRect(140, 7, 20, 31));
        label_11_bpoy = new QLabel(tab_3);
        label_11_bpoy->setObjectName(QStringLiteral("label_11_bpoy"));
        label_11_bpoy->setGeometry(QRect(140, 30, 16, 31));
        toolButton_18 = new QToolButton(tab_3);
        toolButton_18->setObjectName(QStringLiteral("toolButton_18"));
        toolButton_18->setGeometry(QRect(290, 60, 121, 31));
        horizontalSlider = new QSlider(tab_3);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(240, 10, 91, 21));
        horizontalSlider->setOrientation(Qt::Horizontal);
        toolButton_20 = new QToolButton(tab_3);
        toolButton_20->setObjectName(QStringLiteral("toolButton_20"));
        toolButton_20->setGeometry(QRect(240, 30, 91, 31));
        toolButton_20->setCheckable(true);
        EditorTab->addTab(tab_3, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QStringLiteral("tab_6"));
        EditorTab->addTab(tab_6, QString());

        verticalLayout->addWidget(EditorTab);


        verticalLayout_8->addWidget(frame_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy3);
        frame->setMinimumSize(QSize(255, 400));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(frame);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setStyleSheet(QLatin1String("QGroupBox#groupBox {\n"
"	\n"
"	border-color: rgb(223, 223, 223);\n"
"\n"
"}"));
        tab_nodes = new QWidget();
        tab_nodes->setObjectName(QStringLiteral("tab_nodes"));
        QSizePolicy sizePolicy4(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(tab_nodes->sizePolicy().hasHeightForWidth());
        tab_nodes->setSizePolicy(sizePolicy4);
        verticalLayout_3 = new QVBoxLayout(tab_nodes);
        verticalLayout_3->setSpacing(5);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(2, 5, 2, 5);
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        treeWidget = new QTreeWidget(tab_nodes);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setAlternatingRowColors(true);
        treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        treeWidget->setHeaderHidden(true);

        verticalLayout_6->addWidget(treeWidget);

        frame_4 = new QFrame(tab_nodes);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
        frame_4->setSizePolicy(sizePolicy5);
        frame_4->setMinimumSize(QSize(0, 150));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        label_5nodename = new QLabel(frame_4);
        label_5nodename->setObjectName(QStringLiteral("label_5nodename"));
        label_5nodename->setGeometry(QRect(0, 30, 71, 20));
        label_5nodename->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_3 = new QLineEdit(frame_4);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(80, 50, 89, 20));
        lineEdit_4 = new QLineEdit(frame_4);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(80, 70, 89, 20));
        lineEdit_5 = new QLineEdit(frame_4);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(80, 90, 89, 20));
        lineEdit_2 = new QLineEdit(frame_4);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(80, 110, 89, 20));
        label_2 = new QLabel(frame_4);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 50, 31, 21));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(frame_4);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(40, 70, 31, 21));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_4 = new QLabel(frame_4);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(40, 90, 31, 21));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label = new QLabel(frame_4);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 110, 61, 21));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButton_5 = new QPushButton(frame_4);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setEnabled(false);
        pushButton_5->setGeometry(QRect(170, 100, 101, 31));
        label_6nodeID = new QLabel(frame_4);
        label_6nodeID->setObjectName(QStringLiteral("label_6nodeID"));
        label_6nodeID->setGeometry(QRect(80, 10, 71, 21));
        lineEdit_nodename = new QLineEdit(frame_4);
        lineEdit_nodename->setObjectName(QStringLiteral("lineEdit_nodename"));
        lineEdit_nodename->setGeometry(QRect(80, 30, 89, 20));
        label_7 = new QLabel(frame_4);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(50, 10, 20, 21));
        pushButton_13 = new QPushButton(frame_4);
        pushButton_13->setObjectName(QStringLiteral("pushButton_13"));
        pushButton_13->setGeometry(QRect(170, 30, 101, 28));
        pushButton_14 = new QPushButton(frame_4);
        pushButton_14->setObjectName(QStringLiteral("pushButton_14"));
        pushButton_14->setGeometry(QRect(170, 60, 101, 28));
        pushButton_20 = new QPushButton(frame_4);
        pushButton_20->setObjectName(QStringLiteral("pushButton_20"));
        pushButton_20->setGeometry(QRect(170, 0, 101, 28));

        verticalLayout_6->addWidget(frame_4);


        verticalLayout_3->addLayout(verticalLayout_6);

        tabWidget->addTab(tab_nodes, QString());
        tab_beams = new QWidget();
        tab_beams->setObjectName(QStringLiteral("tab_beams"));
        verticalLayout_7 = new QVBoxLayout(tab_beams);
        verticalLayout_7->setSpacing(5);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(2, 5, 2, 5);
        treeWidget_2 = new QTreeWidget(tab_beams);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QStringLiteral("1"));
        treeWidget_2->setHeaderItem(__qtreewidgetitem1);
        treeWidget_2->setObjectName(QStringLiteral("treeWidget_2"));
        treeWidget_2->setSelectionMode(QAbstractItemView::ExtendedSelection);
        treeWidget_2->setHeaderHidden(true);

        verticalLayout_7->addWidget(treeWidget_2);

        groupBox = new QGroupBox(tab_beams);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy6);
        groupBox->setMinimumSize(QSize(0, 170));
        lineEdit_6 = new QLineEdit(groupBox);
        lineEdit_6->setObjectName(QStringLiteral("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(70, 50, 61, 22));
        lineEdit_7 = new QLineEdit(groupBox);
        lineEdit_7->setObjectName(QStringLiteral("lineEdit_7"));
        lineEdit_7->setGeometry(QRect(70, 70, 61, 22));
        lineEdit_8 = new QLineEdit(groupBox);
        lineEdit_8->setObjectName(QStringLiteral("lineEdit_8"));
        lineEdit_8->setGeometry(QRect(70, 90, 61, 22));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 50, 53, 21));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 70, 53, 21));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(0, 90, 61, 21));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButton_15 = new QPushButton(groupBox);
        pushButton_15->setObjectName(QStringLiteral("pushButton_15"));
        pushButton_15->setGeometry(QRect(140, 80, 101, 31));
        pushButton_16 = new QPushButton(groupBox);
        pushButton_16->setObjectName(QStringLiteral("pushButton_16"));
        pushButton_16->setGeometry(QRect(140, 20, 101, 28));
        pushButton_17 = new QPushButton(groupBox);
        pushButton_17->setObjectName(QStringLiteral("pushButton_17"));
        pushButton_17->setGeometry(QRect(140, 50, 101, 28));
        pushButton_21_hidebeams = new QPushButton(groupBox);
        pushButton_21_hidebeams->setObjectName(QStringLiteral("pushButton_21_hidebeams"));
        pushButton_21_hidebeams->setGeometry(QRect(30, 20, 93, 28));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(30, 120, 191, 16));
        pushButton_21 = new QPushButton(groupBox);
        pushButton_21->setObjectName(QStringLiteral("pushButton_21"));
        pushButton_21->setGeometry(QRect(140, 130, 101, 28));

        verticalLayout_7->addWidget(groupBox);

        tabWidget->addTab(tab_beams, QString());
        tab_wheels = new QWidget();
        tab_wheels->setObjectName(QStringLiteral("tab_wheels"));
        QSizePolicy sizePolicy7(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(tab_wheels->sizePolicy().hasHeightForWidth());
        tab_wheels->setSizePolicy(sizePolicy7);
        verticalLayout_4 = new QVBoxLayout(tab_wheels);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        listWidget = new QListWidget(tab_wheels);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        verticalLayout_4->addWidget(listWidget);

        frame_6 = new QFrame(tab_wheels);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        sizePolicy6.setHeightForWidth(frame_6->sizePolicy().hasHeightForWidth());
        frame_6->setSizePolicy(sizePolicy6);
        frame_6->setMinimumSize(QSize(0, 200));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        lineEdit_hubwheel = new QLineEdit(frame_6);
        lineEdit_hubwheel->setObjectName(QStringLiteral("lineEdit_hubwheel"));
        lineEdit_hubwheel->setGeometry(QRect(60, 20, 91, 22));
        lineEdit_hubwheel_2 = new QLineEdit(frame_6);
        lineEdit_hubwheel_2->setObjectName(QStringLiteral("lineEdit_hubwheel_2"));
        lineEdit_hubwheel_2->setGeometry(QRect(60, 40, 91, 22));
        lineEdit_hubwheel_3 = new QLineEdit(frame_6);
        lineEdit_hubwheel_3->setObjectName(QStringLiteral("lineEdit_hubwheel_3"));
        lineEdit_hubwheel_3->setGeometry(QRect(60, 60, 91, 22));
        lineEdit_hubwheel_4 = new QLineEdit(frame_6);
        lineEdit_hubwheel_4->setObjectName(QStringLiteral("lineEdit_hubwheel_4"));
        lineEdit_hubwheel_4->setGeometry(QRect(60, 80, 91, 22));
        lineEdit_hubwheel_5 = new QLineEdit(frame_6);
        lineEdit_hubwheel_5->setObjectName(QStringLiteral("lineEdit_hubwheel_5"));
        lineEdit_hubwheel_5->setGeometry(QRect(60, 100, 91, 22));
        lineEdit_hubwheel_6 = new QLineEdit(frame_6);
        lineEdit_hubwheel_6->setObjectName(QStringLiteral("lineEdit_hubwheel_6"));
        lineEdit_hubwheel_6->setGeometry(QRect(60, 120, 91, 22));
        label_hubwheel = new QLabel(frame_6);
        label_hubwheel->setObjectName(QStringLiteral("label_hubwheel"));
        label_hubwheel->setGeometry(QRect(0, 20, 53, 21));
        label_hubwheel_2 = new QLabel(frame_6);
        label_hubwheel_2->setObjectName(QStringLiteral("label_hubwheel_2"));
        label_hubwheel_2->setGeometry(QRect(0, 40, 53, 21));
        label_hubwheel_3 = new QLabel(frame_6);
        label_hubwheel_3->setObjectName(QStringLiteral("label_hubwheel_3"));
        label_hubwheel_3->setGeometry(QRect(0, 60, 53, 21));
        label_hubwheel_4 = new QLabel(frame_6);
        label_hubwheel_4->setObjectName(QStringLiteral("label_hubwheel_4"));
        label_hubwheel_4->setGeometry(QRect(0, 80, 61, 21));
        label_hubwheel_5 = new QLabel(frame_6);
        label_hubwheel_5->setObjectName(QStringLiteral("label_hubwheel_5"));
        label_hubwheel_5->setGeometry(QRect(0, 100, 53, 21));
        label_hubwheel_6 = new QLabel(frame_6);
        label_hubwheel_6->setObjectName(QStringLiteral("label_hubwheel_6"));
        label_hubwheel_6->setGeometry(QRect(0, 120, 61, 21));
        pushButton = new QPushButton(frame_6);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(160, 110, 93, 31));
        pushButton_2 = new QPushButton(frame_6);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(160, 20, 93, 31));

        verticalLayout_4->addWidget(frame_6);

        tabWidget->addTab(tab_wheels, QString());

        verticalLayout_2->addWidget(tabWidget);


        horizontalLayout_2->addWidget(frame);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

        verticalLayout_5->addLayout(horizontalLayout);

        frame_5 = new QFrame(centralWidget);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        sizePolicy1.setHeightForWidth(frame_5->sizePolicy().hasHeightForWidth());
        frame_5->setSizePolicy(sizePolicy1);
        frame_5->setMinimumSize(QSize(0, 30));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        spinBox = new QSpinBox(frame_5);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(10, 0, 61, 31));
        spinBox->setMaximum(500);
        spinBox->setSingleStep(10);
        spinBox->setValue(100);
        comboBox_3_views = new QComboBox(frame_5);
        comboBox_3_views->setObjectName(QStringLiteral("comboBox_3_views"));
        comboBox_3_views->setGeometry(QRect(80, 0, 161, 31));

        verticalLayout_5->addWidget(frame_5);


        horizontalLayout_2->addLayout(verticalLayout_5);


        verticalLayout_8->addLayout(horizontalLayout_2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1159, 26));
        menuBar->setAutoFillBackground(true);
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuGenerate = new QMenu(menuBar);
        menuGenerate->setObjectName(QStringLiteral("menuGenerate"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuNobeBeam = new QMenu(menuBar);
        menuNobeBeam->setObjectName(QStringLiteral("menuNobeBeam"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuNobeBeam->menuAction());
        menuBar->addAction(menuGenerate->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave_As);
        menuFile->addSeparator();
        menuFile->addAction(actionImport_BeamNG);
        menuFile->addAction(actionImport_Rigs_of_Rods);
        menuFile->addAction(actionImport_OBJ);
        menuFile->addSeparator();
        menuFile->addAction(actionExport_to_BeamNG);
        menuFile->addAction(actionExport_to_Rigs_of_Rods);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionSettings);
        menuGenerate->addAction(actionChassis_Generator);
        menuGenerate->addAction(actionTracks);
        menuHelp->addAction(actionAbout);
        menuNobeBeam->addAction(actionEdit_details);

        retranslateUi(MainWindow);

        EditorTab->setCurrentIndex(0);
        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionNew->setText(QApplication::translate("MainWindow", "New", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        actionSave_As->setText(QApplication::translate("MainWindow", "Save As ...", 0));
        actionExport_to_BeamNG->setText(QApplication::translate("MainWindow", "Export to BeamNG ...", 0));
        actionExport_to_Rigs_of_Rods->setText(QApplication::translate("MainWindow", "Export to Rigs of Rods ...", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionImport_Rigs_of_Rods->setText(QApplication::translate("MainWindow", "Import Rigs of Rods ...", 0));
        actionImport_BeamNG->setText(QApplication::translate("MainWindow", "Import BeamNG ...", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        actionEdit_details->setText(QApplication::translate("MainWindow", "Edit details", 0));
        actionChassis_Generator->setText(QApplication::translate("MainWindow", "Chassis", 0));
        actionTracks->setText(QApplication::translate("MainWindow", "Tracks", 0));
        actionImport_OBJ->setText(QApplication::translate("MainWindow", "Import Wavefront OBJ ...", 0));
        actionSettings->setText(QApplication::translate("MainWindow", "Settings", 0));
#ifndef QT_NO_TOOLTIP
        lineEdit_zcoordinate->setToolTip(QApplication::translate("MainWindow", "The Z coordinate of new nodes.", 0));
#endif // QT_NO_TOOLTIP
        label_zcoordinate->setText(QApplication::translate("MainWindow", "Z coordinate", 0));
        label_7grids->setText(QApplication::translate("MainWindow", "Grid size", 0));
        label_zcoordinate_2->setText(QApplication::translate("MainWindow", "Name prefix", 0));
        toolButton->setText(QApplication::translate("MainWindow", "Add", 0));
        toolButton_2->setText(QApplication::translate("MainWindow", "Move", 0));
        toolButton_3->setText(QApplication::translate("MainWindow", "Scale", 0));
        toolButton_4->setText(QApplication::translate("MainWindow", "Rotate", 0));
        toolButton_5->setText(QApplication::translate("MainWindow", "Duplicate", 0));
        toolButton_6->setText(QApplication::translate("MainWindow", "Rect. Select", 0));
        toolButton_7->setText(QApplication::translate("MainWindow", "Delete", 0));
        toolButton_13->setText(QApplication::translate("MainWindow", "Apply", 0));
        checkBox->setText(QApplication::translate("MainWindow", "Show node names", 0));
        toolButton_14->setText(QApplication::translate("MainWindow", "Extrude", 0));
        toolButton_15->setText(QApplication::translate("MainWindow", "Mirror", 0));
        toolButton_16->setText(QApplication::translate("MainWindow", "Array", 0));
        checkBox_gridsnap->setText(QApplication::translate("MainWindow", "Snap to grid", 0));
        checkBox_2->setText(QApplication::translate("MainWindow", "Show axis arrows", 0));
        EditorTab->setTabText(EditorTab->indexOf(tab), QApplication::translate("MainWindow", "Nodes", 0));
        toolButton_8->setText(QApplication::translate("MainWindow", "Add Single", 0));
        toolButton_9->setText(QApplication::translate("MainWindow", "Add Cont", 0));
        toolButton_10->setText(QApplication::translate("MainWindow", "Delete", 0));
        toolButton_11->setText(QApplication::translate("MainWindow", "Arguments", 0));
        toolButton_12->setText(QApplication::translate("MainWindow", "Apply", 0));
        EditorTab->setTabText(EditorTab->indexOf(tab_2), QApplication::translate("MainWindow", "Beams", 0));
        toolButton_19->setText(QApplication::translate("MainWindow", "Add hubwheels", 0));
        EditorTab->setTabText(EditorTab->indexOf(tab_4), QApplication::translate("MainWindow", "Wheels", 0));
        toolButton_17->setText(QApplication::translate("MainWindow", "Load blueprint", 0));
        label_10_bluepname->setText(QApplication::translate("MainWindow", "No blueprint loaded", 0));
        label_11_currView->setText(QApplication::translate("MainWindow", "TOP VIEW", 0));
#ifndef QT_NO_TOOLTIP
        lineEdit_bluep_scale->setToolTip(QApplication::translate("MainWindow", "Blueprint width in meters", 0));
#endif // QT_NO_TOOLTIP
        label_10->setText(QApplication::translate("MainWindow", "Scale", 0));
#ifndef QT_NO_TOOLTIP
        lineEdit_9_bpoffX->setToolTip(QApplication::translate("MainWindow", "Blueprint offset Y", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lineEdit_9_bpoffY->setToolTip(QApplication::translate("MainWindow", "Blueprint offset X", 0));
#endif // QT_NO_TOOLTIP
        label_11_bpox->setText(QApplication::translate("MainWindow", "X", 0));
        label_11_bpoy->setText(QApplication::translate("MainWindow", "Y", 0));
        toolButton_18->setText(QApplication::translate("MainWindow", "Test in BeamNG", 0));
#ifndef QT_NO_TOOLTIP
        toolButton_20->setToolTip(QApplication::translate("MainWindow", "Set scale of the blueprint by showing known distance", 0));
#endif // QT_NO_TOOLTIP
        toolButton_20->setText(QApplication::translate("MainWindow", "Set scale", 0));
        EditorTab->setTabText(EditorTab->indexOf(tab_3), QApplication::translate("MainWindow", "Editor", 0));
        EditorTab->setTabText(EditorTab->indexOf(tab_6), QApplication::translate("MainWindow", "Page", 0));
        label_5nodename->setText(QApplication::translate("MainWindow", "Node Name", 0));
        label_2->setText(QApplication::translate("MainWindow", "LocX", 0));
        label_3->setText(QApplication::translate("MainWindow", "LocY", 0));
        label_4->setText(QApplication::translate("MainWindow", "LocZ", 0));
        label->setText(QApplication::translate("MainWindow", "Properties", 0));
        pushButton_5->setText(QApplication::translate("MainWindow", "Apply", 0));
        label_6nodeID->setText(QString());
        label_7->setText(QApplication::translate("MainWindow", "ID", 0));
        pushButton_13->setText(QApplication::translate("MainWindow", "New Group", 0));
        pushButton_14->setText(QApplication::translate("MainWindow", "Move to Group", 0));
        pushButton_20->setText(QApplication::translate("MainWindow", "Hide all", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_nodes), QApplication::translate("MainWindow", "Nodes", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Beam properties", 0));
        label_5->setText(QApplication::translate("MainWindow", "Node 1", 0));
        label_6->setText(QApplication::translate("MainWindow", "Node 2", 0));
        label_8->setText(QApplication::translate("MainWindow", "Properties", 0));
        pushButton_15->setText(QApplication::translate("MainWindow", "Apply", 0));
        pushButton_16->setText(QApplication::translate("MainWindow", "New Group", 0));
        pushButton_17->setText(QApplication::translate("MainWindow", "Move to Group", 0));
        pushButton_21_hidebeams->setText(QApplication::translate("MainWindow", "Hide all", 0));
        label_9->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_21->setToolTip(QApplication::translate("MainWindow", "Choose arguments for selected beams", 0));
#endif // QT_NO_TOOLTIP
        pushButton_21->setText(QApplication::translate("MainWindow", "Set arguments", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_beams), QApplication::translate("MainWindow", "Beams", 0));
        label_hubwheel->setText(QApplication::translate("MainWindow", "Name", 0));
        label_hubwheel_2->setText(QApplication::translate("MainWindow", "Node1", 0));
        label_hubwheel_3->setText(QApplication::translate("MainWindow", "Node2", 0));
        label_hubwheel_4->setText(QApplication::translate("MainWindow", "NodeArm", 0));
        label_hubwheel_5->setText(QApplication::translate("MainWindow", "NodeS", 0));
        label_hubwheel_6->setText(QApplication::translate("MainWindow", "Direction", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Apply", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "Arguments", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_wheels), QApplication::translate("MainWindow", "Hubwheels", 0));
#ifndef QT_NO_TOOLTIP
        spinBox->setToolTip(QApplication::translate("MainWindow", "View zoom", 0));
#endif // QT_NO_TOOLTIP
        spinBox->setSuffix(QApplication::translate("MainWindow", "%", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0));
        menuGenerate->setTitle(QApplication::translate("MainWindow", "Generate", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        menuNobeBeam->setTitle(QApplication::translate("MainWindow", "NobeBeam", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
