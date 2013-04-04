#ifndef GOTOCELLDIALOG_H
#define GOTOCELLDIALOG_H

#include <QtGui/QDialog>
#include "ui_gotocelldialog.h"

class GoToCellDialog : public QDialog , public Ui::GoToCellDialog
{
    Q_OBJECT
    
public:
    explicit GoToCellDialog(QWidget *parent = 0);
    
private slots :
    void on_cellLocation_textChanged();

};

#endif // GOTOCELLDIALOG_H
