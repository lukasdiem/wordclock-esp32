#include "logging.h"

#include <NeoPixelBus.h> // Only need NeoTopology

#include "ClockFaceMatrix.h"

// Matrix dimensions.
#define NEOPIXEL_MATRIX_ROWS 16
#define NEOPIXEL_MATRIX_COLS 16

// Number of LEDs on the whole strip.
#define NEOPIXEL_COUNT (NEOPIXEL_MATRIX_ROWS * NEOPIXEL_MATRIX_COLS)

// static
int ClockFaceMatrix::pixelCount()
{
  return NEOPIXEL_MATRIX_ROWS * NEOPIXEL_MATRIX_COLS;
}

uint16_t ClockFaceMatrix::map(int16_t x, int16_t y)
{
  switch (_position)
  {
  case LightSensorPosition::Top:
  {
    static NeoTopology<ColumnMajorAlternating90Layout> sensor_on_top(
        NEOPIXEL_MATRIX_ROWS, NEOPIXEL_MATRIX_COLS);
    return sensor_on_top.Map(x, y);
  }
  case LightSensorPosition::Bottom:
  {
    static NeoTopology<ColumnMajorAlternating270Layout> sensor_on_bottom(
        NEOPIXEL_MATRIX_ROWS, NEOPIXEL_MATRIX_COLS);
    return sensor_on_bottom.Map(x, y);
  }
  default:
    DCHECK(false, static_cast<int>(_position));
    return 0;
  }
}

uint16_t ClockFaceMatrix::mapMinute(Corners corner)
{
  switch (_position)
  {
  case LightSensorPosition::Bottom:
    return (static_cast<uint16_t>(corner) + 2) % 4;
  case LightSensorPosition::Top:
    return static_cast<uint16_t>(corner);
  default:
    DCHECK(false, static_cast<int>(corner)); 
    return 0;
  }
}


// Constants to match the GermanClockFaceMatrix.
//
// Letters in lowercase below are not used by the clock.

// TODO!!!!
// ITlISasAM PM
// A c QUARTER dc
// TWENTY FIVE x
// HALF s TEN u TO
// PAST bu NINE
// ONE SIX THREE
// FOUR FIVE TWO
// EIGHT ELEVEN
// SEVEN TWELVE
// TEN sz O'CLOCK

// All the segments of words on the board. The first two numbers are the
// coordinate of the first letter of the word, the last is the length. A word
// must always be on one row.

// Minute circles
#define DE_M_1 0,  0,  1
#define DE_M_2 15, 0,  1
#define DE_M_3 15, 15, 1
#define DE_M_4 0,  15, 1

#define DE_S_IT 2, 2, 2  // ES
#define DE_S_IS 5, 2, 3  // IST

#define DE_H_ONE    10, 11,  4  // EINS
#define DE_H_TWO     3,  7,  4  // ZWEI
#define DE_H_THREE   2,  8,  4  // DREI
#define DE_H_FOUR    6,  12, 4  // VIER
#define DE_H_FIVE    9,  8,  4  // FÜNF
#define DE_H_SIX     4,  11, 5  // SECHS
#define DE_H_SEVEN   8,  7,  6  // SIEBEN
#define DE_H_EIGHT   3,  10, 4  // ACHT
#define DE_H_NINE    6,  9,  4  // NEUN
#define DE_H_TEN     7,  10, 4  // ZEHN
#define DE_H_ELEVEN  2,  9,  3  // ELF
#define DE_H_TWELVE  8,  6,  5  // ZWÖLF

//#define DE_M_A 0, 1, 1
#define DE_M_PAST 4, 5, 4 // NACH
#define DE_M_TO   9, 5, 3 // VOR

#define DE_M_TEN        2,  3,  4   // ZEHN
#define DE_M_QUARTER    7,  4,  7   // VIERTEL
#define DE_M_TWENTY     7,  3,  7   // ZWANZIG
//#define DE_M_TWENTYFIVE 0,  2,  10
#define DE_M_FIVE      10,  2,  4   // FÜNF
#define DE_M_HALF       2,  6,  4   // HALB
//#define DE_M_QUART      0,  9,  5   // VIERTEL
#define DE_M_QUARTS     3,  4, 11   // DREIVIERTEL

