#pragma once
#include <iostream>
#include "fft_component_impl.hpp"
#ifdef __cplusplus
extern "C"
#endif

#define DLLEXPORT __declspec(dllexport)

namespace audio_analysis_lib
{
	//ëOï˚êÈåæ
	class FFTComponentImpl;

	class FFTComponent
	{
	public:
		DLLEXPORT FFTComponent(int fftSize);

		DLLEXPORT int fft(const float* input_re, const float* input_im, float* output_re, float* output_im, bool ifftOn);

		DLLEXPORT int getFFTSize();

		DLLEXPORT ~FFTComponent();

	private:
		std::unique_ptr<FFTComponentImpl> pImpl;
	};

}
