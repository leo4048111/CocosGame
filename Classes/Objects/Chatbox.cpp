#include "Chatbox.h"
#include "Objects/Player.h"

USING_NS_CC;

Chatbox* Chatbox::_instance = NULL;

Chatbox* Chatbox::createChatbox()
{
	if(_instance == NULL)
	_instance = Chatbox::create();
	return _instance;
}

bool Chatbox::init()
{
	if (!Node::init())
	{
		return false;
	}

	//init input box
	m_inputBox = TextFieldTTF::textFieldWithPlaceHolder("Press enter to talk", "fonts/arial.ttf", 10);
	this->addChild(m_inputBox);
	m_inputBox->setMaxLineWidth(200);
	m_inputBox->enableBold();
	m_inputBox->setColor(Color3B(255, 255, 255));

	//init message box
	m_messageBox = LabelTTF::create("", "fonts/MomcakeThin-9Y6aZ.otf", 10);
	this->addChild(m_messageBox);
	m_messageBox->setPosition(Vec2(m_inputBox->getPosition().x+5, m_inputBox->getPosition().y + 100));
	m_messageBox->setColor(Color3B(255, 255, 255));
	
	//init cheat map
	mapAllCheats();

	m_isActivated = false;

	return true;
}

void Chatbox::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
	if(m_isActivated)
	event->stopPropagation();
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_ENTER:
		if (m_isActivated)
		{
			m_inputBox->detachWithIME();
			sendText();
		}
		else
		{
			m_inputBox->attachWithIME();
		}
		m_isActivated = !m_isActivated;
		break;
	default:
		break;
	}
}

void Chatbox::setControlOnListen()
{
	auto* dispatcher = Director::getInstance()->getEventDispatcher();
	auto* keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(Chatbox::onKeyPressed, this);
	dispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
}

void Chatbox::sendText()
{
	const int MAX_MESSAGES_SHOW = 15;
	m_currentText = m_inputBox->getString();

	if (m_currentText == "")
		return;

	if (m_currentText[0] == '/')
	{
		runCheat(m_currentText);
		if (m_currentText == "")
			return;
	}
	else
	{
		Player* player = dynamic_cast<Player*>(this->getParent()->getParent()->getChildByName("Map")->getChildByName("SpriteLayer")->getChildByName(Specs::getInstance()->getPlayerName()));
		player->speak(m_currentText);
		m_currentText = Specs::getInstance()->getPlayerName() + " says: " + m_currentText;
	}

	while (m_allMessages.size() >= MAX_MESSAGES_SHOW)
		m_allMessages.pop_front();

	m_allMessages.push_back(m_currentText);
	std::string showText = "";
	for (auto message:m_allMessages)
	{
		showText += message + "\n";
	}

	m_messageBox->setString(showText);
	m_inputBox->setString("");

	//run some actions 
	auto fadein = FadeIn::create(0.3f);
	auto fadeout = FadeOut::create(3.0f);
	auto sequence = Sequence::create(fadein, fadeout, NULL);
	m_messageBox->runAction(sequence);

}

void Chatbox::runCheat(std::string command)
{
	Specs::CheatFp fp = m_cheatFunctionMap[command];
	if (fp != NULL)
	{
		(Specs::getInstance()->*fp)(); //run the function pointer
		m_currentText = "";
		m_inputBox->setString("");
	}
	else if (command == "/help")
		showHelp();
	else
	m_currentText = "Invalid cheat! Please type /help for help";
}

void Chatbox::mapAllCheats()
{
	m_cheatFunctionMap.insert(std::make_pair("/invincible", &Specs::toggleInvincible));
	m_cheatFunctionMap.insert(std::make_pair("/infiniteAmmo", &Specs::toggleInfiniteAmmo));
	m_cheatFunctionMap.insert(std::make_pair("/aimbot", &Specs::toggleAimbot));
	m_cheatFunctionMap.insert(std::make_pair("/allWeapons", &Specs::toggleAllWeapon));


}

void Chatbox::showHelp()
{
	m_currentText = "available cheats:/invincible /infiniteAmmo\n/aimbot /allWeapons";
}