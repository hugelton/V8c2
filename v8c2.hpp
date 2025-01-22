#pragma once
#include <memory>
#include <nanovg.h>
#include <rack.hpp>
#include <vector>

class V8C2Display {
public:
  enum DisplayType { MONO = 0, GRAYSCALE = 1 };

  V8C2Display(int width, int height, int resolution_w, int resolution_h,
              DisplayType type = MONO);
  virtual ~V8C2Display();

  // Basic operations
  void clear();
  void fill();
  void setBufferPixel(int x, int y, bool set);
  bool getBufferPixel(int x, int y) const;
  void sendBuffer();

  // Drawing primitives
  void drawPixel(int x, int y);
  void drawLine(int x0, int y0, int x1, int y1);
  void drawHLine(int x, int y, int w);
  void drawVLine(int x, int y, int h);
  void drawRect(int x, int y, int w, int h);
  void drawBox(int x, int y, int w, int h, bool set = true);
  void drawCircle(int x0, int y0, int r);
  void fillCircle(int x0, int y0, int r);
  void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2);
  void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2);
  // Bitmap operations
  void drawXBM(int x, int y, int width, int height, const uint8_t *bitmap);
  // Display control
  void setDisplayRotation(uint8_t r);
  void invertDisplay(bool i);
  void beginUpdate();
  void endUpdate();
  bool needsUpdate() const { return dirty || forceUpdate; }
  void setForceUpdate(bool force) { forceUpdate = force; }
  void setUpdateThrottling(bool enable) { throttleUpdates = enable; }

  // Text functions
  void setTextSize(uint8_t size) { textSize = size; }
  void setCursor(int16_t x, int16_t y) {
    cursor_x = x;
    cursor_y = y;
  }
  void drawChar(int16_t x, int16_t y, unsigned char c, bool color = true,
                uint8_t size = 1);
  void write(uint8_t c);
  void print(const char *str);
  void printf(const char *fmt, ...);

  // Getters
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  int getResolutionW() const { return resolution_w; }
  int getResolutionH() const { return resolution_h; }
  DisplayType getType() const { return type; }
  const std::vector<uint8_t> &getBuffer() const { return buffer; }

private:
  int width;
  int height;
  int resolution_w;
  int resolution_h;
  int display_width;
  int display_height;
  DisplayType type;

  // Buffer management
  std::vector<uint8_t> buffer;
  std::vector<uint8_t> backBuffer;
  bool dirty = false;
  bool forceUpdate = false;
  bool inverted = false;
  uint8_t rotation = 0;

  // Update control
  bool batchUpdate = false;
  bool throttleUpdates = true;
  uint64_t lastUpdateTime = 0;
  static const uint64_t MIN_UPDATE_INTERVAL = 16667; // ~60Hz

  // Helper functions
  bool isValidCoordinate(int x, int y) const;
  void swapBuffers();
  int getBufferIndex(int x, int y) const;
  uint8_t getBitMask(int x, int y) const;

  // Text state
  int16_t cursor_x = 0;
  int16_t cursor_y = 0;
  uint8_t textSize = 1;

  static constexpr int FONT_WIDTH = 6;
  static constexpr int FONT_HEIGHT = 8;
  static constexpr int FIRST_CHAR = 0x20;
  static constexpr int LAST_CHAR = 0x7E;
};