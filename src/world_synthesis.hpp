#pragma once
#include "world_synthesis_impl.hpp"
#include <memory>

namespace audio_analysis_lib
{
	using namespace my_world_voice_feature;

	class world_synthesis_impl;
	struct world_parameters;

	class world_synthesis
	{
	public:
		world_synthesis(int init_fs, float init_frame_period, int init_input_length);
		~world_synthesis();
		void realtime_synth(double f0_shift, double* frame_data, double* output_data);
	private:
		std::unique_ptr<my_world_voice_feature::world_synthesis_impl> pimpl;
	};
}

