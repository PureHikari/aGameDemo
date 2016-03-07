#include "HelloWorldScene.h"

#define VISIBLE_SIZE Director::getInstance()->getVisibleSize()

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    
    auto layer = HelloWorld::create();

	/*layer->m_world=scene->getPhysicsWorld();

	layer->m_world->setGravity(Vec2(0, 0));*/

    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	//初始化地图
	this->m_map = GameMap::create(WINDOW_W / GRID, WINDOW_H / GRID);
	this->m_map->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	this->addChild(m_map);

	/*m_bg = Sprite::create();
	m_bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	this->addChild(m_bg);*/
	
	//添加主角
	m_hero = Hero::create();
	m_map->addChild(m_hero, Z_HERO);

	/*m_node = Node::create();
	m_bg->addChild(m_node, 1, 1);
	m_node->setPosition(0, 0);*/

	
	intro();

	//加载界面ui
	initUI();

	//添加鼠标监听事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onEnd, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//添加键盘监听事件
	auto lis = EventListenerKeyboard::create();
	lis->onKeyPressed = CC_CALLBACK_2(HelloWorld::first, this);
	lis->onKeyReleased = CC_CALLBACK_2(HelloWorld::second, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, this);


	this->firstTouchPos = Vec2(GRID * 2, GRID * 2);

	auto buttom = Sprite::create("CloseNormal.png");
	buttom->setPosition(this->firstTouchPos);
	buttom->setOpacity(50);
	this->addChild(buttom, 99);

	scheduleUpdate();
    return true;
}

void HelloWorld::first(EventKeyboard::KeyCode keyCode, Event* event)
{
	m_hero->control(keyCode, true);
}

void HelloWorld::second(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		if (Director::getInstance()->isPaused())
		{
			Director::getInstance()->resume();
		}
		else
		{
			Director::getInstance()->pause();
		}
	}

	m_hero->control(keyCode, false);
}

Vec2 HelloWorld::getLoc(int x, int y)
{
	Vec2 pos;
	pos.x = x*GRID + GRID / 2;
	pos.y = y*GRID + GRID / 2;

	return pos;
}

void HelloWorld::update(float)
{
	checkCoolDown();

	//当主角状态为释放技能时释放技能
	if (m_hero->getStatus() == STATUS::SKILL_EXE)
	{
		auto sp = Skill::create(m_hero->skillAtk(), m_hero);
		if (sp)
		{
			sp->execute();
			bullets.pushBack(sp);
		}
	}

	/*if (m_hero->getStatus() == STATUS::AIM_TAR)
	{
		for (auto ms : monsters)
		{
			ms->waitForAim(true);
		}
	}
	else
	{
		for (auto ms : monsters)
		{
			ms->waitForAim(false);
		}
	}*/

	m_hero->getMap(this->m_map);

	//遍历怪物，清除死亡的怪物
	for (auto ms : monsters)
	{
		if (!ms->isAlive())
		{
			removeable_ms.pushBack(ms);
			ms->removeFromParent();
		}
		else
		{
			ms->getMap(this->m_map);
			ms->getTarget(m_hero);

			ms->AiCycle();

			if (ms->getStatus() == STATUS::SKILL_EXE)
			{
				auto sp = Skill::create(ms->skillAtk(), ms);
				if (sp)
				{
					sp->execute();
					bullets.pushBack(sp);
				}
			}
		}

	}

	//技能攻击判断
	for (auto bu : bullets)
	{
		if (bu->isOver)
		{
			removeable_bu.pushBack(bu);
			bu->removeFromParent();
			continue;
		}

		if (bu->skill.Aim_type == SkillAimType::SELF_EXE)
			continue;
			
		//英雄的技能
		if (bu->skill.isHero)
		{
			for (auto ms : monsters)
			{
				if (!ms->isInwin() && ms->getBoundingBox().intersectsRect(bu->getBoundingBox()))
				{
					ms->hit(bu->skill,bu->dmg);
					bu->hit();
				}
			}
		}
		//敌人的技能
		else
		{
			if (!m_hero->isInwin() && m_hero->getBoundingBox().intersectsRect(bu->getBoundingBox()))
			{
				m_hero->hit(bu->skill,bu->dmg);
				bu->hit();
			}
		}
	}

	for (auto ms : removeable_ms)
	{
		monsters.eraseObject(ms);
	}
	removeable_ms.clear();

	for (auto bu : removeable_bu)
	{
		bullets.eraseObject(bu);
	}
	removeable_bu.clear();

	//如果怪物被消灭完并且触碰到了出口则进入下一关
	/*if (m_end&&m_end->getBoundingBox().intersectsRect(m_hero->getBoundingBox())&&monsters.empty())
	{
		intro();
	}*/
	if (monsters.empty() && !this->m_isGameStop)
	{
		intro();
	}
}

