#pragma once
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define __CL_ENABLE_EXCEPTIONS
#include "Filter.h"
#include <CL/cl.hpp>
#include <vector>
#include <string>
#include "chrono"

class FilterDevice : public Filter
{
public:
	FilterDevice(Log *log);
	float compute() override;
	void setParameter(Parameter parameter) override;
	std::vector<std::string> getDevices();
	virtual ~FilterDevice();
private:
	void programBuild();
	float compute3x3();
	float compute5x5();
private:
	std::vector<cl::Platform> _platforms;
	std::vector<cl::Device> _devices;
	cl::Program _program;
	cl::Context _context;
};

