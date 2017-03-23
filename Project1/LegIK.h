#pragma once

#include "matrix34.h"
#include "vector3.h"
#include "Joint.h"
#include <math.h>

#define PI 3.14159265358979323846

class LegIK
{
public:
	LegIK();
	virtual ~LegIK();
	void addRoot(Joint*);
	void addHip(Joint*);
	void addKnee(Joint*);
	void update(float,float);
	void reset();
	Vector3 getE(float,float);
	float kneeX;
	float hipX;
	float hipY;

private:
	Joint* root;
	Joint* hip;
	Joint* knee;
	Matrix34 H;
	int state; // 0 = default state; 1 = start; 2 = moving
	Vector3 E;
	float zrear, zfront; // z range of the leg movement
};

