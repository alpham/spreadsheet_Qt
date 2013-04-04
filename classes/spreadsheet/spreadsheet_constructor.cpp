#include "spreadsheet.h"
#include "../cell/cell.h"

Spreadsheet::Spreadsheet(QWidget *parent) :
    QTableWidget(parent)
{
    autoreCalculate=true;
    setItemPrototype(new Cell);
    setSelectionMode(ContiguousSelection);
    connect(this,SIGNAL(itemChanged(QTableWidgetItem*)),
            this,SLOT(somethingChanged()));
    clear();
}

void Spreadsheet::clear(){
    setRowCount(0);
    setColumnCount(0);
    setRowCount(RowCount);
    setColumnCount(ColumnCount);
    for(int i=0;i< ColumnCount ;i++){
        QTableWidgetItem *item=new QTableWidgetItem;
        item->setText(QString(QChar('A'+i)));
        setHorizontalHeaderItem(i,item);
    }
    setCurrentCell(0,0);
}



bool SpreadsheetCompare::operator()(const QStringList &row1,
                                    const QStringList &row2) const
{
    for (int i = 0; i < KeyCount; ++i) {
        int column = Keys[i];
        if (column != -1) {
            if (row1[column] != row2[column]) {
                if (ascending[i]) {
                    return row1[column] < row2[column];
                } else {
                    return row1[column] > row2[column];
                }
            }
        }
    }
    return false;
}


