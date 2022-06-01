#pragma once
#include "world_harvest_f0.hpp"
#include "world/harvest.h"

namespace audio_analysis_lib
{
	namespace my_world_voice_feature
	{
		// implÇÃêÈåæÅAé¿ëï
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
	}

	// harvestÇÃé¿ëï
	harvest_f0::harvest_f0(int fs, int frame_period)
		:pimpl(std::make_unique<my_world_voice_feature::harvest_f0_impl>(fs, frame_period))
	{ }

	harvest_f0::~harvest_f0() {}

	void harvest_f0::f0_estimate(const double* x, int x_length, double* temporal_positions, double* f0)
	{
		pimpl->f0_estimate(x, x_length, temporal_positions, f0);
	}

	int harvest_f0::get_f0_sample_num(int x_length)
	{
		return pimpl->get_f0_sample_num(x_length);
	}
}

// DLLä÷êîé¿ëï
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
