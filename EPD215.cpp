/*********************************************************************
This is a library for the Pervasive Displays Epaper Screen E2215CS062.
This library has been written for and fully supports the Teensy-LC and the rest of the Teensy 3.x series and uses their native SPI library.
It can also be used with Arduino, ESP8266, and most available microcontrollers, optionally using a slower bitbanged interface.

This code and further updates will be hosted at https://github.com/jarek319/EPD215

These displays use SPI to communicate. For more information on the signaling protocol used, please see the official datasheet for all the details: http://www.pervasivedisplays.com/LiteratureRetrieve.aspx?ID=232068

Written by Jarek Lupinski for Soniktech LLC  
Adapted from Adafruit GFX library driver code
BSD license, check license.txt for more information
All text above must be included in any redistribution
*********************************************************************/
#include <stdlib.h>
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "EPD215.h"

#define HEIGHT 208
#define WIDTH 112
#define SPACING 2

// the memory buffer for the screen
// Image generated by LCD Assistant
// http://en.radzio.dxp.pl/bitmap_converter/
uint8_t _buffer[ 2912 ] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x0F, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
  0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
  0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x3F, 0xE0, 0x00, 0x00, 0x0F, 0xF8,
  0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x01, 0xFF, 0xFC, 0x00, 0x00, 0x7F, 0xFF, 0x00, 0x00,
  0x00, 0x30, 0x0C, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0x00, 0x01, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x30,
  0x0C, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x30, 0x0C, 0x00,
  0x00, 0x3F, 0xFF, 0xFF, 0xE0, 0x0F, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x7F,
  0xFF, 0xFF, 0xF0, 0x1F, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x30, 0x0C, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
  0xF8, 0x3F, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x30, 0x0C, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFC, 0x7F,
  0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x30, 0x0C, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFC, 0x7F, 0xFF, 0xFF,
  0xF0, 0x3E, 0x00, 0x30, 0x0C, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xC0, 0x7C,
  0x00, 0x30, 0x0C, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x81, 0xF0, 0x00, 0x30,
  0x0C, 0x00, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x83, 0xC0, 0x00, 0x30, 0x0C, 0x00,
  0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xC0, 0x00, 0x30, 0x0C, 0x00, 0x07, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0xC0, 0x40, 0x30, 0x0C, 0x00, 0x0F, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFE, 0x03, 0xC0, 0xC0, 0x30, 0x0C, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFE, 0x03, 0xC3, 0xC0, 0x30, 0x0C, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
  0x01, 0xCF, 0xC0, 0x30, 0x0C, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x01, 0xFE,
  0xC0, 0x30, 0x0C, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFC, 0xC0, 0x30,
  0x0C, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x00, 0x71, 0xC0, 0x30, 0x0C, 0x00,
  0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0x00, 0x41, 0xC0, 0x30, 0x0C, 0x00, 0x07, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x83, 0x80, 0x03, 0x80, 0x30, 0x0C, 0x00, 0x07, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFE, 0x01, 0xC0, 0x07, 0x00, 0x30, 0x0C, 0x00, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xF8, 0x01, 0xE0, 0x0F, 0x00, 0x30, 0x0C, 0x00, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x03,
  0xFC, 0x7E, 0x00, 0x30, 0x0C, 0x00, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x0F, 0xFF, 0xF8,
  0x00, 0x30, 0x0C, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x1F, 0xFF, 0xE0, 0x00, 0x30,
  0x0C, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x7F, 0xFF, 0x00, 0x00, 0x30, 0x0C, 0x00,
  0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x01, 0xFF, 0xFF, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x01, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFC, 0x07, 0xFF, 0xFE, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFC, 0x0F, 0xFF, 0xFE, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
  0x3F, 0xFF, 0xFC, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF,
  0xFC, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00,
  0x00, 0x30, 0x0C, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x30,
  0x0C, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x30, 0x0C, 0x00,
  0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x0F,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x07, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
  0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00,
  0x00, 0x30, 0x0C, 0x00, 0x07, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x30,
  0x0C, 0x00, 0x3F, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00,
  0x7F, 0xFF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0xF0, 0x1F,
  0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x01, 0xC0, 0x07, 0x07, 0xFF,
  0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x03, 0x80, 0x03, 0x83, 0xFF, 0xFF, 0xFF,
  0xFF, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x03, 0x84, 0x01, 0xC7, 0xFF, 0xFF, 0xFF, 0xFE, 0x00,
  0x00, 0x00, 0x00, 0x30, 0x0C, 0x07, 0x1E, 0x01, 0xCF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00,
  0x00, 0x30, 0x0C, 0x07, 0x3E, 0x00, 0xFE, 0x3F, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x30,
  0x0C, 0x06, 0xFF, 0x00, 0xF8, 0x1F, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x07,
  0xE3, 0x00, 0xE0, 0x0F, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x07, 0xC3, 0x80,
  0xC0, 0x07, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x07, 0x01, 0xC0, 0xC0, 0x03,
  0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x04, 0x01, 0xC0, 0xC0, 0x01, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x01, 0xE1, 0xC0, 0x00, 0xFF, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x07, 0xC1, 0xC0, 0x00, 0x7F, 0xFC, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x30, 0x0C, 0x00, 0x0F, 0x03, 0x80, 0x00, 0x3F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
  0x0C, 0x00, 0x3E, 0x07, 0x00, 0x00, 0x1F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00,
  0xF8, 0x0F, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x7E, 0xFC,
  0x00, 0x00, 0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x3F, 0xF8, 0x00, 0x00,
  0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x01, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
  0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
  0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x30, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0,
  0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x00, 0x1C,
  0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00,
  0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x00, 0x00, 0x58, 0x3C, 0x3C, 0x5C, 0x00, 0x16, 0x82, 0x1C, 0x41, 0x00, 0x10, 0x5C,
  0x1E, 0xFE, 0x70, 0x66, 0x66, 0x66, 0x00, 0x22, 0x82, 0x22, 0x63, 0x00, 0x7C, 0x62, 0x23, 0xDA,
  0x70, 0x42, 0x42, 0x42, 0x00, 0x22, 0x92, 0x02, 0x22, 0x00, 0x10, 0x42, 0x41, 0x92, 0x58, 0x7E,
  0x7E, 0x42, 0x00, 0x22, 0xAA, 0x3E, 0x22, 0x00, 0x10, 0x40, 0x41, 0x92, 0x48, 0x40, 0x40, 0x42,
  0x00, 0x7F, 0xAA, 0x42, 0x14, 0x00, 0x10, 0x40, 0x41, 0x92, 0x44, 0x60, 0x60, 0x44, 0x00, 0x41,
  0xAA, 0x46, 0x14, 0x00, 0x10, 0x40, 0x62, 0x92, 0x42, 0x3E, 0x3E, 0x78, 0x00, 0x41, 0xC4, 0x3A,
  0x08, 0x00, 0x10, 0x40, 0x3C, 0x92, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xC7, 0x00, 0x00, 0x04, 0x00, 0x04,
  0x20, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x21, 0x00, 0x00, 0x04, 0x00, 0x04, 0x20, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x21, 0x00, 0x00, 0x04, 0x00, 0x04, 0x20, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x04, 0x61, 0x01, 0xC1, 0xE4, 0x60, 0x04, 0x21, 0xE1, 0x03, 0xC3, 0xC0, 0x00,
  0x00, 0x07, 0xC1, 0x02, 0x22, 0x04, 0xC0, 0x07, 0xE2, 0x31, 0x06, 0x64, 0x00, 0x00, 0x00, 0x04,
  0x61, 0x00, 0x24, 0x05, 0x00, 0x04, 0x24, 0x11, 0x04, 0x24, 0x00, 0x00, 0x00, 0x04, 0x21, 0x03,
  0xE4, 0x06, 0x00, 0x04, 0x24, 0x11, 0x07, 0xE3, 0x80, 0x00, 0x00, 0x04, 0x21, 0x04, 0x24, 0x05,
  0x80, 0x04, 0x24, 0x11, 0x04, 0x00, 0x40, 0x00, 0x00, 0x04, 0x61, 0x04, 0x66, 0x04, 0xC0, 0x04,
  0x26, 0x21, 0x06, 0x00, 0x40, 0x00, 0x00, 0x07, 0xC7, 0xE3, 0xA1, 0xE4, 0x60, 0x04, 0x23, 0xC7,
  0xE3, 0xE7, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x04, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x02, 0x04, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0xC5, 0xC1, 0xE0, 0x01, 0xEF, 0xE5, 0xC3, 0xC5, 0xC0, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x26, 0x22, 0x20, 0x02, 0x32, 0x06, 0x26, 0x66, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24,
  0x24, 0x20, 0x04, 0x12, 0x04, 0x24, 0x24, 0x20, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE4, 0x24, 0x20,
  0x04, 0x12, 0x04, 0x27, 0xE4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x24, 0x24, 0x20, 0x04, 0x12,
  0x04, 0x24, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x64, 0x26, 0x60, 0x06, 0x23, 0x04, 0x26,
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xA4, 0x23, 0xA0, 0x03, 0xC1, 0xE4, 0x23, 0xE4, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00,
  0x3C, 0x1E, 0x42, 0x5C, 0x1E, 0x3C, 0x3C, 0x00, 0x1E, 0x10, 0x00, 0x00, 0x00, 0x00, 0x40, 0x23,
  0x42, 0x62, 0x20, 0x66, 0x40, 0x00, 0x23, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x40, 0x41, 0x42, 0x42,
  0x40, 0x42, 0x40, 0x00, 0x41, 0x10, 0x00, 0x00, 0x00, 0x00, 0x38, 0x41, 0x42, 0x40, 0x40, 0x7E,
  0x38, 0x00, 0x41, 0x10, 0x00, 0x00, 0x00, 0x00, 0x04, 0x41, 0x42, 0x40, 0x40, 0x40, 0x04, 0x00,
  0x41, 0x10, 0x00, 0x00, 0x00, 0x00, 0x04, 0x62, 0x46, 0x40, 0x60, 0x60, 0x04, 0x00, 0x62, 0x10,
  0x00, 0x00, 0x00, 0x00, 0x78, 0x3C, 0x3A, 0x40, 0x1E, 0x3E, 0x78, 0x00, 0x3C, 0x10, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x30, 0x40, 0x30, 0x70, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,
  0x00, 0x14, 0x00, 0x00, 0x30, 0x40, 0x30, 0x10, 0x00, 0x20, 0x30, 0x00, 0x00, 0x00, 0x00, 0x14,
  0x00, 0x00, 0x00, 0x40, 0x00, 0x10, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x5C, 0x5C,
  0x70, 0x5C, 0x70, 0x10, 0x1C, 0xFE, 0x70, 0x1E, 0x5C, 0x00, 0x00, 0x22, 0x62, 0x62, 0x10, 0x62,
  0x10, 0x10, 0x22, 0x20, 0x10, 0x23, 0x62, 0x00, 0x00, 0x22, 0x42, 0x42, 0x10, 0x42, 0x10, 0x10,
  0x02, 0x20, 0x10, 0x41, 0x42, 0x00, 0x00, 0x22, 0x42, 0x42, 0x10, 0x42, 0x10, 0x10, 0x3E, 0x20,
  0x10, 0x41, 0x42, 0x00, 0x00, 0x7F, 0x42, 0x42, 0x10, 0x42, 0x10, 0x10, 0x42, 0x20, 0x10, 0x41,
  0x42, 0x00, 0x00, 0x41, 0x42, 0x42, 0x10, 0x42, 0x10, 0x10, 0x46, 0x30, 0x10, 0x62, 0x42, 0x00,
  0x00, 0x41, 0x42, 0x42, 0x7E, 0x42, 0x7E, 0x7E, 0x3A, 0x1E, 0x7E, 0x3C, 0x42, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x10, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0F, 0x1E, 0x78, 0xE1, 0x37, 0xE3, 0xC7, 0x96, 0x00, 0x3C, 0x7B, 0xF8, 0x00, 0x00, 0x10,
  0x33, 0x64, 0x21, 0x61, 0x04, 0x2C, 0x19, 0x00, 0x60, 0xCF, 0x68, 0x00, 0x00, 0x1C, 0x21, 0x44,
  0x21, 0x81, 0x07, 0xE8, 0x11, 0x00, 0x40, 0x86, 0x48, 0x00, 0x00, 0x03, 0x21, 0x44, 0x21, 0x41,
  0x04, 0x08, 0x11, 0x00, 0x40, 0x86, 0x48, 0x00, 0x00, 0x01, 0x33, 0x44, 0x21, 0x21, 0x06, 0x0C,
  0x11, 0x18, 0x60, 0xCE, 0x48, 0x00, 0x00, 0x1E, 0x1E, 0x44, 0xF9, 0x11, 0xE3, 0xE7, 0x91, 0x18,
  0x3C, 0x7A, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// the most basic function, set a single pixel
