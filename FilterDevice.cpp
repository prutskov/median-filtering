#include "stdafx.h"
#include "FilterDevice.h"
#include <fstream>

FilterDevice::FilterDevice(Log * log) : Filter(log)
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

float FilterDevice::compute()
{
	if (_parameter.mask == Mask::MASK3X3)
	{
		return compute3x3();
	}
	else
	{
		return compute5x5();
	}
}

void FilterDevice::setParameter(Parameter parameter)
{
	_parameter = parameter;
	programBuild();
}

float FilterDevice::compute3x3()
{
	Frame result = _frame.clone();
	const int nRows = result.nRows;
	const int nCols = result.nCols;

	cl::CommandQueue comqueque(_context, _context.getInfo<CL_CONTEXT_DEVICES>()[0]);

	auto start = std::chrono::high_resolution_clock::now();
	cl::Buffer imageRIn = cl::Buffer(_context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		(_frame.nRows*_frame.nCols) * sizeof(uchar), _frame.dataRPtr.get());

	cl::Buffer imageGIn = cl::Buffer(_context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		(_frame.nRows*_frame.nCols) * sizeof(uchar), _frame.dataGPtr.get());

	cl::Buffer imageBIn = cl::Buffer(_context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		(_frame.nRows*_frame.nCols) * sizeof(uchar), _frame.dataBPtr.get());

	cl::Buffer imageROut = cl::Buffer(_context,
		CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		(result.nRows*result.nCols) * sizeof(uchar), result.dataRPtr.get());

	cl::Buffer imageGOut = cl::Buffer(_context,
		CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		(result.nRows*result.nCols) * sizeof(uchar), result.dataGPtr.get());

	cl::Buffer imageBOut = cl::Buffer(_context,
		CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		(result.nRows*result.nCols) * sizeof(uchar), result.dataGPtr.get());

	cl::Kernel kernel(_program, "nativeFilter3x3");

	kernel.setArg(0, result.nRows);
	kernel.setArg(1, result.nCols);
	kernel.setArg(2, imageRIn);
	kernel.setArg(3, imageGIn);
	kernel.setArg(4, imageBIn);
	kernel.setArg(5, imageROut);
	kernel.setArg(6, imageGOut);
	kernel.setArg(7, imageBOut);

	comqueque.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(result.nRows, result.nCols), cl::NDRange(4, 4));
	comqueque.finish();

	comqueque.enqueueReadBuffer(imageROut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(uchar), result.dataRPtr.get());
	comqueque.enqueueReadBuffer(imageGOut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(uchar), result.dataGPtr.get());
	comqueque.enqueueReadBuffer(imageBOut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(uchar), result.dataBPtr.get());

	auto end = std::chrono::high_resolution_clock::now();
	float duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0F;
	_frame = result;
	return duration;
}

float FilterDevice::compute5x5()
{
	Frame result = _frame.clone();
	const int nRows = result.nRows;
	const int nCols = result.nCols;

	cl::CommandQueue comqueque(_context, _context.getInfo<CL_CONTEXT_DEVICES>()[0]);

	auto start = std::chrono::high_resolution_clock::now();
	cl::Buffer imageRIn = cl::Buffer(_context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		(_frame.nRows*_frame.nCols) * sizeof(uchar), _frame.dataRPtr.get());

	cl::Buffer imageGIn = cl::Buffer(_context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		(_frame.nRows*_frame.nCols) * sizeof(uchar), _frame.dataGPtr.get());

	cl::Buffer imageBIn = cl::Buffer(_context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		(_frame.nRows*_frame.nCols) * sizeof(uchar), _frame.dataBPtr.get());

	cl::Buffer imageROut = cl::Buffer(_context,
		CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		(result.nRows*result.nCols) * sizeof(uchar), result.dataRPtr.get());

	cl::Buffer imageGOut = cl::Buffer(_context,
		CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		(result.nRows*result.nCols) * sizeof(uchar), result.dataGPtr.get());

	cl::Buffer imageBOut = cl::Buffer(_context,
		CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		(result.nRows*result.nCols) * sizeof(uchar), result.dataGPtr.get());

	cl::Kernel kernel(_program, "nativeFilter5x5");

	kernel.setArg(0, result.nRows);
	kernel.setArg(1, result.nCols);
	kernel.setArg(2, imageRIn);
	kernel.setArg(3, imageGIn);
	kernel.setArg(4, imageBIn);
	kernel.setArg(5, imageROut);
	kernel.setArg(6, imageGOut);
	kernel.setArg(7, imageBOut);

	comqueque.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(result.nRows, result.nCols), cl::NDRange(4, 4));
	comqueque.finish();

	comqueque.enqueueReadBuffer(imageROut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(uchar), result.dataRPtr.get());
	comqueque.enqueueReadBuffer(imageGOut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(uchar), result.dataGPtr.get());
	comqueque.enqueueReadBuffer(imageBOut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(uchar), result.dataBPtr.get());

	auto end = std::chrono::high_resolution_clock::now();
	float duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0F;
	_frame = result;
	return duration;
}


FilterDevice::~FilterDevice()
{
}

void FilterDevice::programBuild()
{
	std::vector<cl::Device> device = { _devices[_parameter.activeDevice] };
	cl::Context context(device);
	_context = context;

	std::ifstream sourceFile("nativeBitonic.cl");
	std::string sourceCode(std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));
	cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length() + 1));
	_program = cl::Program(_context, source);

	try
	{
		_program.build(_context.getInfo<CL_CONTEXT_DEVICES>());
	}
	catch (const cl::Error &err)
	{
		_log->add(std::string(err.what())+ ": " + _program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(_context.getInfo<CL_CONTEXT_DEVICES>()[0]));
	}
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
