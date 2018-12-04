
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 3

#define LED_BLACK    0

#define LED_RED_VERYLOW   (3 <<  11)
#define LED_RED_LOW     (7 <<  11)
#define LED_RED_MEDIUM    (15 << 11)
#define LED_RED_HIGH    (31 << 11)

#define LED_GREEN_VERYLOW (1 <<  5)
#define LED_GREEN_LOW     (15 << 5)
#define LED_GREEN_MEDIUM  (31 << 5)
#define LED_GREEN_HIGH    (63 << 5)

#define LED_BLUE_VERYLOW  3
#define LED_BLUE_LOW    7
#define LED_BLUE_MEDIUM   15
#define LED_BLUE_HIGH     31

#define LED_ORANGE_VERYLOW  (LED_RED_VERYLOW + LED_GREEN_VERYLOW)
#define LED_ORANGE_LOW    (LED_RED_LOW     + LED_GREEN_LOW)
#define LED_ORANGE_MEDIUM (LED_RED_MEDIUM  + LED_GREEN_MEDIUM)
#define LED_ORANGE_HIGH   (LED_RED_HIGH    + LED_GREEN_HIGH)

#define LED_PURPLE_VERYLOW  (LED_RED_VERYLOW + LED_BLUE_VERYLOW)
#define LED_PURPLE_LOW    (LED_RED_LOW     + LED_BLUE_LOW)
#define LED_PURPLE_MEDIUM (LED_RED_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_PURPLE_HIGH   (LED_RED_HIGH    + LED_BLUE_HIGH)

#define LED_CYAN_VERYLOW  (LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_CYAN_LOW    (LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_CYAN_MEDIUM   (LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_CYAN_HIGH   (LED_GREEN_HIGH    + LED_BLUE_HIGH)

#define LED_WHITE_VERYLOW (LED_RED_VERYLOW + LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_WHITE_LOW   (LED_RED_LOW     + LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_WHITE_MEDIUM  (LED_RED_MEDIUM  + LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_WHITE_HIGH    (LED_RED_HIGH    + LED_GREEN_HIGH    + LED_BLUE_HIGH)

#define BRIGHTNESS          60
#define FRAMES_PER_SECOND  120

