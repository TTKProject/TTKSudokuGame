#ifndef SUDOKUDATA_H
#define SUDOKUDATA_H

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

#include <ctime>

#define ITEM_ROW    9
#define ITEM_COL    9
#define ITEM_ZHL    10

class SudokuData
{
public:
	SudokuData();

	void clear();
	void erase();

    void setInputData(int row, int column, int num);
    void setUserAnswer(int row, int column, int num);

    void getInputData(int(*data)[ITEM_COL]) const;
    void getInvalidDataRecord(int(*data)[ITEM_COL]) const;
    void getNewPuzzle(int(*data)[ITEM_COL]);

	bool checkUserAnswer();

private:
	void initial();

    int getAnswerNum(int max, int limit = 100000000);
    bool dfsNum(int order, int &ans, int max, time_t beg, int limit);
	bool dfsPrevious(int order);
	bool dfsNext(int order);

	bool isEnd() const;
    bool isValidNum(int(*data)[ITEM_COL], int row, int column, int num) const;
	bool canContinue() const;

	void tryNum(int row, int column, int num);
    void checkDataValid(int(*data)[ITEM_COL]);
	bool findOrederPos(int &row, int &column, int order) const;
	bool findBestPos(int &row, int &column) const;
    void fillAnswerNum(int(*data)[ITEM_COL], int &row, int &column, int num, int order);
    void unfillAnswerNum(int(*data)[ITEM_COL], int row, int column);

    int m_inputData[ITEM_ROW][ITEM_COL];
    int m_invalidDataRecord[ITEM_ROW][ITEM_COL];
    bool m_dataIsValid;

    int m_userAnswerData[ITEM_ROW][ITEM_COL];
    int m_previousAnswerData[ITEM_ROW][ITEM_COL];
    int m_currentAnswerData[ITEM_ROW][ITEM_COL];
    int m_nextAnswerData[ITEM_ROW][ITEM_COL];

    int m_solutions;
    int m_currentAnswerOrder;
    bool m_dfsInitialed;

    int m_filledBoxs;
    int m_fillOrder[ITEM_ROW][ITEM_COL];
    int m_numCanTry[ITEM_ROW * ITEM_COL + 1][ITEM_ROW][ITEM_COL][ITEM_ZHL];

};

#endif
