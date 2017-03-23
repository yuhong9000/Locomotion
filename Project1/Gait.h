#pragma once

#include "LegIK.h"
#include "Joint.h"
#include <vector>

class Gait
{
public:
	Gait();
	virtual ~Gait();
	void update(float);
	void move(float);
	void set(std::vector<Joint*>);
	void reset();
	void walk();
	void run();

private:
	int num_leg; // number of legs
	int num_tail; // number of tail joints for use of wasp
	float gait_period;
	float* duty_f; // duty factor for each leg as array
	float* step_t; // step_trigger for each leg as array
	std::vector<LegIK*> legs; 
	std::vector<Joint*> tail;
	int state;
};

