#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QByteArray FixCommas(QByteArray JbeamText);
    void FixCommas_NextChar(QString &sample, QChar &nextchar, int &nextchar_i);
    void FixCommas_PrevChar(QString &sample, QChar &prevchar, int &prevchar_i);

    //Comment remove
    QByteArray JBEAM_RemoveComments(QByteArray JbeamText);

    //
    //Find JBEAM node from string
    bool FindNodeContainer(QString JBEAM_box, QString nodename, int &NodeBegin, int &NodeEnd);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
