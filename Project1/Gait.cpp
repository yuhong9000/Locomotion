#include "Gait.h"


Gait::Gait()
{
	num_leg = 6;
	num_tail = 4;
	gait_period = 7;
	duty_f = new float[num_leg];
	step_t = new float[num_leg];

	for (int i = 0; i < num_leg; i++)
	{
		legs.push_back(new LegIK());
	}

}


Gait::~Gait()
{
	delete[] duty_f;
	delete[] step_t;
	for (int i = 0; i < num_leg; i++)
	{
		delete legs[i];
	}
}

void Gait::set(vector<Joint*> jlist)
{
	legs[0]->addHip(jlist[7]);
	legs[0]->addKnee(jlist[8]);
	legs[1]->addHip(jlist[9]);
	legs[1]->addKnee(jlist[10]);
	legs[2]->addHip(jlist[11]);
	legs[2]->addKnee(jlist[12]);
	legs[3]->addHip(jlist[15]);
	legs[3]->addKnee(jlist[16]);
	legs[4]->addHip(jlist[17]);
	legs[4]->addKnee(jlist[18]);
	legs[5]->addHip(jlist[19]);
	legs[5]->addKnee(jlist[20]);

	walk();

	for (int i = 0; i < 6; i++)
	{
		legs[i]->addRoot(jlist[0]);
		duty_f[i] = 0.7;
	}

	tail.push_back(jlist[3]);
	tail.push_back(jlist[4]);
	tail.push_back(jlist[5]);
	tail.push_back(jlist[6]);
}

void Gait::update(float t)
{
	float p = fmod(t, gait_period);

	p = p / gait_period;

	// update leg movement using analytical IK ///
	for (int i = 0; i < num_leg; i++)
	{
		float step_p; // step phase

		// calculate step phase from gait phase
		if (p >= step_t[i]){
			step_p = p - step_t[i];
		}
		else{
			step_p = p + 1 - step_t[i];
		}
		if (1){
			legs[i]->update(step_p, duty_f[i]);
		}
	}
	// end of leg movement update ////////////////
	// update tail movment ///////////////////////

	for (int i = 0; i < num_tail; i++){
		float r;
		if (p < 0.25){
			r = p;
		}
		else if (p < 0.75){
			r = 0.5 - p;
		}
		else {
			r = p - 1;
		}
		r = r / 4;
		tail[i]->ydof.setValue(r);
	}
	// end of tail movement update ///////////////
}

void Gait::reset()
{
	for (int i = 0; i < num_leg; i++)
	{
		legs[i]->reset();
	}
}

void Gait::walk()
{
	step_t[0] = 0.5;
	step_t[1] = 0.8;
	step_t[2] = 0.1;
	step_t[3] = 0.0;
	step_t[4] = 0.3;
	step_t[5] = 0.6;

	gait_period = 7;
}

void Gait::run()
{
	step_t[0] = 0.5;
	step_t[1] = 0.0;
	step_t[2] = 0.5;
	step_t[3] = 0.0;
	step_t[4] = 0.5;
	step_t[5] = 0.0;

	gait_period = 4;
}
