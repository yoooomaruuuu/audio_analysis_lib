#pragma once
#include <iostream>
#include "fft_component_impl.hpp"
#ifdef __cplusplus
extern "C"
#endif

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

		DLLEXPORT fft_component(int fftSize);

		DLLEXPORT fft_exception fft(const float* input_re, const float* input_im, float* output_re, float* output_im);

		DLLEXPORT fft_exception ifft(const float* input_re, const float* input_im, float* output_re, float* output_im);

		DLLEXPORT fft_exception fft_mode_setting(fft_mode mode);

		DLLEXPORT int get_fft_size();

		DLLEXPORT ~fft_component();

	private:
		std::unique_ptr<fft_component_impl> pImpl;
	};

}
