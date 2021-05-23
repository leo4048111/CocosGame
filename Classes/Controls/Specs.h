#pragma once
#include <iostream>
#include <ctime>
using namespace std;

class Specs
{
public:
	typedef void (_stdcall Specs::*CheatFp)();

private: 

Specs();

static Specs* _instance;

public:
    static Specs* getInstance() 
    { 
        if (_instance == NULL)
            _instance = new Specs();
        return _instance; 
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

	//cheat settings
	void _stdcall toggleInvincible();
	bool isInvincibleActivated();
	void _stdcall toggleInfiniteAmmo();
	bool isInfiniteAmmoActivated();
	void _stdcall toggleAimbot();
	bool isAimbotActivated();

	//player name setting
	void setPlayerName(std::string name);

	std::string getPlayerName();
private:
	time_t m_startTime;
	time_t m_currentTime;

	//player name
	std::string m_playerName;

	//round status
	int m_currentRound;
	int m_lastRound;
	bool m_isLost;
	bool m_isWin;

	//Score board specs
	int m_currentScore;

	//cheat specs
	bool m_isInvincible;
	bool m_isInfiniteAmmo;
	bool m_isAimbot;
};

