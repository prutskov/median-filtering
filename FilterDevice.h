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
	FilterDevice(const Parameter &parameter, const Frame &frame, Log *log);
	void compute() override;

	/*Set device with numDevice number as active for computing*/
	void activateDevice(const size_t &numDevice);
	/*Get vector of available devices*/
	std::vector<std::string> getDevices();
	virtual ~FilterDevice();
private:
	void programBuild();
private:
	std::vector<cl::Platform> _platforms;
	std::vector<cl::Device> _devices;
	size_t _activeDevice = 0;
	cl::Program _program;
	cl::Context _context;
};

