#include "sound_convolution.hpp"
#include "fft_component.hpp"
#include "fft_exception.hpp"

namespace audio_analysis_lib
{
	namespace my_signal_processing
	{
		using namespace my_world_voice_feature;
		class sound_convolution_impl
		{
		public:
			sound_convolution_impl(int init_frame_size);
			~sound_convolution_impl();
			float* frame_convolution(const float* src1, const float* src2);

		private:
			int m_frame_size, m_fft_size;
			float *m_s1_input_re, *m_s1_input_im, *m_s2_input_re, *m_s2_input_im;
			float *m_s1_output_re, *m_s1_output_im, *m_s2_output_re, *m_s2_output_im;
			float *m_conv_output;
			fft_component _fft;
		};

		sound_convolution_impl::sound_convolution_impl(int init_frame_size)
			:m_frame_size(init_frame_size),
			 m_fft_size(init_frame_size * 2 - 1),
			 m_s1_input_re((float*)malloc(sizeof(float) * m_fft_size)),
			 m_s1_input_im((float*)malloc(sizeof(float) * m_fft_size)),
			 m_s2_input_re((float*)malloc(sizeof(float) * m_fft_size)),
			 m_s2_input_im((float*)malloc(sizeof(float) * m_fft_size)),
			 m_s1_output_re((float*)malloc(sizeof(float) * m_fft_size)),
			 m_s1_output_im((float*)malloc(sizeof(float) * m_fft_size)),
			 m_s2_output_re((float*)malloc(sizeof(float) * m_fft_size)),
			 m_s2_output_im((float*)malloc(sizeof(float) * m_fft_size)),
			 m_conv_output((float*)malloc(sizeof(float) * m_fft_size)),
		 	 _fft(m_fft_size)
		{
		}

		sound_convolution_impl::~sound_convolution_impl()
		{
			free(m_s1_input_re);
			free(m_s1_input_im);
			free(m_s2_input_re);
			free(m_s2_input_im);
			free(m_s1_output_re);
			free(m_s1_output_im);
			free(m_s2_output_re);
			free(m_s2_output_im);
			free(m_conv_output);
		}

		float* sound_convolution_impl::frame_convolution(const float* src1, const float* src2)
		{
			return nullptr;
		}
	}
	sound_convolution::sound_convolution(int init_frame_size)
		: pImpl(std::make_unique<my_signal_processing::sound_convolution_impl>(init_frame_size))
	{
	}

	sound_convolution::~sound_convolution() { }

	float* sound_convolution::frame_convolution(const float* src1, const float* src2)
	{
		return pImpl->frame_convolution(src1, src2);
	}
}
