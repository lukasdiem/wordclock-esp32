
// Define debug to turn on debug logging.
// #define DEBUG 1
#include <Arduino.h>
#include <RTClib.h>

#include "src/logging.h"
#include "src/Display.h"
//#include "src/ClockFace.h"
#include "src/ClockFaceMatrix.h"
#include "src/Iot.h"

RTC_DS3231 rtc;
GermanClockFaceMatrix clockFace(ClockFace::LightSensorPosition::Top);
//EnglishClockFace clockFace(ClockFace::LightSensorPosition::Top);
// FrenchClockFace clockFace(ClockFace::LightSensorPosition::Bottom);
Display display(clockFace);
Iot iot(&display, &rtc);

void setup()
{
  setupLogging();

  /*bool result = rtc.begin();
  DCHECK(result, "RTC didn't start");
  if (rtc.lostPower())
  {
    DCHECK("RTC lost power. Battery was removed ?");
  }*/

  display.setup();
  iot.setup();

  DLOG("Setup done ... ");
}

void loop()
{
  //DateTime now = rtc.now();
  //display.updateForTime(now.hour(), now.minute(), now.second());
  //display.updateForTime(14, 12, 30);
  display.loop();
  iot.loop();

  delay(3000);
}
