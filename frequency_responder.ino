#include <Adafruit_CircuitPlayground.h>

#include "config.h"
#include "hardware.h"
#include "fft.hpp"

FftWrapper fft = FftWrapper();

const bool verboseOutput = false;

bool alarmActive = false;
long int firstAlarmTime_ms = 0;

void setup() {
  initHardware();
  initDebug();
} // setup

void loop() {
  //    while (Serial.available() <= 0);
  //    Serial.read();

  while (!alarmActive && readMic() < MIN_AMPLITUDE);

  long int time_end, sample_start;
  long int time_start = micros();
  for (uint16_t i = 0; i < fft.samples; i++) {
    sample_start = micros();
    fft.setRealData(i, readMic());
    fft.setImagData(i, 0);
    while (micros() - sample_start < 100); // Slow sampling down
  }
  time_end = micros();
  double freq = fft.setFreqFromTime_us(time_end - time_start);

  if (verboseOutput) {
    Serial.print("Frequency: ");
    Serial.println(freq);
  }
  /* Print the results of the simulated sampling according to time */
  //  if (verboseOutput) {
  //    Serial.println("Data:");
  //    fft.printReal(BY_TIME);
  //  }

  fft.computeFft();
  long int time_fft = micros() - (time_end);

  double important_frequency = fft.getMaxFreq();
  long int time_calc = micros() - (time_end + time_fft);

  if (important_frequency > 0)
    startAlarm();
  else
    endAlarm();

  //  fft.printReal(BY_FREQ);

  //    Serial.print("Measured at t = ");
  //    Serial.print((time_end - time_start) / 1000);
  //    Serial.print(", fft at t = ");
  //    Serial.print(time_fft / 1000);
  //    Serial.print(", calculations at t = ");
  //    Serial.println(time_calc / 1000);

  //    Serial.println("Computed magnitudes:");
  //    fft.printReal(BY_FREQ);
  //    Serial.println(important_frequency, 6);
} // loop

void startAlarm() {
  if (!alarmActive) {
    firstAlarmTime_ms = millis();
    alarmActive = true;
  }
  if (millis() - firstAlarmTime_ms > MIN_ALARM_SOUND_TIME_MS) {
    digitalWrite(OUTPUT_PIN, HIGH);
  }
}

void endAlarm() {
  alarmActive = false;
  firstAlarmTime_ms = 0;
  digitalWrite(OUTPUT_PIN, LOW);
}
