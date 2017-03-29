#include "stm32f10x.h"
#include "lcd.h"
#include"System_Configuration.h"
#include "5X8ziku.h"

const uint8_t graphicPS[] = 
{
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,
0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x0F,0x0F,0x0F,
0x0F,0x07,0x07,0x01,0x00,0x00,0x00,0x07,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x1F,0x1F,0x1F,0x1F,0x0F,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF1,0xE1,0xE1,0xE1,
0xE1,0xE1,0xE1,0xE1,0xE1,0xE1,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF9,0xF9,0xF9,0xF9,0xF9,0xF9,0xF9,0xF9,0xF9,0xF9,
0xF9,0xF9,0xF9,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0xFC,0xFC,0xFC,
0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xF8,0xF8,0xF8,
0xF8,0xF0,0xF0,0xC0,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x80,0x80,0x87,0x8F,0x8F,0x8F,0x8F,0x8D,0x8D,0x9F,0x9F,0x9F,0x9F,0x9F,0x8D,0x8F,
0x8F,0x8F,0x8F,0x8E,0x80,0x80,0x80,0x80,0x80,0x80,0x8C,0x8E,0x8F,0x8F,0x8F,0x8F,
0x8F,0x9F,0x9F,0x9F,0x9F,0x9F,0x8F,0x8F,0x8F,0x8F,0x8F,0x8F,0x8E,0x8C,0x80,0x80,
0x80,0x80,0x80,0x80,0x87,0x8F,0x8F,0x8F,0x8F,0x0F,0x1F,0x1F,0x1F,0x1F,0x0F,0x0F,
0x0F,0x0F,0x0F,0x07,0x07,0x00,0x80,0x80,0x80,0x80,0x80,0x81,0x81,0x81,0x9D,0x9D,
0x9D,0x9D,0x1D,0x1D,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1D,0x1D,0x1D,0x19,0x01,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x08,0x0C,0xFC,0xFC,0xFC,0xFC,0xCC,0xDC,0xFC,0xFC,0xF8,0xF8,0xF8,0xF8,0xFC,0xFC,
0xDC,0x9C,0x1C,0x1C,0x08,0x00,0x00,0x00,0x00,0x20,0xF0,0xFC,0xFC,0xFC,0xFC,0xFC,
0xFC,0xFC,0xFC,0xF8,0xF8,0xF8,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xEC,0xE0,0x00,0x00,
0x00,0x00,0x00,0x00,0xF0,0xF0,0xF0,0xF0,0xE0,0xE0,0xF8,0xFC,0xFC,0xFC,0xFC,0xEC,
0xFC,0xFC,0xFC,0xFC,0xFC,0x1C,0x18,0x08,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x8C,0x8C,0xFC,0xFC,0xFC,0xFC,0xF8,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
const uint8_t graphic1[]=
{
/*--  宽度x高度=128x64  --*/
0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x81,0xC1,0x41,0x41,0x41,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x81,0x01,0x01,0x81,0x81,0xC1,0x01,0x81,0xC1,0x81,0x81,0xC1,0xC1,0x01,
0x81,0x81,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xE0,0xBC,0x84,0xC4,0x74,0x10,0x18,0x08,0x08,0x08,0x60,0xF0,0xB0,
0xD0,0xF0,0xF8,0xFC,0xFE,0xFF,0xE7,0xF3,0xFD,0xF8,0xFC,0xFE,0xFE,0xFF,0xFD,0xFC,
0xFC,0xFE,0xF6,0xF2,0xD0,0x80,0x00,0x00,0x80,0x00,0xE0,0xC0,0xF0,0xF8,0xFE,0xFC,
0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x7F,0x7F,0x7F,0x7F,0xFF,0xFF,
0xFF,0xFE,0xFE,0xFF,0xF9,0xFC,0xF4,0x60,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0xF0,0xF8,0xF8,0xFC,0xF8,0xFC,0xFC,0xFE,0xFE,
0xFA,0xFA,0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x02,0x47,0xC7,0xCF,0xDF,0xEF,0xEE,0xEE,0xEF,0xF7,0x77,0x7F,0x7F,
0x3F,0x1F,0x1F,0x3F,0x3F,0x3F,0xFF,0xDF,0xD7,0xD7,0xD5,0xC5,0xE5,0xF1,0xFF,0xFF,
0xFF,0xFF,0xFF,0x7F,0xFF,0xD9,0xFD,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,
0xBF,0xDF,0x4F,0x77,0x83,0x03,0x01,0x00,0x00,0xC0,0xC0,0xF0,0xF8,0xFC,0xFE,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x3F,0x72,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,
0xF0,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0x03,0x01,0x00,0x01,0x01,0x03,0x03,0x07,0x07,
0x05,0x05,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x81,0xC1,0x43,0xC7,0xC3,0x07,0x03,0x02,0x00,
0x00,0xFC,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEF,0xE7,0xF7,0xFF,0xFF,0xF7,
0xFB,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDF,0xCF,0xC7,0x85,0x86,0x87,
0x8F,0x8F,0x1D,0x19,0x3D,0x3C,0x3C,0x7C,0x7D,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFE,0xF7,0xFB,0xF8,0xFC,0xF8,0xF8,0xFC,0xFE,0xF8,0xFC,0xFE,0xFC,0xFF,0xFF,
0xFF,0xFF,0x7F,0x7F,0x3F,0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xE0,0xC0,0x67,0x7C,0x3F,0x3B,0x39,0x1C,0x1C,0x0E,0x0F,
0x0F,0x0F,0x07,0x07,0x07,0x0F,0x0F,0x0F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,
0x1F,0x0F,0x0F,0x0F,0x07,0x07,0x03,0x03,0x03,0x03,0x03,0x01,0x21,0x71,0x59,0x9B,
0xEF,0x7F,0x3F,0x03,0x03,0x01,0x01,0x03,0x00,0x00,0x03,0x07,0x07,0x03,0x07,0x07,
0x0F,0x07,0x07,0x0F,0x07,0x0F,0x0F,0x0F,0x1F,0x9F,0x9F,0xDF,0xFB,0xFB,0x79,0x39,
0x19,0x18,0x08,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x80,0x80,0x00,0x00,0x80,0x00,0x00,
0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,
0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x80,0x01,0x01,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x7F,0x7F,0x04,0x04,0x7F,0x7F,0x02,0x3F,0x7F,0x42,0x02,
0x3F,0x7F,0x42,0x00,0xFE,0xFE,0x42,0x42,0x7E,0x3C,0x00,0x66,0x66,0x00,0x60,0x1E,
0x01,0x60,0x1E,0x01,0x7E,0x7E,0x00,0x7F,0x7F,0x00,0x3C,0x7E,0x42,0x42,0x7E,0x3C,
0x00,0x1E,0x7E,0x60,0x7E,0x1E,0x00,0x3C,0x7E,0x4A,0x4A,0x6E,0x2C,0x00,0x7F,0x7F,
0x00,0x66,0x7E,0x18,0x7E,0x66,0x00,0x7F,0x7F,0x00,0x60,0x60,0x00,0x3C,0x7E,0x4A,
0x4A,0x6E,0x2C,0x00,0x3C,0x7E,0x4A,0x4A,0x6E,0x2C,0x02,0x3F,0x7F,0x42,0x00,0x3C,
0x7E,0x42,0x42,0x7E,0x3C,0x00,0xFE,0xFE,0x42,0x42,0x7E,0x3C,0x00,0x60,0x60,0x00,
0x3C,0x7E,0x42,0x66,0x24,0x00,0x7E,0x7E,0x02,0x02,0x7E,0x7C,0x00,0x00,0x00,0xFF,
0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x81,0x81,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x81,0x81,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xFF,
};

void LCM_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB | 	RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOG  , ENABLE );	//使能GPIOB 、GPIOE时钟
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6|GPIO_Pin_7| GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}
//reset
void HDReset()
{
//	LCM_RESET = 0;
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	
	delay_ms(2);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
//	LCM_RESET = 1;
	delay_ms(4);
}

