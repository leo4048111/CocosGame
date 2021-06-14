#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <iostream>
#include <ctime>
#include <regex>
#include <experimental/filesystem> 
#include <vector>
#include "Network/SocketBase.h"
#include <map>

using namespace std;

#define MAX_PLAYER 8
#define MAP_LEFT_BORDER 46.25
#define MAP_RIGHT_BORDER 1235
#define MAP_TOP_BORDER 1242.29
#define MAP_BOTTOM_BORDER 53.54

typedef enum JsonMsgType
{
	PlayerList, SCommand, AddOb, PlayerCount, PlayerData, PlayerName, Speak, AddTarget, TargetData, TargetAttack, AddAi
};

typedef enum SocketCommand
{
	JOIN, CANT_JOIN, IS_JOIN, DISCON, START
};

class Specs
{
public:
	typedef void(_stdcall Specs::* CheatFp)();

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

	bool isSinglePlayer();

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

	//socket related
	void asServer(bool value);

	bool isServer();

	//preload
	std::vector<std::string> getAllBgMusic();

	void getAllFiles(std::string path, std::vector<std::string>& files, std::string format);

	int getNextSong();

	int getPreviousSong();

	int getCurrentSong();

	void initBgMusicPath();

	//Scene toggle

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

	//socket related
	bool m_isServer;

	public:
	std::map<HSocket,std::string> m_allPlayerSocket;
	std::vector<std::string> m_allPlayerName;

	//audio files
	private:
	std::vector<std::string> m_allBgMusic;
	int m_currentBgMusic;
};

