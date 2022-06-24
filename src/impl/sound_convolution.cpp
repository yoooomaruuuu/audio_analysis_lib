#include "sound_convolution.hpp"
#include "fft_component.hpp"
#include "fft_exception.hpp"

// Overlap-add法で実装
// 参考: https://www.kazetest.com/vcmemo/overlap-add-method/overlap-add-method.htm
// TODO: 上手くいってない気もするので要検証
namespace audio_analysis_lib
{
	namespace my_signal_processing
	{
		using namespace my_world_voice_feature;
		class sound_convolution_impl
		{
		public:
			sound_convolution_impl(uint32_t init_frame_size, uint32_t init_filter_tap);
			~sound_convolution_impl();
			void set_conv_filter(const float* filter);
			void frame_freq_convolution(const float* tar, float* out);

		private:
			uint32_t m_frame_size;
			uint32_t m_filter_tap;
			fft_component _fft;
			int m_fft_size;
			float* m_filter_factor;
			float* m_filter_freq_re, * m_filter_freq_im;
			float* m_fft_input_re, * m_fft_input_im;
			float* m_fft_output_re, * m_fft_output_im;
			float* m_ifft_input_re, * m_ifft_input_im;
			float* m_ifft_output_re, * m_ifft_output_im;
			float* m_pre_tar;
			float* m_conv_output;
		};

		// frame_size + filter_tap - 1について2のべき乗である制約
		sound_convolution_impl::sound_convolution_impl(uint32_t init_frame_size, uint32_t init_filter_tap)
			:m_frame_size(init_frame_size), m_filter_tap(init_filter_tap),
			 _fft(init_frame_size + init_filter_tap - 1),
			 m_fft_size(_fft.get_fft_size()),
			 m_filter_factor((float*)malloc(sizeof(float) * m_fft_size)),
			 m_filter_freq_re((float*)malloc(sizeof(float) * m_fft_size)),
			 m_filter_freq_im((float*)malloc(sizeof(float) * m_fft_size)),
			 m_fft_input_re((float*)malloc(sizeof(float) * m_fft_size)),
			 m_fft_input_im((float*)malloc(sizeof(float) * m_fft_size)),
			 m_fft_output_re((float*)malloc(sizeof(float) * m_fft_size)),
			 m_fft_output_im((float*)malloc(sizeof(float) * m_fft_size)),
			 m_ifft_input_re((float*)malloc(sizeof(float) * m_fft_size)),
			 m_ifft_input_im((float*)malloc(sizeof(float) * m_fft_size)),
			 m_ifft_output_re((float*)malloc(sizeof(float) * m_fft_size)),
			 m_ifft_output_im((float*)malloc(sizeof(float) * m_fft_size)),
			 m_pre_tar((float*)malloc(sizeof(float) * (init_filter_tap - 1)))
		{
			std::memset(m_fft_input_re, 0.0f, m_fft_size * sizeof(float));
			std::memset(m_fft_input_im, 0.0f, m_fft_size * sizeof(float));
			std::memset(m_fft_output_re, 0.0f, m_fft_size * sizeof(float));
			std::memset(m_fft_output_im, 0.0f, m_fft_size * sizeof(float));
			std::memset(m_ifft_input_re, 0.0f, m_fft_size * sizeof(float));
			std::memset(m_ifft_input_im, 0.0f, m_fft_size * sizeof(float));
			std::memset(m_ifft_output_re, 0.0f, m_fft_size * sizeof(float));
			std::memset(m_ifft_output_im, 0.0f, m_fft_size * sizeof(float));
			std::memset(m_filter_factor, 0.0f, m_fft_size * sizeof(float));
			std::memset(m_filter_freq_re, 0.0f, m_fft_size * sizeof(float));
			std::memset(m_filter_freq_im, 0.0f, m_fft_size * sizeof(float));
			std::memset(m_pre_tar, 0.0f, (init_filter_tap-1) * sizeof(float));
		}

		sound_convolution_impl::~sound_convolution_impl()
		{
			free(m_fft_input_re);
			free(m_fft_input_im);
			free(m_fft_output_re);
			free(m_fft_output_im);
			free(m_ifft_input_re);
			free(m_ifft_input_im);
			free(m_ifft_output_re);
			free(m_ifft_output_im);
			free(m_filter_factor);
			free(m_filter_freq_re);
			free(m_filter_freq_im);
			free(m_pre_tar);
		}

		// 畳み込みに用いるフィルタの設定
		void sound_convolution_impl::set_conv_filter(const float* filter)
		{
			std::memcpy(m_filter_factor, filter, m_filter_tap * sizeof(float));
			_fft.fft_mode_setting(audio_analysis_lib::fft_mode::FFT);
			float* in_im = ((float*)malloc(sizeof(float) * m_fft_size));
			std::memset(in_im, 0.0f, m_fft_size * sizeof(float));
			_fft.fft(m_filter_factor, in_im, m_filter_freq_re, m_filter_freq_im);
		}

		void sound_convolution_impl::frame_freq_convolution(const float* tar, float* out)
		{
			std::memcpy(m_fft_input_re, tar, m_frame_size * sizeof(float));
			_fft.fft_mode_setting(audio_analysis_lib::fft_mode::FFT);
			fft_exception ex = _fft.fft(m_fft_input_re, m_fft_input_im, m_fft_output_re, m_fft_output_im);
			for (int i = 0; i < m_fft_size; i++)
			{
				m_ifft_input_re[i] = m_fft_output_re[i] * m_filter_freq_re[i];
				m_ifft_input_im[i] = m_fft_output_im[i] * m_filter_freq_im[i];
			}
			_fft.fft_mode_setting(audio_analysis_lib::fft_mode::IFFT);
			ex = _fft.ifft(m_ifft_input_re, m_ifft_input_im, m_ifft_output_re, m_ifft_output_im);
			for (int i = 0; i < m_fft_size; i++)
			{
				m_ifft_output_re[i] = m_ifft_output_re[i] / (float)m_fft_size; // fftの結果をもとに振幅調整
				m_ifft_output_im[i] = m_ifft_output_im[i] / (float)m_fft_size;
			}
			std::memcpy(out, m_ifft_output_re, m_frame_size * sizeof(float));
			for (int i = 0; i < m_filter_tap - 1; i++)
			{
				out[i] += m_pre_tar[i];
			}
			std::memcpy(m_pre_tar, m_ifft_output_re + m_frame_size, (m_filter_tap - 1) * sizeof(float));
		}
	}
	sound_convolution::sound_convolution(uint32_t init_frame_size, uint32_t init_filter_tap)
		: pImpl(std::make_unique<my_signal_processing::sound_convolution_impl>(init_frame_size, init_filter_tap))
	{
	}

	sound_convolution::~sound_convolution() { }

	void sound_convolution::set_conv_filter(const float* filter)
	{
		pImpl->set_conv_filter(filter);
	}

	void sound_convolution::frame_freq_convolution(const float* tar, float* out)
	{
		pImpl->frame_freq_convolution(tar, out);
	}
}
