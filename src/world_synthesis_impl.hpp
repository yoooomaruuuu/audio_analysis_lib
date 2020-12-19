#pragma once
#include <iostream>
#include "world/synthesisrealtime.h"
#include "world_harvest_f0.hpp"
#include "world_cheaptrick_sp.hpp"
#include "world_d4c_ap.hpp"

namespace audio_analysis_lib
{
    namespace my_world_voice_feature
    {
		typedef struct {
		  double frame_period;
		  int fs;

		  double *f0;
		  double *time_axis;
		  int f0_length;

		  double **spectrogram;
		  double **aperiodicity;
		  int fft_size;
		} world_parameters;


		class world_synthesis_impl
		{
		public:
			world_synthesis_impl(int init_fs, float init_frame_period);
			~world_synthesis_impl();
			void realtime_synth(double f0_shift, double* frame_data, double* output_data);
		private:
			void f0_estimate(double* frame_data);
			void sp_estimate(double* frame_data);
			void ap_estimate(double* frame_data);
			void f0_shift(double shift);
			world_parameters* param;
			WorldSynthesizer synthesizer;
			harvest_f0* harvest;
			cheaptrick_sp* cheaptrick;
			d4c_ap* d4c;
			int frame_length;
		};
    }
}
