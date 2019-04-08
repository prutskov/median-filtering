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
	void compute(bool isLogging) override;
	void setParameter(ParameterDevice parameter);
	/*Get vector of available devices*/
	std::vector<std::string> getDevices();
	virtual ~FilterDevice();
private:
	void programBuild();
	void compute3x3(bool isLogging);
	void compute5x5(bool isLogging);
private:
	std::vector<cl::Platform> _platforms;
	std::vector<cl::Device> _devices;
	cl::Program _program;
	cl::Context _context;
	ParameterDevice _parameter;
};

