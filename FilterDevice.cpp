#include "stdafx.h"
#include "FilterDevice.h"
#include <fstream>

FilterDevice::FilterDevice(const Parameter & parameter, const Frame & frame, Log *log)
	: Filter(parameter, frame, log)
{
	_platforms.clear();
	_devices.clear();

	/*Get platfroms*/
	cl::Platform::get(&_platforms);

	/*Get all devices*/
	for (cl::Platform plat : _platforms)
	{
		std::vector<cl::Device> device;
		plat.getDevices(CL_DEVICE_TYPE_ALL, &device);
		_devices.insert(_devices.end(), device.begin(), device.end());
	}
}

void FilterDevice::compute()
{
	const int nRows = _frame.nRows - 2;
	const int nCols = _frame.nCols - 2;
	Frame result(nRows, nCols,
		std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()));

	std::vector<cl::Device> device = { _devices[_activeDevice] };
	cl::Context context(device);
	cl::CommandQueue comqueque(context, device[0]);

	cl::Buffer imageIn = cl::Buffer(context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		(_frame.nRows*_frame.nCols) * sizeof(float), _frame.dataPtr.get());

	cl::Buffer imageOut = cl::Buffer(context,
		CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		(result.nRows*result.nCols) * sizeof(float), result.dataPtr.get());

	std::ifstream sourceFile("kernel.cl");
	std::string sourceCode(std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));
	cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length() + 1));
	cl::Program program = cl::Program(context, source);

	try
	{
		program.build(device);
	}
	catch (const cl::Error &err)
	{
		_log->add("Build error: " + program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device[0]));
	}

	cl::Kernel kernel(program, "nativeFilter3x3");

	kernel.setArg(0, result.nRows);
	kernel.setArg(1, result.nCols);
	kernel.setArg(2, imageIn);
	kernel.setArg(3, imageOut);

	comqueque.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(result.nRows, result.nCols));
	comqueque.finish();


	comqueque.enqueueReadBuffer(imageOut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(float), result.dataPtr.get());

	_frame = result;

}


void FilterDevice::activateDevice(const size_t &numDevice)
{
	_activeDevice = numDevice;
	_log->add("Selected OpenCL device: " + _devices[_activeDevice].getInfo<CL_DEVICE_NAME>());
}

FilterDevice::~FilterDevice()
{
}

std::vector<std::string> FilterDevice::getDevices()
{
	std::vector<std::string> deviceNames(_devices.size());

	for (size_t i = 0; i < _devices.size(); i++)
	{
		deviceNames[i] = _devices[i].getInfo<CL_DEVICE_NAME>();
	}

	return deviceNames;
}
