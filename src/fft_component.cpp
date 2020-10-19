#include "fft_component.hpp"

using namespace audio_analysis_lib;

DLLEXPORT fft_component::fft_component(int fftSize)
	:pImpl(std::make_unique<fft_component_impl>(fftSize)) { }

DLLEXPORT fft_component::~fft_component() {};

DLLEXPORT int fft_component::fft(const float* input_re, const float* input_im, float* output_re, float* output_im, bool ifftOn)
{
	return pImpl->fft(input_re, input_im, output_re, output_im, ifftOn);
}

DLLEXPORT int fft_component::get_fft_size()
{
	return pImpl->get_fft_size();
}
