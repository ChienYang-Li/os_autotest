#include "buzzercheck.h"

#define REAL 0
#define IMAG 1
#define DOWNSAMPLE_RATE 1

void acquire_data(fftw_complex* signal,AudioFile<double>& file) {
    /* Generate two sine waves of different frequencies and
     * amplitudes.
     */
    int numSamples = file.getNumSamplesPerChannel();
    int i,idx=0;
    int channel = 0;
    for (i = 0; i < numSamples; i+=DOWNSAMPLE_RATE) {
        signal[idx][REAL] = file.samples[channel][i];
        signal[idx][IMAG] = 0;
        ++idx;
    }
}

void compute_magnitude(fftw_complex* result,double* mag,int num) {
    int i;
    for (i = 0; i < num; ++i) {
        mag[i] = sqrt(result[i][REAL] * result[i][REAL] +
                      result[i][IMAG] * result[i][IMAG]);

        //printf("%g\n", mag);
    }
}


int main(int argc, char* argv[]) {
    AudioFile<double> audioFile;
    audioFile.load(argv[1]);
    int sampleRate = audioFile.getSampleRate();
    int bitDepth = audioFile.getBitDepth();

    int numSamples = audioFile.getNumSamplesPerChannel();
    double lengthInSeconds = audioFile.getLengthInSeconds();

    int numChannels = audioFile.getNumChannels();
    bool isMono = audioFile.isMono();
    bool isStereo = audioFile.isStereo();

    // or, just use this quick shortcut to print a summary to the console
    audioFile.printSummary();

    int channel = 0;
    //int numSamples = audioFile.getNumSamplesPerChannel();
    int idx = 0;
    // std::ofstream ofs;
    // ofs.open("wav.txt");
    // if (!ofs.is_open()) {
    //     std::cout << "Failed to open file.\n";
    //     return 1; // EXIT_FAILURE
    // }

    // std::cout << "write txt start\n";
    // for (int i = 0; i < numSamples; i+=DOWNSAMPLE_RATE)
    // {
    //     double currentSample = audioFile.samples[channel][i];
    //     ofs << audioFile.samples[channel][i] << "\n";
    // }
    // ofs.close();
    //std::cout << "write txt end\n";

    //ofs.open("wav_fft.txt");
    int N = numSamples/DOWNSAMPLE_RATE;
    fftw_complex *in, *out;
    fftw_plan p;
    // 空間配置
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    // 製作輸入資料
    acquire_data(in,audioFile);
    // 產生最佳化的程式碼
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    // 執行這個最佳化的程式碼
    fftw_execute(p); /* repeat as needed */

    double *mag = new double [N];
    compute_magnitude(out,mag,N);

    int on = 0;
    int threshold = 400;
    //std::cout << "write txt start\n";
    for (int i = 25000; i < 27000; ++i)
    {
        if(mag[i]>threshold){
            on = 1;
            //break;
        }
        //double currentSample = audioFile.samples[channel][i];
        //ofs << mag[i] << "\n";
    }
    //ofs.close();
    //std::cout << "write txt end\n";
    // 釋放空間
    std::cout << "buzzer : " << on << std::endl;
    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);

    delete [] mag;
    return 0;
}
