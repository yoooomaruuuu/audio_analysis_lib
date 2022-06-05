#pragma once
#include <iostream>

namespace audio_analysis_lib
{
	struct RIFF
	{
		uint32_t chunk_size = 0;
	};
	enum class WAV_FORMAT_CODE : uint16_t
	{
		WAVE_FORMAT_PCM = 0x0001
	};
	
	struct fmt
	{
		uint16_t wav_format_code = 0;
		uint16_t ch = 0;
		uint32_t sampling_freq = 0;
		uint32_t byte_per_sec = 0;
		uint16_t block_align = 0;
		uint16_t bit_per_sample = 0;
	};
	struct data
	{
		uint32_t wave_data_size = 0;
		void* wave_data = nullptr;
	};
	struct wav_data
	{

	};
}