// drawPixel function cribbed from adafruit gfx library
void EPD215::drawPixel( int16_t x, int16_t y, uint16_t color ) { 
  if (( x < 0 ) || ( y < 0 ) || ( x >= WIDTH ) || ( y >= HEIGHT )) return;
  uint32_t t;
  switch ( _rotation ) {
    case 1:
      t = x;
      x = WIDTH  - 1 - y;
      y = t;
      break;
    case 2:
      x = WIDTH  - 1 - x;
      y = HEIGHT - 1 - y;
      break;
    case 3:
      t = x;
      x = y;
      y = HEIGHT - 1 - t;
      break;
  }
  uint8_t *ptr = &_buffer[( x / 8 ) + y * (( WIDTH + 7 ) / 8 )];
  if ( color ) *ptr |= 0x80 >> ( x & 7 );
  else         *ptr &= ~( 0x80 ) >> ( x & 7 );
}

EPD215::EPD215( uint32_t cs, uint32_t dc, uint32_t rs, uint32_t bs, int32_t d0, int32_t d1 ) : Adafruit_GFX ( WIDTH, HEIGHT ) {
  _cs = cs;
  _dc = dc;
  _rs = rs;
  _bs = bs;
  _d0 = d0;
  _d1 = d1;
}

void EPD215::softwareSpi( uint8_t data ) {
  for ( int i = 0; i < 8; i++ ) {
    if ((( data >> (7 - i) ) & 0x01 ) == 1 ) digitalWrite( _d1, HIGH );
    else digitalWrite( _d1, LOW );
    digitalWrite( _d0, HIGH );
    digitalWrite( _d0, LOW );
  }
}

