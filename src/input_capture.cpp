#include "input_capture.hpp"
#include <string.h>
#include <locale>
#include <codecvt>

using namespace audio_analysis_lib;

input_capture::input_capture()
	:ptr(std::make_unique<input_capture_impl>())
{
}

void input_capture::init(DWORD sample_rate, WORD channels, WORD bits_per_sample, int frame_ms, int device_index)
{
	ptr->init(sample_rate, channels, bits_per_sample, frame_ms, device_index);
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


DLLEXPORT void audio_analysis_lib::create_input_capture(void** func_object)
{
	*func_object = new audio_analysis_lib::input_capture();
}

DLLEXPORT void audio_analysis_lib::init_input_capture(DWORD sample_rate, WORD channels, WORD bits_per_sample, int frame_ms, int device_index, void* func_object)
{
	((input_capture*)func_object)->init(sample_rate, channels, bits_per_sample, frame_ms, device_index);
}

DLLEXPORT int audio_analysis_lib::get_buf_size(void* func_object)
{
	return ((input_capture*)func_object)->get_buf_size();
}

DLLEXPORT int audio_analysis_lib::get_input_devices_list_size(void* func_object)
{
	return ((input_capture*)func_object)->get_input_devices_list_size();
}

DLLEXPORT void audio_analysis_lib::get_input_devices_list(int index, char16_t* tmp, void* func_object)
{
	// char buf[devicesDescMaxSize] = { 0x00 };
	// strcpy_s(buf, devicesDescMaxSize, ((input_capture*)func_object)->get_input_devices_list()[index].c_str());
	// char16_t* buf16 = new char16_t[devicesDescMaxSize];
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	std::string buf8 = ((input_capture*)func_object)->get_input_devices_list()[index];
	std::u16string buf = converter.from_bytes(buf8);
	//unityC#‚ªchar16_t‚µ‚©Žó‚¯“ü‚ê‚È‚»‚¤‚È‚½‚ß•ÏŠ·
	//‚Ü‚½mbrtoc16‚ª1‚¸‚Â‚µ‚©“®‚©‚È‚©‚Á‚½‚Ì‚Åfor‚Å‰ñ‚·
	for (int i = 0; i < devicesDescMaxSize; i++)
	{
		tmp[i] = buf.c_str()[i];
	}
	//delete[] buf16;
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


