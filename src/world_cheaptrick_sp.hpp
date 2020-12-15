#pragma once
#include "world_cheaptrick_sp_impl.hpp"
#include <memory>

namespace audio_analysis_lib
{
	using namespace my_world_voice_feature;
	class cheaptrick_sp_impl;

	class cheaptrick_sp
	{
	public:
		cheaptrick_sp(int init_fs);
		void spectrogram_estimated(double* x, int x_length, double* temporal_position, double* f0, int f0_length, double** spectrogram);
		int get_fftsize_for_cheaptrick();
		//void get_sp_size(int& dim1, int& dim2);
	private:
		std::unique_ptr<my_world_voice_feature::cheaptrick_sp_impl> pimpl;
	};
}
