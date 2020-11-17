#include "input_capture_impl.hpp"

using namespace audio_analysis_lib;

BOOL audio_analysis_lib::DSEnumProc(LPGUID lpGUID, LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext)
{ 
	if (lpGUID != NULL) { //  NULL only for "Primary Sound Driver". 
		((DEVICE_MAP*)lpContext)->first.push_back(lpGUID);
#ifdef UNICODE
		char test[devicesDescMaxSize] = { 0x00 };
		WideCharToMultiByte(CP_ACP, 0, lpszDesc, -1, test, 256, NULL, NULL);
		((DEVICE_MAP*)lpContext)->second.push_back(std::string(test));
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
	//DEVICE_MAP tmp = (std::make_pair(lp_guid, lp_desc));
	DirectSoundCaptureEnumerate((LPDSENUMCALLBACK)DSEnumProc, (VOID*)&devices);
}


void input_capture_impl::init(DWORD sample_rate, WORD channels, WORD bits_per_sample, int frame_ms, int device_index)
{
	m_wfx = { WAVE_FORMAT_PCM, channels, sample_rate, sample_rate * channels * bits_per_sample / 8U, channels * bits_per_sample / 8U, bits_per_sample, 0 };
	m_frame_ms = frame_ms;
	m_buffer_describer = { sizeof(DSCBUFFERDESC), 0, m_wfx.nAvgBytesPerSec * 1, 0, &m_wfx, 0, NULL };
	DirectSoundCaptureCreate8(devices.first[device_index], &m_capture_device, NULL );
	m_copied_buffer = (char*)malloc(
		m_wfx.nAvgBytesPerSec * m_wfx.nChannels * m_frame_ms / 1000.0f); 
	HRESULT tst = m_capture_device->CreateCaptureBuffer(&m_buffer_describer,&m_capture_buffer,NULL);
}

int input_capture_impl::get_buf_size()
{
	return m_wfx.nAvgBytesPerSec * m_frame_ms / 1000.0f;
}

int input_capture_impl::get_input_devices_list_size()
{
	// �T�E���h�f�o�C�X����������ꍇ�Ɏg�p
	return devices.second.size();
}

std::vector<std::string> input_capture_impl::get_input_devices_list()
{
	// �T�E���h�f�o�C�X����������ꍇ�Ɏg�p
	return devices.second;
}

HRESULT input_capture_impl::start()
{
	m_hr = m_capture_buffer->Start(DSCBSTART_LOOPING);
	Sleep(1000);
	return m_hr;
}

HRESULT input_capture_impl::capture_data(char** tmp)
{
	m_capture_buffer->GetCurrentPosition(&m_captured_pos, &m_readable_pos);
	if  ( m_readable_pos > m_read_buffer_pos ) m_lock_length = m_readable_pos - m_read_buffer_pos;
	else m_lock_length = m_buffer_describer.dwBufferBytes - m_read_buffer_pos + m_readable_pos;
	if (m_lock_length < get_buf_size()) return E_FAIL;
	m_lock_length = get_buf_size();

	// printf("Lock startRead:%d, readable:%d, locklen:%d, captured:%d\n",
	//	readBufferPos, readablePos, lockLength, capturedPos);
	//�L���v�`���o�b�t�@��ringbuffer�Ȃ��߁A�̈悪2�ɕ��f����邱�Ƃ�����
	//���̂��߁AwappedCaputreData���p�ӂ��āA2�������o���Ă���
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
		//m_copied_length += m_captured_length;
		m_read_buffer_pos += m_captured_length;
		if (m_read_buffer_pos >= m_buffer_describer.dwBufferBytes)
			m_read_buffer_pos = 0;
	}

	if (m_wrapped_captured_data != NULL) { // Ring buffer wrapped
		memcpy(m_copied_buffer, m_wrapped_captured_data, m_wrapped_captured_length);
		//m_copied_length += m_wrapped_captured_length;
		m_read_buffer_pos = m_wrapped_captured_length;
	}

	m_hr = m_capture_buffer->Unlock( m_captured_data, m_captured_length,
		m_wrapped_captured_data, m_wrapped_captured_length);
	*tmp = m_copied_buffer;
	//Sleep(m_frame_ms);
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

