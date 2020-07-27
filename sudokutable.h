#ifndef SUDOKUTABLE_H
#define SUDOKUTABLE_H

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

#include "sudokucell.h"
#include "sudokudata.h"

class SudokuTable :public QTableWidget
{
	Q_OBJECT
public:
    explicit SudokuTable(QWidget *parent = nullptr);

public slots:
    void newGame();
	void erase();
    void clear();
	void somethingChanged(int row, int column);

private:
    void drawBasicBackgroud();
    SudokuCell *cell(int row, int column);

    SudokuData *m_sudokuData;
    bool m_isGodMode;

};

#endif
