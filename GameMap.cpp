#include "GameMap.h"

GameMap * GameMap::create(int x, int y)
{
	auto gameMap = new GameMap;
	if (gameMap&&gameMap->init(x, y))
	{
		gameMap->autorelease();
		return gameMap;
	}
	CC_SAFE_DELETE(gameMap);
	return nullptr;
}

void GameMap::addBarrel()
{
	for (int i = 0; i < this->m_w; i++)
	{
		if (i == 0 || i == this->m_w - 1)
		{
			for (int j = 0; j < this->m_h; j++)
			{
				this->addElement(i, j, MapElement::BARREL_POS);
			}
		}
		else
		{
			this->addElement(i, 0, MapElement::BARREL_POS);
			this->addElement(i, this->m_h - 1, MapElement::BARREL_POS);
		}
	}
}

bool GameMap::init(int x, int y)
{
	do
	{
		this->m_w = x;
		this->m_h = y;

		//this->m_map.resize(x*y);
		this->m_temp = Node::create();
		this->m_temp->setPosition(0, 0);
		this->addChild(m_temp,1);

		//添加背景和四周的障碍
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				auto sp = Sprite::create("BGRID.png");
				sp->setPosition(i*GRID + GRID / 2, j*GRID + GRID / 2);
				this->addChild(sp, Z_BACK_GROUD);
			}
		}

		this->addBarrel();
		//this->firstMap();

	} while (0);

	return true;
}

void GameMap::PaintElement(mapGrid mg)
{
	auto sp = Sprite::create();

	switch (mg.type)
	{
		case MapElement::START_POS:
		{
			/*sp->initWithFile("closeNormal.png");
			this->m_temp->addChild(sp, Z_START_N_END);*/
		}
		break;
		case MapElement::END_POS:
		{
			sp->initWithFile("closeNormal.png");
			this->m_temp->addChild(sp, Z_START_N_END);
		}
		break;
		case MapElement::MONSTER_POS:
		{
			sp->initWithFile("closeNormal.png");
			this->m_temp->addChild(sp, Z_MONSTER);
		}
		break;
		case MapElement::ITEM_POS:
		{
			sp->initWithFile("closeNormal.png");
			this->m_temp->addChild(sp, Z_BARREL);
		}
		break;
		case MapElement::BARREL_POS:
		{
			sp->initWithFile("step.png");
			this->addChild(sp, Z_BARREL);
		}
			break;
	}

	sp->setPosition(Vec2(mg.x_pox*GRID + GRID / 2, mg.y_pos*GRID + GRID / 2));
}

void GameMap::paint()
{
	for (auto el : this->m_map)
	{
		this->PaintElement(el);
	}
}

void GameMap::addElement(int x, int y, MapElement type)
{
	mapGrid temp;
	temp.x_pox = x;
	temp.y_pos = y;
	temp.type = type;
	this->m_map.push_back(temp);

	this->PaintElement(temp);
}

bool GameMap::isBarrel(Vec2 pos)
{
	int x = pos.x / GRID;
	int y = pos.y / GRID;

	for (auto el : m_map)
	{
		if (el.x_pox == x&&el.y_pos == y)
		{
			if (el.type == MapElement::BARREL_POS)
				return true;
		}
	}
	return false;
}

void GameMap::firstMap()
{
	//按顺序添加地图元素
	this->addBarrel();

	this->addElement(16, 9, MapElement::START_POS);
	//绘制
	this->paint();
}

void GameMap::anotherMap()
{
	//清除原有的地图元素
	this->m_map.clear();
	this->m_temp->removeAllChildren();
	//随机生成地图元素

	this->addElement(16, 9, MapElement::START_POS);

	//to do


	//绘制
	this->addBarrel();
	this->paint();
}

Vec2 GameMap::getHeroPos()
{
	for (auto el : m_map)
	{
		if (el.type == MapElement::START_POS)
		{
			return Vec2(el.x_pox*GRID + GRID / 2, el.y_pos*GRID + GRID / 2);
		}
	}

	return Vec2(0, 0);
}