void HelloWorld::checkCoolDown()
{
	auto lab_q = (Label*)this->getChildByTag(1);
	auto lab_e = (Label*)this->getChildByTag(2);
	auto lab_r = (Label*)this->getChildByTag(3);
	auto lab_f = (Label*)this->getChildByTag(4);
	auto lab_hp = (Label *)this->getChildByTag(5);
	auto lab_mp = (Label *)this->getChildByTag(6);
	auto lab_space = (Label*)this->getChildByTag(7);
	
	lab_q->setString(StringUtils::format("O%d", m_hero->getCDTime(1)/60));
	if (m_hero->isSkillAlbe(1))
	{
		lab_q->setColor(Color3B::GREEN);
		skill_ori->setColor(Color3B::GREEN);
	}
	else
	{
		lab_q->setColor(Color3B::RED);
		skill_ori->setColor(Color3B::RED);
	}
	lab_e->setString(StringUtils::format("Q%d", m_hero->getCDTime(2) / 60));
	if (m_hero->isSkillAlbe(2))
	{
		lab_e->setColor(Color3B::GREEN);
		skill_q->setColor(Color3B::GREEN);
	}
	else
	{
		lab_e->setColor(Color3B::RED);
		skill_q->setColor(Color3B::RED);
	}
	lab_r->setString(StringUtils::format("E%d", m_hero->getCDTime(3) / 60));
	if (m_hero->isSkillAlbe(3))
	{
		lab_r->setColor(Color3B::GREEN);
		skill_e->setColor(Color3B::GREEN);
	}
	else
	{
		lab_r->setColor(Color3B::RED);
		skill_e->setColor(Color3B::RED);
	}
	lab_f->setString(StringUtils::format("R%d", m_hero->getCDTime(4) / 60));
	if (m_hero->isSkillAlbe(4))
	{
		lab_f->setColor(Color3B::GREEN);
		skill_r->setColor(Color3B::GREEN);
	}
	else
	{
		lab_f->setColor(Color3B::RED);
		skill_r->setColor(Color3B::RED);
	}

	lab_space->setString(StringUtils::format("F%d", m_hero->getCDTime(5) / 60));
	if (m_hero->isSkillAlbe(5))
	{
		lab_space->setColor(Color3B::GREEN);
		skill_f->setColor(Color3B::GREEN);
	}
	else
	{
		lab_space->setColor(Color3B::RED);
		skill_f->setColor(Color3B::RED);
	}

	auto hp = m_hero->getHp();
	lab_hp->setString(StringUtils::format("%d", hp));

	auto mp = m_hero->getMp();
	lab_mp->setString(StringUtils::format("%d", mp));
}

