#pragma once
#include <iostream>
#include <ctime>
using namespace std;

class Specs
{
private: 

Specs();

static Specs* instance;

public:
    static Specs* getInstance() 
    { 
        if (instance == NULL)
            instance = new Specs();
        return instance; 
    }

	void refreshInstance();

	//Score Specs
	void addScore(int score);

	int getScore();

	//round status
	int getCurrentRound();

	void goToNextRound();

	void setWinOrLose(bool status);

	bool isWin();

	bool isEnd();

	//time specs
	time_t getStartTime();

	time_t getCurrentTime();

private:
	time_t m_startTime;
	time_t m_currentTime;

	//round status
	int m_currentRound;
	int m_lastRound;
	bool m_isLost;
	bool m_isWin;

	//Score board specs
	int m_currentScore;
};

