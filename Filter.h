#pragma once
#include "CVHelper.h"
#include "Log.h"

enum Mask
{
	MASK3X3,
	MASK5X5
};

struct Parameter
{
	Mask mask = Mask::MASK3X3;
	size_t activeDevice = 0;
};

class Filter
{
public:
	Filter(Log *log);
	virtual Frame getFrame();
	virtual void setFrame(const Frame &frame);
	virtual void setParameter(Parameter parameter);
	virtual void generateNoise(float percent);
	virtual float compute() 
	{
		return 0;
	};
	virtual std::vector<std::string> getDevices();

	virtual ~Filter() {}
protected:
	Frame _frame;
	Log * _log;
	Parameter _parameter;
};

