#pragma once

#include "cocos2d.h"
#include "ui/UIListView.h"
#include "Controls/Specs.h"
#include <list>
#include <map>


class Chatbox :public cocos2d::Node
{
private:
	static Chatbox* _instance;

	Chatbox() {};

	~Chatbox() {
		this->removeFromParentAndCleanup(1); _instance = NULL;
	};

public:
	static Chatbox* createChatbox();

	virtual bool init();

	CREATE_FUNC(Chatbox);

	static Chatbox* getInstance() {
		if (_instance == NULL)
			_instance = createChatbox();
		return _instance;
	};

	//control methods
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	void setControlOnListen();

	//display methods
	void sendText();

	void appendTextInChat(std::string str);

	void _stdcall showHelp();

	//run cheat
	void runCheat(std::string command);

	void mapAllCheats();

private:
	cocos2d::TextFieldTTF* m_inputBox;

	cocos2d::LabelTTF* m_messageBox;

	std::string m_currentText;

	std::list<std::string> m_allMessages;

	bool m_isActivated;

	//all cheats
	map<std::string, Specs::CheatFp> m_cheatFunctionMap;
};