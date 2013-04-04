#include <QtGui>
#include "spreadsheet.h"

void Spreadsheet::somethingChanged()
{
    if(autoreCalculate)
        recalculate();
    emit modified();
}

void Spreadsheet::cut()
{
    copy();
    del();
}

void Spreadsheet::paste()
{
    QTableWidgetSelectionRange range=selectedRange();
    QString str = QApplication::clipboard()->text();
    QStringList rows=str.split('\n');
    int numRows=rows.count();
    int numColumns=rows.first().count('\t')+1;
    if(range.rowCount() * range.columnCount() !=1 &&
            (range.rowCount() != numRows ||
            range.columnCount() != numColumns)){
        QMessageBox::warning(this,
                             tr("SpreadSheet"),
                             tr("The information cannot be pasted because the copy "
                                  "and paste areas aren't the same size."));
    }
    for(int i=0 ; i<numRows;++i){
        QStringList col=rows[i].split('\t');
        for(int j=0;j<numColumns;++j){
            int r=range.topRow()+i;
            int c=range.leftColumn()+i;
            if(r<RowCount && c<ColumnCount){
                setFormula(r,c,col[j]);
            }
        }
    }
    somethingChanged();
}

void Spreadsheet::del()
{
    foreach (QTableWidgetItem *item , selectedItems()) {
        delete item;
    }
}

void Spreadsheet::selectCurrentRow()
{
    selectRow(currentRow());
}

void Spreadsheet::recalculate()
{
    for(int row=0;row<RowCount;++row){
        for(int column=0;column<ColumnCount;++column){
            if(cell(row,column)){
                cell(row,column)->setDirty();
            }
        }
    }
    viewport()->update();
}

void Spreadsheet::setAutoRecalculate(bool recalc)
{
    bool AreCalc=recalc;
    if(AreCalc)
        recalculate();
}

void Spreadsheet::selectCurrentColumn()
{
    selectColumn(currentColumn());
}


void Spreadsheet::copy()
{
    QTableWidgetSelectionRange range= selectedRange();
    QString str="";
    for(int i=0 ; i<range.rowCount();++i){
        if(i>0){
            str+="\n";
        }
        for(int j=0 ; j<range.columnCount();++j){
            if(j>0){
                str+="\t";
            }
            str=formula( range.topRow() + 1 , range.leftColumn() + 1 );
        }
    }
    QApplication::clipboard()->setText(str);
}

void Spreadsheet::findNext(const QString &str, Qt::CaseSensitivity cs)
{
    int row=currentRow();
    int column = currentColumn()+1;
    while(row<RowCount){
        while(column<ColumnCount){
            if(text(row,column).contains(str,cs)){
                clearSelection();
                setCurrentCell(row,column);
                activateWindow();
                return;
            }
            ++column;
        }
        column=0;
        ++row;
    }
    QApplication::beep();
    QMessageBox::warning(this,
                         tr("Spreadsheet"),
                         tr("No text matches next."));

}

void Spreadsheet::fnidPrev(const QString &str, Qt::CaseSensitivity cs)
{
    int row=currentRow();
    int column = currentColumn() - 1;
    while (row>=0) {
        while (column>=0) {
            if(text(row,column).contains(str,cs)){
                clearSelection();
                setCurrentCell(row,column);
                activateWindow();
                return;
            }
            --column;
        }
        column = ColumnCount - 1;
        --row;
    }
    QApplication::beep();
    QMessageBox::warning(this,
                         tr("Spreadsheet"),
                         tr("No text matches previous."));
}
