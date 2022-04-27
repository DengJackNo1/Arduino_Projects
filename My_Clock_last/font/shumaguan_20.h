#include <pgmspace.h>
// 数码管字体 0-9:,显示时钟用,20像素
const uint8_t  shumaguan_20[] PROGMEM = {
0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x14, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x01, 
0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0C, 
0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x01, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x10, 
0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x0F, 
0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 
0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0F, 
0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0C, 
0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x01, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x10, 
0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x0F, 
0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 
0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0F, 
0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0C, 
0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x01, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00, 0x0F, 
0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x0E, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 
0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0F, 
0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x39, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0C, 
0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x01, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3A, 0x00, 0x00, 0x00, 0x09, 
0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x09, 
0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0xB8, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9B, 0x04, 0x00, 0xA3, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD0, 0xB0, 0x33, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xBB, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x66, 0x13, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x7B, 0xCE, 0xD8, 0x90, 0x46, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x57, 0x9F, 0xD8, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xDF, 0x7F, 0x3D, 0xB6, 0xF2, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9B, 0x08, 0x00, 0x00, 0x08, 0xBB, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x99, 0x06, 0x00, 0x00, 0x00, 0x0C, 0x00, 
0x63, 0xE7, 0xD2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xBF, 0xFC, 0xFF, 0x00, 0x15, 0x66, 0x2A, 0x7D, 0xCE, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 
0xFF, 0xFF, 0x28, 0xC5, 0xFF, 0x00, 0x00, 0x68, 0x00, 0x1F, 0xCC, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9B, 0x15, 0x00, 0x59, 0xF2, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xBB, 0xFC, 0xFF, 0x00, 0x0A, 0x7F, 0xF4, 
0xFF, 0xFF, 0xFF, 0xDA, 0x83, 0x2E, 0x11, 0x66, 0xD0, 0x8A, 0x44, 0x00, 
0x24, 0x57, 0x92, 0xBD, 0xF2, 0x99, 0x26, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xDF, 0x7F, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0xEE, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC7, 0x1F, 0x00, 0x00, 0x44, 0xEB, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x59, 0x59, 0xF2, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x96, 0x15, 0x00, 0x00, 0x24, 0xD0, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xBB, 0xFC, 0xFF, 0x28, 0xB2, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xDA, 0x81, 0x06, 0x11, 0x66, 0x3B, 0x9F, 0xF4, 0xC9, 
0xA5, 0x72, 0x50, 0x24, 0x00, 0x44, 0x8A, 0xD0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xDF, 0x7F, 0x00, 0x24, 0xCC, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9B, 0x08, 0x00, 0x59, 0xF2, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x96, 0x06, 0x00, 0x22, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0xFA, 0x83, 0x0C, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0xE7, 0xFF, 0xFF, 0xE7, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xD2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xBB, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xBB, 0xFC, 0xFF, 0x66, 0x13, 0x2E, 0x83, 
0xDA, 0xFF, 0xFF, 0xDA, 0x77, 0x04, 0x11, 0x66, 0x00, 0x13, 0x7F, 0xE3, 
0xC7, 0x96, 0x57, 0x22, 0x00, 0x44, 0x8A, 0xD0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0xC3, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x00, 0x2C, 0xC9, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD8, 0x2E, 0xA3, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9D, 0x0A, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFC, 0xBB, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x13, 0x2E, 0x83, 
0xDA, 0xFF, 0xFF, 0xFF, 0xF0, 0x92, 0x0E, 0x00, 0x00, 0x13, 0x7F, 0xE3, 
0xC7, 0x96, 0x57, 0x22, 0x00, 0x44, 0x8A, 0xD0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xDF, 0x7F, 0x00, 0x59, 0xFC, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9B, 0x08, 0x00, 0x6E, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x99, 0x06, 0x00, 0x00, 0x2C, 0xC9, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD8, 0x2E, 0xA3, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9D, 0x0A, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFC, 0xBB, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x13, 0x06, 0x7B, 
0xCE, 0xEE, 0xEB, 0xEB, 0xD6, 0x92, 0x0E, 0x00, 0xD0, 0x8A, 0x44, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x8A, 0xD0, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xDF, 0x7F, 0x3F, 0xB8, 0xF4, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9B, 0x08, 0x00, 0x00, 0x08, 0xBB, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x99, 0x06, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0x2A, 0x39, 0xD0, 0x00, 0x96, 0xFF, 0xFF, 
0xFF, 0xFF, 0xDF, 0x50, 0x00, 0xA5, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xBB, 0xFC, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x66, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x7D, 0xCE, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0xC5, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x00, 0x19, 0xB8, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9B, 0x15, 0x00, 0xA3, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xA1, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xBB, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xFF, 0xFF, 0x66, 0x13, 0x06, 0x7B, 
0xCE, 0xEE, 0xEB, 0xE1, 0x5F, 0x02, 0x41, 0xC1, 0xD8, 0x8E, 0x44, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x8A, 0xD0, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xDF, 0x7F, 0x3F, 0xB8, 0xF4, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9B, 0x08, 0x00, 0x00, 0x08, 0xBB, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x99, 0x06, 0x00, 0x00, 0x19, 0xB8, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9B, 0x15, 0x00, 0xA3, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xA1, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD8, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xD8, 0xFF, 0xFF, 0x68, 0x24, 0x11, 0x59, 
0xA7, 0xEE, 0xF6, 0xAC, 0x6C, 0x15, 0x24, 0x68, 0x00, 0x0C, 0x79, 0xDF, 
0xD2, 0xA5, 0x6A, 0x44, 0x06, 0x22, 0x77, 0xCE, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0xE1, 0x7F, 0x00, 0x0E, 0xBD, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9B, 0x08, 0x00, 0x15, 0xCE, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x99, 0x06, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
0x14, 0x44, 0x69, 0x67, 0x69, 0x66, 0x61, 0x63, 0x65, 0x57, 0x69, 0x64, 
0x65, 0x20, 0x52, 0x65, 0x67, 0x75, 0x6C, 0x61, 0x72, 0x00, 0x0C, 0x44, 
0x69, 0x67, 0x69, 0x66, 0x61, 0x63, 0x65, 0x57, 0x69, 0x64, 0x65, 0x01,
};