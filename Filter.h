#pragma once
#include "CVHelper.h"

struct Parameter
{
	size_t filterSize;
};

class Filter
{
public:
	Filter() {}
	virtual Frame getFrame() = 0;
	virtual void setFrame(const Frame &frame) = 0;
	virtual void compute() = 0;

	virtual ~Filter() {}
protected:
	Frame _frame;
	Parameter _parameter;
};

