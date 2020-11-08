#include <stdint.h>

#include "fft.hpp"
#include "config.h"

void FftWrapper::printReal(int scaleType) {
  int bufferSize = samples;
  if (scaleType == PLOT) {
    for (uint16_t i = 0; i < bufferSize; i++) {
      Serial.print(getIndexFreq(i));
      Serial.print(" ");
      Serial.println(vReal[i]);
    }
    return;
  }
  if (scaleType == BY_TIME) {
    bufferSize = (samples >> 1);
  }
  for (uint16_t i = 0; i < bufferSize; i++) {
    double abscissa;
    /* Print abscissa value */
    switch (scaleType) {
      case BY_INDEX:
        abscissa = (i * 1.0);
        break;
      case BY_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
        break;
      case BY_FREQ:
        abscissa = getIndexFreq(i);
        break;
    } // switch
    Serial.print(abscissa, 6);
    if (scaleType == BY_FREQ) {
      Serial.print("Hz");
    }
    Serial.print(" ");
    Serial.println(vReal[i], 4);
  }
  Serial.println();
} // PrintVector

double FftWrapper::getMaxFreq() {
  //  Serial.println("Analyzing for max freq:");
  vReal[0] = 0; // Needed for default indexing
  //  return FFT.MajorPeak(vReal, samples, samplingFrequency);
  const uint16_t kDrop_range = 2; // Values in this range need to be much smaller than values out of this range
  int16_t i;

  double avg = 0, stddev = 0;

  /* Get edges of window to process */
  uint16_t first = getFreqIndex(kMIN_ALARM_FREQ), last = getFreqIndex(kMAX_ALARM_FREQ);
  uint16_t minInd = first, maxInd = first, actual_samples = last - first;
  i = 0;
  for (i = first; i < last; i++) {
    avg += vReal[i];
    if (vReal[i] < vReal[minInd]) minInd = i;
    if (vReal[i] > vReal[maxInd]) maxInd = i;
  }
  avg = avg / actual_samples;

  // Confidence is not based on the peak, but only on the existence of any sharp peak/set of peaks.
  double confidence = (vReal[maxInd] / avg);
  if (confidence <= MIN_CONFIDENCE)
    return -1;

  //  Serial.print("first = ");
  //  Serial.println(first);
  //  Serial.print("last  = ");
  //  Serial.println(last);

  //  Serial.print("avg = ");
  //  Serial.println(avg);
  //  Serial.print("min = ");
  //  Serial.println(vReal[minInd]);
  //  Serial.print("max = ");
  //  Serial.println(vReal[maxInd]);
  //  Serial.print("dev = ");
  //  Serial.println(stddev);
  //  Serial.print("samples = ");
  //  Serial.println(actual_samples);

  // Range of acceptable values is upper 50% full scale
  double min_acceptable = (vReal[maxInd] + vReal[minInd]) / 2;
  uint16_t peak = 0;
  // Check for values at 50% full scale that are surrounded by values not in that range
  bool acceptable_freq = false;
  i = 0;
  for (i = first; i < last; i++) {
    if (vReal[i] > min_acceptable) {
      //      Serial.print(", : In range, ");
      acceptable_freq = false;
      for (uint16_t j = i - kDrop_range; j < i; j++) {
        if (vReal[j] < min_acceptable)
          acceptable_freq = true;
      }
      for (uint16_t j = i; j < i + kDrop_range; j++) {
        if (vReal[j] < min_acceptable)
          acceptable_freq = true;
      }
      if (!acceptable_freq)
        continue;
      //      Serial.print("peak, ");
      if (vReal[i] > vReal[peak]) {
        //        Serial.print("new peak.");
        peak = i;
      }
    }
  }
  //  Serial.print("Chose: ");
  //  Serial.print(getIndexFreq(peak));
  //  Serial.print("Hz (");
  //  Serial.print((vReal[maxInd] / avg));
  //  Serial.println(" confidence)");
  //  Serial.println();
  return getIndexFreq(peak);
} /* getMaxFreq */
