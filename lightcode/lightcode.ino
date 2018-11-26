#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define BRIGHTNESS          60
#define FRAMES_PER_SECOND  120


#define DATA_PIN    3
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    280
CRGB leds[NUM_LEDS];
int ledLookup[NUM_LEDS];
const uint8_t kMatrixWidth = 20;
const uint8_t kMatrixHeight = 14;
const bool    kMatrixSerpentineLayout = true;

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  if ( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if ( kMatrixSerpentineLayout == true) {
    if ( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  return i;
}


// Param for different pixel layouts


void setup() {
  int pixelHue = 200;
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  //  drawFastHLine( 10, 11,3, 200);
  //  drawFastHLine( 3, 11 ,3, 100);
  //  drawFastHLine( 17, 11,3, 0);
//  drawFastHLine( 0, 7, 3, 20);
  //  drawFastHLine(10,10,10 , 256);
//    drawRect(0,0, 5, 5,20);
//    drawRect(5, 10, 2, 2,100);
//    drawRect(4, 9, 4, 4,0);
//    drawRect(3, 8, 6, 6,200);


          drawRect(5, 9, 2, 2,100);
//      
      drawRect(4, 8, 4, 4,0);
      
      drawRect(3, 7, 6, 6,200);
      
//

      

        drawRect(9, 10, 2, 2,100);
      
      drawRect(8, 9, 4, 4,0);
      
      drawRect(7, 8, 6, 6,200);
      delay(500);



      
  //
  //leds[ XY(0, 0)]  = CHSV( pixelHue, 255, 255);
  //leds[ XY(1, 0)]  = CHSV( pixelHue, 255, 255);
  //leds[ XY(0, 1)]  = CHSV( pixelHue, 255, 255);
  //leds[ XY(8, 0)]  = CHSV( pixelHue, 255, 255);
  //leds[ XY(2, 2)]  = CHSV( 10, 255, 255);
  //leds[ XY(5, 15)]  = CHSV( pixelHue, 255, 255);
  //leds[ XY(20, 5)]  = CHSV( pixelHue, 255, 255);

}
int xValue = 5;
int c2 = 10;
int yValue2 = 2;
void loop() {
  // put your main code here, to run repeatedly:
  int xValue = random(3, 17);
  if (yValue2 > 10) {
    yValue2 = 2;
  }
  FastLED.delay(1000 / FRAMES_PER_SECOND);
  int x2 = random(0, kMatrixWidth);
  int y2 = random(0, kMatrixHeight);
//  //
//    for (int x = 0; x < 5; x++) {
//      int offSet = 3* x;
////      drawFastVLine( 10, 14-offSet, offSet, 200);
////      drawFastVLine( 3, 14-offSet , offSet, 100);
////      drawFastVLine( 17, 14-offSet, offSet, 0);
////      drawFastHLine( 0, 4, offSet, 50);
////      drawFastHLine( 0, 9, offSet, 80);
//      drawRect(5, 10, 2, 2,100);
//      delay(500);
//      drawRect(4, 9, 4, 4,0);
//      delay(500);
//      drawRect(3, 8, 6, 6,200);
//      delay(500);
//      
//     
//      clearAll();
//  
//    }



  //  for(int x = 3; x < 17; x++) {
  //
  //        drawCircle(x, 3, 17, 17-x , 3, random(0, 255));
  ////          delay(100);
  ////        drawCircle(x, 3, 17, 17-x , 2, random(0, 255));
  ////  delay(100);
  ////  drawCircle(x, 4, x, 9 , 3, random(0, 255));
  //
  ////    delay(100);
  //    clearAll();
  //    }
  //
  //
  //    for(int x = 17; x > 3; x--) {
  //
  //        drawCircle(x, 3, 17, 17-x , 3, random(0, 255));
  ////          delay(100);
  //        drawCircle(x, 3, 17, 17-x , 2, random(0, 255));
  ////  delay(100);
  ////  drawCircle(x, 4, x, 9 , 3, random(0, 255));
  //
  //    delay(100);
  //    clearAll();
  //    }




  //  drawCircle(xValue,yValue2,2,random(0,255));

  //  drawCircle(c2,yValue2,1,random(0,255));

  //  drawCircle(xValue,5,2,random(0,255));
  ////  drawCircle(c2,yValue2,2,random(0,255));
  ////
  // delay(500);
  //  drawCircle(xValue,5,3,random(0,255));
  //////  drawCircle(c2,yValue2,3,random(0,255));
  //  delay(500);
  //  drawCircle(xValue,5,4,random(0,255));
  ////  delay(10);
  //////  drawCircle(c2,yValue2,4,random(0,255));
  //  delay(200);
  ////  clearAll();
  //  yValue2++;
  //  xValue +=3;
  //  drawCircle(5,5,5,random(0,255));
  //  delay(1000);

}

void clearAll() {
  for (int16_t i = 0; i < NUM_LEDS; i++) {
    leds[i]  = CHSV( 0, 0, 0);
  }

}

void startWrite() {}

void endWrite() {
  FastLED.show();
}

void writePixel(int x, int y, int pixelHue) {

  int  xy =  XY(x, y);
  if (xy < NUM_LEDS) {
    leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);


  }

}


void drawInitialCirclePoints(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  writePixel(x0  , y0 + r, color);
  writePixel(x0  , y0 - r, color);
  writePixel(x0 + r, y0  , color);
  writePixel(x0 - r, y0  , color);


}

void drawFullCirclePoints(int16_t x0, int16_t y0, int16_t x, int16_t y, uint16_t color) {

  writePixel(x0 + x, y0 + y, color);
  writePixel(x0 - x, y0 + y, color);
  writePixel(x0 + x, y0 - y, color);
  writePixel(x0 - x, y0 - y, color);
  writePixel(x0 + y, y0 + x, color);
  writePixel(x0 - y, y0 + x, color);
  writePixel(x0 + y, y0 - x, color);
  writePixel(x0 - y, y0 - x, color);


}






bool pixelHasValue(int pixel) {
//  return true;
//  Serial.println(leds[pixel].blue);
  return  leds[pixel].red != 0 ||  leds[pixel].blue != 0 ||  leds[pixel].green != 0; 
//  return true;
  
  }



void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
               int pixelHue, char direction) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    int ledLocation = (direction == 'v') ? XY(y0, x0) : XY(x0, y0);
    if(pixelHasValue(ledLocation)) {
        leds[ledLocation]  = CHSV( 120, 255, 255);
      
      } else {
        
        leds[ledLocation]  = CHSV( pixelHue, 255, 255);
        }
    

    
