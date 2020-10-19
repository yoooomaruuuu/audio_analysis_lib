#include "fft_component_impl.hpp"

using namespace audio_analysis_lib;

fft_component_impl::fft_component_impl(int initFFTSize)
	:m_fft_size(initFFTSize), 
	 m_input((fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_fft_size)),
	 m_output((fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_fft_size)),
	 m_plan()
{ 
} 

fft_component_impl::~fft_component_impl()
{
	fftw_free(m_input);
	fftw_free(m_output);
}

int fft_component_impl::fft(const float* input_re, const float* input_im, float* output_re, float* output_im, bool ifftOn)
{
	if (ifftOn)
	{
		m_plan = fftw_plan_dft_1d(m_fft_size, m_input, m_output, FFTW_BACKWARD, FFTW_ESTIMATE);
	}
	else
	{
		m_plan = fftw_plan_dft_1d(m_fft_size, m_input, m_output, FFTW_FORWARD, FFTW_ESTIMATE);
	}
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
	return 0;
}

int fft_component_impl::get_fft_size() { return m_fft_size; }