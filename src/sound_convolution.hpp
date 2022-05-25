#pragma once
#include <iostream>
#include <memory>
#include "impl/sound_convolution_impl.hpp"
#include "fft_component.hpp"

namespace audio_analysis_lib
{
	class sound_convolution_impl;
	class fft_component;

	class sound_convolution
	{
	public:
		sound_convolution(int init_fft_size);
		~sound_convolution();
		void frame_convolution(const float* src1, const float* src2);

	private:
		std::unique_ptr<sound_convolution_impl> pImpl;
	};
}
