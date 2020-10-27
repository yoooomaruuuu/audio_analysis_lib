#pragma once
#include <iostream>
#include "fft_component_impl.hpp"
#include "define_macro.hpp"

namespace audio_analysis_lib
{
	//前方宣言
	class fft_component_impl;

	class fft_component
	{
	public:
		using fft_mode = fft_component_impl::fft_mode;
		using fft_exception = fft_component_impl::fft_exception;

		fft_component(int fft_size);

		fft_exception fft(const float* input_re, const float* input_im, float* output_re, float* output_im);

		fft_exception ifft(const float* input_re, const float* input_im, float* output_re, float* output_im);

		fft_exception fft_mode_setting(fft_mode mode);

		int get_fft_size();

		~fft_component();

	private:
		std::unique_ptr<fft_component_impl> pImpl;
	};
}

