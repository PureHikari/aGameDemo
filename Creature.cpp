#include "Creature.h"

Creature *Creature::create()
{
	auto creature = new Creature();
	if (creature&&creature->initWithType())
	{
		creature->autorelease();
		return creature;
	}
	CC_SAFE_DELETE(creature);
	return nullptr;
}

bool Creature::initWithType()
{
	do
	{
		//initSkill();

	} while (0);

	return true;
}
void Creature::move(DIRECTION dr)
{
	if (this->m_status != STATUS::NORMAL)
		return;

	switch (dr)
	{
	case DIRECTION::UP:
	{
		this->setDirection(DIRECTION::UP);

		schedule([=](float){
			if (this->m_map->isBarrel(this->getPosition() + Vec2(0, GRID)))
				this->stop(DIRECTION::UP);
			else
				this->runAction(MoveBy::create(this->m_speed, Vec2(0, GRID / 20)));
		}, this->m_speed / 20, "MOVE_UP");
	}
		break;
	case DIRECTION::DOWN:
	{
		this->setDirection(DIRECTION::DOWN);

		schedule([=](float){
			if (this->m_map->isBarrel(this->getPosition() + Vec2(0, -GRID)))
				this->stop(DIRECTION::DOWN);
			else
				this->runAction(MoveBy::create(this->m_speed, Vec2(0, -GRID / 20)));
		}, this->m_speed / 20, "MOVE_DOWN");
	}
		break;
	case  DIRECTION::LEFT:
	{
		this->setDirection(DIRECTION::LEFT);

		schedule([=](float){
			if (this->m_map->isBarrel(this->getPosition() + Vec2(-GRID, 0)))
				this->stop(DIRECTION::LEFT);
			else
				this->runAction(MoveBy::create(this->m_speed, Vec2(-GRID / 20, 0)));
		}, this->m_speed / 20, "MOVE_LEFT");
	}
		break;
	case  DIRECTION::RIGHT:
	{
		this->setDirection(DIRECTION::RIGHT);

		schedule([=](float){
			if (this->m_map->isBarrel(this->getPosition() + Vec2(GRID, 0)))
				this->stop(DIRECTION::RIGHT);
			else
				this->runAction(MoveBy::create(this->m_speed, Vec2(GRID / 20, 0)));
		}, this->m_speed / 20, "MOVE_RIGHT");
	}
		break;
	}
}

void Creature::stop(DIRECTION dr)
{
	switch (dr)
	{
	case DIRECTION::UP:
	{
		unschedule("MOVE_UP");
	}
		break;
	case DIRECTION::DOWN:
	{
		unschedule("MOVE_DOWN");
	}
		break;
	case DIRECTION::LEFT:
	{
		unschedule("MOVE_LEFT");
	}
		break;
	case DIRECTION::RIGHT:
	{
		unschedule("MOVE_RIGHT");
	}
		break;
	}
}

void Creature::restoreHp(int hp)
{
	if (this->m_hp == this->m_maxHp)
		return;

	//回血时数字特效
	auto lab1 = Label::createWithSystemFont(StringUtils::format("%d", hp), "Consolas", 40);
	lab1->setColor(Color3B::GREEN);
	lab1->setPosition(this->getContentSize() / 2);
	this->addChild(lab1, 4, 1);
	lab1->runAction(Sequence::create(MoveBy::create(0.2f, Vec2(random(-GRID, GRID), random(-GRID, GRID))), CallFuncN::create([=](Ref *){
		lab1->removeFromParent();
	}), nullptr));

	this->m_hp += hp;
	if (this->m_hp > this->m_maxHp)
		this->m_hp = m_maxHp;
}

void Creature::restoreMp(int mp)
{
	if (this->m_mp == this->m_maxMp)
		return;

	//回蓝时数字特效
	auto lab1 = Label::createWithSystemFont(StringUtils::format("%d", mp), "Consolas", 40);
	lab1->setColor(Color3B::BLUE);
	lab1->setPosition(this->getContentSize() / 2);
	this->addChild(lab1, 4, 1);
	lab1->runAction(Sequence::create(MoveBy::create(0.2f, Vec2(random(-GRID, GRID), random(-GRID, GRID))), CallFuncN::create([=](Ref *){
		lab1->removeFromParent();
	}), nullptr));

	this->m_mp += mp;
	if (this->m_mp > this->m_maxMp)
		this->m_mp = m_maxMp;
}

