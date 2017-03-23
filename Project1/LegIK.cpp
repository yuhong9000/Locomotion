#include "LegIK.h"


LegIK::LegIK()
{
	H.Identity();
	state = 0;
	zrear = 0.05;
	zfront = -0.35;
}


LegIK::~LegIK()
{
}

void LegIK::addHip(Joint* h)
{
	hip = h;
}

void LegIK::addKnee(Joint* k)
{
	knee = k;
}

void LegIK::addRoot(Joint* r)
{
	root = r;
}

void LegIK::update(float p, float d)
{
	Matrix34* t = new Matrix34();
	E = knee->dendw + getE(p, d);
	
	t->Identity();
	t->d = hip->offset;

	H.Dot(*(hip->parentW), *t);
	H.Inverse();

	// calculate goal in hip space
	Vector3 g;
	H.Transform(E, g);

	// rotate knee around x axis /////////////////
	// rotate hip around x axis then y axis //////
	float th = acosf((powf(hip->length, 2) + powf(knee->length, 2) - powf(g.Mag(),2)) / (2 * hip->length*knee->length));
	kneeX = th - PI;
	hipX = asinf(knee->length*sinf(th) / g.Mag()) + asinf(g.y/g.Mag())+PI/2;
	if (g.z > 0){
		hipY = atanf(g.x / g.z) - PI;
	}
	else{
		hipY = atanf(g.x / g.z);
	}
	

	knee->xdof.setValue(kneeX);
	knee->ydof.setValue(0);
	knee->zdof.setValue(0);

	hip->xdof.setValue(hipX);
	hip->ydof.setValue(hipY);
	hip->zdof.setValue(0);
	delete t;
}

// get end position vector of the leg based on step phase and duty factor //
Vector3 LegIK::getE(float p,float d)
{
	Vector3 end;
	float e;
	if (p <= (1 - d)){
		e = zrear - (zrear - zfront ) * p / (1 - d);
	}
	else{
		e = zfront + (zrear - zfront) * (p - (1 - d)) / d;
	}
	end = root->local.c;
	end.Normalize();
	end.x *= e;
	end.y *= e;
	end.z *= e;

	return end;
}

void LegIK::reset()
{
	hip->reset();
}
