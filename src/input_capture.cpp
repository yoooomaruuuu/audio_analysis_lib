#include "input_capture.hpp"
#include <string.h>
#include <locale>
#include <codecvt>

using namespace audio_analysis_lib;

input_capture::input_capture()
	:ptr(std::make_unique<input_capture_impl>())
{
}

HRESULT input_capture::init(DWORD sample_rate, WORD channels, WORD bits_per_sample, int frame_ms, int device_index)
{
	return ptr->init(sample_rate, channels, bits_per_sample, frame_ms, device_index);
}

int input_capture::get_buf_size()
{
	return ptr->get_buf_size();
}

int input_capture::get_input_devices_list_size()
{
	return ptr->get_input_devices_list_size();
}

std::vector<std::wstring> input_capture::get_input_devices_list()
{
	return ptr->get_input_devices_list();
}

HRESULT input_capture::start()
{
	return ptr->start();
}

HRESULT input_capture::capture_data(char** tmp, int& copy_length)
{
	return ptr->capture_data(tmp, copy_length);
}

HRESULT input_capture::stop()
{
	return ptr->stop();
}

input_capture::~input_capture()
{
}


DLLEXPORT void audio_analysis_lib::create_input_capture(void** func_object)
{
	*func_object = new audio_analysis_lib::input_capture();
}

DLLEXPORT HRESULT audio_analysis_lib::init_input_capture(DWORD sample_rate, WORD channels, WORD bits_per_sample, int frame_ms, int device_index, void* func_object)
{
	return ((input_capture*)func_object)->init(sample_rate, channels, bits_per_sample, frame_ms, device_index);
}

DLLEXPORT int audio_analysis_lib::get_buf_size(void* func_object)
{
	return ((input_capture*)func_object)->get_buf_size();
}

DLLEXPORT int audio_analysis_lib::get_input_devices_list_size(void* func_object)
{
	return ((input_capture*)func_object)->get_input_devices_list_size();
}

DLLEXPORT void audio_analysis_lib::get_input_devices_list(int index, wchar_t* tmp, void* func_object)
{
	wcscpy_s(tmp, devicesDescMaxSize, ((input_capture*)func_object)->get_input_devices_list()[index].c_str());
}

DLLEXPORT HRESULT audio_analysis_lib::start(void* func_object)
{
	return ((input_capture*)func_object)->start();
}

DLLEXPORT HRESULT audio_analysis_lib::caputre_data(char** tmp, int& copy_length, void* func_object)
{
	return ((input_capture*)func_object)->capture_data(tmp, copy_length);
}

DLLEXPORT HRESULT audio_analysis_lib::stop(void* func_object)
{
	return ((input_capture*)func_object)->stop();
}

DLLEXPORT void audio_analysis_lib::delete_input_capture(void** func_object)
{
	delete ((input_capture*)*func_object);
}


