#include "world_d4c_ap_impl.hpp"

using namespace audio_analysis_lib::my_world_voice_feature;

d4c_ap_impl::d4c_ap_impl(int init_fs)
	: fs(init_fs), option()
{
	InitializeD4COption(&option);
	option.threshold = 0.85;
}

void d4c_ap_impl::aperiodicity_estimated(const double *x, int x_length, const double *temporal_position, const double *f0, int f0_length, int fft_size, double **aperiodicity)
{
	D4C(x, x_length, fs, temporal_position, f0, f0_length, fft_size, &option, aperiodicity);
}



