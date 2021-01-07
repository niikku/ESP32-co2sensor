#include "SparkFun_SCD30_Arduino_Library.h"

SCD30 airSensor;

void setup() {
  Serial.begin(115200);
 
  Wire.begin();

  if (airSensor.begin(Wire, false) == false)
  {
    Serial.println("Air sensor not detected. Please check wiring. Freezing...");
    while (1);
  }

  Serial.println("Device will calibrate in 5 minutes... Make sure the device is exposed to outside air.");
  delay(300000);
  
  airSensor.setForcedRecalibrationFactor(450);
}

void loop() { 
}
