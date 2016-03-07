#pragma once
#include"cocos2d.h"
#include "CommonData.h"
#include "Creature.h"
USING_NS_CC;



class Skill :public Sprite
{
public:
	static Skill *create(Skills,Creature*);
	virtual bool initWithType(Skills,Creature*);
	
	void refresh();
	void execute();
	void flash();
	void hitPay();

	Skills skill;

	Creature *target = nullptr;
	Creature *releaser = nullptr;

	void hit();

	bool isOver = false;
	bool isHit = false;

	int dmg = 0;
};