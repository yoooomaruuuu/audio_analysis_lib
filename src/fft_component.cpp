#include "fft_component.hpp"

using namespace audio_analysis_lib;

DLLEXPORT FFTComponent::FFTComponent(int fftSize)
	:pImpl(std::make_unique<FFTComponentImpl>(fftSize)) { }

DLLEXPORT FFTComponent::~FFTComponent() {};

DLLEXPORT int FFTComponent::fft(const float* input_re, const float* input_im, float* output_re, float* output_im, bool ifftOn)
{
	return pImpl->fft(input_re, input_im, output_re, output_im, ifftOn);
}

DLLEXPORT int FFTComponent::getFFTSize()
{
	return pImpl->getFFTSize();
}
