#pragma once
#include <memory>

namespace audio_analysis_lib
{
	// 前方宣言
	namespace my_world_voice_feature
	{
		class d4c_ap_impl;
	}
	
	// ap推定
	class d4c_ap
	{
	public:
		d4c_ap(int init_fs);
		~d4c_ap();
		void aperiodicity_estimated(const double *x, int x_length, const double *temporal_position, const double *f0, int f0_length, int fft_size, double **aperiodicity);
	private:
		std::unique_ptr<my_world_voice_feature::d4c_ap_impl> pimpl;
	};
}
