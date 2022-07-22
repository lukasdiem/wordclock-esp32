#pragma once

#include <NeoPixelAnimator.h>
#include <NeoPixelBrightnessBus.h>

#include "BrightnessController.h"
#include "ClockFace.h"

// The pin to control the matrix
//#define NEOPIXEL_PIN 32

// Ignored on the ESP8266 platform
// => Uses GPIO3 by default!
#define NEOPIXEL_PIN -1

// DEFAULT = 400
#define TIME_CHANGE_ANIMATION_SPEED 10

class Display
{
public:
  Display(ClockFace &clockFace, uint8_t pin = NEOPIXEL_PIN);

  void setup();
  void loop();
  void loopTest(int delayMs);
  void setColor(const RgbColor &color);

  // Sets the sensor sensitivity of the brightness controller.
  int setSensorSensitivity(int value) { _brightnessController.setSensorSensitivity(value); return 1; }

  // Sets whether to show AM/PM information on the display.
  void setShowAmPm(bool show_ampm) { _show_ampm = show_ampm; }

  // Starts an animation to update the clock to a new time if necessary.
  void updateForTime(int hour, int minute, int second, int animationSpeed = TIME_CHANGE_ANIMATION_SPEED);

private:
  // Updates pixel color on the display.
  void _update(int animationSpeed = TIME_CHANGE_ANIMATION_SPEED);

  // To know which pixels to turn on and off, one needs to know which letter
  // matches which LED, and the orientation of the display. This is the job
  // of the clockFace.
  ClockFace &_clockFace;

  // Whether the display should show AM/PM information.
  bool _show_ampm = 0;

  // Color of the LEDs. Can be manipulated via Web configuration interface.
  RgbColor _color;

  // Addressable bus to control the LEDs.
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> _pixels;
  //NeoPixelBus<NeoGrbFeature, NeoEsp32BitBangWs2812xMethod> _pixels;

  // Reacts to change in ambient light to adapt the power of the LEDs
  BrightnessController _brightnessController;

  //
  // Animation time management object.
  // Uses centiseconds as precision, so an animation can range from 1/100 of a
  // second to a little bit more than 10 minutes.
  //
  NeoPixelAnimator _animations;

  int _testIndex = 0;
};
