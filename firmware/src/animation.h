#pragma once

#include <OctoWS2811.h>

const uint8_t kMatrixWidth = 12;
const uint8_t kMatrixHeight = 24;
#define LED_COUNT    		(kMatrixWidth * kMatrixHeight)
#define MAX_DIMENSION   ((kMatrixWidth > kMatrixHeight) ? kMatrixWidth : kMatrixHeight)

struct Animation {
public:
  Animation(OctoWS2811* leds) : leds{leds} {}

  virtual void step() = 0;

  void setBrightness(float bri) {
    // expects [0 - 1]
    // 0 - 64 levels
    brightness = bri;
  }

protected:
  OctoWS2811* leds;
  float brightness = 0;

  void setPixel(uint8_t x, uint8_t y, int rgb) {
    int r = ((rgb>>16)&0xFF);
    int g = ((rgb>>8)&0xFF);
    int b = (rgb&0xFF);
    setPixel(x, y, r, g, b);
  }

  inline void setPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
    if (brightness >= 1.0) {
      leds->setPixel(XY(x, y), (r << 16) | (g << 8) | b);
    } else {
      // convert r, g, b to h, s, l

      // apply brightness as l
      // convert back to r, g, b

      leds->setPixel(XY(x, y), ((int)(r * brightness) << 16) | ((int)(g * brightness) << 8) | ((int)(b * brightness)));

    }
  }

  int makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness) {
  	unsigned int red, green, blue;
  	unsigned int var1, var2;

  	if (hue > 359) hue = hue % 360;
  	if (saturation > 100) saturation = 100;
  	if (lightness > 100) lightness = 100;

  	// algorithm from: http://www.easyrgb.com/index.php?X=MATH&H=19#text19
  	if (saturation == 0) {
  		red = green = blue = lightness * 255 / 100;
  	} else {
  		if (lightness < 50) {
  			var2 = lightness * (100 + saturation);
  		} else {
  			var2 = ((lightness + saturation) * 100) - (saturation * lightness);
  		}
  		var1 = lightness * 200 - var2;
  		red = h2rgb(var1, var2, (hue < 240) ? hue + 120 : hue - 240) * 255 / 600000;
  		green = h2rgb(var1, var2, hue) * 255 / 600000;
  		blue = h2rgb(var1, var2, (hue >= 120) ? hue - 120 : hue + 240) * 255 / 600000;
  	}
  	return (red << 16) | (green << 8) | blue;
  }

unsigned int rgb2hsl(int rgb) {
  //R, G and B input range = 0 ÷ 255
  //H, S and L output range = 0 ÷ 1.0

  float var_R = ((rgb>>16)&0xFF) / 255;
  float var_G = ((rgb>>8)&0xFF) / 255;
  float var_B = (rgb&0xFF) / 255;

  int var_Min = min( min( var_R, var_G ), var_B );    //Min. value of RGB
  int var_Max = max( max( var_R, var_G ), var_B );    //Max. value of RGB
  int del_Max = var_Max - var_Min;             //Delta RGB value

  int L = ( var_Max + var_Min ) / 2;

  if ( del_Max == 0 )                     //This is a gray, no chroma...
  {
      // H = 0
      // S = 0
  }
  else                                    //Chromatic data...
  {
  //    if ( L < 0.5 ) S = del_Max / ( var_Max + var_Min )
  //    else           S = del_Max / ( 2 - var_Max - var_Min )
  //
  //    del_R = ( ( ( var_Max - var_R ) / 6 ) + ( del_Max / 2 ) ) / del_Max
  //    del_G = ( ( ( var_Max - var_G ) / 6 ) + ( del_Max / 2 ) ) / del_Max
  //    del_B = ( ( ( var_Max - var_B ) / 6 ) + ( del_Max / 2 ) ) / del_Max
  //
  //    if      ( var_R == var_Max ) H = del_B - del_G
  //    else if ( var_G == var_Max ) H = ( 1 / 3 ) + del_R - del_B
  //    else if ( var_B == var_Max ) H = ( 2 / 3 ) + del_G - del_R
  //
  //     if ( H < 0 ) H += 1
  //     if ( H > 1 ) H -= 1
  }
  return 0;
}

unsigned int h2rgb(unsigned int v1, unsigned int v2, unsigned int hue)
{
	if (hue < 60) return v1 * 60 + (v2 - v1) * hue;
	if (hue < 180) return v2 * 60;
	if (hue < 240) return v1 * 60 + (v2 - v1) * (240 - hue);
	return v1 * 60;
}


uint16_t XY(uint8_t x, uint8_t y) {
  // TOP is 0
	// strips are serpentine layout
	if (x & 0x01) {
		// backwards
		return (x * kMatrixHeight) + (kMatrixHeight - 1 - y);
	} else {
		return (x * kMatrixHeight) + y;
	}
}

// uint16_t revXY(uint8_t x, uint8_t y) {
//   // BOTTOM is 0
//
// 	// strips are serpentine layout
// 	if (x & 0x01) {
// 		return (x * kMatrixHeight) + y;
// 	} else {
// 		// backwards
// 		return (x * kMatrixHeight) + (kMatrixHeight - 1 - y);
// 	}
// }

};
