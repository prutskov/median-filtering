#pragma once
#include "Filter.h"

class FilterHost :	public Filter
{
public:
	FilterHost();
	FilterHost(const Parameter &parameter, const Frame &frame);
	virtual Frame getFrame();
	virtual void setFrame(const Frame &frame);
	virtual void compute();
	virtual ~FilterHost();
private:
	void quickSort(float* data, int size);
	float medianGet(int xCenterMask, int yCenterMask, const Frame& frame);
};

