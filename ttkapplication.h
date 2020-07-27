#ifndef TTKAPPLICATION_H
#define TTKAPPLICATION_H

/* =================================================
 * This file is part of the TTKSudokuGame project
 * Copyright (C) 2017 - 2020 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QMainWindow>

class SudokuTable;

class TTKApplication : public QMainWindow
{
    Q_OBJECT
public:
    explicit TTKApplication(QWidget *parent = nullptr);

private:
    void createActions();

    SudokuTable * m_sudokuTable;
};

#endif
