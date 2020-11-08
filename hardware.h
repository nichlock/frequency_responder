#ifndef ANA_HARDWARE_H
#define ANA_HARDWARE_H

#include <Adafruit_CircuitPlayground.h>
#include <stdint.h>

#include "config.h"

inline void initHardware() {
  CircuitPlayground.begin();
  while (CircuitPlayground.soundSensor() < 0);
} /* readMic */

inline void initDebug(){
  Serial.begin(2000000);
  Serial.println("Ready");
} /* initDebug */

#define debugPrint(a) Serial.println(a)

inline int readMic() {
  return CircuitPlayground.soundSensor();
} /* readMic */

#endif /* end of include guard: ANA_HARDWARE_H */
