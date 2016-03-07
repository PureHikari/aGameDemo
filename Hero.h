#pragma once;
#include "Creature.h"

class Hero :public Creature
{
public:
	static Hero*create();
	virtual bool init();

	void control(EventKeyboard::KeyCode,bool);
	void click(Vec2);

	void initNormalAtk();

	void initSkill1();
	void initSkill2();

	void getSkill(int);

	int getCDTime(int);
	void getPower(int, int);
private:
	//std::vector<Skills> m_skills;

	bool m_isAtk = false;
};