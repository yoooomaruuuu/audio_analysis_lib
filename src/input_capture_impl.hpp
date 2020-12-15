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
			LPDIRECTSOUNDCAPTURE m_capture_device;//DirectSoundCaptureDevice�I�u�W�F�N�g
			LPDIRECTSOUNDCAPTUREBUFFER m_capture_buffer;//DirectSoundCaptureBuffer�I�u�W�F�N�g

			WAVEFORMATEX m_wfx;
			// �P����PCM��WAVE�f�[�^���`
			// wFormatTag�AWave�̃t�H�[�}�b�g
			// nChannels�@���m����1�@�X�e���I2�i�f�[�^�Z�b�g�̎�ށj
			// nSamplesPerSec�@1�b������̃T���v����
			// mAvgBytesPerSec�A1�b������̃o�C�g���BnSamplesPerSec��nBlockAlign�B
			// nBlockAlign 1�T���v���̃o�C�g���BnChannels�~wBitsPerSample��8�@8�E�E�E8�r�b�g=�P�o�C�g
			// wBitsPerSample�@1�T���v��������̃r�b�g���B8��16
			// cbSize�@���0

			DSCBUFFERDESC m_buffer_describer;
			// DirectSound Capture Buffer DESC �L���v�`�� �o�b�t�@���L�q����\����
			// dwSize ���̍\���̂̃T�C�Y(=sizeof(DSCBUFFERDESC))
			// dwFlags �f�o�C�X�t���\�͂̎w��t���O�i���g�p�ɂ�0�j
			// dwBufferBytes �o�b�t�@�T�C�Y(byte)
			// dwReserved �\��̈�(=0)
			// lpwfxFormat �L���v�`���t�H�[�}�b�g��WAVEFORMATX�\���̂Ŏw��
			// dwFXCount �G�t�F�N�g���g�p���Ȃ��ꍇ��0
			// lpDSCFXDesc �n�[�h�E�F�A�T�|�[�g�̃G�t�F�N�g�w��

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
