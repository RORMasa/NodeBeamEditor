#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QSettings>
#include <QString>

namespace Ui {
class settings;
}

class settings : public QDialog
{
    Q_OBJECT
    
public:
    explicit settings(QWidget *parent = 0);
    ~settings();

    /* Save settings */
    void SaveSettings();
    QString readsetting(QString key1);

    QSettings *appsett;

signals:
    void SettingsUpdated();
private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_listWidget_currentRowChanged(int currentRow);

    void on_pushButton_3_clicked();

private:
    Ui::settings *ui;
    void showEvent(QShowEvent *);


    //Colors
    QColor bg_color;
    QColor grid_color;

};

#endif // SETTINGS_H
