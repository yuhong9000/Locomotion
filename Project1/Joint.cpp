#include "Joint.h"
#define M_PI 3.14159265358979323846
Joint::Joint()
{
	local.Identity();
	world.Identity();
	pose.Set(0, 0, 0);

}


Joint::~Joint()
{
	for (std::list<Joint*>::iterator i = jointlist.begin(); i != jointlist.end(); i++)
	{
		delete *i;
	}
}
////////////////////////////////////////////////////////////////////////////////

void Joint::update(Matrix34& m)
{
	Matrix34* t1 = new Matrix34();
	local.Identity();

	// apply rotations xyz //////////////////////
	t1->MakeRotateX(xdof.getValue());
	local.Dot(*t1, local);

	t1->MakeRotateY(ydof.getValue());
	local.Dot(*t1, local);

	t1->MakeRotateZ(zdof.getValue());
	local.Dot(*t1, local);

	// apply offset /////////////////////////////
	local.d = offset;

	// update parent world matrix   /////////////
	// for use of IK                /////////////
	parentW = &m;

	world.Dot(m, local);

	// update joint end point in default local world space ////
	world.Transform(dendl, dendw);
	
	// update jointlist /////////////////////////
	for (std::list<Joint*>::iterator i = jointlist.begin(); i != jointlist.end(); i++)
	{
		(*i)->update(world);
	}

	delete t1;
}
////////////////////////////////////////////////////////////////////////////////

void Joint::addChild(Joint& j)
{
	jointlist.push_back(&j);
}

void Joint::removeChild(Joint& j)
{
	jointlist.remove(&j);
}

////////////////////////////////////////////////////////////////////////////////

void Joint::update(Matrix34& m,vector<Matrix34*>& w)
{
	Matrix34* t1 = new Matrix34();
	local.Identity();

	// apply rotations xyz //////////////////////
	t1->MakeRotateX(xdof.getValue());
	local.Dot(*t1,local);
	
	t1->MakeRotateY(ydof.getValue());
	local.Dot(*t1,local);
	
	t1->MakeRotateZ(zdof.getValue());
	local.Dot(*t1,local);

	// apply offset /////////////////////////////
	local.d = offset;

	// update parent world matrix   /////////////
	// for use of IK                /////////////
	parentW = &m;

	// update world matrix //////////////////////
	world.Dot(m, local);

	// update list of world matrix  /////////////
	// for use of skin              /////////////
	w.push_back(&world);
	
	// update joint end point in default local world space ////
	world.Transform(dendl, dendw);

	// update jointlist /////////////////////////
	for (std::list<Joint*>::iterator i = jointlist.begin(); i != jointlist.end(); i++)
	{
		(*i)->update(world,w);
	}

	delete t1;
}

////////////////////////////////////////////////////////////////////////////////

void Joint::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(world);
	drawSolidBox(boxmin.x, boxmin.y, boxmin.z, boxmax.x, boxmax.y, boxmax.z);
	/*
	if (strstr(name, "hip") != NULL) {
		drawSolidBox(boxmin.x, boxmin.y, boxmin.z, boxmax.x, boxmax.y, boxmax.z);
	}

	if (strstr(name, "knee") != NULL) {
		drawSolidBox(boxmin.x, boxmin.y, boxmin.z, boxmax.x, boxmax.y, boxmax.z);
		// contains
		Matrix34* t1 = new Matrix34();
		t1->Identity();
		glLoadMatrixf(*t1);
		drawSolidSphere(dendw.x, dendw.y, dendw.z);
		//drawSolidSphere(dendw.x, dendw.y, dendw.z + 0.05);
		//drawSolidSphere(dendw.x, dendw.y, dendw.z - 0.35);
		delete t1;
	}
	*/
	for (std::list<Joint*>::iterator i = jointlist.begin(); i != jointlist.end(); i++)
	{
		(*i)->draw();
	}
}

////////////////////////////////////////////////////////////////////////////////

void Joint::load(Tokenizer& token, int& num, vector<DOF*>& d, vector<Joint*>& j)
{
	char tmp[256];
	
	token.GetToken(tmp);
	strcpy(name, tmp);
	token.FindToken("{");
	j.push_back(this);

	while (1){
		token.GetToken(tmp);
		if (strcmp(tmp, "offset") == 0)
		{
			offset.x = token.GetFloat();
			offset.y = token.GetFloat();
			offset.z = token.GetFloat();
		}
		else if (strcmp(tmp, "boxmin") == 0){
			boxmin.x = token.GetFloat();
			boxmin.y = token.GetFloat();
			boxmin.z = token.GetFloat();
		}
		else if (strcmp(tmp, "boxmax") == 0){
			boxmax.x = token.GetFloat();
			boxmax.y = token.GetFloat();
			boxmax.z = token.GetFloat();
		}
		else if (strcmp(tmp, "rotxlimit") == 0){
			xdof.setMin(token.GetFloat());
			xdof.setMax(token.GetFloat());
			doflist.push_back(&xdof);
		}
		else if (strcmp(tmp, "rotylimit") == 0){
			ydof.setMin(token.GetFloat());
			ydof.setMax(token.GetFloat());
			doflist.push_back(&ydof);
		}
		else if (strcmp(tmp, "rotzlimit") == 0){
			zdof.setMin(token.GetFloat());
			zdof.setMax(token.GetFloat());
			doflist.push_back(&zdof);
		}
		else if (strcmp(tmp, "pose") == 0){
			pose.x = token.GetFloat();
			pose.y = token.GetFloat();
			pose.z = token.GetFloat();
		}
		else if (strcmp(tmp, "balljoint") == 0){
			Joint* joint = new Joint();
			
			num++; // increase total joint for the skeleton
			
			this->addChild(*joint);
			joint->load(token,num,d,j);
		}
		else if (strcmp(tmp, "}") == 0)
		{
			xdof.setValue(pose.x);
			ydof.setValue(pose.y);
			zdof.setValue(pose.z);
			
			// d keeps a list of all dof in the skeleton ///////
			d.push_back(&xdof);
			d.push_back(&ydof);
			d.push_back(&zdof);

			// calculate joint length //////////////////////////
			length = 0 - boxmin.y;

			// set joint end point /////////////////////////////
			dendl.Set(0,boxmin.y,0);
			return;
		}
		else{
			cout << "GG" << endl;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void Joint::reset()
{
	xdof.setValue(pose.x);
	ydof.setValue(pose.y);
	zdof.setValue(pose.z);
	for (std::list<Joint*>::iterator i = jointlist.begin(); i != jointlist.end(); i++)
	{
		(*i)->reset();
	}
}

////////////////////////////////////////////////////////////////////////////////
// root reset wont change root /////////////////////////////////////////////////
void Joint::rootreset()
{
	for (std::list<Joint*>::iterator i = jointlist.begin(); i != jointlist.end(); i++)
	{
		(*i)->reset();
	}
}