void HelloWorld::initUI()
{
	auto lab1 = Label::createWithSystemFont("no", "Consolas", 40);
	lab1->setPosition(Vec2(lab1->getContentSize().width, VISIBLE_SIZE.height - lab1->getContentSize().height / 2));
	this->addChild(lab1, 4, 1);

	auto lab2 = Label::createWithSystemFont("no", "Consolas", 40);
	lab2->setPosition(lab1->getPosition() + Vec2(0, -lab2->getContentSize().height));
	this->addChild(lab2, 4, 2);

	auto lab3 = Label::createWithSystemFont("no", "Consolas", 40);
	lab3->setPosition(lab2->getPosition() + Vec2(0, -lab2->getContentSize().height));
	this->addChild(lab3, 4, 3);

	auto lab4 = Label::createWithSystemFont("no", "Consolas", 40);
	lab4->setPosition(lab3->getPosition() + Vec2(0, -lab2->getContentSize().height));
	this->addChild(lab4, 4, 4);

	auto lab5 = Label::createWithSystemFont("no", "Consolas", 40);
	lab5->setPosition(lab4->getPosition() + Vec2(0, -lab2->getContentSize().height));
	this->addChild(lab5, 4, 7);

	auto lab_hp = Label::createWithSystemFont("0", "Consolas", 40);
	lab_hp->setPosition(Vec2(VISIBLE_SIZE.width - lab_hp->getContentSize().width, VISIBLE_SIZE.height - lab_hp->getContentSize().height / 2));
	this->addChild(lab_hp, 4, 5);

	auto lab_mp = Label::createWithSystemFont("0", "Consolas", 40);
	lab_mp->setPosition(lab_hp->getPosition() + Vec2(0, -lab_hp->getContentSize().height));
	this->addChild(lab_mp, 4, 6);

	auto level = Label::createWithSystemFont("0", "Consolas", 40);
	level->setPosition(lab_mp->getPosition() + Vec2(0, -lab_mp->getContentSize().height));
	this->addChild(level, 4, 8);

	skill_ori = Label::createWithSystemFont("O", "Consolas", 40);
	skill_ori->setPosition(level->getPosition() + Vec2(0, -level->getContentSize().height));
	this->addChild(skill_ori, 4, 9);

	skill_q = Label::createWithSystemFont("Q", "Consolas", 40);
	skill_q->setPosition(skill_ori->getPosition() + Vec2(0, -skill_ori->getContentSize().height));
	this->addChild(skill_q, 4, 9);

	skill_e = Label::createWithSystemFont("E", "Consolas", 40);
	skill_e->setPosition(skill_q->getPosition() + Vec2(0, -skill_q->getContentSize().height));
	this->addChild(skill_e, 4, 10);

	skill_r = Label::createWithSystemFont("R", "Consolas", 40);
	skill_r->setPosition(skill_e->getPosition() + Vec2(0, -skill_e->getContentSize().height));
	this->addChild(skill_r, 4, 11);

	skill_f = Label::createWithSystemFont("F", "Consolas", 40);
	skill_f->setPosition(skill_r->getPosition() + Vec2(0, -skill_r->getContentSize().height));
	this->addChild(skill_f, 4, 12);
}

void HelloWorld::createEnemy(float)
{
	//to do

	for (int i = 0; i < 1; i++)
	{
		auto ms = Monster::create(40, 1);
		ms->setPosition(VISIBLE_SIZE / 2);
		this->addChild(ms, Z_MONSTER);

		this->monsters.pushBack(ms);
	}

	if (m_level >= 5)
	{
		for (int i = 0; i < 10; i++)
		{
			auto ms = Monster::create(32767, 20);
			ms->setPosition(VISIBLE_SIZE / 2);
			this->addChild(ms, Z_MONSTER);

			this->monsters.pushBack(ms);
		}
	}
}

bool HelloWorld::onBegan(Touch *touch, Event* event)
{
	auto touchPos = touch->getLocation();

	if (this->m_isGameStop)
	{
		touchPos -= VISIBLE_SIZE / 2;
		if (this->power_up->getBoundingBox().containsPoint(touchPos))
		{
			m_hero->getPower(this->m_level, 1);

			this->m_ui->removeAllChildren();
			this->m_isGameStop = false;
			Director::getInstance()->resume();

			return false;
		}
		if (this->heal_up->getBoundingBox().containsPoint(touchPos))
		{
			m_hero->getPower(this->m_level, 2);

			this->m_ui->removeAllChildren();
			this->m_isGameStop = false;
			Director::getInstance()->resume();

			return false;
		}
		if (this->speed_up->getBoundingBox().containsPoint(touchPos))
		{
			m_hero->getPower(this->m_level, 3);

			this->m_ui->removeAllChildren();
			this->m_isGameStop = false;
			Director::getInstance()->resume();

			return false;
		}

		return false;
	}

	if (m_hero->getStatus() == STATUS::AIM_TAR)
	{
		for (auto ms : monsters)
		{
			if (ms->getBoundingBox().containsPoint(touchPos))
			{
				m_hero->aim(ms);
			}
		}
	}
	else if ((touchPos.x > VISIBLE_SIZE.width / 2 && m_hero->getStatus() == STATUS::NORMAL) || m_hero->getStatus() == STATUS::AIM_POS)
	{
		if (skill_ori->getBoundingBox().containsPoint(touchPos))
		{
			m_hero->executeSkill(1);
			return false;
		}
		if (skill_q->getBoundingBox().containsPoint(touchPos))
		{
			m_hero->executeSkill(2);
			return false;
		}
		if (skill_e->getBoundingBox().containsPoint(touchPos))
		{
			m_hero->executeSkill(3);
			return false;
		}
		if (skill_r->getBoundingBox().containsPoint(touchPos))
		{
			m_hero->executeSkill(4);
			return false;
		}
		if (skill_f->getBoundingBox().containsPoint(touchPos))
		{
			m_hero->executeSkill(5);
			return false;
		}
		m_hero->click(touchPos);
	}
	else
	{
		touchPos -= this->firstTouchPos;

		if (fabs(touchPos.x) > GRID * 3 || fabs(touchPos.y) > GRID * 3)
			return false;

		if (fabs(touchPos.y) > fabs(touchPos.x))
		{
			if (touchPos.y > 0)
			{
				//up
				m_hero->move(DIRECTION::UP);
			}
			else
			{
				//down
				m_hero->move(DIRECTION::DOWN);
			}
		}
		else
		{
			if (touchPos.x > 0)
			{
				//right
				m_hero->move(DIRECTION::RIGHT);
			}
			else
			{
				//left
				m_hero->move(DIRECTION::LEFT);
			}
		}
		return true;
	}
	
	return false;
}

