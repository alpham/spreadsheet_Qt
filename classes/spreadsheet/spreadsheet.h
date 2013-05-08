#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <QTableWidget>
#include "../cell/cell.h"

class SpreadsheetCompare;

class Spreadsheet : public QTableWidget
{
    Q_OBJECT
public:
    Spreadsheet(QWidget *parent = 0);
    bool autoreCalc(){ return autoreCalculate;}
    QString currentLocation()const ;
    QString currentFormula() const ;
    QTableWidgetSelectionRange selectedRange() const ;
    void clear();
    bool readFile(const QString &);
    bool writeFile(const QString &);
    void sort(const SpreadsheetCompare &compare);

public slots:
    void copy();
    void cut();
    void paste();
    void del();
    void selectCurrentColumn();
    void selectCurrentRow();
    void recalculate();
    void setAutoRecalculate(bool recalc);
    void findNext(const QString &str , Qt::CaseSensitivity cs);
    void findPrev(const QString &str , Qt::CaseSensitivity cs);

private slots:
    void somethingChanged();

signals:
    void modified();

private:
    enum { MagicNumber=0x7F51C883, RowCount=999, ColumnCount=26};
    Cell *cell(int row,int column) const;
    QString text(int row,int column) const;
    QString formula(int row,int column) const;
    void setFormula(int row,int column,const QString &formula);
    bool autoreCalculate;

};

class SpreadsheetCompare
{
public:
    bool operator()(const QStringList &row1,
                    const QStringList &row2) const;
    enum{KeyCount = 3};
    int Keys[KeyCount];
    bool ascending[KeyCount];

};


#endif // SPREADSHEET_H
