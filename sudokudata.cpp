#include "sudokudata.h"

#include <QDateTime>
#include <QMessageBox>

SudokuData::SudokuData()
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
	clear();
}

void SudokuData::clear()
{
    memset(m_inputData, 0, sizeof(m_inputData));
    memset(m_invalidDataRecord, 0, sizeof(m_invalidDataRecord));

    memset(m_userAnswerData, 0, sizeof(m_userAnswerData));
    memset(m_previousAnswerData, 0, sizeof(m_previousAnswerData));
    memset(m_currentAnswerData, 0, sizeof(m_currentAnswerData));
    memset(m_nextAnswerData, 0, sizeof(m_nextAnswerData));

    m_currentAnswerOrder = 0;
    m_solutions = 1;
    m_filledBoxs = 0;

    for(int i=0; i<ITEM_ROW; i++)
    {
        for(int j=0; j<ITEM_COL; j++)
        {
            m_fillOrder[i][j] = -1;
        }
    }

    for(int i=0; i<ITEM_ROW; i++)
    {
        for(int j=0; j<ITEM_COL; j++)
        {
            for(int k=0; k<ITEM_ZHL; k++)
            {
                m_numCanTry[0][i][j][k] = (k == 0) ? ITEM_ROW : 1;
            }
        }
    }
}

void SudokuData::erase()
{
    int data[ITEM_ROW][ITEM_COL];
    getInputData(data);
	clear();

    memcpy(m_inputData, data, sizeof(data));
    memcpy(m_userAnswerData, data, sizeof(data));
    checkDataValid(m_inputData);
}

void SudokuData::setInputData(int row, int column, int num)
{
    m_inputData[row][column] = num;
    checkDataValid(m_inputData);
}

void SudokuData::setUserAnswer(int row, int column, int num)
{
    m_userAnswerData[row][column] = num;
    checkDataValid(m_userAnswerData);
}

void SudokuData::getInputData(int(*data)[ITEM_COL]) const
{
    memcpy(data, m_inputData, sizeof(m_inputData));
}

void SudokuData::getInvalidDataRecord(int(*data)[ITEM_COL]) const
{
    memcpy(data, m_invalidDataRecord, sizeof(m_invalidDataRecord));
}

void SudokuData::getNewPuzzle(int(*data)[ITEM_COL])
{
    const int maxCry = 1000;
    const int maxCryTimeLimit = 1000;
    int row = 0, column = 0, num = 0;

    clear();

    int count = 0;
    while(true)
    {
        row = qrand() % ITEM_ROW;
        column = qrand() % ITEM_COL;
        if(m_inputData[row][column])
        {
            continue;
        }

        num = qrand() % ITEM_ROW + 1;
        if(!isValidNum(m_inputData, row, column, num))
        {
            continue;
        }
        m_inputData[row][column] = num;

        const clock_t beg = clock();
        const int ans_num = getAnswerNum(m_solutions + 1, maxCryTimeLimit);
        const clock_t now = clock();

        if(now - beg > maxCryTimeLimit)
        {
            getNewPuzzle(data);
            return;
        }

        if(ans_num == m_solutions)
        {
            break;
        }
        else if(ans_num > m_solutions)
        {
            continue;
        }
        else
        {
            m_inputData[row][column] = 0;
            if(++count > maxCry)
            {
                getNewPuzzle(data);
                return;
            }
        }
    }

    memcpy(m_userAnswerData, m_inputData, sizeof(m_inputData));
    memcpy(data, m_userAnswerData, sizeof(m_userAnswerData));
}

bool SudokuData::checkUserAnswer()
{
    for(int i=0; i<ITEM_ROW; i++)
    {
        for(int j=0; j<ITEM_COL; j++)
        {
            if(!m_userAnswerData[i][j])
            {
				return false;
            }
        }
    }
    checkDataValid(m_userAnswerData);
    return m_dataIsValid;
}

void SudokuData::initial()
{
    m_filledBoxs = 0;
    for(int i=0; i<ITEM_ROW; i++)
    {
        for(int j=0; j<ITEM_COL; j++)
        {
            m_fillOrder[i][j] = -1;
        }
    }

    for(int i=0; i<ITEM_ROW; i++)
    {
        for(int j=0; j<ITEM_COL; j++)
        {
            for(int k=0; k<ITEM_ZHL; k++)
            {
                m_numCanTry[0][i][j][k] = (k == 0) ? ITEM_ROW : 1;
            }
        }
    }

    for(int i=0; i<ITEM_ROW; i++)
    {
        for(int j=0; j<ITEM_COL; j++)
        {
            if(m_currentAnswerData[i][j])
			{
                ++m_filledBoxs;
                tryNum(i, j, m_currentAnswerData[i][j]);
                m_fillOrder[i][j] = -1;
			}
			else
            {
                m_fillOrder[i][j] = 0;
            }
        }
    }
}

