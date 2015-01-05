#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QListWidgetItem>
#include "NodeBeam.h"
#include <QTreeWidget>
#include "beamdefaultsdialog.h"
#include "settings.h"
#include <QTime>
#include <QLabel>

namespace Ui {
class MainWindow;

}
class GLWidget;
class GLWidgetOrtho;
class NodeBeam;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//    NodeBeam *ActiveNodeBeams;
    settings * AppSettings;
    QLabel * StatusBar_info;
    QLabel * StatusBar_mode;
    QLabel * StatusBar_nodecount;

public slots:
    void MainNodeBeamUpdated();
    void SettingsUpdated();
    void UpdateSelection();
    void ShowContextMenu_Beams(const QPoint &position);
    void JBEAM_AddNode();
    void JBEAM_AddBeam();
    void JBEAM_AddComment(int CursorPos, QString Comment);
    void JBEAM_UpdateSelectedNodes();
    void JBEAM_UpdateAllNodes();
    void JBEAM_Update();
    void AdjustBlueprint(int WidgetID);
    void LUA_message(QString msg);

signals:
    void ZoomChanged();

private slots:
    //oid on_pushButton_clicked();

    //void on_pushButton_2_clicked();

    void on_actionOpen_triggered();

    void on_actionNew_triggered();

    void on_pushButton_6_clicked();

    void on_lineEdit_zcoordinate_textChanged(const QString &arg1);

    void on_checkBox_gridsnap_stateChanged(int arg1);

    void on_spinBox_grids_valueChanged(const QString &arg1);

    void on_treeWidget_itemSelectionChanged();

    void on_pushButton_5_clicked();

    void on_pushButton_13_clicked();

    void on_treeWidget_itemCollapsed(QTreeWidgetItem *item);

    void on_treeWidget_itemExpanded(QTreeWidgetItem *item);

    void on_treeWidget_2_itemCollapsed(QTreeWidgetItem *item);

    void on_treeWidget_2_itemExpanded(QTreeWidgetItem *item);

    void on_actionExport_to_BeamNG_triggered();

    void on_treeWidget_2_itemSelectionChanged();

    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_16_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(const QString &arg1);

    void on_pushButton_19_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_hidebeams_clicked();

    void on_pushButton_22_shownodenumbers_clicked();

    void on_actionImport_BeamNG_triggered();

    void on_actionChassis_Generator_triggered();

    void on_actionTracks_triggered();

    void on_actionImport_OBJ_triggered();

    void on_actionImport_Rigs_of_Rods_triggered();

    void on_pushButton_3_clicked();

    void on_pushButton_DeleteNode_clicked();

    void on_actionExit_triggered();


    void on_pushButton_21_clicked();

    void on_actionExport_to_Rigs_of_Rods_triggered();

    void on_actionSave_As_triggered();

    void on_pushButton_14_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_4_clicked();

    void on_toolButton_7_clicked();

    void on_toolButton_6_clicked();

    void on_toolButton_5_clicked();

    void on_pushButton_17_clicked();

    void on_toolButton_8_clicked();

    void on_toolButton_9_clicked();

    void on_toolButton_11_clicked();

    void on_toolButton_12_clicked();

    void on_toolButton_13_clicked();

    void on_checkBox_clicked();

    void on_toolButton_10_clicked();

    void on_toolButton_14_clicked();

    void on_lineEdit_editingFinished();

    void on_lineEdit_textChanged(const QString &arg1);

    void keyPressEvent(QKeyEvent * eventti);
    void keyReleaseEvent(QKeyEvent *eventti);
    void dropEvent(QDropEvent * dropevent);
    void dragEnterEvent(QDragEnterEvent *event);

    void on_comboBox_3_views_activated(int index);

    void on_checkBox_2_clicked();

    void on_toolButton_17_clicked();

    void on_lineEdit_bluep_scale_textEdited(const QString &arg1);

    void on_lineEdit_9_bpoffY_textEdited(const QString &arg1);

    void on_lineEdit_9_bpoffX_textEdited(const QString &arg1);

    void on_toolButton_18_clicked();

    void on_actionSettings_triggered();

    void on_toolButton_19_clicked();

    void on_listWidget_activated(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_toolButton_20_clicked();

    void on_actionAbout_triggered();

    void on_doubleSpinBox_editingFinished();

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_checkBox_3_clicked();

    void on_toolButton_21_clicked();

    void on_toolButton_22_clicked();

    void on_toolButton_16_clicked();

    void on_toolButton_26_clicked();

    void on_toolButton_25_clicked();

    void on_toolButton_27_clicked();

    void on_actionRigs_of_Rods_Wiki_triggered();

    void on_actionBeamNG_Wiki_triggered();

    void on_actionRun_triggered();

    void on_checkBox_5_clicked();

    void on_pushButton_SetNodeCursor_clicked();

    void on_pushButton_SetBeamCursor_clicked();

    void on_textEdit_JBEAM_textChanged();

    void on_toolButton_29_clicked();

    void on_toolButton_28_clicked();

    void on_actionRun_again_triggered();

private:
    Ui::MainWindow *ui;
    GLWidget *glWidget;
    GLWidget *glWidgetViews[2];
    GLWidgetOrtho *glWidgetO;
    GLWidgetOrtho* glWidgetOViews[4];

    NodeBeam *CurrentNodeBeam;
    BeamDefaultsDialog *BeamProperties;
    BeamDefaultsDialog *HubWheelProperties;
    QTreeWidgetItem *item;
    QTreeWidgetItem *item2;
    QTabWidget *OpenGLViews;

    QTime autosave;

    void loadFile(const QString &fileName);
    int CurrentBeamGroupi;
    int CurrentNodeGroupi;

    //Don't refresh GUI
    bool RefreshLock;

    //Lift all nodes tab buttons up, except buttoni
    void ButtsUp(int buttoni);

    QString AboutBox;

    bool EditorMode; //BeamNG or RoR Axises in use

    //Parse JBEAM TextEdit Box
    void JBEAM_ParseTextEdit();

    //Find JBEAM node or beam from string
    bool FindNodeContainer(QString JBEAM_box, QString nodename, int &NodeBegin, int &NodeEnd, bool FindComma, int &RealNodeEnd);
    bool FindBeamContainer(QString JBEAM_box, QString beam, int &Begin, int &End, bool FindComma,int &RealEnd);

    void JBEAM_UpdateCursors(QString JBEAM_box);
    //Node JBEAM cursor; if is set, add new nodes at cursor position. Value is -1 if not set
    int JBEAM_NodeCursor;

    //Beam JBEAM cursor; if is set, add new beams at cursor position. Value is -1 if not set
    int JBEAM_BeamCursor;

    void JBEAM_AddFromTemp();
    void JBEAM_DeleteNodes();
    void JBEAM_DeleteBeams();

    QString EditorTitle;

    //File name of previously ran script
    QString LastScript;
};

