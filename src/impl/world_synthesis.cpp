#include "world_synthesis.hpp"

using namespace audio_analysis_lib;

world_synthesis::world_synthesis(int init_fs, float init_frame_period, int init_input_length)
	: pimpl(std::make_unique<my_world_voice_feature::world_synthesis_impl>(init_fs, init_frame_period, init_input_length))
{
}

world_synthesis::~world_synthesis()
{ } 

void world_synthesis::realtime_synth(double f0_shift, double* frame_data, double* output_data)
{
	pimpl->realtime_synth(f0_shift, frame_data, output_data);
}