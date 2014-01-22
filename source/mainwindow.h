#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QListWidgetItem>
#include "NodeBeam.h"
#include <QTreeWidget>
#include "beamdefaultsdialog.h"
#include "settings.h"

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
public slots:
    void MainNodeBeamUpdated();
    void SettingsUpdated();
    void UpdateSelection();
    void ShowContextMenu_Beams(const QPoint &position);

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

private:
    Ui::MainWindow *ui;
    GLWidget *glWidget;
    GLWidgetOrtho *glWidgetO;
    NodeBeam *CurrentNodeBeam;
    BeamDefaultsDialog *BeamProperties;
    BeamDefaultsDialog *HubWheelProperties;
    QTreeWidgetItem *item;
    QTreeWidgetItem *item2;
    QTabWidget *OpenGLViews;

    void loadFile(const QString &fileName);
    int CurrentBeamGroupi;
    int CurrentNodeGroupi;

    //Don't refresh GUI
    bool RefreshLock;

    //Lift all nodes tab buttons up, except buttoni
    void ButtsUp(int buttoni);

    QString AboutBox;
};

