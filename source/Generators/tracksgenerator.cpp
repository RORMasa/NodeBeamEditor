#include "tracksgenerator.h"
#include "ui_tracksgenerator.h"

TracksGenerator::TracksGenerator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TracksGenerator)
{
    ui->setupUi(this);
}

TracksGenerator::~TracksGenerator()
{
    delete ui;
}
