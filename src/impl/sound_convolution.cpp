#include "sound_convolution.hpp"

audio_analysis_lib::sound_convolution::sound_convolution(int init_fft_size)
	:pImpl(std::make_unique<sound_convolution_impl>(init_fft_size))
{
}

audio_analysis_lib::sound_convolution::~sound_convolution() { }

void audio_analysis_lib::sound_convolution::frame_convolution(const float* src1, const float* src2)
{
	pImpl->frame_convolution(src1, src2);
}