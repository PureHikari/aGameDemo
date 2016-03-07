#include "Hero.h"

Hero *Hero::create()
{
	auto hero = new Hero();
	if (hero&&hero->init())
	{
		hero->autorelease();
		return hero;
	}
	CC_SAFE_DELETE(hero);
	return nullptr;
}

bool Hero::init()
{
	do
	{
		this->initWithFile("CloseNormal.png");

		this->m_dmg = 10;
		this->m_hp = 10;
		this->m_maxHp = 100;
		this->m_mp = 10;
		this->m_maxMp = 10;

		this->m_inwinTime = 0.5f;
		this->m_isHero = true;

		this->initNormalAtk();
		this->initSkill1();
		this->initSkill2();

		/*this->getSkill(2);
		this->getSkill(1);
		this->getSkill(2);
		this->getSkill(1);*/
		this->refresh();
	} while (0);

	return true;
}

void Hero::initNormalAtk()
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
	temp.cd = 60 * 0.5;
	temp.cdTime = 0;
	temp.isHero = true;
	temp.isAble = true;
	temp.Exist_time = 0.3f;
	temp.Delay_time = 0.3f;
	temp.is180 = false;

	this->m_skills.push_back(temp);
}

void Hero::initSkill1()
{
	/*Skills temp;
	temp.id = 1;
	temp.dmg = 1;
	temp.reque_hp = 0;
	temp.reque_mp = 2;
	temp.effect = SkillEffect::NONE;
	temp.effectNum = 0;
	temp.Aim_type = SkillAimType::DIRE_ATK;
	temp.pay = SkillPay::MP_RES;
	temp.pay_num = 5;
	temp.pos = Vec2::ZERO;
	temp.cd = 5;
	temp.cdTime = 0;
	temp.isHero = true;
	temp.isAble = true;
	temp.Exist_time = 0.3f;
	temp.Delay_time = 0.3f;
	temp.is180 = false;

	this->m_skills.push_back(temp);*/
}

void Hero::initSkill2()
{
	/*Skills temp;
	temp.isHero = true;
	temp.pos = this->getPosition();
	temp.reque = 0;
	temp.id = "atk_normal01.png";
	temp.cd = 30;
	temp.cdTime = 0;
	temp.dmg = 1;
	temp.effect = SkillEffect::NONE;
	temp.effectNum = 0;
	temp.type = SkillType::AIM_ATK;
	temp.isAble = false;
	temp.time = 0.3f;
	this->m_skills.push_back(temp);*/
}


void Hero::control(EventKeyboard::KeyCode keyCode,bool isPress)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
	{
		if (this->m_status == STATUS::AIM_DIR)
		{
			this->m_skills.at(this->skill_num).pos = this->getPosition() + Vec2(-GRID, 0);
			this->m_status = STATUS::SKILL_EXE;

			this->setDirection(DIRECTION::LEFT);
			this->m_skills.at(this->skill_num).is180 = false;
		}
		else
		{
			if (isPress)
			{
				this->move(DIRECTION::LEFT);
			}
			else
			{
				this->stop(DIRECTION::LEFT);
			}
		}
	}
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
	{
		if (this->m_status == STATUS::AIM_DIR)
		{
			this->m_skills.at(this->skill_num).pos = this->getPosition() + Vec2(GRID, 0);
			this->m_status = STATUS::SKILL_EXE;

			this->setDirection(DIRECTION::RIGHT);
			this->m_skills.at(this->skill_num).is180 = false;
		}
		else
		{
			if (isPress)
			{
				this->move(DIRECTION::RIGHT);
			}
			else
			{
				this->stop(DIRECTION::RIGHT);
			}
		}
	}
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
	{
		if (this->m_status == STATUS::AIM_DIR)
		{
			this->m_skills.at(this->skill_num).pos = this->getPosition() + Vec2(0, GRID);
			this->m_status = STATUS::SKILL_EXE;

			this->setDirection(DIRECTION::UP);
			this->m_skills.at(this->skill_num).is180 = true;
		}
		else
		{
			if (isPress)
			{
				this->move(DIRECTION::UP);
			}
			else
			{
				this->stop(DIRECTION::UP);
			}
		}
	}
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
	{
		if (this->m_status == STATUS::AIM_DIR)
		{
			this->m_skills.at(this->skill_num).pos = this->getPosition() + Vec2(0, -GRID);
			this->m_status = STATUS::SKILL_EXE;

			this->setDirection(DIRECTION::DOWN);
			this->m_skills.at(this->skill_num).is180 = true;
		}
		else
		{
			if (isPress)
			{
				this->move(DIRECTION::DOWN);
			}
			else
			{
				this->stop(DIRECTION::DOWN);
			}
		}
	}
	break;
	case cocos2d::EventKeyboard::KeyCode::KEY_Q:
	{		
			this->executeSkill(2);
	}
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_E:
	{

			this->executeSkill(3);
	}
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_R:
	{
			this->executeSkill(4);
	}
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F:
	{
			this->executeSkill(5);
	}
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
	{
		this->executeSkill(1);
	}
		break;
	}
}

