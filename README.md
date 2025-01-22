# V8c2
U8g2 sim lib for VCVRack.

## Overview
This is a set of drawing routines and widgets to simulate 3 sizes of OLED with VCVRack.

It can simulate a monochrome display using SSD1306.

### Note
+ This library is made to simulate U8g2 and does not contain original U8g2 code.
+ The screen may flicker often. This can be a problem in certain situations, but I left it in because it looks more like the behavior of a real I2C display.

## Usage
+ prepare `glcdfont.hpp` or other font header.
+ include on your project header file.

## Display variant

| define | model | reso. [px] | size [mm] |
| - | - | - | - |
| Display096 | 0.96" | 128 × 64 | 24.384 × 12.192 |
| Display091 | 0.91" | 128 × 32 | 23.114 × 5.778 |
| Display042 | 0.42" | 72 × 40 | 10.668 × 5.926 |

## Example

```cpp

// in task
YourModule::YourModule(){
  // other dsp
  display042.reset(new V8C2Display042());
  updateDisplay();
}

// display
void YourModule::updateDisplay() {
  if (!display042)
    return;

  float bpm = 120.5f;

  display042->clear();                // clear virtual buffer
  display042->setTextSize(2);         // set text size to x2
  display042->setCursor(10, 10);      // set text cursor to 10,10
  display042->printf("%.1f", bpm);    // print text

  display042->drawBox(20,20,10,10);   // draw the 10x10 box at 20,20
  
}

```

