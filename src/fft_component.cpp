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
DLLEXPORT void init_fft_component(int init_fft_size, void* func_object)
{
	func_object = new fft_component(init_fft_size);
}

DLLEXPORT fft_component::fft_exception mylib_fft(const float* input_re, const float* input_im, float* output_re, float* output_im, void* func_object)
{
	return static_cast<fft_component*>(func_object)->fft(input_re, input_im, output_re, output_im);
}

DLLEXPORT fft_component::fft_exception mylib_ifft(const float* input_re, const float* input_im, float* output_re, float* output_im, void* func_object)
{
	return static_cast<fft_component*>(func_object)->ifft(input_re, input_im, output_re, output_im);
}

DLLEXPORT fft_component::fft_exception fft_mode_setting(fft_component::fft_mode mode, void* func_object)
{
	return static_cast<fft_component*>(func_object)->fft_mode_setting(mode);
}

DLLEXPORT int32_t get_fft_size(void* func_object)
{
	return static_cast<int32_t>(static_cast<fft_component*>(func_object)->get_fft_size());
}

DLLEXPORT void delete_fft_component(void* func_object)
{
	delete static_cast<fft_component*>(func_object);
}

DLLEXPORT int32_t get_fft_component_size()
{
	return static_cast<int32_t>(sizeof(fft_component));
}

