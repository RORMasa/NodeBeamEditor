#pragma once

#include <QDialog>

namespace Ui {
class TracksGenerator;
}

class TracksGenerator : public QDialog
{
    Q_OBJECT

public:
    explicit TracksGenerator(QWidget *parent = 0);
    ~TracksGenerator();

private:
    Ui::TracksGenerator *ui;
};

