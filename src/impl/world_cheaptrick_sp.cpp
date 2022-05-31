#include "world_cheaptrick_sp.hpp"
#include "world/cheaptrick.h"

namespace audio_analysis_lib
{
	namespace my_world_voice_feature
	{
		// implÇÃêÈåæÅAé¿ëï
		class cheaptrick_sp_impl
		{
		public:
			cheaptrick_sp_impl(int init_fs);
			~cheaptrick_sp_impl();
			void spectrogram_estimated(double* x, int x_length, double* temporal_position, double* f0, int f0_length, double** spectrogram);
			int get_fftsize_for_cheaptrick();
			//void get_sp_size(int& dim1, int& dim2);
		private:
			CheapTrickOption option;
			int fs;
		};

		cheaptrick_sp_impl::cheaptrick_sp_impl(int init_fs)
			: fs(init_fs), option({ 0 })
		{
			InitializeCheapTrickOption(fs, &option);
			option.f0_floor = 71.0;
		}
		
		cheaptrick_sp_impl::~cheaptrick_sp_impl(){}

		void cheaptrick_sp_impl::spectrogram_estimated(double* x, int x_length, double* temporal_position, double* f0, int f0_length, double** spectrogram)
		{
			CheapTrick(x, x_length, fs, temporal_position, f0, f0_length, &option, spectrogram);

		}
		int cheaptrick_sp_impl::get_fftsize_for_cheaptrick()
		{
			return GetFFTSizeForCheapTrick(fs, &option);
		}
	}


	// cheaptrickÇÃé¿ëï
	cheaptrick_sp::cheaptrick_sp(int init_fs)
		: pimpl(std::make_unique<my_world_voice_feature::cheaptrick_sp_impl>(init_fs))
	{ }

	cheaptrick_sp::~cheaptrick_sp(){}

	void cheaptrick_sp::spectrogram_estimated(double* x, int x_length, double* temporal_position, double* f0, int f0_length, double** spectrogram)
	{
		pimpl->spectrogram_estimated(x, x_length, temporal_position, f0, f0_length, spectrogram);
	}

	int cheaptrick_sp::get_fftsize_for_cheaptrick()
	{
		return pimpl->get_fftsize_for_cheaptrick();
	}
}

