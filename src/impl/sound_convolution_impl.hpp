#pragma once

namespace audio_analysis_lib
{
	class sound_convolution_impl
	{
	public:
		sound_convolution_impl(int init_fft_size);
		~sound_convolution_impl();
		void frame_convolution(const float* src1, const float* src2);

	private:
		float* m_s1_input_re, m_s1_input_im, m_s2_input_re, m_s2_input_im;
		float* m_s1_output_re, m_s1_output_im, m_s2_output_re, m_s2_output_im;
		float* m_conv_output;
	};
}
