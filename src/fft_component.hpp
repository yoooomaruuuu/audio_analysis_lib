#pragma once
#include <iostream>
#include "fft_component_impl.hpp"

#define DLLEXPORT __declspec(dllexport) 

namespace audio_analysis_lib
{
	//前方宣言
	class fft_component_impl;

	class fft_component
	{
	public:
		using fft_mode = fft_component_impl::fft_mode;
		using fft_exception = fft_component_impl::fft_exception;

		fft_component(int fft_size);

		fft_exception fft(const float* input_re, const float* input_im, float* output_re, float* output_im);

		fft_exception ifft(const float* input_re, const float* input_im, float* output_re, float* output_im);

		fft_exception fft_mode_setting(fft_mode mode);

		int get_fft_size();

		~fft_component();

	private:
		std::unique_ptr<fft_component_impl> pImpl;
	};

#ifdef __cplusplus  
	extern "C" {
#endif
		DLLEXPORT void __stdcall init_fft_component(int init_fft_size, void* func_object);

		DLLEXPORT fft_component::fft_exception __stdcall fft(const float* input_re, const float* input_im, float* output_re, float* output_im, void* func_object);

		DLLEXPORT fft_component::fft_exception __stdcall ifft(const float* input_re, const float* input_im, float* output_re, float* output_im, void* func_object);

		DLLEXPORT fft_component::fft_exception __stdcall fft_mode_setting(fft_component::fft_mode mode, void* func_object);

		DLLEXPORT int32_t __stdcall get_fft_size(void* func_object);

		DLLEXPORT void __stdcall delete_fft_component(void* func_object);

		DLLEXPORT int32_t __stdcall get_fft_component_size();
#ifdef __cplusplus  
	}
#endif
}

