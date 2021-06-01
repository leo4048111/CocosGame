#include "AudioControlPanel.h"
#include "../Controls/Specs.h"

USING_NS_CC;

AudioControlPanel* AudioControlPanel::createAudioControlPanel()
{
	return AudioControlPanel::create();
}

bool AudioControlPanel::init()
{
	if (!Node::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto resumeImage = MenuItemImage::create("objects/UI/ui_resume.png", "objects/UI/ui_resume.png");
	resumeImage->setTag(0);
	auto pauseImage = MenuItemImage::create("objects/UI/ui_pause.png", "objects/UI/ui_pause.png");
	pauseImage->setTag(1);

	auto toggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(AudioControlPanel::menuToggleCallback, this), pauseImage, resumeImage,nullptr);
	toggle->setTag(0);

	auto previousImage = MenuItemImage::create("objects/UI/ui_previous.png", "objects/UI/ui_previous.png",CC_CALLBACK_1(AudioControlPanel::menuCallback,this));
	auto nextImage = MenuItemImage::create("objects/UI/ui_next.png", "objects/UI/ui_next.png", CC_CALLBACK_1(AudioControlPanel::menuCallback, this));
	previousImage->setTag(1);
	nextImage->setTag(2);

	auto menu = Menu::create(previousImage,toggle, nextImage,NULL);
	menu->alignItemsHorizontallyWithPadding(10);
	this->addChild(menu);
	this->setScale(0.2f);

	auto test1 = Specs::getInstance()->getAllBgMusic();
	auto test2 = Specs::getInstance()->getCurrentSong();

	m_songName = Label::createWithTTF("Playing: " + Specs::getInstance()->getAllBgMusic()[Specs::getInstance()->getCurrentSong()]
		, "fonts/HashedBrowns-WyJgn.ttf", 50);
	this->addChild(m_songName);
	m_songName->setPosition(Vec2(visibleSize.width/ 2, visibleSize.height/2+m_songName->getContentSize().height));

	return true;
}

void AudioControlPanel::menuCallback(Ref* sender)
{
	MenuItem* item = dynamic_cast<MenuItem*>(sender);
	auto allSongs = Specs::getInstance()->getAllBgMusic();
	switch (item->getTag())
	{
	case 1:
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(("Audio/BG/"+allSongs[Specs::getInstance()->getPreviousSong()]).c_str(),true);
		m_songName->setString("Playing: " + Specs::getInstance()->getAllBgMusic()[Specs::getInstance()->getCurrentSong()]);
		break;
	case 2:
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(("Audio/BG/" + allSongs[Specs::getInstance()->getNextSong()]).c_str(), true);
		m_songName->setString("Playing: " + Specs::getInstance()->getAllBgMusic()[Specs::getInstance()->getCurrentSong()]);
		break;
	default:
		break;
	}
}

void AudioControlPanel::menuToggleCallback(Ref* sender)
{
	MenuItemToggle* item = dynamic_cast<MenuItemToggle*>(sender);

	switch (item->getSelectedIndex())
	{
	case 0:
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		break;
	case 1:
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		break;
	default:
		break;
	}
}