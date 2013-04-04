#include "spreadsheet.h"
#include "../cell/cell.h"

Cell *Spreadsheet::cell(int row, int column) const{
    return static_cast<Cell *>(item(row, column));
}

QString Spreadsheet::text(int row, int column) const
{
    Cell *c=cell(row,column);
    if(c){
        return c->text();
    } else {
        return "";
    }
}

QString Spreadsheet::formula(int row, int column) const
{
    Cell *c=cell(row,column);
    if(c){
        return c->formula();
    } else {
        return "";
    }
}

void Spreadsheet::setFormula(int row, int column, const QString &formula)
{
    Cell *c=cell(row,column);
    if(!c){
        c=new Cell;
        setItem(row,column,c);
    }
    c->setFormula(formula);
}
