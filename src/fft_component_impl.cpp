#include "fft_component_impl.hpp"

using namespace audio_analysis_lib;

FFTComponentImpl::FFTComponentImpl(int initFFTSize)
	:fftSize(initFFTSize), plan()
{ 
	input = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * fftSize);
	output = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * fftSize);
} 

FFTComponentImpl::~FFTComponentImpl()
{
	fftw_free(input);
	fftw_free(output);
}

int FFTComponentImpl::fft(const float* input_re, const float* input_im, float* output_re, float* output_im, bool ifftOn)
{
	if (ifftOn)
	{
		plan = fftw_plan_dft_1d(fftSize, input, output, FFTW_BACKWARD, FFTW_ESTIMATE);
	}
	else
	{
		plan = fftw_plan_dft_1d(fftSize, input, output, FFTW_FORWARD, FFTW_ESTIMATE);
	}
	for (int i = 0; i < fftSize; i++)
	{
		input[i][0] = input_re[i];
		input[i][1] = input_im[i];
	}
	fftw_execute(plan);
	for (int i = 0; i < fftSize; i++)
	{
		output_re[i] = output[i][0];
		output_re[i] = output[i][1];
	}
	return 0;
}

int FFTComponentImpl::getFFTSize() { return fftSize; }