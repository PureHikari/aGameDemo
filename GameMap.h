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

	//�������еĵ�ͼԪ��
	void paint();

	//���ɵ���صĵ�ͼ
	void firstMap();

	//������һ�صĵ�ͼ
	void anotherMap();

	//��ӵ����ĵ�ͼԪ��
	void addElement(int x, int y, MapElement type);

	//����ÿ�������ĵ�ͼԪ��
	void PaintElement(mapGrid mg);

	//�����Ƿ��λ���Ƿ���ͨ��
	bool isBarrel(Vec2 pos);

	//������ܵ��ϰ���
	void addBarrel();

	//��ȡӢ��λ��
	Vec2 getHeroPos();
private:
	int m_h;
	int m_w;

	Node * m_temp = nullptr;

	std::vector<mapGrid> m_map;
};