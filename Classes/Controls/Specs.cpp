#include "Specs.h"
#include "cocos2d.h"
#include "Layer/SpriteLayer.h"

USING_NS_CC;

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
	_isFieldTrip = false;

	//init cheats
	m_isAimbot = false;
	m_isInfiniteAmmo = false;
	m_isInvincible = true;
	m_isAllWeapon = false;

	//init player name
	m_playerName = "Player Unknown";

	//init lines
	initLines();

	//init bg music path
	initBgMusicPath();
	m_currentBgMusic = 0;
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
	m_isInvincible = true;
	m_isAllWeapon = false;

	//init socket specs
	m_allPlayerSocket.clear();

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

void Specs::toggleAllWeapon()
{
	m_isAllWeapon = !m_isAllWeapon;
}

bool Specs::isAllWeaponActivated()
{
	return m_isAllWeapon;
}

void Specs::setPlayerName(std::string name)
{
	m_playerName = name;
}

std::string Specs::getPlayerName()
{
	return m_playerName;
}

void Specs::initLines()
{
	m_targetLines.push_back("Rest in peace");
	m_targetLines.push_back("Fresh flesh");
	m_targetLines.push_back("I used to be an adventurer like you,\nthen I took an arrow to the knee");
	m_targetLines.push_back("Fus Ro Dah");
	m_targetLines.push_back("If these ruins frighten you,\n take comfort from the knowledge that I am here");
	m_targetLines.push_back("I fight for the men I've held in my arms,\ndying on foreign soil.");
	m_targetLines.push_back("I just wanted things... \nto stay the way they were. ");
	m_targetLines.push_back("You must kill... me.......");
	m_targetLines.push_back("I have my skill,\n not powers or magic.");
	m_targetLines.push_back("Religion and thieves make odd bedfellows.");
	m_targetLines.push_back("I stand witness for the courage of the soul before us.");
	m_targetLines.push_back("We grieve.");
	m_targetLines.push_back("We weep.");
	m_targetLines.push_back("We take our leave.");

}

std::string Specs::speakRandom()
{
	srand((unsigned int)time(NULL));
	return m_targetLines[rand() % m_targetLines.size()];
}

void Specs::setGamemodeAsSinglePlayer(bool value)
{
	m_isSinglePlayer = value;
}

bool Specs::isSinglePlayer()
{
	return m_isSinglePlayer;
}

void Specs::setMaxPlayer(int num)
{
	m_maxPlayer = num;
}

int Specs::getMaxPlayer()
{
	return m_maxPlayer;
}

std::vector<std::string> Specs::getAllBgMusic()
{
	return m_allBgMusic;
}

void Specs::initBgMusicPath()
{
	getAllFiles("Audio/BG", m_allBgMusic, "mp3");
	m_currentBgMusic = m_allBgMusic.size();
}

void Specs::getAllFiles(std::string path, std::vector<string>& files, std::string format)
{
	regex fileSuffix("(.*)(." + format + ")");// *.jpg, *.png  
	for (const auto& entry : std::experimental::filesystem::directory_iterator(path))
	{
		string file = entry.path().filename().string();
		if (std::regex_match(file, fileSuffix))
		{
			files.push_back(file);
		}
	}
}

int Specs::getNextSong()
{
	m_currentBgMusic++;
	if (m_currentBgMusic >= m_allBgMusic.size())
		m_currentBgMusic = 0;
	return m_currentBgMusic;
}

int Specs::getPreviousSong()
{
	m_currentBgMusic--;
	if (m_currentBgMusic < 0)
		m_currentBgMusic = m_allBgMusic.size()-1;
	return m_currentBgMusic;
}

int Specs::getCurrentSong()
{
	return m_currentBgMusic;
}

void Specs::asServer(bool value)
{
	m_isServer = value;
}

bool Specs::isServer()
{
	return m_isServer;
}

void Specs::toggleFieldTrip(bool value)
{
	_isFieldTrip = value;
}

bool Specs::isFieldTrip()
{
	return _isFieldTrip;
}

void Specs::addTarget()
{
	SpriteLayer::getInstance()->addTarget();
}