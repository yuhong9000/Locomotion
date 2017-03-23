#pragma once

#include <list>
#include <GL/glut.h>

#include "Joint.h"
#include <vector>
#include "matrix34.h"

using namespace std;

class Skeleton
{
public:
	Skeleton();
	~Skeleton();
	void load(const char* file);
	void update();
	void draw();
	int numJoint();
	void reset();
	void rootreset();

	vector<Matrix34*> worldlist; // list of world matrices used for skin class
	vector<Joint*> jointlist;
	vector<DOF*> doflist;
	DOF* currentDof;
	int currentDofIndex;

private:
	Joint* root;
	Matrix34 local;
	
	int numjoint; // number of joints in this skeleton
};

