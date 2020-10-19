#include <iostream>
#include "fft_component.hpp"

int main()
{	
	constexpr int FRAMESIZE = 1024;
	constexpr int SAMPLERATE = 44100;
	audio_analysis_lib::fft_component fft(FRAMESIZE);
	FILE *fin, *fout;
	//const int FRAMESIZE = fft.getFFTSize() * 2.0;

	if (fopen_s(&fin, "input.pcm", "rb") != 0) return 1;
	if (fopen_s(&fout, "output.pcm", "wb") != 0) return 1;
	
	float* inputRe = new float[FRAMESIZE];
	float* inputIm = new float[FRAMESIZE];
	float* waveAnaRe = new float[FRAMESIZE];
	float* waveAnaIm = new float[FRAMESIZE];
	float* outputRe = new float[FRAMESIZE];
	float* outputIm = new float[FRAMESIZE];
	for (int i = 0; i < FRAMESIZE; i++)
	{
		inputRe[i] = 0.0;
		inputIm[i] = 0.0;
		waveAnaRe[i] = 0.0;
		waveAnaIm[i] = 0.0;
		outputRe[i] = 0.0;
		outputIm[i] = 0.0;
	}

	while(fread(inputRe, sizeof(float), FRAMESIZE, fin) == FRAMESIZE)
	{
		fft.fft(inputRe, inputIm, waveAnaRe, waveAnaIm, false);
		fft.fft(waveAnaRe, waveAnaIm, outputRe, outputIm, true);

		for (int i = 0; i < FRAMESIZE; i++)
		{
			outputRe[i] /= FRAMESIZE;
		}

		fwrite(outputRe, sizeof(float), FRAMESIZE, fout);
	}

	delete[] inputRe;
	delete[] inputIm;
	delete[] waveAnaRe;
	delete[] waveAnaIm;
	delete[] outputRe;
	delete[] outputIm;

	fclose(fin);
	fclose(fout);

	return 0;
}