#include <QtGui/QtGui>
#include "gotocelldialog.h"

GoToCellDialog::GoToCellDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
    cellLocation->setValidator(new QRegExpValidator(regExp,this));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
    connect(okButton,SIGNAL(clicked()),this,SLOT(accept()));
}

void GoToCellDialog::on_cellLocation_textChanged(){
    okButton->setEnabled(cellLocation->hasAcceptableInput());
}