void EPD215::clearScreen() {
  for ( int i = 0; i < 2912; i++ ) _buffer[ i ] = 0x00;
}

void EPD215::writeToBuffer( uint8_t *buf ) {
  for ( int i = 0; i < 2912; i++ ) _buffer[ i ] = buf[ i ];
}

void EPD215::initScreen() {
  if ( _d0 == 0xFF ) _spiMode = 1;
  if ( _spiMode ) {
    Serial.println( "Beginning SPI" );
    SPI.begin();
    //SPI.beginTransaction( SPISettings( 1000000, MSBFIRST, SPI_MODE0 ));
  }
  else {
    Serial.println( "Beginning Software SPI" );
    pinMode( _d0, OUTPUT );
    pinMode( _d1, OUTPUT );
  }
  pinMode( _cs, OUTPUT );
  pinMode( _dc, OUTPUT );
  pinMode( _rs, OUTPUT );
  pinMode( _bs, INPUT_PULLUP );
  Serial.println( "Resetting screen" );
  digitalWrite( _rs, HIGH );        //RES# = 1
  delay( 1 );                       //Delay 1ms
  digitalWrite( _cs, HIGH );        //CS# = 1
  Serial.println( "Waiting for busy signal" );
  while ( digitalRead( _bs ) == HIGH ); //Make sure BUSY = LOW
}

