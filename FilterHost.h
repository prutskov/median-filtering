#pragma once
#include "Filter.h"

class FilterHost :	public Filter
{
public:
	FilterHost(Log *log);
	void compute() override;
	void setParameter(Parameter parameter);
	virtual ~FilterHost();
private:
	void quickSort(float* data, int size);
	void compute3x3();
	void compute5x5();
	Parameter _parameter;
};

