#ifndef CONFIG_H
#define CONFIG_H
/**
   Defines the number of samples taken for the FFT
*/

#define kSAMPLES 256 // This value MUST ALWAYS be a power of 2

/**
   The range of frequencies that may be cause for alarm
*/
#define kMIN_ALARM_FREQ 600
#define kMAX_ALARM_FREQ 4300

#if (kMIN_ALARM_FREQ > kMAX_ALARM_FREQ)
#pragma \
  GCC error "CONFIGURATION ERROR: Invalid frequencies chosen; min must be less than or equal to max."
#endif /* if (kMIN_ALARM_FREQ > kMAX_ALARM_FREQ) */

#define OUTPUT_PIN 13

#define MIN_CONFIDENCE 5

#define MIN_AMPLITUDE 15

#define MIN_ALARM_SOUND_TIME_MS 0

#endif
