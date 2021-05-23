#include "Specs.h"

Specs* Specs::_instance = NULL;

Specs::Specs()
{
	//init time
	m_startTime = std::time(NULL);
	m_currentTime = std::time(NULL);

	//init score
	m_currentScore = 0;

	//init round specs
	m_currentRound = 0;
	m_lastRound = -1;
	m_isLost = false;
	m_isWin = false;

	//init cheats
	m_isAimbot = false;
	m_isInfiniteAmmo = false;
	m_isInvincible = false;

	//init player name
	m_playerName = "Player Unknown";
}

void Specs::refreshInstance()
{
	//init time
	m_startTime = std::time(NULL);
	m_currentTime = std::time(NULL);

	//init score
	m_currentScore = 0;

	//init round specs
	m_currentRound = 0;
	m_lastRound = -1;
	m_isLost = false;
	m_isWin = false;

	//init cheats
	m_isAimbot = false;
	m_isInfiniteAmmo = false;
	m_isInvincible = false;
}

void Specs::addScore(int score)
{
	m_currentScore += score;
}

int Specs::getScore()
{
	return m_currentScore;
}

int Specs::getCurrentRound()
{
	return m_currentRound;
}

void Specs::goToNextRound()
{
	m_lastRound = m_currentRound;
	m_currentRound++;
}

void Specs::setWinOrLose(bool status)
{
	if (status)
		m_isWin = true;
	else
		m_isLost = true;
}

bool Specs::isWin()
{
	return m_isWin;
}

bool Specs::isEnd()
{
	return m_isWin || m_isLost;
}

time_t Specs::getCurrentTime()
{
	m_currentTime = std::time(NULL);
	return m_currentTime;
}

time_t Specs::getStartTime()
{
	return m_startTime;
}

void Specs::toggleInvincible()
{
	m_isInvincible = !m_isInvincible;
}

bool Specs::isInvincibleActivated()
{
	return m_isInvincible;
}

void Specs::toggleInfiniteAmmo()
{
	m_isInfiniteAmmo = !m_isInfiniteAmmo;
}

bool Specs::isInfiniteAmmoActivated()
{
	return m_isInfiniteAmmo;
}

void Specs::toggleAimbot()
{
	m_isAimbot = !m_isAimbot;
}

bool Specs::isAimbotActivated()
{
	return m_isAimbot;
}

void Specs::setPlayerName(std::string name)
{
	m_playerName = name;
}

std::string Specs::getPlayerName()
{
	return m_playerName;
}