int SudokuData::getAnswerNum(int max, int limit)
{
    memcpy(m_currentAnswerData, m_inputData, sizeof(m_inputData));
	initial();

	int ans = 0;
	clock_t beg = clock();
    dfsNum(1, ans, max, beg, limit);
	return ans;
}

bool SudokuData::dfsNum(int order, int &ans, int max, time_t beg, int limit)
{
	clock_t now = clock();
    if(now - beg > limit)
    {
		return true;
    }

    if(!canContinue())
    {
		return false;
    }

    int row = 0, column = 0;
    if(!findBestPos(row, column))
	{
        if(isEnd())
		{
			ans++;
            return ans == max;
		}
    }

    for(int i=1; i<=ITEM_COL; i++)
	{
        if(!m_numCanTry[m_filledBoxs][row][column][i])
        {
            continue;
        }
        fillAnswerNum(m_currentAnswerData, row, column, i, order);

        if(dfsNum(order + 1, ans, max, beg, limit))
        {
            return true;
        }
        unfillAnswerNum(m_currentAnswerData, row, column);
	}
	return false;
}

bool SudokuData::dfsPrevious(int order)
{
    if(!canContinue())
    {
		return false;
    }

    int row = 0, column = 0;
	bool hasBegin = false;

    if(!m_dfsInitialed)
	{
		hasBegin = true;
        if(findOrederPos(row, column, order))
		{
            if(dfsPrevious(order + 1))
            {
				return true;
            }
		}
		else
		{
            m_dfsInitialed = true;
			return false;
		}
	}
    else if(!findBestPos(row, column))
	{
        if(isEnd())
        {
            return true;
        }
	}

    for(int i = ITEM_COL; i >= 1; i--)
	{
        if(hasBegin)
		{
			hasBegin = false;
            i = m_previousAnswerData[row][column];
            unfillAnswerNum(m_previousAnswerData, row, column);
			continue;
		}

        if(!m_numCanTry[m_filledBoxs][row][column][i])
        {
            continue;
        }
        fillAnswerNum(m_previousAnswerData, row, column, i, order);

        if(dfsPrevious(order + 1))
        {
            return true;
        }
        unfillAnswerNum(m_previousAnswerData, row, column);
	}
	return false;
}

bool SudokuData::dfsNext(int order)
{
    if(!canContinue())
    {
		return false;
    }

    int row = 0, column = 0;
	bool hasBegin = false;

    if(m_currentAnswerOrder != 0 && !m_dfsInitialed)
	{
		hasBegin = true;
        if(findOrederPos(row, column, order))
		{
            if(dfsNext(order + 1))
            {
				return true;
            }
		}
		else
		{
            m_dfsInitialed = true;
			return false;
		}
	}
    else if(!findBestPos(row, column))
	{
        if(isEnd())
        {
			return true;
        }
	}

    for(int i=1; i<=ITEM_COL; i++)
	{
        if(hasBegin)
		{
			hasBegin = false;
            i = m_nextAnswerData[row][column];
            unfillAnswerNum(m_nextAnswerData, row, column);
			continue;
		}

        if(!m_numCanTry[m_filledBoxs][row][column][i])
        {
            continue;
        }
        fillAnswerNum(m_nextAnswerData, row, column, i, order);

        if(dfsNext(order + 1))
        {
            return true;
        }
        unfillAnswerNum(m_nextAnswerData, row, column);
	}
	return false;
}

bool SudokuData::isEnd() const
{
    return m_filledBoxs == 81;
}

bool SudokuData::isValidNum(int(*data)[ITEM_COL], int row, int column, int num) const
{
    if(num == 0)
    {
        return true;
    }

    for(int i=0; i<ITEM_COL; i++)
	{
        if(i != column && data[row][i] == num)
        {
			return false;
        }

        if(i != row && data[i][column] == num)
        {
			return false;
        }
	}

	int newr = row / 3;
	int newc = column / 3;

    for(int i=newr * 3; i<(newr + 1) * 3; i++)
    {
        for(int j=newc * 3; j<(newc + 1) * 3; j++)
		{
            if(i == row && j == column)
            {
                continue;
            }

            if(data[i][j] == num)
            {
				return false;
            }
		}
    }
	return true;
}

bool SudokuData::canContinue() const
{
    for(int i=0; i<ITEM_ROW; i++)
    {
        for(int j=0; j<ITEM_COL; j++)
        {
            if(m_numCanTry[m_filledBoxs][i][j][0] == 0)
            {
				return false;
            }
        }
    }
	return true;
}