void EPD215::updateScreen( uint8_t temperature ) {
  uint8_t data1[] = { 0xCF, 0x00 };
  _sendIndexData( DRIVEROUTPUTCONTROL, data1, 2 );
  uint8_t data2[] = { 0x00 };
  _sendIndexData( GATESCANSTART, data2, 1 );
  uint8_t data3[] = { 0x03 };
  _sendIndexData( DATAENTRYMODE, data3, 1 );
  uint8_t data4[] = { 0x00, 0x0D };
  _sendIndexData( SETRAMXSTARTEND, data4, 2 );
  uint8_t data5[] = { 0x00, 0xCF };
  _sendIndexData( SETRAMYSTARTEND, data5, 2 );
  uint8_t data6[] = { 0x00 };
  _sendIndexData( SETRAMXADDRESS, data6, 1 );
  uint8_t data7[] = { 0x00 };
  _sendIndexData( SETRAMYADDRESS, data7, 1 );
  uint8_t data8[] = { 0x80 };
  _sendIndexData( SETBORDERWAVEFORM, data8, 1 );
  uint8_t data9[] = { 0x10, 0x0A };
  _sendIndexData( SETGATEDRIVINGVOLT, data9, 2 );
  uint8_t data10[] = { 0x00 };
  _sendIndexData( NORMALANALOGMODE1, data10, 1 );
  uint8_t data11[] = { 0x00, 0x00, 0x00 };
  _sendIndexData( NORMALANALOGMODE2, data11, 3 );
  //Send Image Data
  _sendIndexData( SENDIMAGEDATA, _buffer, 2912 );
  //Update command sequence
  while ( digitalRead( _bs ) == HIGH ); //Make sure BUSY = LOW
  uint8_t data12[] = { temperature, 0x00 };
  _sendIndexData( SETTEMPERATURE, data12, 2 );
  uint8_t data13[] = { 0xF7 };
  _sendIndexData( SETUPDATESEQUENCE, data13, 1 );
  uint8_t data14[] = { 0x00 };
  _sendIndexData( ACTIVEDISPLAYUPDATE, data14, 1 );
  while ( digitalRead( _bs ) == HIGH ); //Make sure BUSY = LOW
}

void EPD215::_sendIndexData( uint8_t index, uint8_t *data, uint16_t len ) {
  digitalWrite( _cs, LOW );      //CS Low
  digitalWrite( _dc, LOW );      //DC Low
  delay(1);
  if ( _spiMode ) SPI.transfer( index );         //SPI Send Index
  else softwareSpi( index );
  delay(1);
  digitalWrite( _dc, HIGH );     //DC High
  delay(1);
  for ( int i = 0; i < len; i++ ) {
    if ( _spiMode ) SPI.transfer( data[ i ] ); //SPI Send Data
    else softwareSpi( data[ i ] );
  }
  delay(1);
  digitalWrite( _cs, HIGH );     //CS High
}
