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
	Mask mask;
};

class Filter
{
public:
	Filter(const Parameter &parameter, const Frame &frame, Log * _log);
	virtual Frame getFrame();
	virtual void setFrame(const Frame &frame);
	virtual void generateNoise(float percent);
	virtual void compute() {};

	virtual ~Filter() {}
protected:
	Frame _frame;
	Parameter _parameter;
	Log * _log;
};

