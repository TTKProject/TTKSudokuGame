#include "sudokutable.h"

#include <QHeaderView>
#include <QMessageBox>

SudokuTable::SudokuTable(QWidget *parent)
    : QTableWidget(parent)
{
    setItemPrototype(new SudokuCell);
    setSelectionMode(NoSelection);
    setStyleSheet("gridline-color: black");
    setShowGrid(true);
    setFocusPolicy(Qt::NoFocus);

    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_sudokuData = new SudokuData;
    clear();

    connect(this, SIGNAL(cellChanged(int, int)), SLOT(somethingChanged(int, int)));
}

void SudokuTable::newGame()
{
    disconnect(this, SIGNAL(cellChanged(int, int)), this, SLOT(somethingChanged(int, int)));

    setCurrentCell(4, 4);
    clear();
    m_isGodMode = false;

    int data[ITEM_ROW][ITEM_COL];
    m_sudokuData->getNewPuzzle(data);

    SudokuCell *cl;
    for(int i=0; i<ITEM_ROW; i++)
    {
        for(int j=0; j<ITEM_COL; j++)
        {
            cl = cell(i, j);
            if(data[i][j])
            {
                cl->setTextColor(SudokuCell::Default);
                cl->setData(Qt::EditRole, data[i][j]);
                cl->setDirty(false);
                cl->setEditEnabled(false);
            }
            else
            {
                cl->setTextColor(SudokuCell::Right);
            }
        }
    }
    connect(this, SIGNAL(cellChanged(int, int)), SLOT(somethingChanged(int, int)));
}

void SudokuTable::erase()
{
    disconnect(this, SIGNAL(cellChanged(int, int)), this, SLOT(somethingChanged(int, int)));

    setCurrentCell(4, 4);
    m_sudokuData->erase();

    int inputData[ITEM_ROW][ITEM_COL];
    m_sudokuData->getInputData(inputData);

    SudokuCell *cl;
    for(int i=0; i<ITEM_ROW; i++)
    {
        for(int j=0; j<ITEM_COL; j++)
        {
            cl = cell(i, j);
            if(!inputData[i][j])
            {
                cl->setEditEnabled(true);
                cl->setData(Qt::EditRole, 0);
                cl->setDirty(false);
            }
        }
    }
    connect(this, SIGNAL(cellChanged(int, int)), SLOT(somethingChanged(int, int)));
}

void SudokuTable::clear()
{
    m_sudokuData->clear();
    m_isGodMode = true;

    setRowCount(0);
    setColumnCount(0);
    setRowCount(ITEM_ROW);
    setColumnCount(ITEM_COL);

    for(int i=0; i<ITEM_ROW; i++)
    {
        setRowHeight(i, 35);
        setColumnWidth(i, 35);
    }

    SudokuCell * cl;
    for(int i=0; i<ITEM_ROW; i++)
    {
        for(int j=0; j<ITEM_COL; j++)
        {
            cl = new SudokuCell;
            setItem(i, j, cl);
        }
    }

    setCurrentCell(4, 4);
    drawBasicBackgroud();
}

void SudokuTable::drawBasicBackgroud()
{
    SudokuCell *cl;
    for(int i=0; i<ITEM_ROW; i++)
    {
        for(int j=0; j<ITEM_COL; j++)
        {
            cl = cell(i, j);
            cl->setBackgroundColor(((i / 3 + j / 3) % 2) ? SudokuCell::White : SudokuCell::Grey);
        }
    }
}

void SudokuTable::somethingChanged(int row, int column)
{
    SudokuCell *cl = cell(row, column);
    if(!cl)
    {
        return;
    }

    if(!cl->dirty())
    {
        return;
    }
    else
    {
        cl->setDirty(false);
    }

    const int num = cl->data(Qt::EditRole).toInt();
    if(m_isGodMode)
    {
        m_sudokuData->setInputData(row, column, num);
        int invalidData[ITEM_ROW][ITEM_COL];
        m_sudokuData->getInvalidDataRecord(invalidData);

        for(int i=0; i<ITEM_ROW; i++)
        {
            for(int j=0; j<ITEM_COL; j++)
            {
                cl = cell(i, j);
                cl->setTextColor(invalidData[i][j] ? SudokuCell::Wrong : SudokuCell::Default);
            }
        }
    }
    else
    {
        m_sudokuData->setUserAnswer(row, column, num);
        if(m_sudokuData->checkUserAnswer())
        {
            for(int i=0; i<ITEM_ROW; i++)
            {
                for(int j=0; j<ITEM_COL; j++)
                {
                    cl = cell(i, j);
                    if(!cl->editEnabled())
                    {
                        continue;
                    }
                    cl->setEditEnabled(false);
                }
            }
            QMessageBox::information(0, "Title", "You Win");
        }
        else
        {
            int invalidData[ITEM_ROW][ITEM_COL];
            m_sudokuData->getInvalidDataRecord(invalidData);

            SudokuCell *cl;
            for(int i=0; i<ITEM_ROW; i++)
            {
                for(int j=0; j<ITEM_COL; j++)
                {
                    cl = cell(i, j);
                    if(!cl->editEnabled())
                    {
                        continue;
                    }

                    cl->setTextColor(invalidData[i][j] ? SudokuCell::Wrong : SudokuCell::Right);
                }
            }
        }
    }
}

SudokuCell *SudokuTable::cell(int row, int column)
{
    return static_cast<SudokuCell *>(item(row, column));
}
