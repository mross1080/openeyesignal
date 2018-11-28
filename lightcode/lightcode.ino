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



//  // Square 1 info
//    int square1Xstart = 3;
//    int square1Ystart = 7;
//    drawRect(5, 9, 2, 2, 100);
//    drawRect(9, 10, 2, 2, 100);
//    delay(500);
//    //
//
//    
//    drawRect(4, 8, 4, 4, 0);
//        drawRect(8, 9, 4, 4, 0);
//
//    delay(500);
//
//  
//    bool foundCollsion = false;
//    drawRect(3, 7, 6, 6, 200);
//    drawRect(7, 8, 6, 6, 200);
//  delay(1000);
//  clearAll();
//  Serial.println("looking for collisions");
//  //
//    for (int ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++) {
//      if (ledLookup[ledIndex]) {
//        foundCollsion = true;
//  
//  
//        leds[ledIndex]  = CRGB::Orange;
//  
//      }
//  
//  
//    }
//  
//    delay(1000);
//  
//    FastLED.show();
//  
//    if (foundCollsion) {
//      Serial.println("prointin coll");
//      //    fillRect(3, 7, 9, 7, 150);
//  
//    }
  //
  //
  //  for (int ledIndex = 3; ledIndex > 0; ledIndex--) {
  //     drawFastVLine(square1Xstart+ ledIndex, square1Ystart+1, 5, 220);
  //     drawFastVLine(square1Xstart+ ledIndex, square1Ystart+1, 5, 220);
  //     FastLED.show();
  //     delay(500);
  //
  //  }
  //  drawFastVLine( 4, 8, 5, 220);
  FastLED.show();
  //            drawRect(7, 8, 6, 6,200);
  //
  //} else {
  //
  //  Serial.println("No collisions");
  //}
//  delay(15000);


  //       drawFastHLine( 3, 8,9, 200);




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
int sensorValue;
int previousValue=0;
int mappedPotValue;
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
  sensorValue = analogRead(A0);  
  mappedPotValue = map(sensorValue,0,1023,3,18);
  if (mappedPotValue != previousValue ) {
    
    previousValue = mappedPotValue;
    
    
    
    
    
 
  Serial.println(mappedPotValue);

//  //Square echo animation
  for (int x = 0; x < mappedPotValue; x++) {
    int offSet = 3 * x;
    drawRect(x, 10, 2, 2, 100);
    delay(500);



    clearAll();

  }




//  //Square echo animation
  for (int x = 0; x < 2; x++) {
    int offSet = 3 * x;
    drawRect(mappedPotValue, 10, 2, 2, 100);
    delay(500);
    drawRect(mappedPotValue-1, 9, 4, 4, 0);
    delay(500);
    drawRect(mappedPotValue-2, 8, 6, 6, 200);
    delay(500);


    clearAll();

  }


  }
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
//    delay(1000);

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


bool pixelHasValue(int pixel) {
  //  return true;
  //  Serial.println(leds[pixel].blue);
  return  leds[pixel].red != 0 ||  leds[pixel].blue != 0 ||  leds[pixel].green != 0;
  //  return true;

}



bool writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
               int pixelHue, char direction) {
  bool triggeredCollision = false;
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  for (; x0 <= x1; x0++) {
    int ledLocation = (direction == 'v') ? XY(y0, x0) : XY(x0, y0);
    if (pixelHasValue(ledLocation)) {
      ledLookup[ledLocation] = true;
      //      leds[ledLocation]  = CHSV( 120, 255, 255);
      //        ledLookup[ledLocation] = true;
      triggeredCollision = true;

    }

    leds[ledLocation]  = CHSV( pixelHue, 255, 255);


  }

  return triggeredCollision;
  //  }
}

void drawFastVLine(int16_t x, int16_t y,
                   int16_t h, uint16_t color) {
  startWrite();
  writeLine(x, y, x, y + h - 1, color, 'v');
  //  endWrite();
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
              uint16_t color) {
  startWrite();
  for (int16_t i = x; i < x + w; i++) {
    drawFastVLine(i, y, h, color);
  }
  endWrite();

}



bool drawRect(int16_t x, int16_t y, int16_t w, int16_t h,
              uint16_t color) {
  startWrite();
  bool col = false;
  //    Serial.println(x);
  //    Serial.println(y);
  col = drawFastHLine(x, y, w, color);
  drawFastHLine(x, y + h - 1, w, color);


  if (w != 2 ) {
    drawFastVLine(x, y + 1, h - 2, color);
    drawFastVLine(x + w - 1, y + 1, h - 2, color);
  }




  endWrite();

  return col;
}




bool drawFastHLine(int16_t x, int16_t y,
                   int16_t w, uint16_t color) {
  startWrite();
  bool col = writeLine(x, y, x + w - 1, y, color, 'h');

  //  endWrite();
  return col;
}
