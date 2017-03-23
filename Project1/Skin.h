#pragma once

#include "Joint.h"
#include "Skeleton.h"
#include "matrix34.h"
#include "DOF.h"
#include "token.h"
#include "vector3.h"
#include "core.h"

#include <GL/glut.h>

class Skin
{
public:
	Skin();
	~Skin();
	void load(const char* file);
	void draw();
	void update(); 
	void setSkel(Skeleton*);

	

private:
	int numvertex; // total number of vertices in skin
	int numjoint;  // total number of joints
	int numtriangle; // total number of triangles
	Vector3* vertex; // vertex raw data
	Vector3* normal; // normal raw data
	Vector3* dvertex; // vertex for draw
	Vector3* dnormal; // normal for draw
	Matrix34* binding;
	int* triangle;
	float* skinweight;
	Skeleton* skel;
};

