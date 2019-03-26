#pragma once
#include "CVHelper.h"

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
	Filter();
	Filter(const Parameter &parameter, const Frame &frame);
	virtual Frame getFrame();
	virtual void setFrame(const Frame &frame);
	virtual void generateNoise(float percent);
	virtual void compute() = 0;

	virtual ~Filter() {}
protected:
	Frame _frame;
	Parameter _parameter;
};

