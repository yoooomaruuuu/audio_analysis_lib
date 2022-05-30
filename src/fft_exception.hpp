#pragma once
namespace audio_analysis_lib
{
	namespace my_world_voice_feature
	{
		enum class fft_exception : int
		{
			SUCCESS = 0,
			SETTINNG_ERROR = 1,
			DATA_OUT_OF_RANGE = 2,
			MODE_ERROR = 3
		};
	}
}
