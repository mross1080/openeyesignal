
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
  Serial.begin(9600);
  delay(3000);
  createEcho( 0, 4, 4, 'y');
  // 1 is coming from the y axis and is closer to the bottom
  createEcho( 1, 3, 9, 'y');
  Serial.println("!!");

  // 2 is coming from x axis and is closest to the left
  createEcho( 2, 4, 3, 'x');
  // 3 is coming from the x axis and is in the middle
  createEcho( 3, 9, 10, 'x');

  // 4is coming from the x axis and is closest to the right
  createEcho( 4, 14, 10, 'x');

  //  createEcho( 2, 3, 10);
  //  Serial.println(echo->originX);
  // put your setup code here, to run once:
  int pixelHue = 200;
  // put your setup code here, to run once:
  ;
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  Serial.println("**");
}

//long previousMillis = 0;

int previousValue0 = 0;
int previousValue1 = 0;
int previousValue4 = 0;
int mappedPotValue1;
int mappedPotValue3;


int offsetLookup [5];
int echoCounters[5];
int counters[5];
bool echoCollisionTriggerLookup[5];
int xAxisEchoOrigin[5];
int xAxisEchoMinimum[5];
int yAxisEchoOrigin[5];
int pixelsInEcho[5][49];
char echoDirectionLookup[5];
long echoPreviousMillisTracker[5];
int echoPreviousPotValueTracker[5];
bool echoInMovement[5];
long previousMillis11 = 0;

void loop() {
  Serial.println("starting loop again");

  //  delay(1000);
  leds[2]  = CHSV( 80, 220, 220);
  long timeDelta;
  Serial.println("asdf");
  unsigned long currentMillis = millis();

  //  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);


  int  sensorValue0 = analogRead(A0);
  int mappedPotValue0 = map(sensorValue0, 0, 1023, 3, 16);
  //  int mappedPotValue1 = 14;
  Serial.println("aaaaaa");
  int  sensorValue1 = analogRead(A3);
  int mappedPotValue1 = map(sensorValue1, 0, 1023, 3, 16);
  Serial.println("ggggg");
  int  sensorValue4 = analogRead(A1);
  int mappedPotValue4 = map(sensorValue4, 0, 1023, 11, 3);
  //  int mappedPotValue3 = 9;
  //  Serial.println("mapping - previous");
  //  Serial.println(mappedPotValue4);
  //  Serial.println(previousValue4);
  if (currentMillis - previousMillis11 > 0) {

    previousMillis11 = currentMillis;

    Serial.println("checking 0 pot value");
    if ( abs(mappedPotValue0 - previousValue0) > 5 &&  !echoInMovement[0]) {
      echoInMovement[0] = true;
      xAxisEchoOrigin[0] = 0;
      previousValue0 = mappedPotValue0;
      if (echoCounters[4] == 10) {
        clearPixels(4);

      }

    }

    if ( echoInMovement[0]) {
      clearPixels(0);

      drawEchoMovement(0, mappedPotValue0, currentMillis);

    } else if (!echoCollisionTriggerLookup[0]) {

      drawEchoAnimation(0, currentMillis);

    }

    if ( abs(mappedPotValue1 - previousValue1) > 3 &&  !echoInMovement[1]) {
      echoInMovement[1] = true;
      xAxisEchoOrigin[1] = 0;
      previousValue1 = mappedPotValue1;
      if (echoCounters[4] == 10) {
        clearPixels(4);

      }
      if (echoCounters[0] == 10) {
        clearPixels(0);

      }

    }

    if ( echoInMovement[1]) {
      clearPixels(1);

      drawEchoMovement(1, mappedPotValue1, currentMillis);

    } else if (!echoCollisionTriggerLookup[1]) {
      if (echoCounters[0] == 10) {
        clearPixels(0);

      }
      drawEchoAnimation(1, currentMillis);

    }
    Serial.println("Checking for pot values");

    runDrawSequence(4, mappedPotValue4, currentMillis);

    
//    if ( abs(mappedPotValue4 - previousValue4) > 3 &&  !echoInMovement[4]) {
//      echoInMovement[4] = true;
//      yAxisEchoOrigin[4] = 17;
//      previousValue4 = mappedPotValue4;
//      if (echoCounters[1] == 10) {
//        clearPixels(4);
//
//      }
//      if (echoCounters[0] == 10) {
//        clearPixels(0);
//
//      }
//      //
//    }
//
//    if ( echoInMovement[4]) {
//      clearPixels(4);
//      drawEchoMovement(4, mappedPotValue4, currentMillis);
//
//    } else if (!echoCollisionTriggerLookup[4]) {
//      if (compareMinMax(1, 4)) {
//        echoCounters[1] = 10;
//        echoCounters[4] = 10;
//
//      }
//
//      if (compareMinMax(0 , 4)) {
//        echoCounters[0] = 10;
//        echoCounters[4] = 10;
//
//      }
//      drawEchoAnimation(4, currentMillis);
//
//    }
  }
  leds[8] = CHSV( random(0, 100), 220, 220);
  FastLED.show();


}


