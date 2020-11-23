#define FFT
#define WORLD
#define INPUT

//#undef FFT
#undef WORLD
#undef INPUT
using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
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

        [DllImport("lib_audio_analysis.dll", EntryPoint = "create_fft_component", CallingConvention = CallingConvention.StdCall)]
        static extern void create_fft_component(int fft_size, ref IntPtr func_object);

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


        [DllImport("lib_audio_analysis.dll", EntryPoint = "create_input_capture", CallingConvention = CallingConvention.StdCall)]
        static extern void create_input_capture(ref IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "delete_input_capture", CallingConvention = CallingConvention.StdCall)]
        static extern void delete_input_capture(ref IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "get_input_devices_list", CallingConvention = CallingConvention.StdCall)]
        static extern void get_input_devices_list(int index, StringBuilder tmp, IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "get_input_devices_list_size", CallingConvention = CallingConvention.StdCall)]
        static extern int get_input_devices_list_size(IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "init_input_capture", CallingConvention = CallingConvention.StdCall)]
        static extern void init_input_capture(UInt32 sample_rate, UInt16 channels, UInt16 bits_per_sample, Int32 frame_ms, int device_index, IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "get_buf_size", CallingConvention = CallingConvention.StdCall)]
        static extern int get_buf_size(IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "start", CallingConvention = CallingConvention.StdCall)]
        static extern long start(IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "caputre_data", CallingConvention = CallingConvention.StdCall)]
        static extern long caputre_data(ref IntPtr data, IntPtr func_object);

        [DllImport("lib_audio_analysis.dll", EntryPoint = "stop", CallingConvention = CallingConvention.StdCall)]
        static extern long stop(IntPtr func_object);

        [StructLayout(LayoutKind.Sequential)]
        public struct HarvestOption {
            public double f0Floor;
            public double f0Ceil;
            public double framePeriod;
        }
        [DllImport("win.dll", EntryPoint = "Harvest", CallingConvention = CallingConvention.StdCall)]
        static extern void Harvest(double[] x, int xLength, int fs, ref HarvestOption option, double[] temporalPositions, double[] f0);

        [DllImport("win.dll", EntryPoint = "InitializeHarvestOption", CallingConvention = CallingConvention.StdCall)]
        static extern void InitializeHarvestOption(ref HarvestOption option);

        [DllImport("win.dll", EntryPoint = "GetSamplesForHarvest", CallingConvention = CallingConvention.StdCall)]
        static extern int GetSamplesForHarvest(int fs, int xLength, double framePeriod);

#if FFT
        static void Main(string[] args)
        {
            IntPtr fft_object = new IntPtr();
            IntPtr ifft_object = new IntPtr();

            const int FFT_SIZE = 1024;
            create_fft_component(FFT_SIZE, ref fft_object);
            create_fft_component(FFT_SIZE, ref ifft_object);

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
#elif INPUT
        static void Main(string[] args)
        {
            IntPtr input_cap = new IntPtr();
            create_input_capture(ref input_cap);
            var list = new StringBuilder[get_input_devices_list_size(input_cap)];
            for(int i=0; i<get_input_devices_list_size(input_cap); i++)
            {
                list[i] = new StringBuilder(256, 256);
                get_input_devices_list(i, list[i], input_cap);
            }

            init_input_capture(48000, 2, 16, 16, 0, input_cap);
            var output_file_name = @"output.pcm";
            var output_stream = new BinaryWriter(new FileStream(output_file_name, FileMode.Create));

            var data = new byte[get_buf_size(input_cap)];
            IntPtr data_ptr = new IntPtr();
            data_ptr = Marshal.AllocCoTaskMem(get_buf_size(input_cap));

            start(input_cap);

            int count = 0;
            Console.WriteLine("start");
            while(true)
            {
                if(caputre_data(ref data_ptr, input_cap) == 0)
                {
                    Marshal.Copy(data_ptr, data, 0, get_buf_size(input_cap));
                    output_stream.Write(data, 0, get_buf_size(input_cap));
                    count++;
                }
                if (count == 1000) break;
                //Thread.Sleep(1);
            }

            stop(input_cap);

            //IntPtr list_str = Marshal.AllocCoTaskMem(get_input_devices_list_size(input_cap));

            //stringのポインタを渡す形ではなく、配列の各要素を直接書き込む形にする
            //stringの要素分の配列を作成し、ref stringで渡して内部を弄る感じ？

            output_stream.Close();
            delete_input_capture(ref input_cap);
            //Marshal.FreeCoTaskMem(data_ptr);
       }
#elif WORLD

        static void Main(string[] args)
        {
            var input_file_name = @"test_cap.pcm";
            var input_stream = new BinaryReader(new FileStream(input_file_name, FileMode.Open));
            HarvestOption option = new HarvestOption();
            InitializeHarvestOption(ref option);
            option.f0Ceil = 8000;
            option.f0Floor = 40;
            option.framePeriod = 16;
            int dataSize = 2048;
            double[] inputData = new double[dataSize];
            for(int i=0; i<dataSize; i++)
            {
                inputData[i] = (double)input_stream.ReadInt16(); // 32767;
            }
            for(int i=0; i<dataSize; i++)
            {
                inputData[i] = inputData[i] / 32767.0;
            }
            double[] tmp = new double[GetSamplesForHarvest(44100, dataSize, 16)];
            double[] f0 = new double[GetSamplesForHarvest(44100, dataSize, 16)];
            Harvest(inputData, dataSize, 44100, ref option, tmp, f0);
            
        }
#endif
    }
}
