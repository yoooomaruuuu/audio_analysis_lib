#include <memory>
#include "define_macro.hpp"
#include "input_capture_impl.hpp"

namespace audio_analysis_lib
{
	class input_capture_impl;

	class input_capture
	{
	public:
		input_capture();

		HRESULT init(DWORD sample_rate, WORD channels, WORD bits_per_sample, int frame_ms, int device_index);

		int get_buf_size();

		int get_input_devices_list_size();
		
		std::vector<std::wstring> get_input_devices_list();

		HRESULT start();

		HRESULT capture_data(char** tmp);

		HRESULT stop();

		~input_capture();

	private:
		std::unique_ptr<input_capture_impl> ptr;
	};

	DLLEXPORT void create_input_capture(void** func_object);

	DLLEXPORT HRESULT init_input_capture(DWORD sample_rate, WORD channels, WORD bits_per_sample, int frame_ms, int device_index, void* func_object);

	DLLEXPORT int get_buf_size(void* func_object);

	DLLEXPORT int get_input_devices_list_size(void* func_object);

	DLLEXPORT void get_input_devices_list(int index, wchar_t* tmp, void* func_object);

	DLLEXPORT HRESULT start(void* func_object);
	
	DLLEXPORT HRESULT caputre_data(char** tmp, void* func_object);

	DLLEXPORT HRESULT stop(void* func_object);

	DLLEXPORT void delete_input_capture(void** func_object);
}
