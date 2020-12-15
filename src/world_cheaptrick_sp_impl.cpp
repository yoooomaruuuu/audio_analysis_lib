#include "world_cheaptrick_sp_impl.hpp"

using namespace audio_analysis_lib::my_world_voice_feature;

cheaptrick_sp_impl::cheaptrick_sp_impl(int init_fs)
	: fs(init_fs), option({ 0 })
{
	InitializeCheapTrickOption(fs, &option);
	option.f0_floor = 71.0;
}
void cheaptrick_sp_impl::spectrogram_estimated(double* x, int x_length, double* temporal_position, double* f0, int f0_length, double** spectrogram)
{
	CheapTrick(x, x_length, fs, temporal_position, f0, f0_length, &option, spectrogram);

}
int cheaptrick_sp_impl::get_fftsize_for_cheaptrick()
{
	return GetFFTSizeForCheapTrick(fs, &option);
}
