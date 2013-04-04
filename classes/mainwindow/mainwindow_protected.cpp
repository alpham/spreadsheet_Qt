#include <QtGui>
#include "mainwindow.h"
#include "../../dialogs/sort/sortdialog.h"
#include "../../dialogs/gotocell/gotocelldialog.h"
#include "../../dialogs/finddialog/finddialog.h"
#include "../spreadsheet/spreadsheet.h"

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(okToContinue()){
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}
