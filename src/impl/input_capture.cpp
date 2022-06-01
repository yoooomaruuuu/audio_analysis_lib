#ifdef _WIN32
#include "input_capture.hpp"
#include <thread>

// TODO: VS側でリンカ指定可能では?
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")

namespace audio_analysis_lib
{
	namespace my_direct_sound
	{
		constexpr int devicesDescMaxSize = 256;
		using DEVICE_MAP = std::pair < std::vector<LPGUID>, std::vector<std::wstring>>;

		BOOL CALLBACK DSEnumProc(LPGUID lpGUID, LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext);

		// impl宣言と実装
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
			void* m_captured_data, * m_wrapped_captured_data;
			char* m_copied_buffer;
			int m_frame_ms;
			HRESULT m_hr;
		};

		BOOL audio_analysis_lib::my_direct_sound::DSEnumProc(LPGUID lpGUID, LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext)
		{
			if (lpGUID != NULL) { //  NULL only for "Primary Sound Driver". 
				((DEVICE_MAP*)lpContext)->first.push_back(lpGUID);
#ifdef UNICODE
				((DEVICE_MAP*)lpContext)->second.push_back(std::wstring(lpszDesc));
#else
				((DEVICE_MAP*)lpContext)->second.push_back(lpszDesc);
#endif
			}
			return TRUE;
		}

		input_capture_impl::input_capture_impl()
			: m_capture_device(NULL), m_capture_buffer(NULL),
			m_wfx({ WAVE_FORMAT_PCM, 0, 0, 0, 0, 0, 0 }),
			m_buffer_describer({}),
			m_readable_pos(0), m_captured_pos(0), m_read_buffer_pos(0), m_lock_length(0), m_captured_length(0), m_wrapped_captured_length(0), m_copied_length(0),
			m_captured_data(nullptr), m_wrapped_captured_data(nullptr), m_copied_buffer(nullptr), m_hr(S_OK), devices(std::make_pair(0, 0)), m_frame_ms(0)
		{
			CoInitialize(NULL);
			DirectSoundCaptureCreate8(NULL, &m_capture_device, NULL);
			DirectSoundCaptureEnumerate((LPDSENUMCALLBACK)DSEnumProc, (VOID*)&devices);
		}


		HRESULT input_capture_impl::init(DWORD sample_rate, WORD channels, WORD bits_per_sample, int frame_ms, int device_index)
		{
			m_wfx = { WAVE_FORMAT_PCM, channels, sample_rate, sample_rate * channels * bits_per_sample / 8U, channels * bits_per_sample / 8U, bits_per_sample, 0 };
			m_frame_ms = frame_ms;
			m_buffer_describer = { sizeof(DSCBUFFERDESC), 0, m_wfx.nAvgBytesPerSec * 1, 0, &m_wfx, 0, NULL };
			HRESULT hr = DirectSoundCaptureCreate8(devices.first[device_index], &m_capture_device, NULL);
			if (hr != DS_OK)
			{
				return hr;
			}
			m_copied_buffer = (char*)malloc(
				m_wfx.nAvgBytesPerSec);// *m_wfx.nChannels* m_frame_ms / 1000.0f);
			hr = m_capture_device->CreateCaptureBuffer(&m_buffer_describer, &m_capture_buffer, NULL);
			if (hr != DS_OK)
			{
				free(m_copied_buffer);
				return hr;
			}
			return hr;
		}

		int input_capture_impl::get_buf_size()
		{
			return m_wfx.nAvgBytesPerSec; // *m_frame_ms / 1000.0f;
		}

		int input_capture_impl::get_input_devices_list_size()
		{
			// サウンドデバイスが複数ある場合に使用
			return devices.second.size();
		}

		std::vector<std::wstring> input_capture_impl::get_input_devices_list()
		{
			// サウンドデバイスが複数ある場合に使用
			return devices.second;
		}

		HRESULT input_capture_impl::start()
		{
			m_hr = m_capture_buffer->Start(DSCBSTART_LOOPING);
			Sleep(100);
			return m_hr;
		}

