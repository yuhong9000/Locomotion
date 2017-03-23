#include "Wasp.h"


Wasp::Wasp()
{
	gait = new Gait();
}


Wasp::~Wasp()
{
	delete gait;
}

void Wasp::setSkel(Skeleton* s)
{
	skel = s;
	gait->set(s->jointlist);
}

void Wasp::update(float t)
{
	skel->rootreset();
	skel->update();
	gait->update(t);
	skel->update();
}

void Wasp::draw()
{
	//skel->update();
	skel->draw();
}

void Wasp::run()
{
	gait->run();
}

void Wasp::walk()
{
	gait->walk();
}

void Wasp::turn(float angle)
{
	float d = skel->jointlist[0]->ydof.getValue();
	skel->jointlist[0]->ydof.setValue(d + angle);
}

void Wasp::move(float dist)
{
	Vector3 end;
	

	end = skel->jointlist[0]->local.c;
	end.Normalize();
	end.x *= dist;
	end.y *= dist;
	end.z *= dist;

	skel->jointlist[0]->offset += end;
}