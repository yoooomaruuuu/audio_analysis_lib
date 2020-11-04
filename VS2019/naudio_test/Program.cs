using System;
using System.Runtime.InteropServices;
using System.IO;
using System.Collections.Generic;
using NAudio.Wave;
using System.Net.Sockets;
using System.Net;

namespace naudio_test
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
            const bool FILEDEBUG = false;
            const int FFTSIZE = 256;
            IntPtr fftfuncs = new IntPtr();
            init_fft_component(FFTSIZE, ref fftfuncs);
            fft_mode_setting(fft_mode.FFT, fftfuncs);
            float[] input_real = new float[FFTSIZE];
            float[] input_imad = new float[FFTSIZE];
            float[] output_real = new float[FFTSIZE];
            float[] output_imad = new float[FFTSIZE];
            for(int i=0; i<FFTSIZE; i++)
            {
                input_real[i] = 0.0f;
                input_imad[i] = 0.0f;
                output_real[i] = 0.0f;
                output_imad[i] = 0.0f;
            }
            Console.WriteLine(WasapiLoopbackCapture.GetDefaultCaptureDevice());
            var devCnt = WaveInEvent.DeviceCount;
            for(int i=0; i<devCnt; i++)
            {
                Console.WriteLine(i + ": " + WaveInEvent.GetCapabilities(i).ProductName);
            }

            Console.Write("inputDeviceNumber : ");
            var line = System.Console.ReadLine();
            var deviceNumber = Int32.Parse(line);

            var waveIn = new WaveInEvent();
            int samplingRate = 48000;
            waveIn.DeviceNumber = deviceNumber;
            waveIn.WaveFormat = new WaveFormat(samplingRate, WaveIn.GetCapabilities(deviceNumber).Channels);
            waveIn.BufferMilliseconds = 16;
            int bufferSize = (int)((waveIn.BufferMilliseconds / 1000.0f) * samplingRate);

            var waveWriter = new WaveFileWriter("tmp.wav", new WaveFormat(48000, 16, 1));

            var waveBuffer1ch = new float[bufferSize];

            UdpClient udp = new UdpClient();
            string hostip = "127.0.0.1";
            int hostPort = 2222;
            waveIn.DataAvailable += (_, ee) =>
            {
                if(FILEDEBUG)
                {
                    waveWriter.Write(ee.Buffer, 0, ee.BytesRecorded);
                    waveWriter.Flush();
                }
                //for(int i=0; i<ee.BytesRecorded; i+=waveIn.WaveFormat.BlockAlign)
                //{
                //    var sample1ch = new byte[] { ee.Buffer[i], ee.Buffer[i + 1]};
                //    if (FILEDEBUG)
                //    {
                //        //waveWriter.Write(sample2ch, 0, 2);
                //        //waveWriter.Flush();
                //    }
                //    waveBuffer1ch[i] = BitConverter.ToInt16(sample1ch, 0);
                //}

                udp.Send(ee.Buffer, ee.BytesRecorded, hostip, hostPort);
            };
            waveIn.RecordingStopped += (_, __) =>
            {
                waveWriter.Flush();
            };

            waveIn.StartRecording();
            Console.Write("何らかの文字を入力したら終了");
            Console.ReadLine();
            waveIn?.StopRecording();
            waveIn?.Dispose();
            waveIn = null;

            waveWriter?.Close();
            waveWriter = null;

        }
    }
}
