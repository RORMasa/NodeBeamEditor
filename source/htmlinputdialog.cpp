#include "htmlinputdialog.h"
#include "ui_htmlinputdialog.h"

HtmlInputDialog::HtmlInputDialog(QWidget *parent, QString html) :
    QDialog(parent),
    ui(new Ui::HtmlInputDialog)
{
    //Create webview and add it in layout
    ui->setupUi(this);
    webview = new QWebView;
    ui->verticalLayout->addWidget(webview);

    //Find out html path
    QString path = qApp->applicationDirPath();
    path+= "/" + html;

    //Load the page
    url_path =  url_path.fromLocalFile(path);
    webview->load(url_path);

    //Connect signal
    QObject::connect(webview,SIGNAL(urlChanged(QUrl)),this,SLOT(UrlChanged(QUrl)));
}

HtmlInputDialog::~HtmlInputDialog()
{
    delete ui;
}

//URL is changed when the form is submitted
void HtmlInputDialog::UrlChanged(const QUrl url)
{
    //Take values from the url
    QString result_str = url.toString();
    if(result_str.length()>url_path.toString().length())
    {
        int i = 0;
        for(i=result_str.length()-1; i>=0 ; i--)
        {
            if(result_str.at(i) == '?') break;
        }
        result_str = result_str.right(result_str.length()-i-1);
        QStringList values = result_str.split("&");

        for(int i=0; i<values.size();i++)
        {
            QStringList temp = values.at(i).split("=");
            result.append(temp);
        }

        this->close();
    }
}