void runDrawSequence(int echoIndex, int potentiometerValue, long currentMillis) {
  if ( abs(potentiometerValue - echoPreviousPotValueTracker[echoIndex]) > 3 &&  !echoInMovement[echoIndex]) {
    echoInMovement[echoIndex] = true;
    if (echoDirectionLookup[echoIndex] == 'x') {
      // start y axis from bottom of the board
      yAxisEchoOrigin[echoIndex] = 17;

      // Reset all counters that are frozen in a collision
      for (int index = 0; index < 2; index++) {
        if (echoCounters[index] == 10) {
          clearPixels(index);
        }
      }

    } else {
      // set counters on the left side back to 0
      xAxisEchoOrigin[echoIndex] = 0;
    }
    echoPreviousPotValueTracker[echoIndex] = potentiometerValue;
  }

  // If we're in movement cycle clear the trail and draw the next phas
  if ( echoInMovement[echoIndex]) {
    clearPixels(echoIndex);
    drawEchoMovement(echoIndex, potentiometerValue, currentMillis);

  } else if (!echoCollisionTriggerLookup[echoIndex]) {

    // before drawing square cycles check for collision
    for (int index = 0; index < 2; index++) {
      if (compareMinMax(index, echoIndex)) {
        echoCounters[index] = 10;
        echoCounters[echoIndex] = 10;
      }

    }

    drawEchoAnimation(echoIndex, currentMillis);

  }
}



//int xAxisEchoMinimum[3];



void drawEchoMovement(int echoIndex, int mappedPotValue, long currentMillis) {
  long previousMillis = echoPreviousMillisTracker[echoIndex];
  //  Serial.print("prev mill ; ");
  //  Serial.println(previousMillis);
  //  Serial.print("currentMillis  ; ");
  //  Serial.println(currentMillis);
  if (currentMillis - previousMillis > 300) {
    //    Serial.print("Analog value is ; ");
    //    Serial.println(mappedPotValue);
    //    Serial.print("X ; ");
    //    Serial.println(xAxisEchoOrigin[echoIndex]);
    //    Serial.print("Y ; ");
    //    Serial.println(yAxisEchoOrigin[echoIndex]);
    fillSquare(echoIndex, xAxisEchoOrigin[echoIndex], yAxisEchoOrigin[echoIndex], 1);
    if (echoDirectionLookup[echoIndex] == 'y') {
      Serial.println("in here helllloooooo");

      xAxisEchoOrigin[echoIndex] += 1;
      if (xAxisEchoOrigin[echoIndex] >= mappedPotValue) {
        echoInMovement[echoIndex] = false;
        Serial.print("going into clear with x value :" );
        Serial.println(xAxisEchoOrigin[echoIndex]);
        clearPixels(echoIndex);

      }
    } else {
      yAxisEchoOrigin[echoIndex] -= 1;
      Serial.println(yAxisEchoOrigin[echoIndex]);
      if (yAxisEchoOrigin[echoIndex] <= mappedPotValue  ) {
        echoInMovement[echoIndex] = false;
        clearPixels(echoIndex);


      }


    }
    Serial.println("Reseting this");
    echoPreviousMillisTracker[echoIndex] = currentMillis;
    //    clearPixels(echoIndex);

  }
}







bool compareMinMax(int echo1Index, int echo2Index) {


  int echo1x = xAxisEchoOrigin[echo1Index];
  int echo1y = yAxisEchoOrigin[echo1Index];
  int echo2x = xAxisEchoOrigin[echo2Index];
  int echo2y = yAxisEchoOrigin[echo2Index];


  int offset1 = offsetLookup[echo1Index];
  int offset2 = offsetLookup[echo2Index];



  // Retrieve the origin of both echos

  // Only check this if the squares echo is big enough
  if ( offset1 > 1 && offset2 > 1) {
    int offset1 = offsetLookup[echo1Index];
    int offset2 = offsetLookup[echo2Index];
    //    Serial.println("!-----");
    //    Serial.println(offset1);
    //    Serial.println(echo1x);
    //    Serial.println(echo1y);
    //    Serial.println("-----!");

    int echo1xMax = echo1x + offset1;
    int echo1yMax = echo1y + offset1;
    int echo1xMin = echo1x - offset1;
    int echo1Min = echo1y - offset1;
    int echo2xMin = echo2x - offset2;
    int echo2yMin = echo2y - offset2;
    //
    //    Serial.println("-----");
    //    Serial.println(echo2xMin);
    //    Serial.println(echo2yMin);
    //    Serial.println("-");
    //    Serial.println(echo1xMax);
    //    Serial.println(echo1yMax);
    //    Serial.println("-------");
    /// Fix collisions for 0 and 4
    if (echo2xMin <= echo1xMax && echo2yMin <= echo1yMax && !echoInMovement[echo2Index]) {

      for (int y = 0; y < 4; y++) {
        //
        if (echo1xMin < echo2xMin && echo1xMin < echo2xMin) {

          leds[XY(echo1xMax, echo1yMax - y)]  = CHSV( 80, 220, 220);
          leds[XY(echo2xMin, echo2yMin + y)]  = CHSV( 80, 220, 220);

        } else {
          leds[XY(echo1xMax - y, echo1yMax)]  = CHSV( 80, 220, 220);
          leds[XY(echo2xMin + y, echo2yMin)]  = CHSV( 80, 220, 220);

        }
        //
      }
      //      //
      //      //      drawCircle(echo2xMin, echo2yMin, echo1xMax, echo1yMax, 2,
      //      //                200);
      //      //        createEcho( 0,echo1xMax, echo1yMax );
      //      //        createEcho( 1, echo2xMin - 1, echo2yMin);
      //      //      echoCounters[0] = 4;
      //      //      echoCounters[1] = 4;
      return true;
    }
  }

  return false;

}





