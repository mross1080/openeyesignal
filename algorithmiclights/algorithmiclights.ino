
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




void setup() {
  // 0 is coming from the y axis top
  createEcho( 0, 16, 4, 'y');
  // 1 is coming from the y axis and is closer to the bottom
  createEcho( 1, 3, 9, 'y');

  
  // 2 is coming from x axis and is closest to the left
  createEcho( 2, 4, 3, 'x');
  // 3 is coming from the x axis and is in the middle
  createEcho( 3, 9, 8, 'x');
 
      // 4is coming from the x axis and is closest to the right 
  createEcho( 4, 14, 17, 'x');

  //  createEcho( 2, 3, 10);
  //  Serial.println(echo->originX);
  // put your setup code here, to run once:
  int pixelHue = 200;
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);


}

//long previousMillis = 0;

void loop() {
  long timeDelta;
  unsigned long currentMillis = millis();
  //  Echo echoLookup[2];
  // put your main code here, to run repeatedly:
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);


  drawEchoAnimation(0,currentMillis);
  drawEchoAnimation(1,currentMillis);
  drawEchoAnimation(2,currentMillis);
//  drawEchoAnimation(3,currentMillis);

  //  delay(400);
  Serial.println("starting compare loop");

  compareMinMax(0, 1);
  compareMinMax(2, 0);

  compareMinMax(1,3);


  FastLED.show();
  //  echo->clearPixels();
  //  echo2->clearPixels();

}


int offsetLookup [5];
int echoCounters[5];
bool echoCollisionTriggerLookup[5];
int xAxisEchoOrigin[5];
int xAxisEchoMinimum[5];
int yAxisEchoOrigin[5];
int pixelsInEcho[5][49];
char echoDirectionLookup[5];
long echoPreviousMillisTracker[5];
//int xAxisEchoMinimum[3];







bool compareMinMax(int echo1Index, int echo2Index) {

  int echo1x = xAxisEchoOrigin[echo1Index];
  int echo1y = yAxisEchoOrigin[echo1Index];
  int echo2x = xAxisEchoOrigin[echo2Index];
  int echo2y = yAxisEchoOrigin[echo2Index];


  int offset1 = offsetLookup[echo1Index];
  int offset2 = offsetLookup[echo2Index];



  // Retrieve the origin of both echos


  if ( offset1 > 1 && offset2 > 1) {
    int offset1 = offsetLookup[echo1Index];
    int offset2 = offsetLookup[echo2Index];

    int echo1xMax = echo1x + offset1;
    int echo1yMax = echo1y + offset1;
    int echo1xMin = echo1x - offset1;
    int echo1Min = echo1y - offset1;
    int echo2xMin = echo2x - offset2;
    int echo2yMin = echo2y - offset2;

    //
    //
    //    Serial.print("X1 : ");
    //    Serial.println(echo1x);
    //    Serial.print("Y1 : ");
    //    Serial.println(echo1y);
    //    Serial.print("X2: ");
    //    Serial.println(echo2x);
    //    Serial.print("Y2 : ");
    //    Serial.println(echo2y);
    //    //
    //    Serial.print("E1 offset : ");
    //    Serial.println(offset1);
    //    Serial.print("E2 offset : ");
    //    Serial.println(offset2);
    //
    //    Serial.print("X1 max : ");
    //    Serial.println(echo1xMax);
    //    Serial.print("Y1 max : ");
    //    Serial.println(echo1yMax);
    //    Serial.print("X2 min : ");
    //    Serial.println(echo2xMin);
    //    Serial.print("Y2 min : ");
    //    Serial.println(echo2yMin);

    if (echo2xMin <= echo1xMax && echo2yMin <= echo1yMax) {
         for (int index = 0; index < 48; index++) {
        leds[pixelsInEcho[echo1Index][index]]  = CRGB::Gray ;

        leds[pixelsInEcho[echo2Index][index]]  = CRGB::Gray ;
      }
      //            clearPixels(0);
      //            clearPixels(1);
      //.fadeToBlackBy( 64 )
      for (int y = 0; y < 4; y++) {

        if (echo1xMin < echo2xMin && echo1xMin < echo2xMin) {

          leds[XY(echo1xMax, echo1yMax - y)]  = CHSV( 80, 220, 220);
          leds[XY(echo2xMin, echo2yMin + y)]  = CHSV( 80, 220, 220);

        } else {

          leds[XY(echo1xMax - y, echo1yMax)]  = CHSV( 80, 220, 220);
          leds[XY(echo2xMin + y, echo2yMin)]  = CHSV( 80, 220, 220);

        }

      }
      //
      //      drawCircle(echo2xMin, echo2yMin, echo1xMax, echo1yMax, 2,
      //                200);
      //        createEcho( 0,echo1xMax, echo1yMax );
      //        createEcho( 1, echo2xMin - 1, echo2yMin);
      //      echoCounters[0] = 4;
      //      echoCounters[1] = 4;
      return true;
    }
  }

  return false;

}





