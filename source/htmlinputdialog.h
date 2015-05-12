#ifndef HTMLINPUTDIALOG_H
#define HTMLINPUTDIALOG_H

#include <QDialog>
#include <QWebView>

namespace Ui {
class HtmlInputDialog;
}

class HtmlInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HtmlInputDialog(QWidget *parent = 0, QString html = 0);
    ~HtmlInputDialog();

    QList <QStringList> result;

private:
    Ui::HtmlInputDialog *ui;

    QWebView * webview;
    QUrl url_path;

private slots:
    void UrlChanged(const QUrl url);
};

#endif // HTMLINPUTDIALOG_H
