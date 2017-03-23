#include "Skeleton.h"


Skeleton::Skeleton()
{
	numjoint = 0;
	local.Identity();
	currentDofIndex = 0;
}


Skeleton::~Skeleton()
{
	delete root;
}

void Skeleton::load(const char* file)
{
	Tokenizer token;
	token.Open(file);
	token.FindToken("balljoint");

	// parse tree
	root = new Joint();
	numjoint = 1;
	root->load(token,numjoint,doflist,jointlist);

	Matrix34 t;
	t.Identity();
	root->update(t);

	// finish
	token.Close();
}

void Skeleton::update()
{
	root->update(local,worldlist);
}

void Skeleton::draw()
{
	root->draw();
}

int Skeleton::numJoint()
{
	return numjoint;
}

void Skeleton::reset()
{
	root->reset();
}

void Skeleton::rootreset()
{
	root->rootreset();
}