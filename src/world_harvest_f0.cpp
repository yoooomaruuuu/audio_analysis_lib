#pragma once
#include "world_harvest_f0.hpp"

using namespace audio_analysis_lib;

harvest_f0::harvest_f0(int fs, int frame_period)
	:pimpl(std::make_unique<my_world_voice_feature::harvest_f0_impl>(fs, frame_period))
{ }

void harvest_f0::f0_estimate(const double* x, int x_length, double* temporal_positions, double* f0)
{
	pimpl->f0_estimate(x, x_length, temporal_positions, f0);
}

int harvest_f0::get_f0_sample_num(int x_length)
{
	return pimpl->get_f0_sample_num(x_length);
}


void audio_analysis_lib::create_harvest_f0(int fs, int frame_period, void** func_object)
{
	*func_object = new harvest_f0(fs, frame_period);
}

void audio_analysis_lib::f0_estimate(const double* x, int x_length, double* temporal_positions, double* f0, void* func_object)
{
	static_cast<harvest_f0*>(func_object)->f0_estimate(x, x_length, temporal_positions, f0);
}

void audio_analysis_lib::get_f0_sample_num(int x_length, void* func_object)
{
	static_cast<harvest_f0*>(func_object)->get_f0_sample_num(x_length);
}

void audio_analysis_lib::delete_harvest_f0(void** func_object)
{
	delete* func_object;
}
