#include "ttkapplication.h"
#include "sudokutable.h"

#include <QAction>
#include <QToolBar>

TTKApplication::TTKApplication(QWidget * parent)
    : QMainWindow(parent)
{
    setWindowTitle("TTKApplication");

    m_sudokuTable = new SudokuTable(this);
    setCentralWidget(m_sudokuTable);

    createActions();
    setFixedSize(317, 351);
}

void TTKApplication::createActions()
{
    QAction *newAction = new QAction("New", this);
    newAction->setIcon(QIcon(":/data/images/new.png"));
    connect(newAction, SIGNAL(triggered()), m_sudokuTable, SLOT(newGame()));

    QAction *eraseAction = new QAction("Clear", this);
    eraseAction->setIcon(QIcon(":/data/images/clear.png"));
    connect(eraseAction, SIGNAL(triggered()), m_sudokuTable, SLOT(erase()));

    QToolBar *toolBar = addToolBar("ToolBar");
    toolBar->addAction(newAction);
    toolBar->addAction(eraseAction);
}
