#pragma once
#include "CVHelper.h"

struct Parameter
{

};
class Filter
{
public:
	Filter();
	virtual Frame getFrame() = 0;
	virtual void setFrame() = 0;
	virtual void compute() = 0;

	virtual ~Filter();
protected:
	Frame _frame;
};

