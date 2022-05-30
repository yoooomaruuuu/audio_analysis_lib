#pragma once
//#include "fftw3.h"
#include "world\fft.h"
#include <stdexcept>
#include "fft_component.hpp"


// implêÈåæ
namespace audio_analysis_lib
{
	namespace my_world_voice_feature
	{
		class fft_component_impl
		{
		public:
			fft_component_impl(int fft_size);
			fft_exception fft(const float* input_re, const float* input_im, float* output_re, float* output_im);
			fft_exception ifft(const float* input_re, const float* input_im, float* output_re, float* output_im);
			fft_exception fft_mode_setting(fft_mode mode);
			int get_fft_size();
			~fft_component_impl();

		private:
			fft_exception fft_(const float* input_re, const float* input_im, float* output_re, float* output_im);
			int m_fft_size;
			fft_complex* m_input;
			fft_complex* m_output;
			fft_plan m_plan;
			fft_mode m_mode;
		};

		fft_component_impl::fft_component_impl(int init_fft_size)
			:m_fft_size(init_fft_size), 
			 m_input((fft_complex*)malloc(sizeof(fft_complex) * m_fft_size)),
			 m_output((fft_complex*)malloc(sizeof(fft_complex) * m_fft_size)),
			 m_plan(), m_mode(fft_mode::ERROR)
		{ 
		} 

		fft_component_impl::~fft_component_impl()
		{
			free(m_input);
			free(m_output);
			fft_destroy_plan(m_plan);
		}

		fft_exception fft_component_impl::fft_mode_setting(fft_mode mode)
		{
			if (mode == fft_mode::FFT)
			{
				m_plan = fft_plan_dft_1d(m_fft_size, m_input, m_output, FFT_BACKWARD, FFT_ESTIMATE);
			}
			else if (mode == fft_mode::IFFT)
			{
				m_plan = fft_plan_dft_1d(m_fft_size, m_input, m_output, FFT_FORWARD, FFT_ESTIMATE);
			}
			else
			{
				return fft_exception::SETTINNG_ERROR;
			}
			m_mode = mode;
			return fft_exception::SUCCESS;
		}

		fft_exception fft_component_impl::fft(const float* input_re, const float* input_im, float* output_re, float* output_im)
		{
			if (m_mode != fft_mode::FFT) return fft_exception::MODE_ERROR;
			return fft_(input_re, input_im, output_re, output_im);
		}

		fft_exception fft_component_impl::ifft(const float* input_re, const float* input_im, float* output_re, float* output_im)
		{
			if (m_mode != fft_mode::IFFT) return fft_exception::MODE_ERROR;
			return fft_(input_re, input_im, output_re, output_im);
		}

		fft_exception fft_component_impl::fft_(const float* input_re, const float* input_im, float* output_re, float* output_im)
		{
			try
			{
				for (int i = 0; i < m_fft_size; i++)
				{
					m_input[i][0] = static_cast<float>(input_re[i]);
					m_input[i][1] = static_cast<float>(input_im[i]);
				}
				fft_execute(m_plan);
				for (int i = 0; i < m_fft_size; i++)
				{
					output_re[i] = static_cast<float>(m_output[i][0]);
					output_im[i] = static_cast<float>(m_output[i][1]);
				}
			}
			catch (std::out_of_range e)
			{
				return fft_exception::DATA_OUT_OF_RANGE;
			}
			return fft_exception::SUCCESS;
		}


		int fft_component_impl::get_fft_size() { return m_fft_size; }

	}
}

using namespace audio_analysis_lib;

fft_component::fft_component(int fft_size)
	:pImpl(std::make_unique<my_world_voice_feature::fft_component_impl>(fft_size)) { }

fft_component::~fft_component() {};

fft_exception fft_component::fft(const float* input_re, const float* input_im, float* output_re, float* output_im)
{
	return pImpl->fft(input_re, input_im, output_re, output_im);
}

fft_exception fft_component::ifft(const float* input_re, const float* input_im, float* output_re, float* output_im)
{
	return pImpl->ifft(input_re, input_im, output_re, output_im);
}

fft_exception fft_component::fft_mode_setting(fft_mode mode)
{
	return pImpl->fft_mode_setting(mode);
}

int fft_component::get_fft_size()
{
	return pImpl->get_fft_size();
}
DLLEXPORT void audio_analysis_lib::create_fft_component(int init_fft_size, void** func_object)
{
	*func_object = new fft_component(init_fft_size);
}

DLLEXPORT fft_exception audio_analysis_lib::mylib_fft(const float* input_re, const float* input_im, float* output_re, float* output_im, void* func_object)
{
	return static_cast<fft_component*>(func_object)->fft(input_re, input_im, output_re, output_im);
}

DLLEXPORT fft_exception audio_analysis_lib::mylib_ifft(const float* input_re, const float* input_im, float* output_re, float* output_im, void* func_object)
{
	return static_cast<fft_component*>(func_object)->ifft(input_re, input_im, output_re, output_im);
}

DLLEXPORT fft_exception audio_analysis_lib::fft_mode_setting(fft_mode mode, void* func_object)
{
	return static_cast<fft_component*>(func_object)->fft_mode_setting(mode);
}

DLLEXPORT int32_t audio_analysis_lib::get_fft_size(void* func_object)
{
	return static_cast<int32_t>(static_cast<fft_component*>(func_object)->get_fft_size());
}

DLLEXPORT void audio_analysis_lib::delete_fft_component(void** func_object)
{
	delete static_cast<fft_component*>(*func_object);
}

DLLEXPORT int32_t audio_analysis_lib::get_fft_component_size()
{
	return static_cast<int32_t>(sizeof(fft_component));
}

