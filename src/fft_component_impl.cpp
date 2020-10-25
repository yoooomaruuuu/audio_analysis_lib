#include "fft_component_impl.hpp"

using namespace audio_analysis_lib;

fft_component_impl::fft_component_impl(int init_fft_size)
	:m_fft_size(init_fft_size), 
	 m_input((fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_fft_size)),
	 m_output((fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_fft_size)),
	 m_plan(), m_mode(fft_mode::ERROR)
{ 
} 

fft_component_impl::~fft_component_impl()
{
	fftw_free(m_input);
	fftw_free(m_output);
}

fft_component_impl::fft_exception fft_component_impl::fft_mode_setting(fft_mode mode)
{
	if (mode == fft_mode::FFT)
	{
		m_plan = fftw_plan_dft_1d(m_fft_size, m_input, m_output, FFTW_BACKWARD, FFTW_ESTIMATE);
	}
	else if (mode == fft_mode::IFFT)
	{
		m_plan = fftw_plan_dft_1d(m_fft_size, m_input, m_output, FFTW_FORWARD, FFTW_ESTIMATE);
	}
	else
	{
		return fft_exception::SETTINNG_ERROR;
	}
	m_mode = mode;
	return fft_exception::SUCCESS;
}

fft_component_impl::fft_exception fft_component_impl::fft(const float* input_re, const float* input_im, float* output_re, float* output_im)
{
	if (m_mode != fft_mode::FFT) return fft_exception::MODE_ERROR;
	fft_(input_re, input_im, output_re, output_im);
}

fft_component_impl::fft_exception fft_component_impl::ifft(const float* input_re, const float* input_im, float* output_re, float* output_im)
{
	if (m_mode != fft_mode::IFFT) return fft_exception::MODE_ERROR;
	fft_(input_re, input_im, output_re, output_im);
}

fft_component_impl::fft_exception fft_component_impl::fft_(const float* input_re, const float* input_im, float* output_re, float* output_im)
{
	try
	{
		for (int i = 0; i < m_fft_size; i++)
		{
			m_input[i][0] = input_re[i];
			m_input[i][1] = input_im[i];
		}
		fftw_execute(m_plan);
		for (int i = 0; i < m_fft_size; i++)
		{
			output_re[i] = m_output[i][0];
			output_re[i] = m_output[i][1];
		}
	}
	catch (std::out_of_range e)
	{
		return fft_exception::DATA_OUT_OF_RANGE;
	}
	return fft_exception::SUCCESS;
}


int fft_component_impl::get_fft_size() { return m_fft_size; }