#include "Entity.h"

USING_NS_CC;

Entity* Entity::createEntity()
{
	return Entity::create();
}

void Entity::bindSprite(Sprite* sprite)
{
	//bind sprite
	m_sprite = sprite;
	m_sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(m_sprite);
}

void Entity::showHealthBar()
{
	auto spriteSize = m_sprite->getContentSize();
	auto healthBarBg = Sprite::create("objects/UI/ui_healthBarBg.png");
	healthBarBg->setPosition(Vec2(spriteSize.width / 2, spriteSize.height));
	m_sprite->addChild(healthBarBg);
	m_healthBar= ProgressTimer::create(Sprite::create("objects/UI/ui_healthBar.png"));
	m_healthBar->setType(ProgressTimer::Type::BAR);
	m_healthBar->setMidpoint(Vec2(0,1.0f));
	m_healthBar->setPosition(Vec2(-2,healthBarBg->getContentSize().height));
	m_healthBar->setBarChangeRate(Vec2(1.0f,0));
	m_healthBar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	m_healthBar->setPercentage(getHealthPercentage());
	healthBarBg->addChild(m_healthBar);
}

bool Entity::init()
{
	if (!Node::init())
	{
		return false;
	}
	m_maxHealth = 100.0f;
	m_currentHealth = 100.0f;
	m_maxStamina = 100;
	m_currentStamina = 0;
	m_currentStaminaRecovery = 0.5f;
	m_currentSpeed = 0.25f;
	m_currentResistance = 20.0f;
	
	m_isMe = false;
	return true;
}

double Entity::getHealthPercentage()
{
	return m_currentHealth/m_maxHealth*100;
}

void Entity::resetHealthBar()
{
	m_healthBar->setPercentage(getHealthPercentage());
}

bool Entity::receiveDamage(double damage)
{
	double actualDamage = damage * (100 - m_currentResistance) / 100.0f;
	m_currentHealth -= actualDamage;
	resetHealthBar();
	if (m_currentHealth > 0)
	{
		return true;
	}
	
	return false;
}

void Entity::deadAndCleanUp(Node* node)
{
	node->removeFromParent();
}

void Entity::runDeadAction()
{
	if (this != nullptr)
	{
		this->stopAllActions();
		this->unscheduleUpdate();
		auto blink = Repeat::create(Blink::create(2.0f, 10), 1);
		CallFuncN* callFunc = CallFuncN::create(CC_CALLBACK_1(Entity::deadAndCleanUp, this));
		auto sequence = Sequence::create(blink, callFunc, NULL);
		this->runAction(sequence);
	}
}

void Entity::addHealth(double heal)
{
	m_currentHealth += heal;
	if (m_currentHealth > m_maxHealth)
		m_currentHealth = m_maxHealth;
	if (m_currentHealth < 0)
		m_currentHealth = 0;
	this->resetHealthBar();
}

void Entity::setHealth(double health)
{
	m_currentHealth = health;
	if (m_currentHealth > m_maxHealth)
		m_currentHealth = m_maxHealth;
	if (m_currentHealth < 0)
		m_currentHealth = 0;
	this->resetHealthBar();

}

void Entity::addSpeed(double speed)
{
	m_currentSpeed += speed;
}

double Entity::getCurrentSpeed()
{
	return m_currentSpeed;
}

double Entity::getSpeedPercentage()
{
	return m_currentSpeed * 100.0f;
}

double Entity::getCurrentStamina()
{
	return m_currentStamina;
}

double Entity::getStaminaPercentage()
{
	return m_currentStamina / m_maxStamina * 100;
}

void Entity::resetStaminaBar()
{
	m_staminaBar->setPercentage(getStaminaPercentage());
}

void Entity::addStamina(double stamina)
{
	m_currentStamina += stamina;
	if (m_currentStamina > m_maxStamina)
		m_currentStamina = m_maxStamina;
	if (m_currentStamina < 0)
		m_currentStamina = 0;
	this->resetStaminaBar();
}

double Entity::getStaminaRecovery()
{
	return m_currentStaminaRecovery;
}

void Entity::addStaminaRecovery(double stamina)
{
	m_currentStaminaRecovery += stamina;
}

void Entity::setStamina(double stamina)
{
	m_currentStamina = stamina;
}

void Entity::showStaminaBar()
{
	auto spriteSize = m_sprite->getContentSize();
	auto StaminaBarBg = Sprite::create("objects/UI/ui_staminaBarBg.png");
	StaminaBarBg->setPosition(Vec2(spriteSize.width+StaminaBarBg->getContentSize().width/2, spriteSize.height/2));
	m_sprite->addChild(StaminaBarBg);
	m_staminaBar = ProgressTimer::create(Sprite::create("objects/UI/ui_staminaBar.png"));
	m_staminaBar->setType(ProgressTimer::Type::BAR);
	m_staminaBar->setMidpoint(Vec2(1.0f, 0));
	m_staminaBar->setPosition(Vec2(-2, StaminaBarBg->getContentSize().height));
	m_staminaBar->setBarChangeRate(Vec2(0, 1.0f));
	m_staminaBar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	m_staminaBar->setPercentage(getStaminaPercentage());
	StaminaBarBg->addChild(m_staminaBar);
}

void Entity::addResistance(double resistance)
{
	m_currentResistance += resistance;
	if (m_currentResistance > 100)
		m_currentResistance = 100;
	if (m_currentResistance < 0)
		m_currentResistance = 0;
}

void Entity::setResistance(double resistance)
{
	m_currentResistance = resistance;
}

double Entity::getCurrentResistance()
{
	return m_currentResistance;
}

void Entity::speak(std::string str)
{
	auto spriteSize = m_sprite->getContentSize();

	auto messageBubble = Sprite::create("objects/UI/ui_messageBubble.png");
	m_sprite->addChild(messageBubble);
	messageBubble->setPosition(Vec2(spriteSize.width+200,spriteSize.height+200));
	messageBubble->setScale(4.0f);
	
	auto sentence = Label::create(str, "fonts/MomcakeThin-9Y6aZ.otf", 15);
	sentence->setColor(Color3B(0,0,0));
	sentence->setPosition(Vec2(messageBubble->getContentSize().width/2,messageBubble->getContentSize().height/2+15));
	messageBubble->addChild(sentence);
	
	auto fadein = FadeIn::create(0.5f);
	auto fadeout = FadeOut::create(4.0f);
	auto callFunc = CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, messageBubble));
	auto sequence = Sequence::create(fadein, fadeout, callFunc,NULL);
	messageBubble->runAction(sequence);
	sentence->runAction(sequence->clone());
}

void Entity::setMe(bool value)
{
	m_isMe = value;
}

bool Entity::isMe()
{
	return m_isMe;
}