void HelloWorld::onEnd(Touch* touch, Event* event)
{
	m_hero->stop(m_hero->getDirection());
}

Vec2 HelloWorld::getP(Vec2 pos)
{
	Vec2 loc;
	loc.x = (int)pos.x / GRID;
	loc.y = (int)pos.y / GRID;

	return loc;
}

void HelloWorld::intro()
{
	m_level++;

	if (m_level == 0)
	{
		this->m_map->firstMap();
	}
	else
	{
		this->m_map->anotherMap();
	}

	m_hero->setPosition(this->m_map->getHeroPos());


	if (m_level > 0)
	{
		m_ui= Node::create();
		m_ui->setPosition(VISIBLE_SIZE / 2);
		this->addChild(m_ui, 99);

		this->power_up= Sprite::create(StringUtils::format("PowerUp0%d.png",this->m_level - 1));
		power_up->setPosition(Vec2(-GRID * 5, 0));
		m_ui->addChild(power_up, 99);

		this->power_up->runAction(Sequence::create(ScaleTo::create(0.3f, 0.5f),ScaleTo::create(0.2f,1),nullptr));

		auto f = Label::createWithSystemFont("P", "Consolas", 40);
		power_up->addChild(f, 99);
		f->setPosition(Vec2(power_up->getContentSize().width / 2, power_up->getContentSize().height + 20));

		this->heal_up = Sprite::create(StringUtils::format("HealthUp0%d.png", this->m_level - 1));
		heal_up->setPosition(Vec2(0, 0));
		m_ui->addChild(heal_up, 99);

		this->heal_up->runAction(Sequence::create(ScaleTo::create(0.3f, 0.5f), ScaleTo::create(0.2f, 1), nullptr));

		auto s = Label::createWithSystemFont("H", "Consolas", 40);
		heal_up->addChild(s, 99);
		s->setPosition(Vec2(heal_up->getContentSize().width / 2, heal_up->getContentSize().height + 20));

		this->speed_up = Sprite::create(StringUtils::format("SpeedUp0%d.png", this->m_level - 1));
		speed_up->setPosition(Vec2(GRID * 5, 0));
		m_ui->addChild(speed_up, 99);

		this->speed_up->runAction(Sequence::create(ScaleTo::create(0.3f, 0.5f), ScaleTo::create(0.2f, 1), CallFuncN::create([=](Ref *){
			this->m_isGameStop = true;
			Director::getInstance()->pause();
		}), nullptr));

		auto t = Label::createWithSystemFont("S", "Consolas", 40);
		speed_up->addChild(t, 99);
		t->setPosition(Vec2(speed_up->getContentSize().width / 2, speed_up->getContentSize().height + 20));
	}

	auto level = (Label*)this->getChildByTag(8);
	if (level)
	{
		level->setString(StringUtils::format("%d", m_level));
	}


	createEnemy(1.0f);
}