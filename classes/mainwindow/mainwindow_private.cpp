#include <QtGui>
#include "mainwindow.h"
#include "../../dialogs/sort/sortdialog.h"
#include "../../dialogs/gotocell/gotocelldialog.h"
#include "../../dialogs/finddialog/finddialog.h"
#include "../spreadsheet/spreadsheet.h"

bool MainWindow::okToContinue()
{
    if(isWindowModified()){
        int response = QMessageBox::warning(this,
                                            tr("Spreadsheet"),
                                            "The current file has been modified.\nDo you want to save the changes ?",
                                            QMessageBox::Yes | QMessageBox::Default,
                                            QMessageBox::No ,
                                            QMessageBox::Cancel | QMessageBox::Escape);

        if(response==QMessageBox::Yes)
            save();
        else if(response==QMessageBox::Cancel)
            return false;
    }
    return true;
}


void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile=fileName;
    setWindowModified(0);
    QString shownName="Untitled";
    if(!curFile.isEmpty()){
        shownName=curFile;
        setWindowTitle(strippedName(curFile));
        recentFiles.removeAll(curFile);
        recentFiles.prepend(curFile);
        updateRecentFileActions();
    }
    setWindowTitle(tr("%1[*] - %2")
                   .arg(shownName)
                   .arg(tr("Spreadsheet")));
}

bool MainWindow::loadFile(const QString &fileName)
{
    if(!spreadsheet->readFile(fileName)){
        statusBar()->showMessage("Loading cancled!!", 5000);
        return false;
    }
    statusBar()->showMessage("File loaded successfully.", 2000);
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    if(!spreadsheet->writeFile(fileName)){
        statusBar()->showMessage("Save cancled!!!",5000);
        return false;
    }
    setCurrentFile(fileName);
    statusBar()->showMessage("File saved successfully.",3000);
    return true;
}

void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);
    while(i.hasNext()){
        if(!QFile::exists(i.next()))
            i.remove();
    }
    for(int j=0;j<MaxRecentFiles;j++)
    {
        if(j<recentFiles.count())
        {
            QString text=tr("&%1 %2")
                         .arg(j+1)
                         .arg(strippedName(recentFiles[j]));
            recentFilesActions[j]->setText(text);
            recentFilesActions[j]->setData(recentFiles[j]);
            recentFilesActions[j]->setVisible(1);
        } else {
            recentFilesActions[j]->setVisible(0);
        }
    }
    separatorAction->setVisible(!recentFiles.isEmpty());
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::readSettings()
{
    QSettings settings("TROSC Team","Spreadsheet");
    QRect rect=settings.value("geometry",
                        QRect(200,200,400,400)).toRect();
    move(rect.topLeft());
    resize(rect.size());
    recentFiles=settings.value("recentFiles").toStringList();
    updateRecentFileActions();
    bool showGrid=settings.value("showGrid",true).toBool();
    showGridAction->setChecked(showGrid);
    bool autoRecalcualte=settings.value("autoRecalculation",true).toBool();
    autoRecalcualteAction->setChecked(autoRecalcualte);

}

void MainWindow::writeSettings()
{
    QSettings settings("TROSC Team","Spreadsheet");
    settings.setValue("geometry",geometry());
    settings.setValue("showGrid",showGridAction->isChecked());
    settings.setValue("recentFiles",recentFiles);
    settings.setValue("autoRecalcualteAction",autoRecalcualteAction->isChecked());
}


