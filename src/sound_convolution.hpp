#pragma once
#include <iostream>
#include <memory>

namespace audio_analysis_lib
{
	namespace my_signal_processing
	{
		class sound_convolution_impl;
	}
	namespace my_world_voice_feature
	{
		class fft_component;
	}

	class sound_convolution
	{
	public:
		sound_convolution(int init_fft_size);
		~sound_convolution();
		void frame_convolution(const float* src1, const float* src2);

	private:
		std::unique_ptr<my_signal_processing::sound_convolution_impl> pImpl;
	};
}