void Write_ST7567_DISPLAY_DATA_serial(uint8_t  DADA)
{   
	uint8_t bit8;
	LCM_RS(Bit_SET);
	LCM_CSB(Bit_RESET);				

	for(bit8 = 0; bit8 < 8; bit8++)
	{
		LCM_SCL(Bit_RESET);
		if((DADA & 0x80) == 0x80)
			LCM_SDA(Bit_SET);
		else
			LCM_SDA(Bit_RESET);
		LCM_SCL(Bit_SET);
		DADA = (DADA <<1 );
	}
}

void Write_ST7567_COM(uint8_t COMDADA)
{
	uint8_t bit8;
        LCM_RS(Bit_RESET);
	LCM_CSB(Bit_RESET);			
	for(bit8=0;bit8<8;bit8++)
	{
		LCM_SCL(Bit_RESET);
		if((COMDADA & 0x80) == 0x80)
			LCM_SDA(Bit_SET);
		else		
			LCM_SDA(Bit_RESET);
		LCM_SCL(Bit_SET);
		COMDADA = (COMDADA <<1 );

	}	
}


void Init_ST7567(void)
{
		HDReset();
		delay_ms(100);

		Write_ST7567_COM(0xe2);	 //Soft rest

		Write_ST7567_COM(0xa2);	 //SET LCD bias(A2-1/9bias; A3-1/7bias)
		Write_ST7567_COM(0xa0);	 //SET ADC NORMAL(OB-POR seg0-00h) A0: NORMAL  A1: REVERSE
		Write_ST7567_COM(0xc8);	   //SET COM OUTPUT SCAN DIRECTION(0XXXB-NORMAL)-POR  COM63-->COM0

		Write_ST7567_COM(0xa4);	   // SET DISPLAY NORMAL (0B-NORMAL)-POR  A4:NORMAL A5:ENTIRE DISPLAY ON
		Write_ST7567_COM(0xa6);	   //SET NORMAL DISPLAY MODE(0B-NORMAL)  A6:NORMAL  A7:REVERSE
		Write_ST7567_COM(0x25);	   //SET INTERNAL REGULATOR RESDASTOR RATIO(100B)-POR

		Write_ST7567_COM(0x81);

		Write_ST7567_COM(0x1a);	   // SET CONTRAST CONTROL REGISTER(00,0000H-11,1111H) 30  对比度请修改此值，调浓增大此值，反之调淡。
		Write_ST7567_COM(0x2f);	   //SET POWER CONTROL REGISTER (ALL INTERNAL)

		Write_ST7567_COM(0x40);	   //end of initialzation
		Write_ST7567_COM(0xaf);	   //DisPlay On
}


