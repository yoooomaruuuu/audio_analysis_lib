using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace debugLibCs
{
    class Program
    {
        public enum fft_mode
        {
            FFT = 0,
            IFFT = 1,
            ERROR = -1
        }

        public enum fft_exception
        {
            SUCCESS = 0,
            SETTING_ERROR = 1,
            DATA_OUT_OF_RANGE = 2,
            MODE_ERROR = 3
        }

        [DllImport("lib_audio_analysis.dll", EntryPoint = "init_fft_component", CallingConvention = CallingConvention.StdCall)]
        static extern void init_fft_component(int fft_size, IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "fft", CallingConvention = CallingConvention.StdCall)]
        static extern fft_exception fft(IntPtr input_re, IntPtr input_im, IntPtr output_re, IntPtr output_im, IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "ifft", CallingConvention = CallingConvention.StdCall)]
        static extern fft_exception ifft(IntPtr input_re, IntPtr input_im, IntPtr output_re, IntPtr output_im, IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "fft_mode_setting", CallingConvention = CallingConvention.StdCall)]
        static extern fft_exception fft_mode_setting(fft_mode mode, IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "get_fft_size", CallingConvention = CallingConvention.StdCall)]
        static extern int get_fft_size(IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "delete_fft_component", CallingConvention = CallingConvention.StdCall)]
        static extern void delete_fft_component(IntPtr func_object);

        static void Main(string[] args)
        {
            //IntPtr m_input_re;
            //IntPtr m_input_im;
            //IntPtr m_output_re;
            //IntPtr m_output_im;
            //int m_frame_size;

            IntPtr fft_object = new IntPtr();

            const int FFT_SIZE = 1024;
            init_fft_component(FFT_SIZE, fft_object);

            get_fft_size(fft_object);

            delete_fft_component(fft_object);

        }
    }
}
