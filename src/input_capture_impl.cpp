#include "input_capture_impl.hpp"

using namespace audio_analysis_lib;

BOOL audio_analysis_lib::DSEnumProc(LPGUID lpGUID, LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext)
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
	DirectSoundCaptureCreate8(NULL, &m_capture_device, NULL );
	DirectSoundCaptureEnumerate((LPDSENUMCALLBACK)DSEnumProc, (VOID*)&devices);
}


HRESULT input_capture_impl::init(DWORD sample_rate, WORD channels, WORD bits_per_sample, int frame_ms, int device_index)
{
	m_wfx = { WAVE_FORMAT_PCM, channels, sample_rate, sample_rate * channels * bits_per_sample / 8U, channels * bits_per_sample / 8U, bits_per_sample, 0 };
	m_frame_ms = frame_ms;
	m_buffer_describer = { sizeof(DSCBUFFERDESC), 0, m_wfx.nAvgBytesPerSec * 1, 0, &m_wfx, 0, NULL };
	HRESULT hr = DirectSoundCaptureCreate8(devices.first[device_index], &m_capture_device, NULL );
	if (hr != DS_OK)
	{
		return hr;
	}
	m_copied_buffer = (char*)malloc(
		m_wfx.nAvgBytesPerSec * m_wfx.nChannels * m_frame_ms / 1000.0f); 
	hr = m_capture_device->CreateCaptureBuffer(&m_buffer_describer,&m_capture_buffer,NULL);
	if (hr != DS_OK)
	{
		free(m_copied_buffer);
		return hr;
	}
	return hr;
}

int input_capture_impl::get_buf_size()
{
	return m_wfx.nAvgBytesPerSec * m_frame_ms / 1000.0f;
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
	Sleep(3000);
	return m_hr;
}

HRESULT input_capture_impl::capture_data(char** tmp)
{
	m_capture_buffer->GetCurrentPosition(&m_captured_pos, &m_readable_pos);
	if  ( m_readable_pos > m_read_buffer_pos ) m_lock_length = m_readable_pos - m_read_buffer_pos;
	else m_lock_length = m_buffer_describer.dwBufferBytes - m_read_buffer_pos + m_readable_pos;
	//取得できたバッファサイズがget_buf_sizeより小さい場合、error
	if (m_lock_length < get_buf_size()) return E_FAIL;
	m_lock_length = get_buf_size();

	//キャプチャバッファはringbufferなため、領域が2つに分断されることがある
	//そのため、wappedCaputreDataも用意して、2つを書き出している
	m_hr = m_capture_buffer->Lock(m_read_buffer_pos, m_lock_length,
		&m_captured_data, &m_captured_length,
		&m_wrapped_captured_data, &m_wrapped_captured_length,
		NULL);
	if (m_hr != DS_OK) {
		printf("Lock error:%x\n", m_hr);
		return m_hr;
	}

	if (m_captured_data != NULL) {
		memcpy(m_copied_buffer, m_captured_data, m_captured_length);
		m_read_buffer_pos += m_captured_length;
		if (m_read_buffer_pos >= m_buffer_describer.dwBufferBytes)
			m_read_buffer_pos = 0;
	}

	if (m_wrapped_captured_data != NULL) { // Ring buffer wrapped
		memcpy(m_copied_buffer, m_wrapped_captured_data, m_wrapped_captured_length);
		m_read_buffer_pos = m_wrapped_captured_length;
	}

	m_hr = m_capture_buffer->Unlock( m_captured_data, m_captured_length,
		m_wrapped_captured_data, m_wrapped_captured_length);
	*tmp = m_copied_buffer;
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

