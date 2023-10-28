#define NV3041A_MADCTL_MX 0x40
#define NV3041A_MADCTL_MV 0x20
#define NV3041A_MADCTL_MY 0x80
#define NV3041A_MADCTL_RGB 0x00
#define NV3041A_CASET 0x2A
#define NV3041A_RASSET 0x2B
#define NV3041A_RAMWR 0x2C

#include "TFT_Novatek_NV3041A.h"
#include <SPI.h>

TFT_Novatek_NV3041A::TFT_Novatek_NV3041A(int8_t dcx, int8_t cs, int8_t rst, int8_t sck, int8_t mosi, int8_t rotation, bool ips);
  //Arduino_TFT(Arduino_DataBus *bus, int8_t dcx, int8_t cs, int8_t rst, int8_t sck, int8_t mosi, int8_t rotation, bool ips); 

bool TFT_Novatek_NV3041A::begin(int32_t speed) {
  // Initialize the SPI bus and TFT controller here (use _dcxPin, _csPin, _rstPin)
  // Setup the SPI bus
  SPI.begin();
  SPI.beginTransaction(SPISettings(speed, MSBFIRST, SPI_MODE0));
  // Set up the DCX and CS pins
  pinMode(_dcxPin, OUTPUT);
  digitalWrite(_dcxPin, HIGH);  // Default to data mode

  pinMode(_csPin, OUTPUT);
  digitalWrite(_csPin, HIGH);  // Deselect TFT controller initially

  // Set up the RST pin
  pinMode(_rstPin, OUTPUT);
  digitalWrite(_rstPin, HIGH);  // Hold TFT Controller in reset initially
  delay(10);                    // Wait for a short time
  digitalWrite(_rstPin, LOW);   // Release TFT controller from reset
  delay(10);                    // Wait for a short time
  digitalWrite(_rstPin, HIGH);  // Finish the reset process

  // Perform TFT controller initialization sequence
  tftInit();

  return true;  // Return true if initialization is successful, false otherwise
}

void TFT_Novatek_NV3041A::reset() {
  // Perform the reset operation here, using _rstPin
  digitalWrite(_rstPin, LOW);
  delay(100);
  digitalWrite(_rstPin, HIGH);
  delay(200);
}

void TFT_Novatek_NV3041A::setRotation(uint8_t rotation) {
  // Implement rotation setting for your specific display if needed
  switch (rotation) {
    case 0:
      // Set the display rotation to 0 degrees (normal orientation)
      writeCommand(NV3041A_MADCTL_MX | NV3041A_MADCTL_MY | NV3041A_MADCTL_RGB);
      break;

    case 1:
      // Set the display rotation to 90 degrees
      writeCommand(NV3041A_MADCTL_MY | NV3041A_MADCTL_MV | NV3041A_MADCTL_RGB);
      break;

    case 2:
      // Set display rotation to 180 degrees
      writeCommand(NV3041A_MADCTL_RGB);
      break;

    case 3:
      // Set the display rotation to 270 degrees
      writeCommand(NV3041A_MADCTL_MX | NV3041A_MADCTL_MV | NV3041A_MADCTL_RGB);
      break;

    default:
      // Invalid rotation value, do nothing or display an error message
      break;
  }
}

void TFT_Novatek_NV3041A::setTextColor(uint16_t color) {
  // Implement setTextColor to set the text color
  _textColor = color;
}

void TFT_Novatek_NV3041A::setTextSize(uint8_t size) {
  // Implement setTextSize to set the text size
  _textSize = size;
}

void TFT_Novatek_NV3041A::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h) {
  // Implement writeAddrWindow to set the display's address window
  // This method is called when you draw on the screen
  // Calculate the coordinates
  int16_t x_end = x + w - 1;
  int16_t y_end = y + h - 1;

  // Set column address range
  writeCommand(NV3041A_CASET);
  writeData(x >> 8);        // Start column high byte
  writeData(x & 0xFF);      // Start column low byte
  writeData(x_end >> 8);    // End column high byte
  writeData(x_end & 0xFF);  // End column low byte

  // Set row address range
  writeCommand(NV3041A_RASSET);
  writeData(y >> 8);        // Start row high byte
  writeData(y & 0xFF);      // Start row low byte
  writeData(y_end >> 8);    // End row high byte
  writeData(y_end & 0xFF);  // End row low byte

  // Set RAM write command
  writeCommand(NV3041A_RAMWR);
}

void TFT_Novatek_NV3041A::setCursor(int16_t x, int16_t y) {
  // Implement setCursor to set the text cursor position
  writeAddrWindow(x, y, 1, 1);  // Set address window to a single pixel
}

void TFT_Novatek_NV3041A::tftInit() {
  // Implement the initialization sequence for the NV3041A TFT controller
  // This method is called in the begin() method
  // Send the initialization commands to the TFT controller
  writeCommand(0x01);  // Send command 0x1 System Reset
  delay(100);          // Wait for some time
  writeCommand(0x11);  // Send command 0x11 Sleep Mode Off
  delay(100);          // Wait for some time

  // After the initialization sequence, set the display on with sending Display On with writeCommand
  writeCommand(NV3041A_DISPON);
}

void TFT_Novatek_NV3041A::fillScreen(uint16_t color) {
  // Implement fillScreen to fill the entire screen with a color
  uint8_t color_high = color >> 8;
  uint8_t color_low = color & 0xFF;
  writeCommand(NV3041A_RAMWR);
  digitalWrite(_dcxPin, HIGH);
  digitalWrite(_csPin, LOW);
  SPI.transfer(color_high);
  SPI.transfer(color_low);
  digitalWrite(_csPin, HIGH);
}

void TFT_Novatek_NV3041A::invertDisplay(bool invert) {
  // Implement invertDisplay to control display inversion if supported by your display
  if (invert) {
    // Send command to enable display inversion
    writeCommand(0x21);  // Send command 0x21 INVON
  } else {
    // Send command to disable inversion
    writeCommand(0x20);  // Send command 0x20 INVOFF
  }
}

void TFT_Novatek_NV3041A::displayOn() {
  // Implement displayOn to turn on the display
  // ...
  writeCommand(NV3041A_DISPON);
}

void TFT_Novatek_NV3041A::displayOff() {
  // Implement displayOff to turn off the display
  // ...
  writeCommand(NV3041A_DISPOFF);
}

void TFT_Novatek_NV3041A::writeCommand(uint8_t command) {
  //implement the function to send a command to the display controler
  //Set DCX pin LOW to indicate that a command is being sent
  digitalWrite(_dcxPin, LOW);
  //Select TFT controller using CS pin
  digitalWrite(_csPin, LOW);
  //send command byte over SPI
  SPI.transfer(command);
  //deselect the tft controler
  digitalWrite(_csPin, HIGH);
}

void TFT_Novatek_NV3041A::writeData(uint8_t data) {
  //implement the function to send a data to the display controler
  //Set DCX pin to HIGH to indicate that data is being sent
  digitalWrite(_dcxPin, HIGH);
  //select TFT controller using CS pin
  digitalWrite(_csPin, LOW);
  //Send data byte over SPI
  SPI.transfer(data);
  //deselect the tft controller
  digitalWrite(_csPin, HIGH);
}

void TFT_Novatek_NV3041A::print(const char *text) {
  setTextColor(_textColor);
  setCursor(_currentX, _currentY);
  for (const char *c = text; *c != '\0'; ++c) {
    SPI.transfer(*c);  // Assuming you are using SPI to send data to your display
  }
}
