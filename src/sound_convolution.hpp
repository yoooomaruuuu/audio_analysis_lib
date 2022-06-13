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
		class fft_component_impl;
	}

	class sound_convolution
	{
	public:
		sound_convolution(uint16_t init_frame_size, uint16_t init_filter_tap);
		~sound_convolution();
		void set_conv_filter(const float* filter);
		void frame_freq_convolution(const float* tar, float* out);

	private:
		std::unique_ptr<my_signal_processing::sound_convolution_impl> pImpl;
	};
}