void Creature::refresh()
{
	schedule([=](float){
		for (int i = 0; i < this->m_skills.size(); i++)
		{
			if (this->m_skills.at(i).cdTime>0)
			{
				this->m_skills.at(i).cdTime--;
			}
		}
	}, 1 / 60, "Cd");

	schedule([=](float){
		this->restoreMp(5);
	}, 5, "mana");
}

void Creature::hit(Skills skill,int dmg)
{
	this->hit(dmg);
	
	auto no = Node::create();
	this->addChild(no);
	this->m_isInwin = true;
	this->setOpacity(100);
	no->runAction(Sequence::create(DelayTime::create(this->m_inwinTime), CallFuncN::create([=](Ref *){
		this->m_isInwin = false;
		this->setOpacity(255);
		no->removeFromParent();
	}), nullptr));

	
	if (skill.effect == SkillEffect::NONE)
		return;

	switch (skill.effect)
	{
		case SkillEffect::REPEL:
		{
			//this->repel(skillgetPosition(), skill->m_effectNum);
		}
			break;
		case SkillEffect::SLOW:
		{
			this->slow(skill.effectNum);
		}
			break;
		case SkillEffect::OVERKILL:
		{
			this->hit(skill.effectNum);
		}
			break;
	}
}

void Creature::hit(int dmg)
{
	//受伤时数字特效
	auto lab1 = Label::createWithSystemFont(StringUtils::format("%d", dmg), "Consolas", 40);
	lab1->setPosition(this->getContentSize() / 2);
	this->addChild(lab1, 4, 1);
	lab1->runAction(Sequence::create(MoveBy::create(0.2f, Vec2(random(-GRID, GRID), random(-GRID, GRID))), CallFuncN::create([=](Ref *){
		lab1->removeFromParent();
	}), nullptr));

	this->m_hp -= dmg;
	if (this->m_hp <= 0)
		this->dead();
}

void Creature::dead()
{
	this->stopAllActions();	
	
	this->runAction(Sequence::create(DelayTime::create(1), CallFuncN::create([=](Ref *){
		//设置为死亡
		this->m_isAlive = false;
	}), nullptr));
}

void Creature::setDirection(DIRECTION dr)
{
	if (dr == this->m_direction)
		return;

	int angle = 0;
	switch (dr)
	{
		case DIRECTION::UP:
		{
			angle = 0;
		}
			break;
		case DIRECTION::DOWN:
		{
			angle = 180;
		}
			break;
		case DIRECTION::LEFT:
		{
			angle = 270;
		}
			break;
		case DIRECTION::RIGHT:
		{
			angle = 90;
		}
			break;
	}

	this->m_direction = dr;
	this->setRotation(angle);
}

void Creature::repel(Vec2 pos, int i)
{
	Vec2 vec = this->getPosition() - pos;

	this->runAction(MoveBy::create(0.2f, vec*i));
}

void Creature::slow(int i)
{
	auto no = Node::create();
	this->addChild(no);

	int temp = this->m_speed;
	this->m_speed *= 1.3;
	no->runAction(Sequence::create(DelayTime::create(i), CallFuncN::create([=](Ref *){
		this->m_speed = temp;
		no->removeFromParent();
	}), nullptr));
}

void Creature::bleed(int i)
{

	schedule([=](float){
		this->m_hp -= this->m_maxHp*0.05;
	}, 2, i, 0, "bleed");
}

Skills Creature::skillAtk()
{
	//扣除mp并刷新cd
	this->m_mp -= this->m_skills.at(this->skill_num).reque_mp;
	this->m_hp -= this->m_skills.at(this->skill_num).reque_hp;
	this->m_skills.at(this->skill_num).cdTime = this->m_skills.at(this->skill_num).cd;
	
	//将状态修正为释放技能以保证释放技能时无法移动和释放其他技能
	this->m_status = STATUS::SKILL_PRE;
	this->runAction(Sequence::create(DelayTime::create(this->m_skills.at(this->skill_num).Delay_time), CallFuncN::create([=](Ref *){
		this->m_status = STATUS::NORMAL;
		//后摇结束后恢复到普通状态
	}), nullptr));

	//返回skill
	return m_skills.at(this->skill_num);
}

void Creature::getMap(GameMap *gm)
{
	this->m_map = gm;
}

void Creature::aim(Creature *target)
{
	this->m_skills.at(this->skill_num).pos = target->getPosition();
	this->m_status = STATUS::SKILL_EXE;
}

