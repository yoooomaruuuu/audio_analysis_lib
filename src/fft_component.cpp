#include "fft_component.hpp"

using namespace audio_analysis_lib;

fft_component::fft_component(int fft_size)
	:pImpl(std::make_unique<fft_component_impl>(fft_size)) { }

fft_component::~fft_component() {};

fft_component::fft_exception fft_component::fft(const float* input_re, const float* input_im, float* output_re, float* output_im)
{
	return pImpl->fft(input_re, input_im, output_re, output_im);
}

fft_component::fft_exception fft_component::ifft(const float* input_re, const float* input_im, float* output_re, float* output_im)
{
	return pImpl->ifft(input_re, input_im, output_re, output_im);
}

fft_component::fft_exception fft_component::fft_mode_setting(fft_mode mode)
{
	return pImpl->fft_mode_setting(mode);
}

int fft_component::get_fft_size()
{
	return pImpl->get_fft_size();
}


//あれ？マーシャリングのダブルポインタ運用ってできたっけ
void init_fft_component(int init_fft_size, void** func_object)
{
	fft_component* tmp = new fft_component(init_fft_size);
	*func_object = static_cast<void*>(tmp);
}