void drawEchoAnimation(int echoLookupIndex, long currentMillis) {
  int counter = echoCounters[echoLookupIndex];
  long previousMillis = echoPreviousMillisTracker[echoLookupIndex];

  Serial.print("prev mill ; ");
  Serial.println(previousMillis);
  Serial.print("currentMillis  ; ");
  Serial.println(currentMillis);
  if (currentMillis - previousMillis > 600) {
    if (counter <= 2 ) {
      fillSquare(echoLookupIndex, xAxisEchoOrigin[echoLookupIndex], yAxisEchoOrigin[echoLookupIndex], counter);
      FastLED.show();
      echoCounters[echoLookupIndex]++;

    } else if (counter == 49) {
      randomizeEcho(echoLookupIndex);
    }  else if (counter == 10 ) {
      for (int index = 0; index < 48; index++) {
        leds[pixelsInEcho[echoLookupIndex][index]]  = CRGB::Gray ;
      }

    }

  }

  if (currentMillis - previousMillis > 2000) {

    if (counter > 2 && counter != 10 ) {
      Serial.println("clearing");
      clearPixels(echoLookupIndex);
      echoPreviousMillisTracker[echoLookupIndex] = currentMillis;
    }
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
  Serial.println("Filling square");
  offsetLookup[echoLookupIndex] = level;

  if (level == 1) {

    offset = 3;

  } else if (level == 2) {
    offset = 5;

  } else if (level == 3) {

    offset = 7;
  }
  Serial.println("aaa");

  for (int x = 0; x < offset; x++) {
    int xindex = xCoordinate - level + x;
    for (int i = 0; i < offset; i++) {
      int yindex = yCoordinate - level + i;
      int ledLocation = XY(xindex, yindex);

      // If nothing is drawn there
      Serial.println("checking for pixel color");

      if (!pixelHasValue(ledLocation)) {
        if (echoLookupIndex != 5 ) {
          if (ledLocation < 270) {
            leds[ledLocation]  = CHSV(pixelHue, 200, 200);
          }
        }

      } else {
      }

      pixelsInEcho[echoLookupIndex][arrayCounter] = ledLocation;
      arrayCounter++;


    }
  }
}




void createEcho(int index, int x, int y, char c) {
  Serial.println("Creating echo with origins");
  Serial.print("X ; ");
  Serial.println(x);
  Serial.print("Y ; ");
  Serial.println(y);
  Serial.println("done echo with origins");

  xAxisEchoOrigin[index] = x;
  yAxisEchoOrigin[index] = y;
  echoCounters[index] = 1;
  counters[index] = 0;
  offsetLookup[index] = 0;
  echoDirectionLookup[index] = c;
  echoPreviousMillisTracker[index] = 0;
  echoInMovement[index] = false;
  echoPreviousPotValueTracker[index] = 0;

}


void clearPixels(int echoLookupIndex) {
  //  echoCounters[index] = 1;
  //
  echoCounters[echoLookupIndex] = 1;
  for (int index = 0; index < 48; index++) {
    //    Serial.println(pixelsInEcho[echoLookupIndex][index]);
    leds[pixelsInEcho[echoLookupIndex][index]]  = CHSV( 0, 0, 0);

  }

  //  for (int x = 0; x < kMatrixWidth; x++) {
  //    for (int y = 0; y < kMatrixHeight; y++) {
  //      leds[XY(x, y)]  = CHSV( 0, 0, 0);
  //    }
  //    //  sizeof(array)/sizeof(array[0]
  //    //  for (int x = 0; x < 48; x++) {
  //    //    leds[pixelsInEcho[echoLookupIndex][x]]  = CHSV( 0, 0, 0);
  //
  //
  //  }
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


