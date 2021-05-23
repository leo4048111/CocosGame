#include "TestScene.h"

USING_NS_CC;

TestScene* TestScene::createTestScene()
{
	return TestScene::create();
}

bool TestScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();


	auto m_inputBox = TextFieldTTF::textFieldWithPlaceHolder("PLACE Holder", "fonts/ThonburiBold.ttf", 30);
	m_inputBox->setPosition(visibleSize / 2);
	this->addChild(m_inputBox);
	m_inputBox->attachWithIME();
	m_inputBox->detachWithIME();
}
