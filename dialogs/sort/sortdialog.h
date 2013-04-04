#ifndef SORTDIALOG_H
#define SORTDIALOG_H
#include <QtGui/QDialog>
#include "ui_sortdialog.h"

class SortDialog :public QDialog ,public Ui::SortDialog
{
    Q_OBJECT
public:
    SortDialog(QWidget *parent = 0);
    void setColumnRange(QChar first,QChar last);
private slots:
    void updateMore();
};

#endif // SORTDIALOG_H
