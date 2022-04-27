#include <pgmspace.h>
// 数码管字体 0-9,显示秒钟用,15像素
const uint8_t  shumaguan_15[] PROGMEM = {
0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0F, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x01, 
0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x09, 
0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x01, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x0D, 
0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0C, 
0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 
0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0B, 
0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x09, 
0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x0D, 
0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0C, 
0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 
0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0B, 
0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x09, 
0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x01, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00, 0x0C, 
0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0B, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 
0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0B, 
0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x39, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x09, 
0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x01, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3A, 0x00, 0x00, 0x00, 0x07, 
0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x07, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0xF8, 0xFF, 
0xFF, 0xFF, 0xFF, 0x96, 0x17, 0x96, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xEE, 0xEE, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xDA, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x6C, 0xC9, 0xE9, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x11, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBF, 0xFC, 0xFF, 
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xEE, 0xEE, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xFF, 0xFF, 0x15, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xA7, 0xAA, 0x00, 0x3B, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x81, 0x00, 0x00, 
0x08, 0x2E, 0xD8, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xCE, 0xFF, 0x00, 
0x41, 0xAC, 0xFA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xA1, 0xFF, 0x00, 
0x59, 0x33, 0xA3, 0xFA, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDA, 0xFF, 0x11, 0x17, 
0x74, 0xA1, 0xB4, 0xBD, 0x68, 0x0E, 0x3B, 0xFC, 0xBD, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA7, 0xAA, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6E, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xD0, 0x3F, 0x59, 0xF4, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x96, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDA, 
0xFF, 0x44, 0xE5, 0xF4, 0xC9, 0xB0, 0x6E, 0x0E, 0x00, 0x4C, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xBD, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA7, 0xAA, 0x5B, 0xF4, 
0xFF, 0xFF, 0xFF, 0xFF, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x08, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2E, 
0xD6, 0xFF, 0xCE, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFA, 0xFF, 0xFF, 0xFF, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xDA, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDA, 
0xFF, 0x3B, 0x0C, 0x6A, 0xC3, 0xB4, 0x6E, 0x19, 0x00, 0x4C, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xBD, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x9B, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x59, 0x00, 0x37, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xB4, 
0x2A, 0x96, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xFF, 0xDA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x3B, 0x0C, 0x6A, 0xC3, 0xB4, 0xA1, 0x6C, 0x22, 0x11, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xBD, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA7, 0xAA, 0x3D, 0xC7, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x81, 0x00, 0x00, 0x37, 0xF8, 0xFF, 0xFF, 
0xFF, 0xFF, 0xB4, 0x2A, 0x96, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xDA, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x4C, 0x00, 0x08, 0x35, 0x63, 0x61, 0x5B, 0x22, 
0x11, 0xFC, 0xBD, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBD, 0xFC, 0xFF, 0xFF, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFF, 0xFF, 0xC9, 0xC9, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA7, 
0xAA, 0x00, 0x3B, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x81, 0x00, 0x3D, 0xCE, 
0xFF, 0xFF, 0xFF, 0xFF, 0xD0, 0x2E, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x96, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xDA, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAC, 
0xFA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA1, 
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0x00, 0x39, 
0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x96, 0x96, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x96, 0x96, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xDA, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xB4, 0xF2, 0xCC, 0x0C, 0x0E, 0x5D, 0x37, 
0x0A, 0x1B, 0x00, 0x11, 0xFF, 0xE1, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBD, 
0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xC9, 0xC9, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xA7, 0xAA, 0x00, 0x3B, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x81, 
0x00, 0x00, 0x39, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x96, 0x96, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x96, 0xFF, 0xFF, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x41, 0x04, 
0x48, 0x96, 0xAA, 0x55, 0x06, 0x02, 0x4E, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xA5, 0xFA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xA7, 0xAA, 0x13, 0x8C, 0xFA, 0xFF, 0xFF, 
0xFF, 0xFF, 0x81, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x14, 0x44, 0x69, 0x67, 0x69, 
0x66, 0x61, 0x63, 0x65, 0x57, 0x69, 0x64, 0x65, 0x20, 0x52, 0x65, 0x67, 
0x75, 0x6C, 0x61, 0x72, 0x00, 0x0C, 0x44, 0x69, 0x67, 0x69, 0x66, 0x61, 
0x63, 0x65, 0x57, 0x69, 0x64, 0x65, 0x01};