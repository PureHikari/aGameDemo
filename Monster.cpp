#include "Monster.h"

Monster *Monster::create(int hp, int dmg) {
	Monster * monster = new Monster();
	if (monster && monster->initWithType(hp, dmg))
	{
		monster->autorelease();
		return monster;
	}
	CC_SAFE_DELETE(monster);
	return nullptr;
}

bool Monster::initWithType(int hp, int dmg)
{
	do
	{
		this->initWithFile("CloseNormal.png");

		this->m_hp = hp;
		this->m_maxHp = hp;
		this->m_maxMp = 10;
		this->m_mp = 10;
		this->m_dmg = dmg;

		this->initNormalAtk();
		this->initSkill();
		this->Behavier();
		this->refresh();
	} while (0);

	return true;
}

void Monster::waitForAim(bool aim)
{
	if (aim)
	{
		this->lock = Sprite::create("target.png");
		lock->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
		this->addChild(lock,1,1);
	}
	else if (lock)
	{
		this->removeChildByTag(1);
	}
	
}

void Monster::AiCycle()
{
	//当与英雄距离过近并且状态不是攻击时切换为攻击状态
	if (this->m_aiSp != MsAiSp::BLIND && this->m_aiStatus!=MsAiStatus::ATK && this->m_aiStatus!=MsAiStatus::RUN_AWAY)
	{
		auto dis = this->m_target->getPosition().distance(this->getPosition());

		if (dis <= GRID * 2)
		{
			this->stopActionByTag(1);

			this->stop(this->m_direction);
			this->m_aiStatus = MsAiStatus::ATK;
		}
	}

	//如果受到攻击则攻击英雄
	if (this->m_aiStatus!=MsAiStatus::ATK && this->m_maxHp > this->m_hp)
	{
		this->stopActionByTag(1);
		this->stop(this->m_direction);

		this->m_aiStatus = MsAiStatus::ATK;
	}

	//如果血量过低则开始逃跑
	//if (this->m_aiStatus!=MsAiStatus::RUN_AWAY && this->m_aiSp != MsAiSp::BRARO && this->m_hp <= this->m_maxHp * 0.3)
	//{
	//	this->stopActionByTag(1);

	//	this->stop(this->m_direction);
	//	this->m_aiStatus = MsAiStatus::RUN_AWAY;
	//}

	if (this->m_aiStatus != MsAiStatus::PETRO)
	{
		this->Behavier();
	}
}

void Monster::Behavier()
{
	switch (this->m_aiStatus)
	{
	case MsAiStatus::PETRO:
	{
		auto act =RepeatForever::create( Sequence::create(CallFuncN::create([=](Ref *){
			this->move((DIRECTION)random(0, 3));
		}), DelayTime::create(this->m_speed * random(1, 4)), CallFuncN::create([=](Ref *){
			this->stop(this->getDirection());
			this->move((DIRECTION)random(0, 3));
		}), DelayTime::create(this->m_speed * random(1, 4)), CallFuncN::create([=](Ref *){
			this->stop(this->getDirection());
		}), nullptr));
		act->setTag(1);
		this->runAction(act);
	}
		break;
	case MsAiStatus::ATK:
	{
		if (this->m_target->getPosition().distance(this->getPosition()) <= GRID)
		{
			this->stop(this->m_direction);
			int i = 0;
			for (auto sk:m_skills)
			{
				if (sk.cdTime == 0 && sk.type == SkillType::ATK)
				{
					i = sk.id;
					break;
				}
			}

			this->executeSkill(i);
		}
		else
		{
			this->move(this->whereAreYou());
		}

		//to do
	}
		break;
	case MsAiStatus::RUN_AWAY:
	{
		if (this->m_target->getPosition().distance(this->getPosition()) >= GRID * 10)
		{
			this->stop(this->m_direction);

			int i = 0;
			for (auto sk : m_skills)
			{
				if (sk.Aim_type == SkillAimType::SELF_EXE)
				{
					i = sk.id;
					break;
				}
			}

			if (i)
			{
				this->executeSkill(i);
			}
		}
		else
		{
			auto dr = this->whereAreYou();
			if (dr == DIRECTION::DOWN)
			{
				dr = DIRECTION::UP;
			}
			else if (dr == DIRECTION::UP)
			{
				dr = DIRECTION::DOWN;
			}
			else if (dr == DIRECTION::LEFT)
			{
				dr = DIRECTION::RIGHT;
			}
			else if (dr == DIRECTION::RIGHT)
			{
				dr = DIRECTION::LEFT;
			}

			if (!this->m_isRunaway)
			{
				this->move(dr);
				this->m_isRunaway = true;
			}
		}
		//to do
	}
		break;
	}
}

void Monster::initNormalAtk()
{
	Skills temp;
	temp.id = 0;
	temp.dmg = 1;
	temp.reque_hp = 0;
	temp.reque_mp = 0;
	temp.effect = SkillEffect::NONE;
	temp.effectNum = 20;
	temp.Aim_type = SkillAimType::DIRE_ATK;
	temp.pay = SkillPay::NONE;
	temp.pay_num = 0;
	temp.pos = Vec2::ZERO;
	temp.cd = 60 * 3;
	temp.cdTime = 0;
	temp.isHero = false;
	temp.isAble = true;
	temp.Exist_time = 0.3f;
	temp.Delay_time = 0.3f;
	temp.is180 = false;

	this->m_skills.push_back(temp);
}

void Monster::initSkill()
{
	Skills temp;
	temp.id = 1;
	temp.dmg = random(1, 4);
	temp.reque_hp = 0;
	temp.reque_mp = random(2, 10);
	temp.effect = (SkillEffect)random(0, 3);
	temp.effectNum = random(2, 20);
	temp.Aim_type = (SkillAimType)random(0, 3);
	temp.pay = (SkillPay)random(0, 4);
	temp.pay_num = random(0, 10);
	temp.pos = Vec2::ZERO;
	temp.cd = 60 * random(0, 10);
	temp.cdTime = 0;
	temp.isHero = false;
	temp.isAble = true;
	temp.Exist_time = 0.3f;
	temp.Delay_time = 0.3f;
	temp.is180 = false;

	this->m_skills.push_back(temp);
}

DIRECTION Monster::whereAreYou()
{
	auto pos = this->m_target->getPosition() - this->getPosition();

	if (fabs(pos.y) > fabs(pos.x))
	{
		if (pos.y > 0)
		{
			//up
			return DIRECTION::UP;
		}
		else
		{
			//down
			return DIRECTION::DOWN;
		}
	}
	else
	{
		if (pos.x > 0)
		{
			//right
			return DIRECTION::RIGHT;
		}
		else
		{
			//left
			return DIRECTION::LEFT;
		}
	}
}