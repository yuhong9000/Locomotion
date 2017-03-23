#pragma once

#include "matrix34.h"
#include "DOF.h"
#include "token.h"
#include "vector3.h"
#include "core.h"

#include <iostream>
#include <list>
#include <vector>
#include <GL/glut.h>

using namespace std;
class Joint
{
public:
	Joint();
	~Joint();
	void addChild(Joint&);
	void removeChild(Joint&);
	void update(Matrix34&);
	void update(Matrix34&,vector<Matrix34*>&);
	void draw();
	void load(Tokenizer&);
	void load(Tokenizer&, int&, vector<DOF*>&, vector<Joint*>&); // load from token, and keep a list of all dof in the skeleton
	void reset();
	void rootreset();

	float length;	// length of the joint/bone
	Matrix34* parentW; // pointer to parent world matrix
	Vector3 offset; // joint offset
	Vector3 dendw; // default end point in world space

	DOF xdof, ydof, zdof;
	char name[256]; // name of the joint
	Vector3 dendl; // default end point in local space

	Matrix34 local;
	Matrix34 world;

private:
	list <Joint*> jointlist;
	list <DOF*> doflist;

	Matrix34 dworld; // default world matrix
	Vector3 pose;
	Vector3 boxmin;
	Vector3 boxmax;
	
	

};

