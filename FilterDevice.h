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
	void compute() override;
	void setParameter(ParameterDevice parameter);
	/*Get vector of available devices*/
	std::vector<std::string> getDevices();
	virtual ~FilterDevice();
private:
	void programBuild();
	void compute3x3();
	void compute5x5();
private:
	std::vector<cl::Platform> _platforms;
	std::vector<cl::Device> _devices;
	cl::Program _program;
	cl::Context _context;
	ParameterDevice _parameter;
};