		HRESULT input_capture_impl::capture_data(char** tmp, int& copy_length)
		{
			m_capture_buffer->GetCurrentPosition(&m_captured_pos, &m_readable_pos);
			if (m_readable_pos > m_read_buffer_pos) m_lock_length = m_readable_pos - m_read_buffer_pos;
			// readable_posとread_buffer_posが一致しているときは大きくとってしまうため、切って置く
			else if (m_readable_pos == m_read_buffer_pos) return E_FAIL;
			else m_lock_length = m_buffer_describer.dwBufferBytes - m_read_buffer_pos + m_readable_pos;
			////取得できたバッファサイズがget_buf_sizeより小さい場合、error
			//if (m_lock_length < get_buf_size()) return E_FAIL;
			//m_lock_length = get_buf_size();

			printf("Lock startRead:%d, readable:%d, locklen:%d, captured:%d\n",
				m_read_buffer_pos, m_readable_pos, m_lock_length, m_captured_pos);
			////キャプチャバッファはringbufferなため、領域が2つに分断されることがある
			////そのため、wappedCaputreDataも用意して、2つを書き出している
			m_hr = m_capture_buffer->Lock(m_read_buffer_pos, m_lock_length, &m_captured_data, &m_captured_length, &m_wrapped_captured_data, &m_wrapped_captured_length, NULL);
			if (m_hr != DS_OK) {
				printf("Lock error:%x\n", m_hr);
				return m_hr;
			}
			else {
				printf("buffer read, buf1:%d, buf2:%d\n", m_captured_length, m_wrapped_captured_length);
			}

			if (m_captured_data != NULL) {
				memcpy(m_copied_buffer, m_captured_data, m_captured_length);
				copy_length = m_captured_length;
				m_read_buffer_pos += m_captured_length;
				if (m_read_buffer_pos >= m_buffer_describer.dwBufferBytes)
					m_read_buffer_pos = 0;
			}

			if (m_wrapped_captured_data != NULL) { // Ring buffer wrapped
				memcpy(m_copied_buffer, m_wrapped_captured_data, m_wrapped_captured_length);
				copy_length += m_wrapped_captured_length;
				m_read_buffer_pos = m_wrapped_captured_length;
			}

			m_hr = m_capture_buffer->Unlock(m_captured_data, m_captured_length, m_wrapped_captured_data, m_wrapped_captured_length);
			*tmp = m_copied_buffer;
			std::this_thread::sleep_for(std::chrono::milliseconds(m_frame_ms));
			return DS_OK;
		}

		HRESULT input_capture_impl::stop()
		{
			m_hr = m_capture_buffer->Stop();
			return m_hr;
		}

		input_capture_impl::~input_capture_impl()
		{
			free(m_copied_buffer);
			CoUninitialize();
		}
	}

	// input_capture実装
	input_capture::input_capture()
	:ptr(std::make_unique<my_direct_sound::input_capture_impl>())
	{
	}

	HRESULT input_capture::init(DWORD sample_rate, WORD channels, WORD bits_per_sample, int frame_ms, int device_index)
	{
	return ptr->init(sample_rate, channels, bits_per_sample, frame_ms, device_index);
	}

	int input_capture::get_buf_size()
	{
	return ptr->get_buf_size();
	}

	int input_capture::get_input_devices_list_size()
	{
	return ptr->get_input_devices_list_size();
	}

	std::vector<std::wstring> input_capture::get_input_devices_list()
	{
	return ptr->get_input_devices_list();
	}

	HRESULT input_capture::start()
	{
	return ptr->start();
	}

	HRESULT input_capture::capture_data(char** tmp, int& copy_length)
	{
	return ptr->capture_data(tmp, copy_length);
	}

	HRESULT input_capture::stop()
	{
	return ptr->stop();
	}

	input_capture::~input_capture()
	{
	}
}

// DLL関数実装
DLLEXPORT void audio_analysis_lib::create_input_capture(void** func_object)
{
	*func_object = new audio_analysis_lib::input_capture();
}

DLLEXPORT HRESULT audio_analysis_lib::init_input_capture(DWORD sample_rate, WORD channels, WORD bits_per_sample, int frame_ms, int device_index, void* func_object)
{
	return ((audio_analysis_lib::input_capture*)func_object)->init(sample_rate, channels, bits_per_sample, frame_ms, device_index);
}

DLLEXPORT int audio_analysis_lib::get_buf_size(void* func_object)
{
	return ((audio_analysis_lib::input_capture*)func_object)->get_buf_size();
}

DLLEXPORT int audio_analysis_lib::get_input_devices_list_size(void* func_object)
{
	return ((audio_analysis_lib::input_capture*)func_object)->get_input_devices_list_size();
}

DLLEXPORT void audio_analysis_lib::get_input_devices_list(int index, wchar_t* tmp, void* func_object)
{
	wcscpy_s(tmp, my_direct_sound::devicesDescMaxSize, ((audio_analysis_lib::input_capture*)func_object)->get_input_devices_list()[index].c_str());
}

DLLEXPORT HRESULT audio_analysis_lib::start(void* func_object)
{
	return ((audio_analysis_lib::input_capture*)func_object)->start();
}

DLLEXPORT HRESULT audio_analysis_lib::caputre_data(char** tmp, int& copy_length, void* func_object)
{
	return ((audio_analysis_lib::input_capture*)func_object)->capture_data(tmp, copy_length);
}

DLLEXPORT HRESULT audio_analysis_lib::stop(void* func_object)
{
	return ((audio_analysis_lib::input_capture*)func_object)->stop();
}

DLLEXPORT void audio_analysis_lib::delete_input_capture(void** func_object)
{
	delete ((audio_analysis_lib::input_capture*)*func_object);
}
#endif

