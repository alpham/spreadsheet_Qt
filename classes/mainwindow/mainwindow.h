#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Spreadsheet;
class FindDialog;
class QAction;
class QLabel;


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void find();
    void goToCell();
    void sort();
    void about();
    void openRecentFile();
    void updateStatusBar();
    void spreadsheetModified();

private:
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool okToContinue();
    void setCurrentFile(const QString &fileName);
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);

    /*private variables*/

    Spreadsheet *spreadsheet;
    FindDialog *findDialog;
    QLabel *locationLabel;
    QLabel *formulaLabel;
    QStringList recentFiles;
    QString curFile;
    enum {MaxRecentFiles=5};
    QAction *recentFilesActions[MaxRecentFiles];
    QAction *separatorAction;

    /*menu bar*/
    QMenu *fileMenu;
    QMenu *editMenu;
        /*Edit>>Select (submenu)*/
        QMenu *selectSubMenu;
    QMenu *toolsMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    /*end of menu bar*/

    /*toolbars*/
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    /*end of toolbars*/


    /*actions*/
    /*File menu*/
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
    /*end of File Menu*/

    /*Edit menu*/
    QAction *copyAction;
    QAction *cutAction;
    QAction *pasteAction;
    QAction *deleteAction;
    /*select submenu*/
    QAction *selectRowAction;
    QAction *selectColumnAction;
    QAction *selectAllAction;
    /*end of select submenu*/
    QAction *findAction;
    QAction *goToCellAction;
    /*end of Edit Menu*/

    /*Tools Menu*/
    QAction *recalculateAction;
    QAction *sortAction;
    /*end of Tools Menu*/

    /*Options menu*/
    QAction *showGridAction;
    QAction *autoRecalcualteAction;
    /*end of Options Menu*/

    /*Help Menu*/
    QAction *aboutAction;
    QAction *aboutQtAction;
    /*end of Help Menu*/
    /*END OF actions*/



};

#endif // MAINWINDOW_H
