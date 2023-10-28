#ifndef _TFT_NOVATEK_NV3041A_H_
#define _TFT_NOVATEK_NV3041A_H_

#include <Arduino.h>
#include <SPI.h>

#define NV3041A_TFTWIDTH 480
#define NV3041A_TFTHEIGHT 272

#define NV3041A_DISPOFF 0x28
#define NV3041A_DISPON 0x29

class TFT_Novatek_NV3041A{
public:
  TFT_Novatek_NV3041A(int8_t dcx, int8_t cs, int8_t rst, int8_t sck, int8_t mosi, int8_t rotation = 0, bool ips = true);

  bool begin(int32_t speed = 0);
  void reset();  // Custom reset function
  void setRotation(uint8_t rotation);
  void invertDisplay(bool invert);
  void displayOn();
  void displayOff();
  void fillScreen(uint16_t color);
  void setCursor(int16_t x, int16_t y);
  void setTextColor(uint16_t color);
  void setTextSize(uint8_t size);
  void print(const char* text);  // Override the base class's print function

    void writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h);

protected:
  void tftInit();

  uint8_t COL_OFFSET1, ROW_OFFSET1;
  uint8_t COL_OFFSET2, ROW_OFFSET2;
  uint8_t _xStart, _yStart;
  int16_t _currentX, _currentY;
  uint16_t _currentW, _currentH;

private:
  int8_t _dcxPin;
  int8_t _csPin;
  int8_t _rstPin;
  int8_t _sckPin;
  int8_t _mosiPin;
  int8_t _rotation;
  bool _ips;

  uint16_t _textColor;
  uint16_t _textSize;

  void writeCommand(uint8_t command);
  void writeData(uint8_t data);
};

#endif
