#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#define MAX_PLAYERS 32

struct Vector3
{

	float fX, fY, fZ;

	Vector3()
	{
		this->fX = 0;
		this->fY = 0;
		this->fZ = 0;
	};

	Vector3(float fX, float fY, float fZ)
	{
		this->fX = fX;
		this->fY = fY;
		this->fZ = fZ;
	}


	float operator[] (const size_t n) const
	{
		switch (n)
		{
		case 0: return fX;
		case 1: return fY;
		case 2: return fZ;
		}
		return 0; //Error
	}

	Vector3 operator / (float fRight) const
	{
		return Vector3(fX / fRight, fY / fRight, fZ / fRight);
	}
};


struct Vector2
{
	float fX, fY;

	Vector2()
	{
		this->fX = 0;
		this->fY = 0;
	};

	Vector2(float fX, float fY)
	{
		this->fX = fX;
		this->fY = fY;
	}
};


class Player;

class EnemyList
{
public:
	Player** v_enemyList;
};

class Player
{
private:
	char pad_0x0000[0x34]; //0x0000
public:
	Vector3 v_Position; //0x0034
	Vector2 v_ViewAngle; //0x0040
private:
	char pad_0x0048[0xB0]; //0x0048
	__int32 i_Health; //0x00F8 
	__int32 i_Armor; //0x00FC 
	char pad_0x0100[0x125]; //0x0100
	char s_name[0x10]; //0x225
	char pad_0x0235[0xF7]; //0x0235
	char b_Team; //0x032C


public:

	void SetHealth(int h)	{ this->i_Health = h; }
	void SetArmor(int a)	{ this->i_Armor = a; }


	int GetHealth()	const	{ return i_Health; }
	int GetArmor() const	{ return i_Armor; }

	const char* GetName() const
	{
		return this->s_name;
	}


	bool SameTeamAs(Player* pOther)
	{
		return (this->b_Team == pOther->b_Team);
	}

	bool isDead()
	{
		return (this->i_Health <= 0 || this->i_Health > 100);
	}

};
