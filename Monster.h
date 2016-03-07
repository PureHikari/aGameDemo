#pragma once
#include "Creature.h"
USING_NS_CC;

enum class MsAiStatus
{
	PETRO,
	ATK,
	RUN_AWAY
};

enum class MsAiSp
{
	NORMAL,
	BLIND,//瞎
	BRARO//不会逃走
};

class Monster:public Creature
{
public:
	static Monster *create(int hp, int dmg);
	virtual bool initWithType(int hp, int dmg);


	void waitForAim(bool);

	void AiCycle();
	void Behavier();
	void getTarget(Creature *target){ this->m_target = target; }
	void initNormalAtk();
	void initSkill();

	DIRECTION whereAreYou();
private:
	int m_exp;//经验
	Sprite* lock = nullptr;

	MsAiStatus m_aiStatus = MsAiStatus::PETRO;
	MsAiSp m_aiSp = MsAiSp::NORMAL;

	bool m_isRunaway = false;
};



