#pragma once
#include "v8c2.hpp"
#include <rack.hpp>

using namespace rack;

class V8C2DisplayWidget : public OpaqueWidget {
protected:
  V8C2Display *display = nullptr;
  NVGcolor backgroundColor;
  NVGcolor foregroundColor;
  float scale;
  int rotation;

  // Helper drawing functions
  void drawNoDisplay(const DrawArgs &args);
  void drawBezelAndBackground(const DrawArgs &args);
  void drawGlassEffect(const DrawArgs &args);

public:
  explicit V8C2DisplayWidget(float displayScale = 1.0f);
  void setColors(NVGcolor bg, NVGcolor fg);
  void setDisplay(V8C2Display *disp) { display = disp; }
  void draw(const DrawArgs &args) override;
  V8C2Display *getDisplay() const { return display; }

  // Display control
  void setRotation(int degrees);
  int getRotation() const { return rotation; }
  void rotateClockwise() { setRotation(rotation + 90); }
  void rotateCounterClockwise() { setRotation(rotation - 90); }
};

// 128x64 0.96 inch OLED Widget
struct V8C2Display096Widget : V8C2DisplayWidget {
  static constexpr float MM_WIDTH = 24.384f;
  static constexpr float MM_HEIGHT = 12.192f;
  V8C2Display096Widget();
};

// 128x32 0.91 inch OLED Widget
struct V8C2Display091Widget : V8C2DisplayWidget {
  static constexpr float MM_WIDTH = 23.114f;
  static constexpr float MM_HEIGHT = 5.778f;
  V8C2Display091Widget();
};

// 72x40 0.42 inch OLED Widget
struct V8C2Display042Widget : V8C2DisplayWidget {
  static constexpr float MM_WIDTH = 10.668f;
  static constexpr float MM_HEIGHT = 5.926f;
  V8C2Display042Widget();
};