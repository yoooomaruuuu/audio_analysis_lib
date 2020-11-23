#pragma once
//#include "fftw3.h"
#include "fft.h"
#include <stdexcept>

namespace audio_analysis_lib
{
	class fft_component_impl
	{
	public:
		enum class fft_mode : int
		{
			FFT = 0,
			IFFT = 1,
			ERROR = -1
		};
		enum class fft_exception : int
		{
			SUCCESS = 0,
			SETTINNG_ERROR = 1,
			DATA_OUT_OF_RANGE = 2,
			MODE_ERROR = 3
		};
		fft_component_impl(int fft_size);
		fft_exception fft(const float* input_re, const float* input_im, float* output_re, float* output_im);
		fft_exception ifft(const float* input_re, const float* input_im, float* output_re, float* output_im);
		fft_exception fft_mode_setting(fft_mode mode);
		int get_fft_size();
		~fft_component_impl();

	private:
		fft_exception fft_(const float* input_re, const float* input_im, float* output_re, float* output_im);
		int m_fft_size;
		fft_complex* m_input;
		fft_complex* m_output;
		fft_plan m_plan;
		fft_mode m_mode;
	};
}
