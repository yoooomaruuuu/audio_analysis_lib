#pragma once
#include "world\cheaptrick.h"

namespace audio_analysis_lib
{
	namespace my_world_voice_feature
	{
		class cheaptrick_sp_impl
		{
		public:
			cheaptrick_sp_impl(int init_fs);
			void spectrogram_estimated(double* x, int x_length, double* temporal_position, double* f0, int f0_length, double** spectrogram);
			int get_fftsize_for_cheaptrick();
			//void get_sp_size(int& dim1, int& dim2);
		private:
			CheapTrickOption option;
			int fs;
		};
	}
}
