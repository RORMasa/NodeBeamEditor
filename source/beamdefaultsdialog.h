#pragma once
#include "NodeBeam.h"
#include <QTableWidget>
#include <QDialog>


namespace Ui {
class BeamDefaultsDialog;
}

class BeamDefaultsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit BeamDefaultsDialog(QWidget *parent = 0);
    ~BeamDefaultsDialog();
    void setNBPointer(QVector<BeamDefs> *NBPointer1);
    void RefreshGUI();
    void LoadAll();

    /* Make new set of beam properties */
    void NewProperties();

    /* Load beam properties from file */
    void LoadPropertiesFromFile(const QString &fileName);

    bool firstload;

    enum {NODE_ARGUMENTS, BEAM_ARGUMENTS, HUBWHEEL_ARGUMENTS};
    int argumenttype;

    bool selecting;
    int selected;

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_comboBox_activated(int index);

    void on_buttonBox_accepted();

private:
    Ui::BeamDefaultsDialog *ui;

    int lastactiveindex;
    QVector <int> RGB_color;

    QVector <BeamDefs> *NBPointer;
    QTableWidgetItem *item;

    QString filepath;
    void SetFilePath();

};
