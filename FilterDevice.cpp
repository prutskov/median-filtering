#include "stdafx.h"
#include "FilterDevice.h"

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
}


FilterDevice::~FilterDevice()
{
}
