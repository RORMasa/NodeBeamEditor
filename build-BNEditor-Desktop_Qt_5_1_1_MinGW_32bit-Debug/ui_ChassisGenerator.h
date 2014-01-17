/********************************************************************************
** Form generated from reading UI file 'ChassisGenerator.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHASSISGENERATOR_H
#define UI_CHASSISGENERATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_ChassisGenerator
{
public:
    QDialogButtonBox *buttonBox;
    QFrame *frame;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;

    void setupUi(QDialog *ChassisGenerator)
    {
        if (ChassisGenerator->objectName().isEmpty())
            ChassisGenerator->setObjectName(QStringLiteral("ChassisGenerator"));
        ChassisGenerator->resize(392, 272);
        buttonBox = new QDialogButtonBox(ChassisGenerator);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(10, 220, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        frame = new QFrame(ChassisGenerator);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(70, 50, 211, 141));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 0, 53, 20));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 30, 53, 21));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 60, 53, 21));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_4 = new QLabel(frame);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(12, 90, 61, 21));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit = new QLineEdit(frame);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(80, 0, 113, 22));
        lineEdit_2 = new QLineEdit(frame);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(80, 30, 113, 22));
        lineEdit_3 = new QLineEdit(frame);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(80, 60, 113, 22));
        lineEdit_4 = new QLineEdit(frame);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(80, 90, 113, 22));

        retranslateUi(ChassisGenerator);
        QObject::connect(buttonBox, SIGNAL(accepted()), ChassisGenerator, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ChassisGenerator, SLOT(reject()));

        QMetaObject::connectSlotsByName(ChassisGenerator);
    } // setupUi

    void retranslateUi(QDialog *ChassisGenerator)
    {
        ChassisGenerator->setWindowTitle(QApplication::translate("ChassisGenerator", "Dialog", 0));
        label->setText(QApplication::translate("ChassisGenerator", "Width", 0));
        label_2->setText(QApplication::translate("ChassisGenerator", "Height", 0));
        label_3->setText(QApplication::translate("ChassisGenerator", "Length", 0));
        label_4->setText(QApplication::translate("ChassisGenerator", "Segments", 0));
    } // retranslateUi

};

namespace Ui {
    class ChassisGenerator: public Ui_ChassisGenerator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHASSISGENERATOR_H