void clear_screen(uint8_t data)
{
	uint8_t i,j;
	for(i=0;i<9;i++)
	{
		LCM_RS(Bit_SET);
		Write_ST7567_COM(0xb0+i);
		Write_ST7567_COM(0x10);
		Write_ST7567_COM(0x00);
		for(j=0;j<132;j++)
			Write_ST7567_DISPLAY_DATA_serial(data);
	}
}

void Display_pic1(void)
{
	uint8_t i,j;
	uint8_t adressd=0;
	for(i=0;i<8;i++)
	{
		LCM_CSB(Bit_RESET);
		Write_ST7567_COM(0xb0+i);
	  Write_ST7567_COM(0x10);
		Write_ST7567_COM(0x00);
		for(j=0;j<128;j++)
		{
			Write_ST7567_DISPLAY_DATA_serial(graphic1[adressd]);
			adressd++;
		}
	}
} 

void Lcd_Address(uint16_t page,uint16_t column)
{
	column=column-0x01;
	Write_ST7567_COM(0xb0+page-1);   /*设置页地址*/ //1-8
	Write_ST7567_COM(0x10+(column>>4&0x0f));	/*设置列地址的高4位*///0-128
	Write_ST7567_COM(column&0x0f);	/*设置列地址的低4位*/	
}


