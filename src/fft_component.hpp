#pragma once
#include <iostream>
#include "fft_component_impl.hpp"
#ifdef __cplusplus
extern "C"
#endif

#define DLLEXPORT __declspec(dllexport)

namespace audio_analysis_lib
{
	//‘O•ûéŒ¾
	class fft_component_impl;

	class fft_component
	{
	public:
		DLLEXPORT fft_component(int fftSize);

		DLLEXPORT int fft(const float* input_re, const float* input_im, float* output_re, float* output_im, bool ifftOn);

		DLLEXPORT int get_fft_size();

		DLLEXPORT ~fft_component();

	private:
		std::unique_ptr<fft_component_impl> pImpl;
	};

}