#define mw 50
#define mh 14
#define DATA_PIN    3
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    700
//CRGB leds[NUM_LEDS];
int ledLookup[NUM_LEDS];
const int LED_COLORS[7] = {LED_WHITE_MEDIUM, LED_CYAN_MEDIUM, LED_PURPLE_MEDIUM, LED_ORANGE_MEDIUM, LED_BLUE_MEDIUM, LED_GREEN_MEDIUM, LED_RED_MEDIUM};
const uint8_t kMatrixWidth = 50;
const uint8_t kMatrixHeight = 14;
const bool    kMatrixSerpentineLayout = true;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(700, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoMatrix *matrix = new Adafruit_NeoMatrix(mw, mh, PIN,
    NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
    NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
    NEO_GRB            + NEO_KHZ800);


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
  matrix->begin();
  matrix->setTextWrap(false);
  strip.begin();
  matrix->show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
  createEcho( 0, 4, 8, 'y');
  // 1 is coming from the y axis and is closer to the bottom
  createEcho( 1, 13, 9, 'y');
  Serial.println("!!");

  // 2 is coming from x axis and is closest to the left
  createEcho( 2, 21, 7, 'x');
  // 3 is coming from the x axis and is in the middle
  createEcho( 3, 9, 10, 'x');

  // 4is coming from the x axis and is closest to the right
  createEcho( 4, 14, 10, 'x');

  // put your setup code here, to run once:
  int pixelHue = 200;
  // put your setup code here, to run once:
  ;



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
long previousMillis = 0;
bool stageTwo  = true;
bool stageOne  = true;
bool stageThree  = true;
bool stageFour  = true;

int NUM_ECHOES = 3;
void loop() {



  long timeDelta;
  unsigned long currentMillis = millis();
  //  Serial.println( currentMillis);




  int  sensorValue0 = analogRead(A0);
  int mappedPotValue0 = map(sensorValue0, 0, 1023, 3, 47);

  if (abs(mappedPotValue0 - previousValue0) > 3) {
    clearPixels(0);
    xAxisEchoOrigin[0] = mappedPotValue0;
    Serial.print("changin 0 x axis origin to ");
    Serial.println(mappedPotValue0);
    previousValue0 = mappedPotValue0;
        usbMIDI.sendControlChange(1, map(sensorValue0, 0, 1023, 0, 100), 11);




  }

  //  //  int mappedPotValue1 = 14;
  //  int  sensorValue1 = analogRead(A3);
  //  int mappedPotValue1 = map(sensorValue1, 0, 1023, 3, 16);
  //  int  sensorValue4 = analogRead(A1);
  //  int mappedPotValue4 = map(sensorValue4, 0, 1023, 11, 3);
  //  int mappedPotValue3 = 9;
  Serial.println("mapping - previous");
  Serial.println(mappedPotValue0);
  Serial.println(previousValue0);
  timeDelta = currentMillis - previousMillis;

  //Play first tone
  if (timeDelta > 857 && !echoInMovement[0] && stageOne) {
//    drawEchoAnimation(0);

    //    drawEchoAnimation(1);
    //    drawEchoAnimation(2);
    for(int index =0; index < NUM_ECHOES; index++) {
       drawEchoAnimation(index);
      
      }
    matrix->show();
    stageOne = false; 


  }
    //Play second tone
  if (timeDelta > 857 * 2  && stageTwo && !echoInMovement[0]) {
    for(int index =0; index < NUM_ECHOES; index++) {
       drawEchoAnimation(index);
      
      }
//    drawEchoAnimation(0);
    //    drawEchoAnimation(1);
    //    drawEchoAnimation(2);

    stageTwo = false;
    matrix->show();

  }

  
    //Play third tone
  if (timeDelta > 857 * 3 && !echoInMovement[0] && stageThree) {
//    drawEchoAnimation(0);
    //    drawEchoAnimation(1);
    //    drawEchoAnimation(2);
    for(int index =0; index < NUM_ECHOES; index++) {
       drawEchoAnimation(index);
      
      }
    matrix->show();
    stageThree = false;


  }

  
    //Play clear color and play last tone
  if (timeDelta > 857 * 4 && !echoInMovement[0] && stageFour) {

    for(int index =0; index < NUM_ECHOES; index++) {
       drawEchoAnimation(index);
      
      }
    matrix->show();
    stageFour = false;
    Serial.println("resetting pixels");
  }
  //  Serial.println("timeDelta before reset ");
  if (currentMillis - previousMillis > 857 * 4 && !echoInMovement[0]) {

    stageOne = true;
    stageTwo = true;
    stageThree = true;
    stageFour = true;
    matrix->show();

    previousMillis = currentMillis;

  }


  //  Serial.println("timeDelta at bottom of loop");
  //  Serial.println(timeDelta);



}


//void run(int echoIndex, long timeDelta) {
//  if (timeDelta < 200 && echoCounters[0] == 1 && !echoInMovement[echoIndex]) {
//    drawEchoAnimation(echoIndex, currentMillis);
//
//  }
//
//  if (timeDelta > 223 && timeDelta < 2000 && echoCounters[echoIndex] == 2 && !echoInMovement[echoIndex]) {
//    drawEchoAnimation(0, currentMillis);
//    drawEchoAnimation(3, currentMillis);
//    drawEchoAnimation(4, currentMillis);
//
//  }
//
//  if (timeDelta > 300 && !echoInMovement[0]) {
//    drawEchoAnimation(0, currentMillis);
//    previousMillis = currentMillis;
//
//    drawEchoAnimation(3, currentMillis);
//    drawEchoAnimation(4, currentMillis);
//  }
//
//
//
//
//
//}

void runDrawSequence(int echoIndex, int potentiometerValue, long currentMillis) {

  // Todo possibly refactor this to use origin value instead of previous value
  if ( abs(potentiometerValue - echoPreviousPotValueTracker[echoIndex]) > 3 &&  !echoInMovement[echoIndex]) {
    Serial.println("detected change in potentiometer moving square");
    Serial.print("absolute difference is : ");
    Serial.println(abs(potentiometerValue - echoPreviousPotValueTracker[echoIndex]));
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
      for (int index = 2; index < 5; index++) {
        if (echoCounters[index] == 10) {
          clearPixels(index);
        }
      }
    }
    echoPreviousPotValueTracker[echoIndex] = potentiometerValue;
  }

  // If we're in movement cycle clear the trail and draw the next phase
  if ( echoInMovement[echoIndex]) {

    long previousMillis = echoPreviousMillisTracker[echoIndex];
    //    Serial.print("prev mill ; ");
    //    Serial.println(previousMillis);
    //    Serial.print("currentMillis  ; ");
    //    Serial.println(currentMillis);
    //    Serial.println("time delta  ; ");
    //    Serial.println(currentMillis - previousMillis);
    if (currentMillis - previousMillis > 50) {
      clearPixels(echoIndex);
      drawEchoMovement(echoIndex, potentiometerValue, currentMillis);
      //    delay(1000);
      echoPreviousMillisTracker[echoIndex] = currentMillis;

    }

  } else if (!echoCollisionTriggerLookup[echoIndex]) {

    // before drawing square cycles check for collision
    if (echoDirectionLookup[echoIndex] == 'x') {

      for (int index = 0; index < 2; index++) {
        if (compareMinMax(index, echoIndex)) {
          echoCounters[index] = 10;
          echoCounters[echoIndex] = 10;
        }

      }
    }

    //    drawEchoAnimation(echoIndex, currentMillis);

  }
}



//int xAxisEchoMinimum[3];



