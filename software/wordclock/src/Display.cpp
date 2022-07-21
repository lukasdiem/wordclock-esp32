#include "logging.h"

#include "Display.h"

Display::Display(ClockFace &clockFace, uint8_t pin)
    : _clockFace(clockFace),
      _pixels(clockFace.pixelCount(), pin),
      _animations(clockFace.pixelCount(), NEO_CENTISECONDS) {}

void Display::setup()
{
  DLOGLN("Pixel count: ");
  DLOGLN(_clockFace.pixelCount());

  _pixels.Begin();
  //_brightnessController.setup();
}

void Display::loop()
{
  //_brightnessController.loop();
  //_animations.UpdateAnimations();
  /*if (_brightnessController.hasChanged())
  {
    _update(30); // Update in 300 ms
  }*/

  _update(30);
  _pixels.Show();
}

void Display::setColor(const RgbColor &color)
{
  DLOGLN("Updating color");
  _color = color;
  _brightnessController.setOriginalColor(color);
  _update();
}

void Display::_update(int animationSpeed)
{
  //DLOGLN("Updating display");

  //_animations.StopAll();
  static const RgbColor black = RgbColor(0x00, 0x00, 0x00);

  // For all the LED animate a change from the current visible state to the new
  // one.
  const std::vector<bool> &state = _clockFace.getState();

  _pixels.ClearTo(black);

  /*for (int index = 0; index < _clockFace.pixelCount(); index++)
  {
    _pixels.SetPixelColor(index, black);
  }*/

  /*for (int index = 0; index < state.size(); index++)
  {
    RgbColor originalColor = _pixels.GetPixelColor(index);
    RgbColor targetColor = state[index] ? _brightnessController.getCorrectedColor() : black;

    //DLOGLN(state[index]);

    AnimUpdateCallback animUpdate = [=](const AnimationParam &param) {
      float progress = NeoEase::QuadraticIn(param.progress);
      RgbColor updatedColor = RgbColor::LinearBlend(
          originalColor, targetColor, progress);
      _pixels.SetPixelColor(index, updatedColor);
    };
    _animations.StartAnimation(index, animationSpeed, animUpdate);
  }*/

  if (_debugIndex % 2 == 0)
    _pixels.SetPixelColor(_debugIndex, RgbColor(0x20, 0x00, 0x00));
  else
    _pixels.SetPixelColor(_debugIndex, RgbColor(0x00, 0x20, 0x00));


  //_pixels.Show();
    
  _debugIndex++;
  _debugIndex = _debugIndex % _clockFace.pixelCount();

  DLOGLN("Updating display, index: ");
  DLOGLN(_debugIndex);
}

void Display::updateForTime(int hour, int minute, int second, int animationSpeed)
{

  if (!_clockFace.stateForTime(hour, minute, second, _show_ampm))
  {
    return; // Nothing to update.
  }

  DLOG("Time: ");
  DLOG(hour);
  DLOG(":");
  DLOGLN(minute);

  _update(animationSpeed);
}
