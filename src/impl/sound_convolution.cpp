#include "sound_convolution.hpp"
#include "fft_component.hpp"

namespace audio_analysis_lib
{
	namespace my_signal_processing
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

		sound_convolution_impl::sound_convolution_impl(int init_fft_size)
		{
		}

		sound_convolution_impl::~sound_convolution_impl()
		{

		}

		void sound_convolution_impl::frame_convolution(const float* src1, const float* src2)
		{

		}
	}
	sound_convolution::sound_convolution(int init_fft_size)
		:pImpl(std::make_unique<my_signal_processing::sound_convolution_impl>(init_fft_size))
	{
	}

	sound_convolution::~sound_convolution() { }

	void sound_convolution::frame_convolution(const float* src1, const float* src2)
	{
		pImpl->frame_convolution(src1, src2);
	}
}