void SudokuData::tryNum(int row, int column, int num)
{
    memcpy(m_numCanTry[m_filledBoxs], m_numCanTry[m_filledBoxs - 1], sizeof(m_numCanTry[0]));

    m_numCanTry[m_filledBoxs][row][column][0] = -1;

    for(int i=0; i<ITEM_COL; i++)
	{
        if(m_numCanTry[m_filledBoxs][row][i][0] != -1 && m_numCanTry[m_filledBoxs][row][i][num])
		{
            m_numCanTry[m_filledBoxs][row][i][num] = 0;
            m_numCanTry[m_filledBoxs][row][i][0]--;
		}

        if(m_numCanTry[m_filledBoxs][i][column][0] != -1 && m_numCanTry[m_filledBoxs][i][column][num])
		{
            m_numCanTry[m_filledBoxs][i][column][num] = 0;
            m_numCanTry[m_filledBoxs][i][column][0]--;
		}
	}

	int newr = row / 3;
	int newc = column / 3;

    for(int i=newr * 3; i<(newr + 1) * 3; i++)
    {
        for(int j=newc * 3; j<(newc + 1) * 3; j++)
        {
            if(m_numCanTry[m_filledBoxs][i][j][0] != -1 && m_numCanTry[m_filledBoxs][i][j][num])
			{
                m_numCanTry[m_filledBoxs][i][j][num] = 0;
                m_numCanTry[m_filledBoxs][i][j][0]--;
			}
        }
    }
}

void SudokuData::checkDataValid(int(*data)[ITEM_COL])
{
    m_dataIsValid = true;
    memset(m_invalidDataRecord, 0, sizeof(m_invalidDataRecord));

    for(int i=0; i<ITEM_ROW; i++)
    {
        for(int j=0; j<ITEM_COL; j++)
        {
            if(!isValidNum(data, i, j, data[i][j]))
			{
                m_dataIsValid = false;
                m_invalidDataRecord[i][j] = 1;
			}
        }
    }
}

bool SudokuData::findOrederPos(int &row, int &column, int order) const
{
    for(int i=0; i<ITEM_ROW; i++)
    {
        for(int j=0; j<ITEM_COL; j++)
        {
            if(m_fillOrder[i][j] == order)
			{
				row = i;
				column = j;
				return true;
			}
        }
    }
	return false;
}

bool SudokuData::findBestPos(int &row, int &column) const
{
	int min = 10;
    for(int i=0; i<ITEM_ROW; i++)
    {
        for(int j=0; j<ITEM_COL; j++)
		{
            const int goodName = m_numCanTry[m_filledBoxs][i][j][0];
            if(goodName != -1 && goodName != 0 && min > goodName)
			{
				min = goodName;
				row = i;
				column = j;
			}
		}
    }

    return min != 10;
}

void SudokuData::fillAnswerNum(int(*data)[ITEM_COL], int &row, int &column, int num, int order)
{
	data[row][column] = num;
    m_fillOrder[row][column] = order;
    ++m_filledBoxs;

    memcpy(m_numCanTry[m_filledBoxs], m_numCanTry[m_filledBoxs - 1], sizeof(m_numCanTry[m_filledBoxs]));
    m_numCanTry[m_filledBoxs][row][column][0] = -1;

    for(int i=0; i<ITEM_COL; i++)
	{
        if(m_numCanTry[m_filledBoxs][row][i][0] != -1 && m_numCanTry[m_filledBoxs][row][i][num])
		{
            m_numCanTry[m_filledBoxs][row][i][num] = 0;
            m_numCanTry[m_filledBoxs][row][i][0]--;
		}

        if(m_numCanTry[m_filledBoxs][i][column][0] != -1 && m_numCanTry[m_filledBoxs][i][column][num])
		{
            m_numCanTry[m_filledBoxs][i][column][num] = 0;
            m_numCanTry[m_filledBoxs][i][column][0]--;
		}
	}

	int newr = row / 3;
	int newc = column / 3;

    for(int i=newr * 3; i<(newr + 1) * 3; i++)
    {
        for(int j=newc * 3; j<(newc + 1) * 3; j++)
        {
            if(m_numCanTry[m_filledBoxs][i][j][0] != -1 && m_numCanTry[m_filledBoxs][i][j][num])
			{
                m_numCanTry[m_filledBoxs][i][j][num] = 0;
                m_numCanTry[m_filledBoxs][i][j][0]--;
			}
        }
    }
}

void SudokuData::unfillAnswerNum(int(*data)[ITEM_COL], int row, int column)
{
	data[row][column] = 0;
    m_fillOrder[row][column] = 0;
    --m_filledBoxs;
}
