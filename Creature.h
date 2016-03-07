#pragma once
#include "cocos2d.h"
#include "CommonData.h"
#include "GameMap.h"

USING_NS_CC;

//��ɫ�ķ���
enum class DIRECTION
{
	UP,
	LEFT,
	DOWN,
	RIGHT
};

//��ɫ��״̬
enum class STATUS
{
	NORMAL,//����
	SKILL_PRE,//׼���ͷż���
	AIM_DIR,//��׼
	AIM_TAR,
	AIM_POS,
	SKILL_EXE,//�����ͷż���
	SKILL_DELARE
};

//ö�ټ�����������Ч��
enum class SkillEffect
{
	NONE,//��Ч��
	REPEL,//����
	SLOW,//����
	OVERKILL//ʹ��Ѫ
};

enum class SkillAimType
{
	DIRE_ATK,//��׼����
	POS_ATK,//��׼λ��
	TAR_ATK,//��׼Ŀ��
	SELF_POS,
	SELF_EXE//�����ͷ�
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
	int dmg;//������ɵ��˺�
	int reque_mp;//���ܵ�����
	int reque_hp;
	SkillEffect effect;//������ɵ�Ч��
	int effectNum;//�������Ч���ĳ̶�

	SkillType type=SkillType::ATK;
	SkillAimType Aim_type;//���ܵ�����

	SkillPay pay;
	SkillPayTime pay_time = SkillPayTime::EVERYTIME;
	int pay_num;

	Vec2 pos;//�����ͷŵ�λ��

	int cd;//���ܵ���ȴ
	int cdTime;//���ܵ���ȴʵʱ��ʱ

	bool isHero;//�ͷ����Ƿ���Ӣ��
	bool isAble;

	float Exist_time;
	float Delay_time;//ʩ����ҡ

	bool is180;
};


class Creature:public Sprite
{
public:
	static Creature *create();
	virtual bool initWithType();

	void restoreHp(int);//�ָ�����ֵ
	void restoreMp(int);//�ָ�ħ��ֵ
	void refresh();//ˢ�¸���cd
	void hit(Skills,int);//�ܵ�����
	void hit(int);
	void dead();//����
	
	void setDirection(DIRECTION dr);//�趨����
	
	//��ȡ�Ƿ���
	bool isAlive(){ return this->m_isAlive; }
	//��ȡ�Ƿ��޵�
	bool isInwin(){ return this->m_isInwin; } 

	//Skill *normalAtk(Vec2);//��ͨ����

	int getHp(){ return this->m_hp; }
	int getMp(){ return this->m_mp; }
	int getDmg(){ return this->m_dmg; }

	void repel(Vec2,int);//����Ч����ʵ��
	void slow(int);//����Ч����ʵ��
	void bleed(int);//��ѪЧ����ʵ��

	bool isHero(){ return this->m_isHero; }

	Skills skillAtk();//ʹ�ü���

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
	int m_hp;//Ŀǰ��Ѫ��
	int m_maxHp;//����Ѫ��

	int m_mp;//Ŀǰ��ħ��ֵ
	int m_maxMp;//����ħ��ֵ

	int m_dmg;//�˺�
	float m_speed = 0.25f;//ÿ�ƶ�һ�������ʱ�䣨s��

	DIRECTION m_direction = DIRECTION::UP;//����

	bool m_isAlive = true;//�Ƿ���
	bool m_isInwin = false;//�Ƿ��޵�
	float m_inwinTime = 0.3f;//�޵е�ʱ��

	bool m_isHero = false;

	STATUS m_status = STATUS::NORMAL;

	std::vector<Skills> m_skills;

	int skill_num = 0;

	Creature * m_target = nullptr;

	GameMap *m_map = nullptr;
};