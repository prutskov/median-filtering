#pragma once
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include "Filter.h"
#include <CL/cl.hpp>
#include <vector>

class FilterDevice : public Filter
{
public:
	FilterDevice(const Parameter &parameter, const Frame &frame, Log *log);
	void compute() override;
	virtual ~FilterDevice();
private:
	std::vector<cl::Platform> _platforms;
	std::vector<cl::Device> _devices;
};

