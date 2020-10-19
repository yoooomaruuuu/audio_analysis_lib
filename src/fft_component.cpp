#include "fft_component.hpp"

using namespace audio_analysis_lib;

DLLEXPORT fft_component::fft_component(int fftSize)
	:pImpl(std::make_unique<fft_component_impl>(fftSize)) { }

DLLEXPORT fft_component::~fft_component() {};

DLLEXPORT fft_component::fft_exception fft_component::fft(const float* input_re, const float* input_im, float* output_re, float* output_im)
{
	return pImpl->fft(input_re, input_im, output_re, output_im);
}

DLLEXPORT fft_component::fft_exception fft_component::ifft(const float* input_re, const float* input_im, float* output_re, float* output_im)
{
	return pImpl->ifft(input_re, input_im, output_re, output_im);
}

DLLEXPORT fft_component::fft_exception fft_component::fft_mode_setting(fft_mode mode)
{
	return pImpl->fft_mode_setting(mode);
}

DLLEXPORT int fft_component::get_fft_size()
{
	return pImpl->get_fft_size();
}
