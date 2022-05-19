#include "world_cheaptrick_sp.hpp"

using namespace audio_analysis_lib;

cheaptrick_sp::cheaptrick_sp(int init_fs)
	: pimpl(std::make_unique<my_world_voice_feature::cheaptrick_sp_impl>(init_fs))
{ }

void cheaptrick_sp::spectrogram_estimated(double* x, int x_length, double* temporal_position, double* f0, int f0_length, double** spectrogram)
{
	pimpl->spectrogram_estimated(x, x_length, temporal_position, f0, f0_length, spectrogram);
}

int cheaptrick_sp::get_fftsize_for_cheaptrick()
{
	return pimpl->get_fftsize_for_cheaptrick();
}
