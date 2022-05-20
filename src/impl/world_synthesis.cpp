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

void audio_analysis_lib::create_world_synthesis(int init_fs, int init_frame_period, int init_input_length, void** func_object)
{
	*func_object = new world_synthesis(init_fs, init_frame_period, init_input_length);
}
void audio_analysis_lib::realitime_synth(double f0_shift, double* frame_data, double* output_data, void* func_object)
{
	static_cast<world_synthesis*>(func_object)->realtime_synth(f0_shift, frame_data, output_data);
}
void audio_analysis_lib::delete_world_synthesis(void** func_object)
{
	delete* func_object;
}