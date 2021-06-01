#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <iostream>
#include <ctime>
#include <regex>
#include <experimental/filesystem> 
#include <vector>
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

	//gamemode setting
	void setGamemodeAsSinglePlayer(bool value);

	void setMaxPlayer(int num);

	int getMaxPlayer();

	//cheat settings
	void _stdcall toggleInvincible();
	bool isInvincibleActivated();
	void _stdcall toggleInfiniteAmmo();
	bool isInfiniteAmmoActivated();
	void _stdcall toggleAimbot();
	bool isAimbotActivated();
	void _stdcall toggleAllWeapon();
	bool isAllWeaponActivated();

	//player name setting
	void setPlayerName(std::string name);

	std::string getPlayerName();

	//target and radio lines 
	void initLines();

	std::string speakRandom();

	//preload
	std::vector<std::string> getAllBgMusic();

    void getAllFiles(std::string path, std::vector<std::string>& files, std::string format);

	int getNextSong();

	int getPreviousSong();

	int getCurrentSong();

	void initBgMusicPath();
private:
	//time settings
	time_t m_startTime;
	time_t m_currentTime;

	//gamemode settings
	bool m_isSinglePlayer;
	int m_maxPlayer;

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
	bool m_isAllWeapon;

	//chatting and radio
	std::vector<std::string> m_targetLines;
	std::vector<std::string> m_radioLines;
	
	//audio files
	std::vector<std::string> m_allBgMusic;
	int m_currentBgMusic;
};

