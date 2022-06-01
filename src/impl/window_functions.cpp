#include "window_functions.hpp"

// 窓関数定義
float hann_window(float x)
{
	return 0.5f - cos(2.f * M_PI * x) * 0.5f;
}

float hamming_window(float x)
{
	return 0.54f - cos(2.f * M_PI * x) * 0.46f;
}
