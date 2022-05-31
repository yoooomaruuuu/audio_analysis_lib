#include "world_d4c_ap.hpp"
#include "world/d4c.h"

namespace audio_analysis_lib
{
	namespace my_world_voice_feature
	{
		// implÇÃêÈåæÅAé¿ëï
		class d4c_ap_impl
		{
		public:
			d4c_ap_impl(int init_fs);
			void aperiodicity_estimated(const double *x, int x_length, const double *temporal_position, const double *f0, int f0_length, int fft_size, double **aperiodicity);
		private:
			int fs;
			D4COption option;
		};

		d4c_ap_impl::d4c_ap_impl(int init_fs)
			: fs(init_fs), option()
		{
			InitializeD4COption(&option);
			option.threshold = 0.85;
		}

		void d4c_ap_impl::aperiodicity_estimated(const double *x, int x_length, const double *temporal_position, const double *f0, int f0_length, int fft_size, double **aperiodicity)
		{
			D4C(x, x_length, fs, temporal_position, f0, f0_length, fft_size, &option, aperiodicity);
		}
	}

	// d4cÇÃé¿ëï
	d4c_ap::d4c_ap(int init_fs)
		: pimpl(std::make_unique<my_world_voice_feature::d4c_ap_impl>(init_fs))
	{ }

	void d4c_ap::aperiodicity_estimated(const double *x, int x_length, const double *temporal_position, const double *f0, int f0_length, int fft_size, double **aperiodicity)
	{
		pimpl->aperiodicity_estimated(x, x_length, temporal_position, f0, f0_length, fft_size, aperiodicity);
	}
}
