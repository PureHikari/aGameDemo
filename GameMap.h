#pragma once

#include "cocos2d.h"
#include "CommonData.h"
USING_NS_CC;

enum class MapElement
{
	START_POS,
	END_POS,
	MONSTER_POS,
	ITEM_POS,
	BARREL_POS
};

struct mapGrid
{
	int x_pox;
	int y_pos;
	MapElement type;
};

class GameMap:public Node
{
public:
	static GameMap* create(int x,int y);
	virtual bool init(int, int);

	//绘制所有的地图元素
	void paint();

	//生成第零关的地图
	void firstMap();

	//生成下一关的地图
	void anotherMap();

	//添加单独的地图元素
	void addElement(int x, int y, MapElement type);

	//绘制每个单独的地图元素
	void PaintElement(mapGrid mg);

	//返回是否该位置是否能通过
	bool isBarrel(Vec2 pos);

	//添加四周的障碍物
	void addBarrel();

	//获取英雄位置
	Vec2 getHeroPos();
private:
	int m_h;
	int m_w;

	Node * m_temp = nullptr;

	std::vector<mapGrid> m_map;
};