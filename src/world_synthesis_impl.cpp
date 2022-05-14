#include "world_synthesis_impl.hpp"

using namespace audio_analysis_lib::my_world_voice_feature;

world_synthesis_impl::world_synthesis_impl(int init_fs, float init_frame_period, int init_input_length)
	: param(), harvest(new harvest_f0(init_fs, init_frame_period)),
	  cheaptrick(new cheaptrick_sp(init_fs)), d4c(new d4c_ap(init_fs)), 
	  frame_length(64), synthesizer({0}), input_length(init_input_length)
{
	param.fs = init_fs;
	param.frame_period = init_frame_period;
	param.f0_length = harvest->get_f0_sample_num(init_input_length);
	param.fft_size = cheaptrick->get_fftsize_for_cheaptrick();
	param.f0 = new double[param.f0_length];
	param.time_axis = new double[param.f0_length];
	param.spectrogram = new double*[param.f0_length];
	param.aperiodicity = new double*[param.f0_length];
	for (int i = 0; i < param.f0_length; i++)
	{
		param.f0[i] = 0.0;
		param.time_axis[i] = 0.0;
		param.spectrogram[i] = new double[param.fft_size / 2.0 + 1];
		param.aperiodicity[i] = new double[param.fft_size / 2.0 + 1];
		for (int j = 0; j < param.fft_size / 2.0 + 1; j++)
		{
			param.spectrogram[i][j] = 0.0;
			param.aperiodicity[i][j] = 0.0;
		}
	}
	InitializeSynthesizer(param.fs, param.frame_period, param.fft_size, frame_length, 1, &synthesizer);
}

world_synthesis_impl::~world_synthesis_impl()
{
	DestroySynthesizer(&synthesizer);
	delete[] param.f0;
	delete[] param.time_axis;
	for (int i = 0; i < param.f0_length; i++)
	{
		delete[] param.spectrogram[i];
		delete[] param.aperiodicity[i];
	}
	delete[] param.spectrogram;
	delete[] param.aperiodicity;
	delete harvest;
	delete cheaptrick;
	delete d4c;
}

void world_synthesis_impl::realtime_synth(double f0_shift_size, double* frame_data, double* output_data)
{
	f0_estimate(frame_data);
	sp_estimate(frame_data);
	ap_estimate(frame_data);
	f0_shift(f0_shift_size);
	RefreshSynthesizer(&synthesizer);
	//AddParameters(param.f0, 1, param.spectrogram, param.aperiodicity, &synthesizer);
	AddParameters(param.f0, param.f0_length, param.spectrogram, param.aperiodicity, &synthesizer);
	for (int i = 0; Synthesis2(&synthesizer) != 0; i++)
	{
		int index = i * frame_length;
		for (int j = 0; j < frame_length; j++)
		{
			output_data[j + index] = synthesizer.buffer[j];
		}
	}
}


void world_synthesis_impl::f0_estimate(double* frame_data)
{
	harvest->f0_estimate(frame_data, input_length, param.time_axis, param.f0);
}
void world_synthesis_impl::sp_estimate(double* frame_data)
{
	cheaptrick->spectrogram_estimated(frame_data, input_length, param.time_axis, param.f0, param.f0_length, param.spectrogram);
}
void world_synthesis_impl::ap_estimate(double* frame_data)
{
	d4c->aperiodicity_estimated(frame_data, input_length, param.time_axis, param.f0, param.f0_length, param.fft_size, param.aperiodicity);
}
void world_synthesis_impl::f0_shift(double shift)
{
	for (int i = 0; i < param.f0_length; i++)
	{
		param.f0[i] *= shift;
	}
}



