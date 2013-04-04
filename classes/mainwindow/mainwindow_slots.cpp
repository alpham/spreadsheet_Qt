#include <QtGui>
#include "mainwindow.h"
#include "../../dialogs/sort/sortdialog.h"
#include "../../dialogs/gotocell/gotocelldialog.h"
#include "../../dialogs/finddialog/finddialog.h"
#include "../spreadsheet/spreadsheet.h"


void MainWindow::newFile()
{
    if(okToContinue()){
        spreadsheet->clear();
        setCurrentFile("");
    }
}

void MainWindow::open()
{
    if(okToContinue()){
        QString fileName=QFileDialog::getOpenFileName(this,
                                                      tr("Open Spreadsheet File"),
                                                      ".",
                                                      tr("Spreadsheet files *.sp"));
        if(!fileName.isEmpty()) {
            loadFile(fileName);
        }
    }
}

bool MainWindow::save()
{
    if(curFile.isEmpty())
        return saveAs();
    else
        return saveFile(curFile);
}

bool MainWindow::saveAs()
{
    QString fileName=QFileDialog::getSaveFileName(this,
                                                  tr("Save Spreadsheet file."),
                                                  ".",
                                                  tr("Spreadsheet flies *.sp"));
    if(fileName.isEmpty())
        return false;
    return saveFile(fileName);
}

void MainWindow::find()
{
    if (!findDialog){
        findDialog=new FindDialog;
        connect(findDialog,SIGNAL(findNext(QString,Qt::CaseSensitivity))
                ,spreadsheet,SLOT(findNext(QString,Qt::CaseSensitivity)));
        connect(findDialog,SIGNAL(findPrev(QString,Qt::CaseSensitivity))
                ,spreadsheet,SLOT(findPrev(QString,Qt::CaseSensitivity)));

    }
    findDialog->show();
    findDialog->activateWindow();
}


void MainWindow::openRecentFile()
{
    if(okToContinue()){
        QAction *action=qobject_cast<QAction *>(QObject::sender());
        if(action){
            loadFile(action->data().toString());
        }
    }
}

void MainWindow::about()
{
    QMessageBox::about(this,
                       tr("About Spreadsheet"),
                       tr("<h1>About Spreadsheet :</h1>"
                          "<p>Copyrihgt &copy; <font color=\"red\"><b>TROSC</b></font> Team.</p>"
                          "<p>[Ahmed Magdy - ahmed.magdy40@gmail.com]</p>"
                          "<p>Spreadsheet is a small training program that aims to demostrate "
                          "the main concepts of Qt library and OpenSource..."));

}

void MainWindow::sort()
{
    SortDialog *sortDialog=new SortDialog(this);
    QTableWidgetSelectionRange range = spreadsheet->selectedRange();
    sortDialog->setColumnRange('A'+range.leftColumn(),
                               'A'+range.rightColumn());
    if(sortDialog->exec()){
        SpreadsheetCompare compare;
               compare.Keys[0] =
                     sortDialog->primaryColumnCombo->currentIndex();
               compare.Keys[1] =
                     sortDialog->secondaryColumnCombo->currentIndex() - 1;
               compare.Keys[2] =
                     sortDialog->tertiaryColumnCombo->currentIndex() - 1;
               compare.ascending[0] =
                     (sortDialog->primaryOrderCombo->currentIndex() == 0);
               compare.ascending[1] =
                     (sortDialog->secondaryOrderCombo->currentIndex() == 0);
               compare.ascending[2] =
                     (sortDialog->tertiaryOrderCombo->currentIndex() == 0);
               spreadsheet->sort(compare);
    }
}

void MainWindow::goToCell()
{
    GoToCellDialog *dialog=new GoToCellDialog(this);
    dialog->setModal(true);
    if(dialog->exec()){
        QString str=dialog->cellLocation->text().toUpper();
        spreadsheet->setCurrentCell(str.mid(1).toInt()-1,str[0].unicode()-'A');
    }
    delete dialog;
}

void MainWindow::updateStatusBar()
{
    locationLabel->setText(spreadsheet->currentLocation());
    formulaLabel->setText(spreadsheet->currentFormula());
}

void MainWindow::spreadsheetModified()
{
    setWindowModified(1);
    updateStatusBar();
}
