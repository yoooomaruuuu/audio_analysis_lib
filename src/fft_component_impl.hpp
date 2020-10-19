#pragma once
#include "fftw3.h"

namespace audio_analysis_lib
{
	class fft_component_impl
	{
	public:
		fft_component_impl(int fft_size);
		int fft(const float* input_re, const float* input_im, float* output_re, float* output_im, bool ifft_on);
		int get_fft_size();
		~fft_component_impl();

	private:
		int m_fft_size;
		fftw_complex* m_input;
		fftw_complex* m_output;
		fftw_plan m_plan;
	};
}
