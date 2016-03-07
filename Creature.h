#pragma once
#include "cocos2d.h"
#include "CommonData.h"
#include "GameMap.h"

USING_NS_CC;

//角色的方向
enum class DIRECTION
{
	UP,
	LEFT,
	DOWN,
	RIGHT
};

//角色的状态
enum class STATUS
{
	NORMAL,//正常
	SKILL_PRE,//准备释放技能
	AIM_DIR,//瞄准
	AIM_TAR,
	AIM_POS,
	SKILL_EXE,//正在释放技能
	SKILL_DELARE
};

//枚举技能所产生的效果
enum class SkillEffect
{
	NONE,//无效果
	REPEL,//击退
	SLOW,//减速
	OVERKILL//使流血
};

enum class SkillAimType
{
	DIRE_ATK,//瞄准方向
	POS_ATK,//瞄准位置
	TAR_ATK,//瞄准目标
	SELF_POS,
	SELF_EXE//自我释放
};

enum class SkillPay
{
	NONE,
	HP_RES,
	MP_RES,
	BUFF_GET,
	CD_REF
};

enum class SkillPayTime
{
	ONLYONE,
	EVERYTIME
};

enum class SkillType
{
	ATK,
	BUFF,
	MANA
};

struct Skills
{
	int id;
	int dmg;//技能造成的伤害
	int reque_mp;//技能的消耗
	int reque_hp;
	SkillEffect effect;//技能造成的效果
	int effectNum;//技能造成效果的程度

	SkillType type=SkillType::ATK;
	SkillAimType Aim_type;//技能的类型

	SkillPay pay;
	SkillPayTime pay_time = SkillPayTime::EVERYTIME;
	int pay_num;

	Vec2 pos;//技能释放的位置

	int cd;//技能的冷却
	int cdTime;//技能的冷却实时计时

	bool isHero;//释放者是否是英雄
	bool isAble;

	float Exist_time;
	float Delay_time;//施法后摇

	bool is180;
};


class Creature:public Sprite
{
public:
	static Creature *create();
	virtual bool initWithType();

	void restoreHp(int);//恢复生命值
	void restoreMp(int);//恢复魔法值
	void refresh();//刷新各种cd
	void hit(Skills,int);//受到攻击
	void hit(int);
	void dead();//死亡
	
	void setDirection(DIRECTION dr);//设定方向
	
	//获取是否存活
	bool isAlive(){ return this->m_isAlive; }
	//获取是否无敌
	bool isInwin(){ return this->m_isInwin; } 

	//Skill *normalAtk(Vec2);//普通攻击

	int getHp(){ return this->m_hp; }
	int getMp(){ return this->m_mp; }
	int getDmg(){ return this->m_dmg; }

	void repel(Vec2,int);//击退效果的实现
	void slow(int);//减速效果的实现
	void bleed(int);//流血效果的实现

	bool isHero(){ return this->m_isHero; }

	Skills skillAtk();//使用技能

	STATUS getStatus(){ return this->m_status; }

	Creature* getTarget(){ return this->m_target; }

	void reSkill(){ this->skill_num = 0; };
	void getMap(GameMap*);

	void aim(Creature *);

	void move(DIRECTION);
	void stop(DIRECTION);

	void executeSkill(int);

	bool isSkillAlbe(int);
	void cutCDTime(int, int);

	DIRECTION getDirection(){ return this->m_direction; }
	void getBuff(int num,float time);
	void getBuff(int num);
protected:
	int m_hp;//目前的血量
	int m_maxHp;//最大的血量

	int m_mp;//目前的魔法值
	int m_maxMp;//最大的魔法值

	int m_dmg;//伤害
	float m_speed = 0.25f;//每移动一格所需的时间（s）

	DIRECTION m_direction = DIRECTION::UP;//朝向

	bool m_isAlive = true;//是否存活
	bool m_isInwin = false;//是否无敌
	float m_inwinTime = 0.3f;//无敌的时间

	bool m_isHero = false;

	STATUS m_status = STATUS::NORMAL;

	std::vector<Skills> m_skills;

	int skill_num = 0;

	Creature * m_target = nullptr;

	GameMap *m_map = nullptr;
};