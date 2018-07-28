#pragma once

#include "Player.h"
#include <math.h>


struct glmatrixf
{
	float v[16];

	/*float operator[](int i) const { return v[i]; }
	float &operator[](int i) { return v[i]; }*/


	float transformx(const Vector3 &p) const
	{
		return p.fX*v[0] + p.fY*v[4] + p.fZ*v[8] + v[12];
	}

	float transformy(const Vector3 &p) const
	{
		return p.fX*v[1] + p.fY*v[5] + p.fZ*v[9] + v[13];
	}

	float transformz(const Vector3 &p) const
	{
		return p.fX*v[2] + p.fY*v[6] + p.fZ*v[10] + v[14];
	}

	float transformw(const Vector3 &p) const
	{
		return p.fX*v[3] + p.fY*v[7] + p.fZ*v[11] + v[15];
	}
};


class Game 
{
private:
	void GetResolution(int& h, int& v) const
	{
		RECT rWindow;
		HWND hWindow = FindWindowA(NULL, "AssaltCube");//GetForegroundWindow();
		GetWindowRect(hWindow, &rWindow);
		h = rWindow.right;
		v = rWindow.bottom;
	}

	int iLenX_, iLenY_;

	glmatrixf* matrix;
	
	int* numEnemies;

public:

	Game() {}

	Game(DWORD d_viewMatrix, DWORD d_numberOfEnemies) {
		this->matrix = (glmatrixf*)d_viewMatrix;
		this->numEnemies = (int*)d_numberOfEnemies;
	}

	int iLenX() const {
		return iLenX_;
	}
	int iLenY() const {
		return iLenY_;
	}

	int GetNumberOfEnemies() {
		return *this->numEnemies;
	}


	bool WorldToScreen(const Vector3 &vecWorld, Vector3* vecScreen)
	{
		GetResolution(iLenX_, iLenY_);

		if (matrix->transformw(vecWorld) < 0.0f)
		{
			return false;
		}

		*vecScreen = Vector3( matrix->transformx(vecWorld), matrix->transformy(vecWorld), matrix->transformz(vecWorld) )  /  matrix->transformw(vecWorld);

		vecScreen->fX *= (float)iLenX_ / 2.0f;
		vecScreen->fY *= -(float)iLenY_ / 2.0f;
		vecScreen->fX += (float)iLenX_ / 2.0f;
		vecScreen->fY += (float)iLenY_ / 2.0f;

		if (vecScreen->fZ < 0.f)
			return false;

		return true;
	}


	float Get3dDistance(Vector3 to, Vector3 from)
	{
		return (float)
			(sqrt(
			((to.fX - from.fX) * (to.fX - from.fX)) +
			((to.fY - from.fY) * (to.fY - from.fY)) +
			((to.fZ - from.fZ) * (to.fZ - from.fZ))
			));
	}

};