//    if (steep) {
//      Serial.println("Writing");
//      Serial.println(y0);
//      Serial.println(x0);
      //            writePixel(y0, x0, color);
//      if (direction == 'v') {
//        leds[ XY(y0, x0)]  = CHSV( pixelHue, 255, 255);
////        Serial.println(pixelHasValue(x0));
//        Serial.print("gr value");
////                  Serial.println(leds[ XY(y0, x0)][0]);
//Serial.println(pixelHasValue(x0));
//                  Serial.println(leds[ XY(y0, x0)].green);
//
//
//      } else {
//        leds[ XY(x0, y0)]  = CHSV( pixelHue, 255, 255);
//        
//        Serial.println(leds[ XY(y0, x0)].blue);
//                Serial.println(leds[ XY(x0, y0)].green);
//                Serial.println(pixelHasValue(x0));
////
////          Serial.println("horizontal");
//
//      }

    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
    FastLED.show();
//  }
}

void drawFastVLine(int16_t x, int16_t y,
                   int16_t h, uint16_t color) {
  startWrite();
  writeLine(x, y, x, y + h - 1, color,'v');
  endWrite();
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
              uint16_t color) {
  startWrite();
  for (int16_t i = x; i < x + w; i++) {
    drawFastVLine(i, y, h, color);
  }
  endWrite();

}



void drawRect(int16_t x, int16_t y, int16_t w, int16_t h,
        uint16_t color) {
    startWrite();
    Serial.println(x);
    Serial.println(y);
    drawFastHLine(x, y, w, color);
    drawFastHLine(x, y+h-1, w, color);

    if(w != 2 ) {
          drawFastVLine(x, y+1, h-2, color);
    drawFastVLine(x+w-1, y+1, h-2, color);
      }

    endWrite();
}




void drawFastHLine(int16_t x, int16_t y,
                   int16_t w, uint16_t color) {
  startWrite();
  writeLine(x, y, x + w - 1, y, color,'h');
  endWrite();
}
