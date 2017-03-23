#pragma once

#include "Skeleton.h"
#include "Gait.h"

class Wasp
{
public:
	Wasp();
	~Wasp();
	void setSkel(Skeleton*);
	void update(float t);
	void draw();
	void run();
	void walk();
	void turn(float angle);
	void move(float dist);

private:
	Gait* gait;
	Skeleton* skel;
};