void drawEchoAnimation(int echoLookupIndex, long currentMillis) {
  int counter = echoCounters[echoLookupIndex];
  long previousMillis = echoPreviousMillisTracker[echoLookupIndex];
if (currentMillis - previousMillis > 600) {
  if (counter <= 2 ) {
    fillSquare(echoLookupIndex, xAxisEchoOrigin[echoLookupIndex], yAxisEchoOrigin[echoLookupIndex], counter);
    echoCounters[echoLookupIndex]++;

  } else if (counter == 49) {
    randomizeEcho(echoLookupIndex);
  } else {
      clearPixels(echoLookupIndex);

  }
  echoPreviousMillisTracker[echoLookupIndex] = currentMillis;
}

}

bool pixelHasValue(int pixel) {

  return  leds[pixel].red != 0 ||  leds[pixel].blue != 0 ||  leds[pixel].green != 0;

}
int offset = 0;


void randomizeEcho(int echoLookupIndex) {

  leds[pixelsInEcho[echoLookupIndex][random(0, 48)]] = CHSV( random(0, 255), 200, 255);
  FastLED.show();


}

void fillSquare(int echoLookupIndex, int xCoordinate, int  yCoordinate,  int level) {
  int pixelHue = random(0, 255);
  int arrayCounter = 0;
  offsetLookup[echoLookupIndex] = level;
  if (level == 1) {

    offset = 3;

  } else if (level == 2) {
    offset = 5;

  } else if (level == 3) {

    offset = 7;
  }
  Serial.print("offset : ");
  Serial.println(offset);

  for (int x = 0; x < offset; x++) {
    int xindex = xCoordinate - level + x;
    for (int i = 0; i < offset; i++) {
      int yindex = yCoordinate - level + i;
      int ledLocation = XY(xindex, yindex);


      if (!pixelHasValue(ledLocation)) {

        leds[ledLocation]  = CHSV(pixelHue, 200, 200);


      } else {
        Serial.println("Skipping");
      }
      pixelsInEcho[echoLookupIndex][arrayCounter] = ledLocation;
      Serial.println(arrayCounter);
      arrayCounter++;

    }
  }

}




void createEcho(int index, int x, int y, char c) {
  xAxisEchoOrigin[index] = x;
  yAxisEchoOrigin[index] = y;
  echoCounters[index] = 1;
  offsetLookup[index] = 0;
  echoDirectionLookup[index] = c;
  echoPreviousMillisTracker[index] = 0;

}


void clearPixels(int echoLookupIndex) {
  //  echoCounters[index] = 1;
  //
  echoCounters[echoLookupIndex] = 1;
  for (int x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {
      leds[XY(x, y)]  = CHSV( 0, 0, 0);
    }
    //  sizeof(array)/sizeof(array[0]
    //  for (int x = 0; x < 48; x++) {
    //    leds[pixelsInEcho[echoLookupIndex][x]]  = CHSV( 0, 0, 0);


  }
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




void drawCircle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r,
                uint16_t color) {
  Serial.println("Entering draw circle");
  Serial.println(x0);
  Serial.println(y0);

  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;



  int circle2X = x1;
  int circle2Y = y1;

  drawInitialCirclePoints(x0,  y0, r, random(0, 255));
  drawInitialCirclePoints(circle2X, circle2Y , r, random(0, 255));
  //    drawInitialCirclePoints(x0+4,  y0+4,r, color);

  while (x < y) {
    Serial.println ("stuck in this loop");
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    drawFullCirclePoints(x0,  y0,  x,  y,  random(0, 255));
    drawFullCirclePoints(circle2X, circle2Y  , x, y , random(0, 255));
    //        drawFullCirclePoints(x0+4,  y0+4,  x+4,  y+4,  random(0,255));
    //
    //        writePixel(x0 + x, y0 + y, color);
    //        writePixel(x0 - x, y0 + y, color);
    //        writePixel(x0 + x, y0 - y, color);
    //        writePixel(x0 - x, y0 - y, color);
    //        writePixel(x0 + y, y0 + x, color);
    //        writePixel(x0 - y, y0 + x, color);
    //        writePixel(x0 + y, y0 - x, color);
    //        writePixel(x0 - y, y0 - x, color);
  }
  Serial.println("ending write");
  FastLED.show();
  Serial.println("leaving draw circle");

}