bool Creature::isSkillAlbe(int num)
{
	if (this->m_skills.size() <= num)
		return false;
	if (this->m_skills.at(num).cdTime > 0)
		return false;
	if (this->m_skills.at(num).reque_mp > this->m_mp)
		return false;
	if (this->m_skills.at(num).reque_hp >= this->m_hp)
		return false;
	return true;
}



void Creature::cutCDTime(int num, int cd)
{
	this->m_skills.at(num + 1).cdTime -= cd * 60;
}

void Creature::getBuff(int num,float time)
{
	//攻击力提高
	if (num == 1)
	{
		auto temp = this->m_dmg;
		this->m_dmg *= 1.2;
		this->runAction(Sequence::create(DelayTime::create(time), CallFuncN::create([=](Ref *){
			this->m_dmg = temp;
		}), nullptr));
	}
	//普通攻击无cd
	if (num == 2)
	{
		auto temp = this->m_skills.at(0).cd;
		this->m_skills.at(0).cd = 0;
		this->runAction(Sequence::create(DelayTime::create(time), CallFuncN::create([=](Ref *){
			this->m_skills.at(0).cd = temp;
		}), nullptr));
	}
	//普通攻击带额外伤害效果
	if (num == 3)
	{
		auto temp = this->m_skills.at(0).effect;
		this->m_skills.at(0).effect = SkillEffect::OVERKILL;
		this->runAction(Sequence::create(DelayTime::create(time), CallFuncN::create([=](Ref *){
			this->m_skills.at(0).effect = temp;
		}), nullptr));
	}
	//e没有cd
	if (num == 4)
	{
		int temp_1 = this->m_skills.at(3).cd;
		int temp_2 = this->m_skills.at(3).reque_hp;
		int temp_3 = this->m_skills.at(3).reque_mp;
		this->m_skills.at(3).cd = 0;
		this->m_skills.at(3).cdTime = 0;
		this->m_skills.at(3).reque_mp = 0;
		this->m_skills.at(3).reque_hp = 0;
		this->runAction(Sequence::create(DelayTime::create(time), CallFuncN::create([=](Ref *){
			this->m_skills.at(3).cd = temp_1;
			this->m_skills.at(3).reque_hp = temp_2;
			this->m_skills.at(3).reque_mp = temp_3;
		}), nullptr));
	}
}

void Creature::getBuff(int num)
{
	this->getBuff(num, 5);
}

void Creature::executeSkill(int num)
{
	if (!this->isSkillAlbe(num))
		return;
	if (this->m_status != STATUS::NORMAL)
		return;

	switch (this->m_skills.at(num).Aim_type)
	{
	case SkillAimType::DIRE_ATK:
	{
		auto pos = this->getPosition();
		bool re = false;
		switch (this->m_direction)
		{
		case DIRECTION::UP:
		{
			pos += Vec2(0, GRID);
			re = true;
		}
			break;
		case DIRECTION::DOWN:
		{
			pos += Vec2(0, -GRID);
			re = true;
		}
			break;
		case DIRECTION::LEFT:
		{
			pos += Vec2(-GRID, 0);
		}
			break;
		case DIRECTION::RIGHT:
		{
			pos += Vec2(GRID, 0);
		}
			break;
		}
		this->m_skills.at(num).pos = pos;
		this->m_skills.at(num).is180 = re;
		this->skill_num = num;
		this->m_status = STATUS::SKILL_EXE;
	}
		break;
	case SkillAimType::POS_ATK:
	{
		if (this->m_isHero)
		{
			this->m_status = STATUS::AIM_POS;
			this->skill_num = num;
		}
		else
		{
			this->skill_num = num;
			this->m_skills.at(num).pos = this->m_target->getPosition();
		}
	}
		break;
	case SkillAimType::TAR_ATK:
	{
		if (this->m_isHero)
		{
			this->m_status = STATUS::AIM_TAR;
			this->skill_num = num;
		}
		else
		{
			this->skill_num = num;
			this->m_skills.at(num).pos = this->m_target->getPosition();
		}
	}
		break;
	case SkillAimType::SELF_EXE:
	{
		this->m_status = STATUS::SKILL_EXE;
		this->skill_num = num;
	}
		break;
	case SkillAimType::SELF_POS:
	{
		this->m_skills.at(num).pos += this->getPosition();
		this->m_status = STATUS::SKILL_EXE;
		this->skill_num = num;
	}
		break;
	}
}