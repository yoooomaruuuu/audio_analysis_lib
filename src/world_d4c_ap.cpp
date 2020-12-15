#include "world_d4c_ap.hpp"

using namespace audio_analysis_lib;

d4c_ap::d4c_ap(int init_fs)
	: pimpl(std::make_unique<my_world_voice_feature::d4c_ap_impl>(init_fs))
{ }

void d4c_ap::spectrogram_estimated(const double *x, int x_length, const double *temporal_position, const double *f0, int f0_length, int fft_size, double **aperiodicity)
{
	pimpl->aperiodicity_estimated(x, x_length, temporal_position, f0, f0_length, fft_size, aperiodicity);
}