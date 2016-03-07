#include "Skill.h"

Skill *Skill::create(Skills sk,Creature * cr)
{
	if (!sk.isAble)
		return nullptr;

	auto skill = new Skill();
	if (skill&&skill->initWithType(sk,cr))
	{
		skill->autorelease();
		return skill;
	}
	CC_SAFE_DELETE(skill);
	return nullptr;
}

bool Skill::initWithType(Skills sk, Creature * cr)
{
	do
	{
		

		this->initWithFile("skill_q.png");
		this->skill = sk;
		this->releaser = cr;
		this->dmg = sk.dmg*cr->getDmg();

		if (sk.is180)
		{
			this->setRotation(90);
		}

	} while (0);

	return true;
}

void Skill::execute()
{
	this->runAction(Sequence::create(DelayTime::create(skill.Exist_time), CallFuncN::create([=](Ref *){
		this->isOver = true;
		if (this->skill.pay_time == SkillPayTime::ONLYONE&&this->isHit)
		{
			this->hitPay();
		}
		this->releaser->reSkill();
	}), nullptr));
	
	if (this->skill.type == SkillType::ATK)
	{
		this->setPosition(skill.pos);
		this->releaser->getParent()->addChild(this, Z_BULLET);

		if (this->skill.Aim_type == SkillAimType::SELF_POS || this->skill.Aim_type == SkillAimType::SELF_EXE)
		{
			this->setPosition(releaser->getPosition());
		}

		if (this->skill.Aim_type == SkillAimType::SELF_EXE)
		{
			this->releaser->restoreHp(this->skill.dmg);
		}

		if (this->skill.Aim_type == SkillAimType::POS_ATK || this->skill.Aim_type == SkillAimType::SELF_POS)
		{
			this->runAction(RepeatForever::create(RotateBy::create(0.5f, 360)));
		}
	}
	else if (this->skill.type == SkillType::BUFF)
	{
		this->releaser->getBuff(this->skill.dmg, this->skill.Exist_time);

		this->setPosition(skill.pos);
		this->releaser->addChild(this, Z_BULLET);
	}
	else
	{
		this->releaser->restoreMp(this->skill.dmg);
	}
	
}

void Skill::hit()
{
	this->isHit = true;

	//»÷ÖĞºó½±Àø
	if (this->skill.pay_time == SkillPayTime::EVERYTIME)
	{
		this->hitPay();
	}
}

void Skill::hitPay()
{
	switch (this->skill.pay)
	{
	case SkillPay::HP_RES:
	{
		this->releaser->restoreHp(this->skill.pay_num);
	}
		break;
	case SkillPay::MP_RES:
	{
		this->releaser->restoreMp(this->skill.pay_num);
	}
		break;
	case SkillPay::BUFF_GET:
	{
		this->releaser->getBuff(this->skill.pay_num);
	}
		break;
	case SkillPay::CD_REF:
	{
		this->releaser->cutCDTime(this->skill.id, this->skill.pay_num);
	}
		break;
	}
}