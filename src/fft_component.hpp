#pragma once
#include <iostream>
#include <memory>
#include "define_macro.hpp"
#include "fft_exception.hpp"

namespace audio_analysis_lib
{
	namespace my_world_voice_feature
	{
		enum class fft_mode : int
		{
			FFT = 0,
			IFFT = 1,
			ERROR = -1
		};
		class fft_component_impl;

		class fft_component
		{
		public:

			fft_component(int fft_size);

			fft_exception fft(const float* input_re, const float* input_im, float* output_re, float* output_im);

			fft_exception ifft(const float* input_re, const float* input_im, float* output_re, float* output_im);

			fft_exception fft_mode_setting(fft_mode mode);

			int get_fft_size();

			~fft_component();

		private:
			std::unique_ptr<my_world_voice_feature::fft_component_impl> pImpl;
		};
	}

	using namespace my_world_voice_feature;

	DLLEXPORT void create_fft_component(int init_fft_size, void** func_object);

	DLLEXPORT fft_exception mylib_fft(const float* input_re, const float* input_im, float* output_re, float* output_im, void* func_object);

	DLLEXPORT fft_exception mylib_ifft(const float* input_re, const float* input_im, float* output_re, float* output_im, void* func_object);

	DLLEXPORT fft_exception fft_mode_setting(fft_mode mode, void* func_object);

	DLLEXPORT int32_t get_fft_size(void* func_object);

	DLLEXPORT void delete_fft_component(void** func_object);

	DLLEXPORT int32_t get_fft_component_size();
}
