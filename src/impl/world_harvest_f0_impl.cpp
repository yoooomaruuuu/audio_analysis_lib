#include "world_harvest_f0_impl.hpp"

using namespace audio_analysis_lib::my_world_voice_feature;

harvest_f0_impl::harvest_f0_impl(int init_fs, int init_frame_period)
	: option(), fs(init_fs)
{
	InitializeHarvestOption(&option);
	option.f0_floor = 40.0;
	option.frame_period = init_frame_period;
}

void harvest_f0_impl::f0_estimate(const double* x, int x_length, double* temporal_positions, double* f0)
{
	Harvest(x, x_length, fs, &option, temporal_positions, f0);
}

int harvest_f0_impl::get_f0_sample_num(int x_length)
{
	return GetSamplesForHarvest(fs, x_length, option.frame_period);
}



