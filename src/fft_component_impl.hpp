#pragma once
#include "fftw3.h"

namespace audio_analysis_lib
{
	class FFTComponentImpl
	{
	public:
		FFTComponentImpl(int fftSize);
		int fft(const float* input_re, const float* input_im, float* output_re, float* output_im, bool ifftOn);
		int getFFTSize();
		~FFTComponentImpl();

	private:
		int fftSize;
		fftw_complex* input;
		fftw_complex* output;
		fftw_plan plan;
	};
}
