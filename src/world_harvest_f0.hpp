#pragma once
#include <memory>
#include "define_macro.hpp"

namespace audio_analysis_lib
{
	// 前方宣言
	namespace my_world_voice_feature
	{
		class harvest_f0_impl;
	}

	// f0推定
	class harvest_f0
	{
	public:
		harvest_f0(int init_fs, int init_frame_period);
		~harvest_f0();
		void f0_estimate(const double *x, int x_length, double *temporal_positions, double *f0);
		int get_f0_sample_num(int x_length);
	private:
		std::unique_ptr<my_world_voice_feature::harvest_f0_impl> pimpl;
	};

	DLLEXPORT void create_harvest_f0(int fs, int frame_period, void** func_object);
	DLLEXPORT void f0_estimate(const double* x, int x_length, double* temporal_positions, double* f0, void* func_object);
	DLLEXPORT void get_f0_sample_num(int x_length, void* func_object);
	DLLEXPORT void delete_harvest_f0(void** func_object);
}