void Hero::click(Vec2 pos)
{
	switch (this->m_status)
	{
		case STATUS::NORMAL://普通状态下进行普通攻击
		{
			this->executeSkill(0);
		}
			break;
		case STATUS::AIM_TAR:
			break;
		case STATUS::AIM_POS://瞄准位置状态下将技能的位置修改并切换状态到执行
		{
			this->m_skills.at(this->skill_num).pos = pos;
			this->m_status = STATUS::SKILL_EXE;
		}
			break;
	}
}

void Hero::getSkill(int num)
{
	switch (this->m_skills.size() - 1)
	{
	case 0:
	{
		if (num == 1)
		{
			//获得增加攻击力的buff
			Skills temp;
			temp.id = 0;
			temp.dmg = 1;
			temp.reque_hp = 5;
			temp.reque_mp = 0;
			temp.effect = SkillEffect::NONE;
			temp.effectNum = 0;
			temp.type = SkillType::BUFF;
			temp.Aim_type = SkillAimType::SELF_EXE;
			temp.pay = SkillPay::NONE;
			temp.pay_num = 0;
			temp.pos = Vec2::ZERO;
			temp.cd = 60 * 30;
			temp.cdTime = 0;
			temp.isHero = true;
			temp.isAble = true;
			temp.Exist_time = 5.0f;
			temp.Delay_time = 0.1f;
			temp.is180 = false;

			this->m_skills.push_back(temp);
		}
		if (num == 2)
		{
			//恢复mp
			Skills temp;
			temp.id = 0;
			temp.dmg = 10;
			temp.reque_hp = 0;
			temp.reque_mp = 0;
			temp.effect = SkillEffect::NONE;
			temp.effectNum = 0;
			temp.type = SkillType::MANA;
			temp.Aim_type = SkillAimType::SELF_EXE;
			temp.pay = SkillPay::NONE;
			temp.pay_num = 0;
			temp.pos = Vec2::ZERO;
			temp.cd = 60 * 30;
			temp.cdTime = 0;
			temp.isHero = true;
			temp.isAble = true;
			temp.Exist_time = 0.1f;
			temp.Delay_time = 0.1f;
			temp.is180 = false;

			this->m_skills.push_back(temp);
		}
		if (num == 3)
		{
			//普通攻击无间隔
			Skills temp;
			temp.id = 0;
			temp.dmg = 2;
			temp.reque_hp = 0;
			temp.reque_mp = 0;
			temp.effect = SkillEffect::NONE;
			temp.effectNum = 0;
			temp.type = SkillType::BUFF;
			temp.Aim_type = SkillAimType::SELF_EXE;
			temp.pay = SkillPay::NONE;
			temp.pay_num = 0;
			temp.pos = Vec2::ZERO;
			temp.cd = 60 * 30;
			temp.cdTime = 0;
			temp.isHero = true;
			temp.isAble = true;
			temp.Exist_time = 5.0f;
			temp.Delay_time = 0.1f;
			temp.is180 = false;

			this->m_skills.push_back(temp);
		}
	}
		break;
	case 1:
	{
		if (num == 1)
		{
			//对所有近身敌人造成一定伤害
			Skills temp;
			temp.id = 1;
			temp.dmg = 2;
			temp.reque_hp = 0;
			temp.reque_mp = 5;
			temp.effect = SkillEffect::NONE;
			temp.effectNum = 0;
			temp.Aim_type = SkillAimType::SELF_POS;
			temp.pay = SkillPay::NONE;
			temp.pay_num = 0;
			temp.pos = Vec2::ZERO;
			temp.cd = 60 * 30;
			temp.cdTime = 0;
			temp.isHero = true;
			temp.isAble = true;
			temp.Exist_time = 0.1f;
			temp.Delay_time = 0.1f;
			temp.is180 = false;

			this->m_skills.push_back(temp);
		}
		else if (num == 2)
		{
			//为自身恢复一定血量
			Skills temp;
			temp.id = 1;
			temp.dmg = 50;
			temp.reque_hp = 0;
			temp.reque_mp = 5;
			temp.effect = SkillEffect::NONE;
			temp.effectNum = 0;
			temp.Aim_type = SkillAimType::SELF_EXE;
			temp.pay = SkillPay::NONE;
			temp.pay_num = 0;
			temp.pos = Vec2::ZERO;
			temp.cd = 60 * 30;
			temp.cdTime = 0;
			temp.isHero = true;
			temp.isAble = true;
			temp.Exist_time = 0.1f;
			temp.Delay_time = 0.1f;
			temp.is180 = false;

			this->m_skills.push_back(temp);
		}
		else
		{
			//普通攻击带额外伤害
			Skills temp;
			temp.id = 1;
			temp.dmg = 3;
			temp.reque_hp = 0;
			temp.reque_mp = 5;
			temp.effect = SkillEffect::NONE;
			temp.effectNum = 0;
			temp.type = SkillType::BUFF;
			temp.Aim_type = SkillAimType::SELF_EXE;
			temp.pay = SkillPay::NONE;
			temp.pay_num = 0;
			temp.pos = Vec2::ZERO;
			temp.cd = 60 * 30;
			temp.cdTime = 0;
			temp.isHero = true;
			temp.isAble = true;
			temp.Exist_time = 5.0f;
			temp.Delay_time = 0.1f;
			temp.is180 = false;

			this->m_skills.push_back(temp);
		}
	}
	break;
	case 2:
	{
		if (num == 1)
		{
			//对正面敌人一次大范围伤害
			Skills temp;
			temp.id = 2;
			temp.dmg = 4;
			temp.reque_hp = 5;
			temp.reque_mp = 5;
			temp.effect = SkillEffect::NONE;
			temp.effectNum = 0;
			temp.Aim_type = SkillAimType::DIRE_ATK;
			temp.pay = SkillPay::NONE;
			temp.pay_num = 0;
			temp.pos = Vec2::ZERO;
			temp.cd =60 * 5;
			temp.cdTime = 0;
			temp.isHero = true;
			temp.isAble = true;
			temp.Exist_time = 0.3f;
			temp.Delay_time = 0.3f;
			temp.is180 = false;

			this->m_skills.push_back(temp);
		}
		else if (num == 2)
		{
			//恢复大量hp
			Skills temp;
			temp.id = 2;
			temp.dmg = 100;
			temp.reque_hp = 0;
			temp.reque_mp = 10;
			temp.effect = SkillEffect::NONE;
			temp.effectNum = 0;
			temp.Aim_type = SkillAimType::SELF_EXE;
			temp.pay = SkillPay::NONE;
			temp.pay_num = 0;
			temp.pos = Vec2::ZERO;
			temp.cd =60 * 5;
			temp.cdTime = 0;
			temp.isHero = true;
			temp.isAble = true;
			temp.Exist_time = 0.3f;
			temp.Delay_time = 0.3f;
			temp.is180 = false;

			this->m_skills.push_back(temp);
		}
		else
		{
			//对正面敌人造成伤害，附带额外伤害
			Skills temp;
			temp.id = 2;
			temp.dmg = 4;
			temp.reque_hp = 0;
			temp.reque_mp = 5;
			temp.effect = SkillEffect::OVERKILL;
			temp.effectNum = 20;
			temp.Aim_type = SkillAimType::DIRE_ATK;
			temp.pay = SkillPay::NONE;
			temp.pay_num = 0;
			temp.pos = Vec2::ZERO;
			temp.cd = 60 * 5;
			temp.cdTime = 0;
			temp.isHero = true;
			temp.isAble = true;
			temp.Exist_time = 0.1f;
			temp.Delay_time = 0.3f;
			temp.is180 = false;

			this->m_skills.push_back(temp);
		}
	}
	break;
	case 3:
	{
		if (num == 1)
		{
			//爆发对周围敌人造成伤害，每击中一人则cd缩短5s
			Skills temp;
			temp.id = 3;
			temp.dmg = 2;
			temp.reque_hp = 2;
			temp.reque_mp = 0;
			temp.effect = SkillEffect::NONE;
			temp.effectNum = 0;
			temp.Aim_type = SkillAimType::SELF_POS;
			temp.pay = SkillPay::CD_REF;
			temp.pay_num = 5;
			temp.pos = Vec2::ZERO;
			temp.cd =60 * 45;
			temp.cdTime = 0;
			temp.isHero = true;
			temp.isAble = true;
			temp.Exist_time = 0.1f;
			temp.Delay_time = 0.1f;
			temp.is180 = false;

			this->m_skills.push_back(temp);
		}
		else if (num == 2)
		{
			//召唤光球爆炸对敌人造成伤害，每击中一个则恢复一定mp
			Skills temp;
			temp.id = 3;
			temp.dmg = 2;
			temp.reque_hp = 0;
			temp.reque_mp = 0;
			temp.effect = SkillEffect::NONE;
			temp.effectNum = 0;
			temp.Aim_type = SkillAimType::POS_ATK;
			temp.pay = SkillPay::MP_RES;
			temp.pay_num = 5;
			temp.pos = Vec2::ZERO;
			temp.cd =60 * 45;
			temp.cdTime = 0;
			temp.isHero = true;
			temp.isAble = true;
			temp.Exist_time = 0.1f;
			temp.Delay_time = 0.1f;
			temp.is180 = false;

			this->m_skills.push_back(temp);
		}
		else
		{
			//旋转一圈，对碰到的敌人造成一定伤害，每击中一个则恢复一定hp
			Skills temp;
			temp.id = 3;
			temp.dmg = 1;
			temp.reque_hp = 0;
			temp.reque_mp = 2;
			temp.effect = SkillEffect::NONE;
			temp.effectNum = 0;
			temp.Aim_type = SkillAimType::SELF_POS;
			temp.pay = SkillPay::HP_RES;
			temp.pay_num = 5;
			temp.pos = Vec2::ZERO;
			temp.cd =60 * 45;
			temp.cdTime = 0;
			temp.isHero = true;
			temp.isAble = true;
			temp.Exist_time = 0.1f;
			temp.Delay_time = 0.1f;
			temp.is180 = false;

			this->m_skills.push_back(temp);
		}
	}
	break;
	case 4:
	{
		if (num == 1)
		{
			//剑刃风暴
			Skills temp;
			temp.id = 4;
			temp.dmg = 1;
			temp.reque_hp = 0;
			temp.reque_mp = 0;
			temp.effect = SkillEffect::NONE;
			temp.effectNum = 0;
			temp.Aim_type = SkillAimType::SELF_POS;
			temp.pay = SkillPay::NONE;
			temp.pay_num = 0;
			temp.pos = Vec2::ZERO;
			temp.cd =60 * 60;
			temp.cdTime = 0;
			temp.isHero = true;
			temp.isAble = true;
			temp.Exist_time = 6.0f;
			temp.Delay_time = 6.0f;
			temp.is180 = false;

			this->m_skills.push_back(temp);
		}
		else if (num == 2)
		{
			//圣光审判
			Skills temp;
			temp.id = 4;
			temp.dmg = 1;
			temp.reque_hp = 0;
			temp.reque_mp = 10;
			temp.effect = SkillEffect::OVERKILL;
			temp.effectNum = 20;
			temp.Aim_type = SkillAimType::POS_ATK;
			temp.pay = SkillPay::HP_RES;
			temp.pay_num = 5;
			temp.pos = Vec2::ZERO;
			temp.cd = 60 * 60;
			temp.cdTime = 0;
			temp.isHero = true;
			temp.isAble = true;
			temp.Exist_time = 6.0f;
			temp.Delay_time = 0.1f;
			temp.is180 = false;

			this->m_skills.push_back(temp);
		}
		else
		{
			Skills temp;
			temp.id = 4;
			temp.dmg = 10;
			temp.reque_hp = 0;
			temp.reque_mp = 0;
			temp.effect = SkillEffect::OVERKILL;
			temp.effectNum = 20;
			temp.Aim_type = SkillAimType::TAR_ATK;
			temp.pay = SkillPay::BUFF_GET;
			temp.pay_time = SkillPayTime::ONLYONE;
			temp.pay_num = 4;
			temp.pos = Vec2::ZERO;
			temp.cd = 60 * 60;
			temp.cdTime = 0;
			temp.isHero = true;
			temp.isAble = true;
			temp.Exist_time = 0.1f;
			temp.Delay_time = 0.1f;
			temp.is180 = false;

			this->m_skills.push_back(temp);
		}
	}
	break;
	}
}

int Hero::getCDTime(int num)
{
	if (num >= this->m_skills.size())
		return 0;
	return this->m_skills.at(num).cdTime;
}

void Hero::getPower(int level, int num)
{
	if (1)
	{
		this->getSkill(num);
	}
}