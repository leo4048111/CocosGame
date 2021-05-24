#include "PreparationScene.h"
#include "SetupAndGTGScene.h"
#include "Controls/Specs.h"
#include "Scene/MenuScene.h"

USING_NS_CC;

PreparationScene* PreparationScene::createPreparationScene()
{
	return PreparationScene::create();
}

bool PreparationScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	Specs::getInstance()->refreshInstance();

	auto singlePlayerLabel = Label::createWithTTF("Single Player", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto singlePlayer = MenuItemLabel::create(singlePlayerLabel, CC_CALLBACK_1(PreparationScene::menuCallBack, this));
	auto multiPlayerLabel = Label::createWithTTF("Multiplayer", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto multiPlayer = MenuItemLabel::create(multiPlayerLabel, CC_CALLBACK_1(PreparationScene::menuCallBack, this));
	auto backLabel = Label::createWithTTF("Back", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto back = MenuItemLabel::create(backLabel, CC_CALLBACK_1(PreparationScene::menuCallBack, this));
	singlePlayer->setTag(0);
	singlePlayer->setColor(Color3B(220, 220, 220));
	singlePlayer->setScale(0.3f);
	multiPlayer->setTag(1);
	multiPlayer->setColor(Color3B(220, 220, 220));
	multiPlayer->setScale(0.3f);
	back->setTag(2);
	back->setColor(Color3B(220, 220, 220));
	back->setScale(0.3f);

	auto menu = Menu::create(singlePlayer, multiPlayer, back, NULL);
	menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menu->alignItemsVerticallyWithPadding(15);
	this->addChild(menu, 20);
	menu->setName("menu");
	menu->setPositionY(menu->getPositionY() - 50);

	return true;
}

void PreparationScene::menuCallBack(Ref* sender)
{
	MenuItem* item = dynamic_cast<MenuItem*>(sender);
	switch (item->getTag())
	{
	case 0:
		Specs::getInstance()->setGamemodeAsSinglePlayer(true);
		goToSetupAndGTGScene();
		break;
	case 1:
		Specs::getInstance()->setGamemodeAsSinglePlayer(false);
		goToSetupAndGTGScene();
		break;
	case 2:
		backToMenuScene();
	}
}

void PreparationScene::backToMenuScene()
{
	MenuScene* menuScene = MenuScene::create();
	auto transition = TransitionFlipX::create(0.5f, menuScene);
	Director::getInstance()->replaceScene(transition);
}

void PreparationScene::goToSetupAndGTGScene()
{
	SetupAndGTGScene* setupAndGTGScene = SetupAndGTGScene::create();
	auto transition = TransitionFlipX::create(0.5f, setupAndGTGScene);
	Director::getInstance()->replaceScene(transition);
}