void MainWindow::createMenus()
{
    /*File Menu */
    fileMenu=menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    separatorAction=fileMenu->addSeparator();
    for(int i=0;i<MaxRecentFiles;i++)
        fileMenu->addAction(recentFilesActions[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    /*END OF File Menu*/

    /*Edit Menu*/

    editMenu=menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(copyAction);
    editMenu->addAction(cutAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(deleteAction);
    selectSubMenu=editMenu->addMenu(tr("&Select"));
        selectSubMenu->addAction(selectRowAction);
        selectSubMenu->addAction(selectColumnAction);
        selectSubMenu->addAction(selectAllAction);
    editMenu->addSeparator();
    editMenu->addAction(findAction);
    editMenu->addAction(goToCellAction);

    /*END OF Edit Menu*/

    /*Tools Menu*/

    toolsMenu=menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(recalculateAction);
    toolsMenu->addAction(sortAction);

    /*END OF Tools Menu*/

    /*Options Menu*/

    optionsMenu=menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(showGridAction);
    optionsMenu->addAction(autoRecalcualteAction);

    /*END OF Options Menu*/

    /*Help Menu*/

    helpMenu=menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

    /*END OF Help Menu*/


}

void MainWindow::createContextMenu()
{
    spreadsheet->addAction(copyAction);
    spreadsheet->addAction(cutAction);
    spreadsheet->addAction(pasteAction);
    spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
    fileToolBar=addToolBar(tr("&File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);

    editToolBar=addToolBar(tr("&Edit"));
    editToolBar->addAction(copyAction);
    editToolBar->addAction(cutAction);
    editToolBar->addAction(pasteAction);

    editToolBar->addSeparator();

    editToolBar->addAction(findAction);
    editToolBar->addAction(goToCellAction);

}

void MainWindow::createStatusBar()
{
    locationLabel=new QLabel("W999");
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());

    formulaLabel=new QLabel;
    formulaLabel->setIndent(3);

    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel,1);

    connect(spreadsheet,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(updateStatusBar()));
    connect(spreadsheet,SIGNAL(modified()),this,SLOT(spreadsheetModified()));

    updateStatusBar();
}


void MainWindow::createActions()
{
    /*create File Menu Actions */
    newAction=new QAction(tr("&New"),this);
    newAction->setShortcut(tr("Ctrl+N"));
    newAction->setStatusTip(tr("Create a new Spreadsheet file."));
    connect(newAction,SIGNAL(triggered()),this,SLOT(newFile()));

    openAction=new QAction(tr("&Open"),this);
    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setStatusTip(tr("Open an exist spreadsheet file."));
    connect(openAction,SIGNAL(triggered()),this,SLOT(open()));

    saveAction=new QAction(tr("&Save"),this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setStatusTip(tr("Save current spreadesheet file."));
    connect(saveAction,SIGNAL(triggered()),this,SLOT(save()));

    saveAsAction=new QAction(tr("Save &as"),this);
    saveAsAction->setShortcut(tr("Shift+Ctrl+S"));
    saveAsAction->setStatusTip(tr("Save the current spreadsheet file as..."));
    connect(saveAsAction,SIGNAL(triggered()),this,SLOT(saveAs()));

    /*Recent Files*/
    for(int i=0; i<MaxRecentFiles ; i++){
        recentFilesActions[i] = new QAction(this);
        recentFilesActions[i]->setVisible(0);
        connect(recentFilesActions[i],SIGNAL(triggered()),this,SLOT(openRecentFile()));
    }
    /*Recent Files*/


    exitAction=new QAction(tr("&Exit"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit SpreadSheet!"));
    /*END OF create File Menu Actions */

    /* create Edit Menu Actions */

    copyAction=new QAction(tr("C&opy"),this);
    copyAction->setShortcut(tr("Ctrl+C"));
    copyAction->setStatusTip(tr("Copy selection to the clipboard."));

    cutAction=new QAction(tr("C&ut"),this);
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setStatusTip(tr("Cut selection."));

    pasteAction=new QAction(tr("&Paste"),this);
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setStatusTip(tr("Paste clipboard content."));

    deleteAction=new QAction(tr("&Delete"),this);
    deleteAction->setShortcut(tr("Del"));
    deleteAction->setStatusTip(tr("Delete selection."));

    /*Select submenu Actions */

        selectRowAction=new QAction(tr("&Row"),this);
        selectRowAction->setStatusTip(tr("Select row."));

        selectColumnAction=new QAction(tr("&Column"),this);
        selectColumnAction->setStatusTip(tr("Select column."));

        selectAllAction=new QAction(tr("&All"),this);
        selectAllAction->setShortcut(tr("Ctrl+A"));
        selectAllAction->setStatusTip(tr("Select all."));
        connect(selectAllAction,SIGNAL(triggered()),spreadsheet,SLOT(selectAll()));

    /*END OF Select submenu Actions*/

    findAction=new QAction(tr("&Find"),this);
    findAction->setShortcut(tr("Ctrl+F"));
    findAction->setStatusTip(tr("Search and find."));
    connect(findAction,SIGNAL(triggered()),this,SLOT(find()));

    goToCellAction=new QAction(tr("&Go To Cell"),this);
    goToCellAction->setShortcut(tr("F5"));
    goToCellAction->setStatusTip(tr("Go to cell."));
    connect(goToCellAction,SIGNAL(triggered()),this,SLOT(goToCell()));

    /*END OF  create Edit Menu Actions */

    /*create Tools Menu Actions*/

    recalculateAction=new QAction(tr("&Recalculate"),this);
    recalculateAction->setShortcut(tr("F9"));
    recalculateAction->setStatusTip(tr("Recalculate the formula..."));

    sortAction=new QAction(tr("&Sort..."),this);
    sortAction->setStatusTip(tr("Sort spreadsheet..."));
    connect(sortAction,SIGNAL(triggered()),this,SLOT(sort()));

    /*END OF create Tools Menu Actions*/

    /*create Options menu Actions*/

    showGridAction=new QAction(tr("Show &Grid"),this);
    showGridAction->setCheckable(1);
    showGridAction->setStatusTip(tr("Show spreadsheet grid..."));
    connect(showGridAction,SIGNAL(triggered(bool)),spreadsheet,SLOT(setShowGrid(bool)));

    autoRecalcualteAction=new QAction(tr("&Auot-Recalculate"),this);
    autoRecalcualteAction->setCheckable(true);
    autoRecalcualteAction->setStatusTip(tr("Recalculate the formulas automatically."));

    /*END OF create Options menu Actions */

    /*create Help menu Actions*/

    aboutAction=new QAction(tr("&About SpreadSheet"),this);
    aboutAction->setShortcut(tr("F1"));
    aboutAction->setStatusTip("About SpreadSheet...");
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(about()));

    aboutQtAction=new QAction(tr("About &Qt"),this);
    aboutQtAction->setStatusTip(tr("About Qt framework..."));
    connect(aboutQtAction,SIGNAL(triggered()),qApp,SLOT(aboutQt()));

    /*END OF create Help menu Actions*/

} /*END OF CREATING ACTIONS*/