#define DE_M_OCLOCK    11, 13, 3 	// UHR

bool GermanClockFaceMatrix::stateForTime(int hour, int minute, int second, bool show_ampm)
{
  if (hour == _hour && minute == _minute && show_ampm == _show_ampm)
  {
    return false;
  }
  _hour = hour;
  _minute = minute;
  _show_ampm = show_ampm;

  DLOGLN("update state");

  // Reset the board to all black
  for (int i = 0; i < NEOPIXEL_COUNT; i++)
    _state[i] = false;

  int leftover = minute % 5;
  minute = minute - leftover;

  if (minute >= 35)
    hour = (hour + 1) % 24; // Switch to "TO" minutes the next hour

  updateSegment(DE_S_IT);
  updateSegment(DE_S_IS);

  if (show_ampm)
  {
    // not used in german
    /*if (hour < 13)
    {
      updateSegment(DE_H_AM);
    }
    else
    {
      updateSegment(DE_H_PM);
    }*/
  }

  switch (hour)
  {
  case 0:
    updateSegment(DE_H_TWELVE);
    break;
  case 1:
  case 13:
    DLOG("EINS");
    updateSegment(DE_H_ONE);
    break;
  case 2:
  case 14:
    DLOG("ZWEI");
    updateSegment(DE_H_TWO);
    break;
  case 3:
  case 15:
    updateSegment(DE_H_THREE);
    break;
  case 4:
  case 16:
    updateSegment(DE_H_FOUR);
    break;
  case 5:
  case 17:
    updateSegment(DE_H_FIVE);
    break;
  case 6:
  case 18:
    updateSegment(DE_H_SIX);
    break;
  case 7:
  case 19:
    updateSegment(DE_H_SEVEN);
    break;
  case 8:
  case 20:
    updateSegment(DE_H_EIGHT);
    break;
  case 9:
  case 21:
    updateSegment(DE_H_NINE);
    break;
  case 10:
  case 22:
    updateSegment(DE_H_TEN);
    break;
  case 11:
  case 23:
    updateSegment(DE_H_ELEVEN);
    break;
  case 12:
    updateSegment(DE_H_TWELVE);
    break;
  default:
    DLOG("Invalid hour ");
    DLOGLN(hour);
  }

  switch (minute)
  {
  case 0:
    updateSegment(DE_M_OCLOCK);
    break;
  case 5:
    updateSegment(DE_M_FIVE);
    updateSegment(DE_M_PAST);
    break;
  case 10:
    updateSegment(DE_M_TEN);
    updateSegment(DE_M_PAST);
    break;
  case 15:
    updateSegment(DE_M_QUARTER);
    updateSegment(DE_M_PAST);
    break;
  case 20:
    updateSegment(DE_M_TWENTY);
    updateSegment(DE_M_PAST);
    break;
  case 25:
    updateSegment(DE_M_HALF);
    updateSegment(DE_M_TO);
    break;
  case 30:
    updateSegment(DE_M_HALF);
    updateSegment(DE_M_PAST);
    break;
  case 35:
    updateSegment(DE_M_HALF);
    updateSegment(DE_M_PAST);
    break;
  case 40:
    updateSegment(DE_M_TWENTY);
    updateSegment(DE_M_TO);
    break;
  case 45:
    updateSegment(DE_M_QUARTER);
    updateSegment(DE_M_TO);
    break;
  case 50:
    updateSegment(DE_M_TEN);
    updateSegment(DE_M_TO);
    break;
  case 55:
    updateSegment(DE_M_FIVE);
    updateSegment(DE_M_TO);
    break;
  default:
    DLOG("Invalid minute ");
    DLOGLN(minute);
  }

  switch (leftover)
  {
  case 4:
    updateSegment(DE_M_4);
  case 3: // fall through
    updateSegment(DE_M_3);
  case 2: // fall through
    updateSegment(DE_M_2);
  case 1: // fall through
    updateSegment(DE_M_1);
  case 0: // fall through
    break;
  }
  return true;
}