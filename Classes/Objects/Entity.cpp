#include "Entity.h"

USING_NS_CC;

void Entity::bindSprite(Sprite* sprite)
{
	//bind sprite
	m_sprite = sprite;
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
	m_maxHealth = m_currentHealth=100.0f;
	return true;
}

double Entity::getHealthPercentage()
{
	return m_currentHealth/m_maxHealth*100;
}

void Entity::setCurrentHealth(double currentHealth)
{
	m_currentHealth = currentHealth;
	m_healthBar->setPercentage(getHealthPercentage());
}

