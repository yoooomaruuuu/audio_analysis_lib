using System;
using System.Runtime.InteropServices;
using System.IO;

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
        static extern void init_fft_component(int fft_size, ref IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "mylib_fft", CallingConvention = CallingConvention.StdCall)]
        static extern fft_exception mylib_fft(float[] input_re, float[] input_im, float[] output_re, float[] output_im, IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "mylib_ifft", CallingConvention = CallingConvention.StdCall)]
        static extern fft_exception mylib_ifft(float[] input_re, float[] input_im, float[] output_re, float[] output_im, IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "fft_mode_setting", CallingConvention = CallingConvention.StdCall)]
        static extern fft_exception fft_mode_setting(fft_mode mode, IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "get_fft_size", CallingConvention = CallingConvention.StdCall)]
        static extern int get_fft_size(IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "delete_fft_component", CallingConvention = CallingConvention.StdCall)]
        static extern void delete_fft_component(ref IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "hann_window", CallingConvention = CallingConvention.StdCall)]
        static extern float hann_window(float x);

        static void Main(string[] args)
        {
            IntPtr fft_object = new IntPtr();
            IntPtr ifft_object = new IntPtr();

            const int FFT_SIZE = 1024;
            init_fft_component(FFT_SIZE, ref fft_object);
            init_fft_component(FFT_SIZE, ref ifft_object);

            get_fft_size(fft_object);
            get_fft_size(ifft_object);

            fft_mode_setting(fft_mode.FFT, fft_object);
            fft_mode_setting(fft_mode.IFFT, ifft_object);

            float[] input_re = new float[FFT_SIZE];
            float[] input_im = new float[FFT_SIZE];
            float[] output_re = new float[FFT_SIZE];
            float[] output_im = new float[FFT_SIZE];
            float[] input2_re = new float[FFT_SIZE];
            float[] input2_im = new float[FFT_SIZE];

            var input_file_name = @"input.pcm";
            var output_file_name = @"output.pcm";
            var input_stream = new BinaryReader(new FileStream(input_file_name, FileMode.Open));
            var output_stream = new BinaryReader(new FileStream(output_file_name, FileMode.OpenOrCreate));

            try
            {
                while (true)
                {
                    for (int i = 0; i < FFT_SIZE; i++)
                    {
                        input_re[i] = hann_window(input_stream.ReadSingle());
                    }

                    var res = mylib_fft(input_re, input_im, output_re, output_im, fft_object);

                    for (int i = 0; i < FFT_SIZE; i++)
                    {
                        output_re[i] /= FFT_SIZE;
                        output_im[i] /= FFT_SIZE;
                    }

                    res = mylib_ifft(output_re, output_im, input2_re, input2_im, ifft_object);

                    if (output_re[0] != 0) Console.WriteLine(output_re[0]);
                }
            }
            catch (EndOfStreamException e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                delete_fft_component(ref fft_object);
                input_stream.Close();
                output_stream.Close();
            }

        }
    }
}
