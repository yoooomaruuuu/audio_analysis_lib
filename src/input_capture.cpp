#include "input_capture.hpp"
#include <string.h>

using namespace audio_analysis_lib;

input_capture::input_capture(DWORD sample_rate, WORD channels, WORD bits_per_sample, int frame_ms)
	:ptr(std::make_unique<input_capture_impl>(sample_rate, channels, bits_per_sample, frame_ms))
{
}

void input_capture::init(int device_index)
{
	ptr->init(device_index);
}

int input_capture::get_buf_size()
{
	return ptr->get_buf_size();
}

int input_capture::get_input_devices_list_size()
{
	return ptr->get_input_devices_list_size();
}

std::vector<std::string> input_capture::get_input_devices_list()
{
	return ptr->get_input_devices_list();
}

HRESULT input_capture::start()
{
	return ptr->start();
}

HRESULT input_capture::capture_data(char** tmp)
{
	return ptr->capture_data(tmp);
}

HRESULT input_capture::stop()
{
	return ptr->stop();
}

input_capture::~input_capture()
{
}


DLLEXPORT void audio_analysis_lib::create_input_capture(DWORD sample_rate, WORD channels, WORD bits_per_sample, int frame_ms, void** func_object)
{
	*func_object = new audio_analysis_lib::input_capture(sample_rate, channels, bits_per_sample, frame_ms);
}

DLLEXPORT void audio_analysis_lib::init_input_capture(int device_index, void* func_object)
{
	((input_capture*)func_object)->init(device_index);
}

DLLEXPORT int audio_analysis_lib::get_buf_size(void* func_object)
{
	return ((input_capture*)func_object)->get_buf_size();
}

DLLEXPORT int audio_analysis_lib::get_input_devices_list_size(void* func_object)
{
	return ((input_capture*)func_object)->get_input_devices_list_size();
}

DLLEXPORT void audio_analysis_lib::get_input_devices_list(int index, char* tmp, void* func_object)
{
	strcpy_s(tmp, 256, ((input_capture*)func_object)->get_input_devices_list()[index].c_str());
}

DLLEXPORT HRESULT audio_analysis_lib::start(void* func_object)
{
	return ((input_capture*)func_object)->start();
}

DLLEXPORT HRESULT audio_analysis_lib::caputre_data(char** tmp, void* func_object)
{
	return ((input_capture*)func_object)->capture_data(tmp);
}

DLLEXPORT HRESULT audio_analysis_lib::stop(void* func_object)
{
	return ((input_capture*)func_object)->stop();
}

DLLEXPORT void audio_analysis_lib::delete_input_capture(void** func_object)
{
	delete ((input_capture*)*func_object);
}
