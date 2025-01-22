#include "glcdfont.hpp"
#include "v8c2.hpp"
#include <algorithm>
#include <cstdarg>
#include <cstdio>
using v8c2::font;

V8C2Display::V8C2Display(int w, int h, int res_w, int res_h, DisplayType t)
    : width(w), height(h), resolution_w(res_w), resolution_h(res_h),
      display_width(w), display_height(h), type(t) {
  int bufferSize = (width * height + 7) / 8;
  buffer.resize(bufferSize, 0);
  backBuffer.resize(bufferSize, 0);
}

V8C2Display::~V8C2Display() {}

bool V8C2Display::isValidCoordinate(int x, int y) const {
  return x >= 0 && x < width && y >= 0 && y < height;
}

int V8C2Display::getBufferIndex(int x, int y) const {
  return (y * width + x) / 8;
}

uint8_t V8C2Display::getBitMask(int x, int y) const {
  return 1 << ((y * width + x) % 8);
}

void V8C2Display::swapBuffers() {
  if (dirty || forceUpdate) {
    buffer.swap(backBuffer);
  }
}

void V8C2Display::beginUpdate() {
  if (batchUpdate)
    return;

  batchUpdate = true;
  std::copy(buffer.begin(), buffer.end(), backBuffer.begin());
}

void V8C2Display::endUpdate() {
  if (!batchUpdate)
    return;

  if (dirty || forceUpdate) {
    swapBuffers();
    sendBuffer();
  }

  batchUpdate = false;
  dirty = false;
  forceUpdate = false;
}

void V8C2Display::setBufferPixel(int x, int y, bool set) {
  if (!isValidCoordinate(x, y))
    return;

  auto &targetBuffer = batchUpdate ? backBuffer : buffer;
  int idx = getBufferIndex(x, y);
  uint8_t mask = getBitMask(x, y);

  if (set) {
    targetBuffer[idx] |= mask;
  } else {
    targetBuffer[idx] &= ~mask;
  }

  dirty = true;
  if (!batchUpdate) {
    sendBuffer();
  }
}

bool V8C2Display::getBufferPixel(int x, int y) const {
  if (!isValidCoordinate(x, y))
    return false;

  int idx = getBufferIndex(x, y);
  uint8_t mask = getBitMask(x, y);
  return (buffer[idx] & mask) != 0;
}

void V8C2Display::drawBox(int x, int y, int w, int h, bool set) {
  beginUpdate();

  int x2 = std::min(x + w, width);
  int y2 = std::min(y + h, height);
  x = std::max(0, x);
  y = std::max(0, y);

  for (int j = y; j < y2; j++) {
    for (int i = x; i < x2; i++) {
      setBufferPixel(i, j, set);
    }
  }

  endUpdate();
}

void V8C2Display::drawRect(int x, int y, int w, int h) {
  beginUpdate();

  // Draw horizontal lines
  for (int i = x; i < x + w; i++) {
    setBufferPixel(i, y, true);
    setBufferPixel(i, y + h - 1, true);
  }

  // Draw vertical lines
  for (int j = y; j < y + h; j++) {
    setBufferPixel(x, j, true);
    setBufferPixel(x + w - 1, j, true);
  }

  endUpdate();
}

void V8C2Display::clear() {
  beginUpdate();
  std::fill(backBuffer.begin(), backBuffer.end(), 0);
  dirty = true;
  endUpdate();
}

void V8C2Display::fill() {
  beginUpdate();
  std::fill(backBuffer.begin(), backBuffer.end(), 0xFF);
  dirty = true;
  endUpdate();
}

void V8C2Display::setDisplayRotation(uint8_t r) {
  rotation = r % 4;
  switch (rotation) {
  case 0:
  case 2:
    display_width = width;
    display_height = height;
    break;
  case 1:
  case 3:
    display_width = height;
    display_height = width;
    break;
  }
  forceUpdate = true;
}

void V8C2Display::drawLine(int x0, int y0, int x1, int y1) {
  beginUpdate();

  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  int sx = x0 < x1 ? 1 : -1;
  int sy = y0 < y1 ? 1 : -1;
  int err = dx - dy;

  while (true) {
    setBufferPixel(x0, y0, true);
    if (x0 == x1 && y0 == y1)
      break;
    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }

  endUpdate();
}

void V8C2Display::drawHLine(int x, int y, int w) {
  beginUpdate();
  for (int i = 0; i < w; i++) {
    setBufferPixel(x + i, y, true);
  }
  endUpdate();
}

void V8C2Display::drawVLine(int x, int y, int h) {
  beginUpdate();
  for (int i = 0; i < h; i++) {
    setBufferPixel(x, y + i, true);
  }
  endUpdate();
}

