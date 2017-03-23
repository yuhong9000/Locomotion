#pragma once
class DOF
{
public:
	DOF();
	~DOF();

	//set and get
	void setValue(float);
	void setMin(float);
	void setMax(float);
	float getValue();

private:
	float value, min, max;
};

