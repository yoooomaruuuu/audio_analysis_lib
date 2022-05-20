#ifdef _WIN32
#include <dsound.h>
#include <time.h>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <vector>
#include <map>

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")

namespace audio_analysis_lib
{
	namespace my_direct_sound
	{
		constexpr int devicesDescMaxSize = 256;
		using DEVICE_MAP = std::pair < std::vector<LPGUID>, std::vector<std::wstring>> ;

		BOOL CALLBACK DSEnumProc(LPGUID lpGUID, LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext);

		class input_capture_impl
		{
		public:
			input_capture_impl();

			HRESULT init(DWORD sample_rate, WORD channels, WORD bits_per_sample, int frame_ms, int device_index);

			int get_buf_size();

			int get_input_devices_list_size();

			std::vector<std::wstring> get_input_devices_list();

			HRESULT start();

			HRESULT capture_data(char** tmp, int& copy_length);

			HRESULT stop();

			~input_capture_impl();

		private:
			DEVICE_MAP devices;
			LPDIRECTSOUNDCAPTURE m_capture_device;//DirectSoundCaptureDeviceオブジェクト
			LPDIRECTSOUNDCAPTUREBUFFER m_capture_buffer;//DirectSoundCaptureBufferオブジェクト

			WAVEFORMATEX m_wfx;
			// 単純なPCMのWAVEデータを定義
			// wFormatTag、Waveのフォーマット
			// nChannels　モノラル1　ステレオ2（データセットの種類）
			// nSamplesPerSec　1秒あたりのサンプル数
			// mAvgBytesPerSec、1秒あたりのバイト数。nSamplesPerSec＊nBlockAlign。
			// nBlockAlign 1サンプルのバイト数。nChannels×wBitsPerSample÷8　8・・・8ビット=１バイト
			// wBitsPerSample　1サンプルあたりのビット数。8か16
			// cbSize　常に0

			DSCBUFFERDESC m_buffer_describer;
			// DirectSound Capture Buffer DESC キャプチャ バッファを記述する構造体
			// dwSize この構造体のサイズ(=sizeof(DSCBUFFERDESC))
			// dwFlags デバイス付加能力の指定フラグ（未使用につき0）
			// dwBufferBytes バッファサイズ(byte)
			// dwReserved 予約領域(=0)
			// lpwfxFormat キャプチャフォーマットをWAVEFORMATX構造体で指定
			// dwFXCount エフェクトを使用しない場合は0
			// lpDSCFXDesc ハードウェアサポートのエフェクト指定

			DWORD m_readable_pos, m_captured_pos, m_read_buffer_pos, m_lock_length, m_captured_length, m_wrapped_captured_length;
			DWORD m_copied_length;
			void *m_captured_data, *m_wrapped_captured_data;
			char *m_copied_buffer;
			int m_frame_ms;
			HRESULT m_hr;
		};
	}
}
#endif
