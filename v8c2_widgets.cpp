#include "v8c2_display_types.hpp"
#include "v8c2_widgets.hpp"

V8C2DisplayWidget::V8C2DisplayWidget(float displayScale)
    : scale(displayScale), rotation(0) {
  backgroundColor = nvgRGBA(0, 0, 0, 255);       // Pure black background
  foregroundColor = nvgRGBA(255, 255, 255, 255); // Pure white pixels
}

void V8C2DisplayWidget::setColors(NVGcolor bg, NVGcolor fg) {
  backgroundColor = bg;
  foregroundColor = fg;
}

void V8C2DisplayWidget::setRotation(int degrees) {
  rotation = ((degrees % 360) / 90 * 90);
  if (rotation < 0)
    rotation += 360;
}

void V8C2DisplayWidget::draw(const DrawArgs &args) {
  if (!display) {
    drawNoDisplay(args);
    return;
  }

  nvgSave(args.vg);

  // Apply rotation if needed
  if (rotation != 0) {
    float cx = box.size.x / 2.0f;
    float cy = box.size.y / 2.0f;
    nvgTranslate(args.vg, cx, cy);
    nvgRotate(args.vg, (float)rotation * M_PI / 180.0f);
    nvgTranslate(args.vg, -cx, -cy);
  }

  // Draw bezel and background
  drawBezelAndBackground(args);

  // Calculate pixel dimensions
  float pixelWidth = box.size.x / display->getWidth();
  float pixelHeight = box.size.y / display->getHeight();
  float gap = 0.5f; // Gap between pixels for better visibility

  // Draw active pixels
  nvgBeginPath(args.vg);
  nvgFillColor(args.vg, foregroundColor);

  // Batch all active pixels into a single path for better performance
  for (int y = 0; y < display->getHeight(); y++) {
    for (int x = 0; x < display->getWidth(); x++) {
      if (display->getBufferPixel(x, y)) {
        float px = x * pixelWidth + gap;
        float py = y * pixelHeight + gap;
        nvgRect(args.vg, px, py, pixelWidth - 2 * gap, pixelHeight - 2 * gap);
      }
    }
  }
  nvgFill(args.vg);

  // Add glass effect overlay
  drawGlassEffect(args);

  nvgRestore(args.vg);
}

void V8C2DisplayWidget::drawNoDisplay(const DrawArgs &args) {
  nvgBeginPath(args.vg);
  nvgMoveTo(args.vg, 0, 0);
  nvgLineTo(args.vg, box.size.x, box.size.y);
  nvgMoveTo(args.vg, box.size.x, 0);
  nvgLineTo(args.vg, 0, box.size.y);
  nvgStrokeColor(args.vg, nvgRGBA(255, 0, 0, 255));
  nvgStroke(args.vg);
}

void V8C2DisplayWidget::drawBezelAndBackground(const DrawArgs &args) {
  float bezelSize = 1.0f;

  // Draw bezel
  nvgBeginPath(args.vg);
  nvgRect(args.vg, -bezelSize, -bezelSize, box.size.x + 2 * bezelSize,
          box.size.y + 2 * bezelSize);
  nvgFillColor(args.vg, nvgRGBA(32, 32, 32, 255));
  nvgFill(args.vg);

  // Draw display background
  nvgBeginPath(args.vg);
  nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
  nvgFillColor(args.vg, backgroundColor);
  nvgFill(args.vg);
}

void V8C2DisplayWidget::drawGlassEffect(const DrawArgs &args) {
  nvgBeginPath(args.vg);
  NVGpaint glassGradient =
      nvgLinearGradient(args.vg, 0, 0, 0, box.size.y,
                        nvgRGBA(255, 255, 255, 10), nvgRGBA(255, 255, 255, 0));
  nvgFillPaint(args.vg, glassGradient);
  nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
  nvgFill(args.vg);
}

// Specific display widget implementations
V8C2Display096Widget::V8C2Display096Widget() : V8C2DisplayWidget(2.0f) {
  box.size = mm2px(Vec(MM_WIDTH, MM_HEIGHT));
}

V8C2Display091Widget::V8C2Display091Widget() : V8C2DisplayWidget(2.0f) {
  box.size = mm2px(Vec(MM_WIDTH, MM_HEIGHT));
}

V8C2Display042Widget::V8C2Display042Widget() : V8C2DisplayWidget(2.0f) {
  box.size = mm2px(Vec(MM_WIDTH, MM_HEIGHT));
}