#pragma once

#include <vector>
#include "ClockFace.h"

class ClockFaceMatrix: public ClockFace
{
public:
  int pixelCount() override;

  ClockFaceMatrix(LightSensorPosition position): ClockFace(position){};

  // Rotates the display.
  void setLightSensorPosition(LightSensorPosition position);

  virtual bool stateForTime(int hour, int minute, int second, bool show_ampm) = 0;
protected:
  uint16_t mapMinute(Corners corner) override;
  uint16_t map(int16_t x, int16_t y) override;
};

class GermanClockFaceMatrix : public ClockFaceMatrix
{
public:
  GermanClockFaceMatrix(LightSensorPosition position) : ClockFaceMatrix(position){};

  virtual bool stateForTime(int hour, int minute, int second, bool show_ampm);
};
