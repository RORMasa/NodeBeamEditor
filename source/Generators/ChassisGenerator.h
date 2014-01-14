#pragma once

#include <QDialog>
#include <QString>
#include <QVector>
#include <QObject>
#include <QColor>

#include "../NodeBeam.h"

struct nodes {
  int node_number;
  float locx;
  float locy;
  float locz;
  QString properties;
};

struct beams {
  int node1;
  int node2;
  QString propertiesb;
};

namespace Ui {
class ChassisGenerator;
}

class ChassisGenerator : public QDialog
{
    Q_OBJECT
    
public:
    explicit ChassisGenerator(QWidget *parent = 0);
    ~ChassisGenerator();

    void generate_chassis();
    void write_file();
    void MoveToEditor();


    QVector< QVector< nodes > > set;
    QVector< QVector< beams > > bset;

    float width, height, length;
    int segments, BeamNumber[15], NodeAmount;

    NodeBeam *NBPointer;

    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::ChassisGenerator *ui;

    void EmptyAll();
    int i, i2, i3;
};

