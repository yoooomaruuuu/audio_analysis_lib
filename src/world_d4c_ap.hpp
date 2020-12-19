#pragma once
#include "world_d4c_ap_impl.hpp"
#include <memory>

namespace audio_analysis_lib
{
	using namespace my_world_voice_feature;
	class d4c_ap_impl;
	
	class d4c_ap
	{
	public:
		d4c_ap(int init_fs);
		void aperiodicity_estimated(const double *x, int x_length, const double *temporal_position, const double *f0, int f0_length, int fft_size, double **aperiodicity);
	private:
		std::unique_ptr<my_world_voice_feature::d4c_ap_impl> pimpl;
	};
}
