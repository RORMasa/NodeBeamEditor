/********************************************************************************
** Form generated from reading UI file 'beamdefaultsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEAMDEFAULTSDIALOG_H
#define UI_BEAMDEFAULTSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BeamDefaultsDialog
{
public:
    QDialogButtonBox *buttonBox;
    QComboBox *comboBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QTableWidget *tableWidget;
    QPushButton *pushButton_4;
    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *pushButton_5;
    QLabel *label_2;
    QWidget *tab_2;
    QPushButton *pushButton_3;

    void setupUi(QDialog *BeamDefaultsDialog)
    {
        if (BeamDefaultsDialog->objectName().isEmpty())
            BeamDefaultsDialog->setObjectName(QStringLiteral("BeamDefaultsDialog"));
        BeamDefaultsDialog->resize(664, 543);
        buttonBox = new QDialogButtonBox(BeamDefaultsDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(10, 500, 641, 31));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);
        comboBox = new QComboBox(BeamDefaultsDialog);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(270, 10, 381, 28));
        pushButton = new QPushButton(BeamDefaultsDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 81, 28));
        pushButton_2 = new QPushButton(BeamDefaultsDialog);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(90, 10, 81, 28));
        tabWidget = new QTabWidget(BeamDefaultsDialog);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 50, 641, 441));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tableWidget = new QTableWidget(tab);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(30, 40, 401, 361));
        pushButton_4 = new QPushButton(tab);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(550, 60, 51, 51));
        lineEdit = new QLineEdit(tab);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(90, 10, 341, 22));
        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 10, 53, 20));
        pushButton_5 = new QPushButton(tab);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(442, 10, 161, 28));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(450, 80, 91, 16));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());
        pushButton_3 = new QPushButton(BeamDefaultsDialog);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(170, 10, 93, 28));

        retranslateUi(BeamDefaultsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), BeamDefaultsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), BeamDefaultsDialog, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(BeamDefaultsDialog);
    } // setupUi

    void retranslateUi(QDialog *BeamDefaultsDialog)
    {
        BeamDefaultsDialog->setWindowTitle(QApplication::translate("BeamDefaultsDialog", "Arguments Editor", 0));
        pushButton->setText(QApplication::translate("BeamDefaultsDialog", "New", 0));
        pushButton_2->setText(QApplication::translate("BeamDefaultsDialog", "Delete", 0));
        pushButton_4->setText(QString());
        label->setText(QApplication::translate("BeamDefaultsDialog", "Name", 0));
        pushButton_5->setText(QApplication::translate("BeamDefaultsDialog", "Apply changes", 0));
        label_2->setText(QApplication::translate("BeamDefaultsDialog", "Choose color:", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("BeamDefaultsDialog", "Beam NG", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("BeamDefaultsDialog", "Rigs of Rods", 0));
        pushButton_3->setText(QApplication::translate("BeamDefaultsDialog", "Duplicate", 0));
    } // retranslateUi

};

namespace Ui {
    class BeamDefaultsDialog: public Ui_BeamDefaultsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEAMDEFAULTSDIALOG_H
