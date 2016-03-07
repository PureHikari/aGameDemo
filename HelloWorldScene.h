#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "CommonData.h"
#include "Hero.h"
#include "Monster.h"
#include "Skill.h"
USING_NS_CC;

enum MapType
{
	NOTHING,
	START,
	STEP,
	MONSTER,
	ITEM,
	END
};

class HelloWorld : public Layer
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorld);

	Vec2 getLoc(int x,int y);
	Vec2 getP(Vec2 pos);

	bool onBegan(Touch *touch, Event* event);
	void onEnd(Touch *touch, Event* event);
	void first(EventKeyboard::KeyCode keyCode, Event* event);
	void second(EventKeyboard::KeyCode keyCode, Event* event);
	void update(float);

	void checkCoolDown();
	void initUI();
	void createEnemy(float);

	void intro();
private:
	//MapType m_grid[RAW][COL];
	//Sprite * m_bg = nullptr;
	//Node * m_node = nullptr;
	Hero * m_hero =nullptr;

	GameMap *m_map = nullptr;

	//怪物的集合
	Vector<Monster *> monsters;
	//需要清理的怪物集合
	Vector<Monster *> removeable_ms;
	//技能集合
	Vector<Skill *> bullets;
	//需要清除的技能集合
	Vector<Skill *> removeable_bu;

	//关卡数
	int m_level = -1;

	Sprite * m_end = nullptr;

	float m_speed = 0.3f;
	PhysicsWorld * m_world = nullptr;

	bool m_isGameStop = false;

	Node* m_ui = nullptr;

	Vec2 firstTouchPos = Vec2(GRID * 3, GRID * 3);

	Sprite* power_up = nullptr;
	Sprite* heal_up = nullptr;
	Sprite* speed_up = nullptr;

	Label* skill_ori = nullptr;
	Label* skill_q = nullptr;
	Label* skill_e = nullptr;
	Label* skill_r = nullptr;
	Label* skill_f = nullptr;
};

#endif // __HELLOWORLD_SCENE_H__
