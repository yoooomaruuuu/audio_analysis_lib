#pragma once
#include "world/harvest.h"

namespace audio_analysis_lib
{
	namespace my_world_voice_feature
	{
		class harvest_f0_impl
		{
		public:
			harvest_f0_impl(int init_fs, int init_frame_period);
			void f0_estimate(const double *x, int x_length, double *temporal_positions, double *f0);
			int get_f0_sample_num(int x_length);
		private:
			HarvestOption option;
			int fs;
		};
	}
}

