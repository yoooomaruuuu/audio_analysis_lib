#pragma once
#include "world/d4c.h"

namespace audio_analysis_lib
{
	namespace my_world_voice_feature
	{
		class d4c_ap_impl
		{
		public:
			d4c_ap_impl(int init_fs);
			void aperiodicity_estimated(const double *x, int x_length, const double *temporal_position, const double *f0, int f0_length, int fft_size, double **aperiodicity);
		private:
			int fs;
			D4COption option;
		};

	}
}
