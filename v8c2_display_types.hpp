#pragma once
#include "v8c2.hpp"

// 128x64 0.96 inch OLED display
class V8C2Display096 : public V8C2Display {
public:
  static constexpr int DISPLAY_WIDTH = 128;
  static constexpr int DISPLAY_HEIGHT = 64;
  static constexpr float MM_WIDTH = 24.384f;
  static constexpr float MM_HEIGHT = 12.192f;

  V8C2Display096()
      : V8C2Display(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_WIDTH * 2,
                    DISPLAY_HEIGHT * 2) {}
};

// 128x32 0.91 inch OLED display
class V8C2Display091 : public V8C2Display {
public:
  static constexpr int DISPLAY_WIDTH = 128;
  static constexpr int DISPLAY_HEIGHT = 32;
  static constexpr float MM_WIDTH = 23.114f;
  static constexpr float MM_HEIGHT = 5.778f;

  V8C2Display091()
      : V8C2Display(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_WIDTH * 2,
                    DISPLAY_HEIGHT * 2) {}
};

// 72x40 0.42 inch OLED display
class V8C2Display042 : public V8C2Display {
public:
  static constexpr int DISPLAY_WIDTH = 72;
  static constexpr int DISPLAY_HEIGHT = 40;
  static constexpr float MM_WIDTH = 10.668f;
  static constexpr float MM_HEIGHT = 5.926f;

  V8C2Display042()
      : V8C2Display(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_WIDTH * 2,
                    DISPLAY_HEIGHT * 2) {}
};