void V8C2Display::drawCircle(int x0, int y0, int r) {
  beginUpdate();

  int x = r;
  int y = 0;
  int err = 0;

  while (x >= y) {
    setBufferPixel(x0 + x, y0 + y, true);
    setBufferPixel(x0 + y, y0 + x, true);
    setBufferPixel(x0 - y, y0 + x, true);
    setBufferPixel(x0 - x, y0 + y, true);
    setBufferPixel(x0 - x, y0 - y, true);
    setBufferPixel(x0 - y, y0 - x, true);
    setBufferPixel(x0 + y, y0 - x, true);
    setBufferPixel(x0 + x, y0 - y, true);

    if (err <= 0) {
      y += 1;
      err += 2 * y + 1;
    }
    if (err > 0) {
      x -= 1;
      err -= 2 * x + 1;
    }
  }

  endUpdate();
}

void V8C2Display::fillCircle(int x0, int y0, int r) {
  beginUpdate();

  for (int y = -r; y <= r; y++) {
    for (int x = -r; x <= r; x++) {
      if (x * x + y * y <= r * r) {
        setBufferPixel(x0 + x, y0 + y, true);
      }
    }
  }

  endUpdate();
}

void V8C2Display::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2) {
  beginUpdate();

  drawLine(x0, y0, x1, y1);
  drawLine(x1, y1, x2, y2);
  drawLine(x2, y2, x0, y0);

  endUpdate();
}

void V8C2Display::fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2) {
  beginUpdate();

  // Sort vertices by y-coordinate
  if (y0 > y1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }
  if (y0 > y2) {
    std::swap(x0, x2);
    std::swap(y0, y2);
  }
  if (y1 > y2) {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }

  int total_height = y2 - y0;

  // First half
  for (int y = y0; y <= y1; y++) {
    float alpha = (float)(y - y0) / total_height;
    float beta = (float)(y - y0) / (y1 - y0);
    int x_left = x0 + (x2 - x0) * alpha;
    int x_right = x0 + (x1 - x0) * beta;
    if (x_left > x_right)
      std::swap(x_left, x_right);
    for (int x = x_left; x <= x_right; x++) {
      setBufferPixel(x, y, true);
    }
  }

  // Second half
  for (int y = y1; y <= y2; y++) {
    float alpha = (float)(y - y0) / total_height;
    float beta = (float)(y - y1) / (y2 - y1);
    int x_left = x0 + (x2 - x0) * alpha;
    int x_right = x1 + (x2 - x1) * beta;
    if (x_left > x_right)
      std::swap(x_left, x_right);
    for (int x = x_left; x <= x_right; x++) {
      setBufferPixel(x, y, true);
    }
  }

  endUpdate();
}

void V8C2Display::drawChar(int16_t x, int16_t y, unsigned char c, bool color,
                           uint8_t size) {
  if (c < FIRST_CHAR || c > LAST_CHAR)
    return;

  beginUpdate();

  for (int8_t i = 0; i < 5; i++) {
    uint8_t line = font[(c - FIRST_CHAR) * 5 + i];
    for (int8_t j = 0; j < 8; j++) {
      if (line & 0x1) {
        if (size == 1) {
          setBufferPixel(x + i, y + j, color);
        } else {
          drawBox(x + i * size, y + j * size, size, size, color);
        }
      }
      line >>= 1;
    }
  }

  endUpdate();
}

void V8C2Display::write(uint8_t c) {
  if (c == '\n') {
    cursor_y += FONT_HEIGHT * textSize;
    cursor_x = 0;
  } else if (c == '\r') {
    // skip em
  } else {
    drawChar(cursor_x, cursor_y, c, true, textSize);
    cursor_x += FONT_WIDTH * textSize;
  }
}

void V8C2Display::print(const char *str) {
  while (*str) {
    write(*str++);
  }
}

void V8C2Display::printf(const char *fmt, ...) {
  char buf[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  print(buf);
}
void V8C2Display::invertDisplay(bool i) {
  if (inverted == i)
    return;

  beginUpdate();
  inverted = i;
  for (size_t idx = 0; idx < backBuffer.size(); idx++) {
    backBuffer[idx] = ~buffer[idx];
  }
  dirty = true;
  endUpdate();
}
void V8C2Display::drawXBM(int x, int y, int width, int height,
                          const uint8_t *bitmap) {
  beginUpdate();

  int byteWidth = (width + 7) / 8;
  uint8_t byte = 0;

  for (int yy = 0; yy < height; yy++) {
    for (int xx = 0; xx < width; xx++) {
      if (xx & 7) {
        byte >>= 1;
      } else {
        byte = bitmap[yy * byteWidth + xx / 8];
      }

      if (byte & 0x01) {
        setBufferPixel(x + xx, y + yy, true);
      }
    }
  }

  endUpdate();
}
// bool V8C2Display::needsUpdate() const { return dirty || forceUpdate; }

// void V8C2Display::setForceUpdate(bool force) { forceUpdate = force; }

// void V8C2Display::setUpdateThrottling(bool enable) { throttleUpdates =
// enable; }

void V8C2Display::sendBuffer() {
  if (!dirty && !forceUpdate)
    return;

  if (throttleUpdates) {
    uint64_t currentTime = rack::system::getTime();
    if (currentTime - lastUpdateTime < MIN_UPDATE_INTERVAL) {
      return;
    }
    lastUpdateTime = currentTime;
  }

  if (!batchUpdate) {
    dirty = false;
    forceUpdate = false;
  }
}