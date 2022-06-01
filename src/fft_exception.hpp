#pragma once
namespace audio_analysis_lib
{
	// fft_componentにおける返り値
	enum class fft_exception : int
	{
		SUCCESS = 0,
		SETTINNG_ERROR = 1,
		DATA_OUT_OF_RANGE = 2,
		MODE_ERROR = 3
	};
}
