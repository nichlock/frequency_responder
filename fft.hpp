#ifndef ANA_FFT_H
#define ANA_FFT_H

#include <arduinoFFT.h>
#include <stdint.h>

#include "config.h"

#define BY_INDEX 0x00
#define BY_TIME 0x01
#define BY_FREQ 0x02
#define PLOT 0x03

class FftWrapper {
private:
arduinoFFT FFT;

public:

double samplingFrequency;
uint16_t samples = kSAMPLES;

double vReal[kSAMPLES];
double vImag[kSAMPLES];

FftWrapper () {
  FFT = arduinoFFT(); /* Create FFT object */
}

inline double getIndexFreq(uint16_t index) {
  return ((index * 1.0 * samplingFrequency) / samples);
} // getIndexFreq

inline double getFreqIndex(double freq) {
  return (freq * samples * 1.0) / (samplingFrequency * 1.0);
} // getIndexFreq

inline double scaleFreq(double freq) {
  return freq * (10 / M_PI);
} // scaleFreq

inline void computeFft() {
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD); /* Weigh data */
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
} /* computeFft */

void printReal(int scaleType);

double getMaxFreq();

inline void setRealData(uint16_t index, double data) {
  vReal[index] = data;
} // setRealData

inline double *real() {
  return vReal;
} // real

inline void setImagData(uint16_t index, double data) {
  vImag[index] = data;
} // setImagData

inline double *imag() {
  return vImag;
} // imag

inline double setFreqFromTime_us(double us) {
  samplingFrequency = (1 / (((float)(us) / samples) / 1000000));
  return samplingFrequency;
} // setFreqFromTime_us
}

;

#endif /* end of include guard: ANA_FFT_H */
