#include <QtGui>
#include "mainwindow.h"
#include "../../dialogs/sort/sortdialog.h"
#include "../../dialogs/gotocell/gotocelldialog.h"
#include "../../dialogs/finddialog/finddialog.h"
#include "../spreadsheet/spreadsheet.h"

MainWindow::MainWindow()
{
    spreadsheet=new Spreadsheet;
    setCentralWidget(spreadsheet);
    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();
    readSettings();
    findDialog=0;
    setWindowIcon(QIcon(":/images/my_spreadsheet_icon_alt.png"));
    setCurrentFile("");

}
