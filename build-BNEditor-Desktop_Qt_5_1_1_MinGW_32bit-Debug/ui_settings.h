/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_settings
{
public:
    QDialogButtonBox *buttonBox;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGroupBox *groupBox;
    QLineEdit *lineEdit;
    QLabel *label;
    QLabel *label_5;
    QWidget *page_2;
    QGroupBox *groupBox_2;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLabel *label_4;
    QLabel *label_6;
    QWidget *page_3;
    QLabel *label_7;
    QGroupBox *groupBox_3;
    QFrame *frame;
    QLabel *label_8;
    QPushButton *pushButton_2;
    QFrame *frame_2;
    QLabel *label_9;
    QPushButton *pushButton_3;
    QListWidget *listWidget;

    void setupUi(QDialog *settings)
    {
        if (settings->objectName().isEmpty())
            settings->setObjectName(QStringLiteral("settings"));
        settings->resize(657, 390);
        buttonBox = new QDialogButtonBox(settings);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(10, 340, 641, 41));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);
        stackedWidget = new QStackedWidget(settings);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setGeometry(QRect(180, 10, 471, 331));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        groupBox = new QGroupBox(page);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(0, 40, 461, 121));
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(190, 30, 181, 28));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 30, 171, 28));
        label_5 = new QLabel(page);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(0, 0, 451, 41));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_5->setFont(font);
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        groupBox_2 = new QGroupBox(page_2);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(0, 40, 461, 131));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 20, 131, 28));
        lineEdit_2 = new QLineEdit(groupBox_2);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(140, 20, 201, 28));
        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(350, 20, 93, 28));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 50, 101, 28));
        lineEdit_3 = new QLineEdit(groupBox_2);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(140, 50, 201, 28));
        lineEdit_4 = new QLineEdit(groupBox_2);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(140, 80, 201, 28));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 80, 101, 28));
        label_6 = new QLabel(page_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(0, 0, 451, 41));
        label_6->setFont(font);
        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        label_7 = new QLabel(page_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(0, 0, 451, 41));
        label_7->setFont(font);
        groupBox_3 = new QGroupBox(page_3);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(0, 40, 471, 141));
        frame = new QFrame(groupBox_3);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(10, 20, 161, 31));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_8 = new QLabel(frame);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 0, 111, 31));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButton_2 = new QPushButton(frame);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(130, 0, 31, 31));
        frame_2 = new QFrame(groupBox_3);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(10, 60, 161, 31));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        label_9 = new QLabel(frame_2);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(10, 0, 111, 31));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButton_3 = new QPushButton(frame_2);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(130, 0, 31, 31));
        stackedWidget->addWidget(page_3);
        listWidget = new QListWidget(settings);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(10, 10, 161, 331));

        retranslateUi(settings);
        QObject::connect(buttonBox, SIGNAL(accepted()), settings, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), settings, SLOT(reject()));

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(settings);
    } // setupUi

    void retranslateUi(QDialog *settings)
    {
        settings->setWindowTitle(QApplication::translate("settings", "Properties", 0));
        groupBox->setTitle(QApplication::translate("settings", "Author details", 0));
        label->setText(QApplication::translate("settings", "Your name / vehicle author", 0));
        label_5->setText(QApplication::translate("settings", "General", 0));
        groupBox_2->setTitle(QApplication::translate("settings", "BeamNG command line arguments", 0));
        label_2->setText(QApplication::translate("settings", "BeamNG exe location", 0));
        pushButton->setText(QApplication::translate("settings", "Browse", 0));
        label_3->setText(QApplication::translate("settings", "Current vehicle", 0));
        label_4->setText(QApplication::translate("settings", "Current level", 0));
        label_6->setText(QApplication::translate("settings", "Testing", 0));
        label_7->setText(QApplication::translate("settings", "User interface", 0));
        groupBox_3->setTitle(QApplication::translate("settings", "Colors", 0));
        label_8->setText(QApplication::translate("settings", "Background", 0));
        pushButton_2->setText(QString());
        label_9->setText(QApplication::translate("settings", "Grid", 0));
        pushButton_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class settings: public Ui_settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