/*显示128x64点阵图像*/
void Display_128x64(u8 *dp)
{
	uint8_t i,j;
	for(j=0;j<8;j++)
	{
		Lcd_Address(j+1,1);
		for (i=0;i<128;i++)
		{	
		  Write_ST7567_DISPLAY_DATA_serial(*dp);	 	/*写数据到LCD,每写完一个8位的数据后列地址自动加1*/
		  dp++;
		}
	}
}

/*显示16x16点阵图像、汉字、生僻字或16x16点阵的其他图标*/
void Display_Graphic_16x16(uint8_t page,uint8_t column,uint8_t *dp)
{
	uint8_t i,j;
	for(j=0;j<2;j++)
	{
		Lcd_Address(page+j,column);
		for (i=0;i<16;i++)
		{	
			Write_ST7567_DISPLAY_DATA_serial(*dp);	 /*写数据到LCD,每写完一个8位的数据后列地址自动加1*/
			dp++;
		}
	}
}

/*显示8x16点阵图像、ASCII, 或8x16点阵的自造字符、其他图标*/
void Display_Graphic_8x16(uint8_t page,uint8_t column,uint8_t *dp)
{
	uint8_t i,j;
	for(j=0;j<2;j++)
	{
		Lcd_Address(page+j,column);
		for (i=0;i<8;i++)
		{	
			Write_ST7567_DISPLAY_DATA_serial(*dp);	 	/*写数据到LCD,每写完一个8位的数据后列地址自动加1*/
			dp++;
		}
	}
}

/*显示5*8点阵图像、ASCII, 或5x8点阵的自造字符、其他图标*/
void Display_Graphic_5x8(uint8_t page,uint8_t column,uint8_t *dp)
{
	uint8_t col_cnt;
    Lcd_Address(page,column);
	for (col_cnt=0;col_cnt<5;col_cnt++)
	{	
		Write_ST7567_DISPLAY_DATA_serial(*dp);
		dp++;
	}
}

void Scribing(void)
{
	uint8_t i;
	Lcd_Address(2, 1);
	for (i=0; i<128; i++)
	{
		Write_ST7567_DISPLAY_DATA_serial(0x08);
	}		
}

void DisplayFont(uint8_t page, uint8_t column, uint8_t No, uint8_t typeface)
{
	uint8_t *dp;
	switch (typeface)
	{
		case FONT_5X8:
			dp = font5x8[No];
			Display_Graphic_5x8(page, column, dp);
			break;
		case FONT_8X16:
			dp = font8x16[No];
			Display_Graphic_8x16(page, column, dp);
			break;
		case FONT_16X16:
			dp = font16x16[No];
			Display_Graphic_16x16(page, column, dp);
			break;
		case FONT_128X64:
			dp = IMG128x64;
			Display_128x64(dp);
			break;
		case SCRIBING:
			Scribing();
			break;
		default:
			break;
	}		   
}


/*全屏清屏*/
void ClearScreen(void)
{
	unsigned char i,j;
	for(i=0;i<9;i++)
	{
		Write_ST7567_COM(0xb0+i);  //从第0页开始
		Write_ST7567_COM(0x10);
		Write_ST7567_COM(0x00);
		for(j=0;j<132;j++)
		{
		 	Write_ST7567_DISPLAY_DATA_serial(0x00);
		}
	}
}

void Display_PS(void)
{
	
	uint8_t i,j;
	uint8_t adressd=0;
	for(i=0;i<8;i++)
	{
		LCM_CSB(Bit_RESET);
		Write_ST7567_COM(0xb0+i);
	  Write_ST7567_COM(0x10);
		Write_ST7567_COM(0x00);
		for(j=0;j<128;j++)
		{
			Write_ST7567_DISPLAY_DATA_serial(graphicPS[adressd]);
			adressd++;
		}
	}
} 
