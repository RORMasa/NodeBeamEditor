/********************************************************************************
** Form generated from reading UI file 'tracksgenerator.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRACKSGENERATOR_H
#define UI_TRACKSGENERATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_TracksGenerator
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *TracksGenerator)
    {
        if (TracksGenerator->objectName().isEmpty())
            TracksGenerator->setObjectName(QStringLiteral("TracksGenerator"));
        TracksGenerator->resize(400, 300);
        buttonBox = new QDialogButtonBox(TracksGenerator);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(TracksGenerator);
        QObject::connect(buttonBox, SIGNAL(accepted()), TracksGenerator, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), TracksGenerator, SLOT(reject()));

        QMetaObject::connectSlotsByName(TracksGenerator);
    } // setupUi

    void retranslateUi(QDialog *TracksGenerator)
    {
        TracksGenerator->setWindowTitle(QApplication::translate("TracksGenerator", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class TracksGenerator: public Ui_TracksGenerator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRACKSGENERATOR_H