void drawEchoMovement(int echoIndex, int mappedPotValue, long currentMillis) {

  Serial.print("Analog value is ; ");
  Serial.println(mappedPotValue);
  Serial.print("X ; ");
  Serial.println(xAxisEchoOrigin[echoIndex]);
  Serial.print("Y ; ");
  Serial.println(yAxisEchoOrigin[echoIndex]);
  //  fillSquare(echoIndex, xAxisEchoOrigin[echoIndex], yAxisEchoOrigin[echoIndex], 1);
  if (echoDirectionLookup[echoIndex] == 'y') {
    Serial.println("in here helllloooooo");

    xAxisEchoOrigin[echoIndex] += 1;
    if (xAxisEchoOrigin[echoIndex] >= mappedPotValue) {
      echoInMovement[echoIndex] = false;
      Serial.print("done with echo movement across board on x axis" );
      //        Serial.println(xAxisEchoOrigin[echoIndex]);
      //        delay(1000);
      clearPixels(echoIndex);

    }
  } else {
    yAxisEchoOrigin[echoIndex] -= 1;
    //      Serial.println(yAxisEchoOrigin[echoIndex]);
    if (yAxisEchoOrigin[echoIndex] <= mappedPotValue  ) {
      echoInMovement[echoIndex] = false;
      Serial.println("done with echo movement across board on y axis");


      clearPixels(echoIndex);


    }


  }
  Serial.println("Reseting this");
  //    clearPixels(echoIndex);


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
          // move this back to green
          strip.setPixelColor(XY(echo1xMax, echo1yMax - y), strip.Color(random(0, 150), 9, 9));
          strip.setPixelColor(XY(echo1xMax, echo1yMax - y), strip.Color(random(0, 150), 9, 9));

        } else {
          strip.setPixelColor(XY(echo1xMax - y, echo1yMax), strip.Color(random(0, 150), 9, 9));
          strip.setPixelColor(XY(echo2xMin + y, echo2yMin), strip.Color(random(0, 150), 9, 9));

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





void drawEchoAnimation(int echoLookupIndex) {
  int counter = echoCounters[echoLookupIndex];
  int xIndex = xAxisEchoOrigin[echoLookupIndex];
  int yIndex = yAxisEchoOrigin[echoLookupIndex];
  Serial.println(160 - counter * 35);
  if (counter <= 3 ) {
    matrix->drawCircle(xIndex, yIndex, counter, LED_COLORS[random(0, 6)]);
    echoCounters[echoLookupIndex]++;
    usbMIDI.sendNoteOn(61, 155 - counter * 35 , echoLookupIndex + 1);

  } else if (counter == 49) {
    randomizeEcho(echoLookupIndex);
  }


  if (counter > 3 ) {
    Serial.println("resetting");
    matrix->drawCircle(xIndex, yIndex, 1, LED_BLACK);
    matrix->drawCircle( xIndex, yIndex, 2, LED_BLACK);
    matrix->drawCircle( xIndex, yIndex, 3, LED_BLACK);
    echoCounters[echoLookupIndex] = 1;
    usbMIDI.sendNoteOn(61, 10 , echoLookupIndex + 1);
    matrix->show();

  }

  //      matrix->show();


}

bool pixelHasValue(int pixel) {

  return  strip.getPixelColor(pixel) != 0;

}
int offset = 0;


void randomizeEcho(int echoLookupIndex) {

  //  leds[pixelsInEcho[echoLookupIndex][random(0, 48)]] = CHSV( random(0, 255), 200, 255);
  //  FastLED.show();


}

//void fillSquare(int echoLookupIndex, int xCoordinate, int  yCoordinate,  int level) {
//  int pixelHuer = random(0, 190);
//  int pixelHueg = random(0, 150);
//  int pixelHueb = random(0, 190);
//  int arrayCounter = 0;
//  offsetLookup[echoLookupIndex] = level;
//
//  if (level == 1) {
//
//    offset = 3;
//
//  } else if (level == 2) {
//    offset = 5;
//
//  } else if (level == 3) {
//
//    offset = 7;
//  }
//
//  for (int x = 0; x < offset; x++) {
//    int xindex = xCoordinate - level + x;
//    for (int i = 0; i < offset; i++) {
//      int yindex = yCoordinate - level + i;
//      int ledLocation = XY(xindex, yindex);
//
//      // If nothing is drawn there
//
//      if (!pixelHasValue(ledLocation)) {
//        if (echoLookupIndex != 5 ) {
//          if (ledLocation < 270) {
//            strip.setPixelColor(ledLocation, strip.Color(50, pixelHueg, 50));
//            usbMIDI.sendNoteOn(61, 100, echoLookupIndex + 1);
//
//
//          }
//        }
//
//      } else {
//      }
//
//      pixelsInEcho[echoLookupIndex][arrayCounter] = ledLocation;
//      arrayCounter++;
//
//
//    }
//  }
//}
//



void createEcho(int index, int x, int y, char c) {

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
  int counter = echoCounters[echoLookupIndex];
  int xIndex = xAxisEchoOrigin[echoLookupIndex];
  int yIndex = yAxisEchoOrigin[echoLookupIndex];
  echoCounters[echoLookupIndex] = 1;
  matrix->drawCircle(xIndex, yIndex, 1, LED_BLACK);
  matrix->drawCircle( xIndex, yIndex, 2, LED_BLACK);
  matrix->drawCircle( xIndex, yIndex, 3, LED_BLACK);
usbMIDI.sendNoteOff(61, 0, echoLookupIndex + 1); // 60 = C4

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
//}






