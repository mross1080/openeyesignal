
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


void setup() {
  // 0 is coming from the y axis top
  matrix->begin();
  matrix->setTextWrap(false);
  strip.begin();
  matrix->show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
  createEcho( 0, 10, 2, 'y');
  // 1 is coming from the y axis and is closer to the bottom
  createEcho( 1, 10, 9, 'y');
  Serial.println("!!");

  // 2 is coming from x axis and is closest to the left
  createEcho( 2, 20, 11, 'y');
  // 3 is coming from the x axis and is in the middle
  createEcho( 3, 30, 11, 'x');

  // 4is coming from the x axis and is closest to the right
  createEcho( 4, 40, 10, 'x');

  // put your setup code here, to run once:
  int pixelHue = 200;
}

//long previousMillis = 0;

int previousValue0 = 0;
int previousValue1 = 0;
int previousValue3 = 0;
int mappedPotValue1;
int mappedPotValue3;


int collisionLookupMap[5];
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
int absorbedEchoLookup[5];
long previousMillis11 = 0;
long previousMillis = 0;
bool stageTwo  = true;
bool stageOne  = true;
bool stageThree  = true;
bool stageFour  = true;
long previousMovementTime = 0;

int NUM_ECHOES = 5;
void loop() {



  long timeDelta;
  unsigned long currentMillis = millis();
  long movementTimeDelta;

  //    Serial.println( currentMillis);

  int  sensorValue0 =  analogRead(A0);
  int mappedPotValue0 = map(sensorValue0, 0, 1023, 3, 47);
  //
  //
  int  sensorValue1 =  analogRead(A3);
  int mappedPotValue1 = map(sensorValue1, 0, 1023, 3, 47);
  int  sensorValue3 = analogRead(A1);
  int mappedPotValue3 = map(sensorValue3, 0, 1023, 11, 1);
//  Serial.println(sensorValue0);

  if (abs(mappedPotValue0 - previousValue0) > 2) {
    clearPixels(0);
    echoCounters[0] = 500;

    previousValue0 = mappedPotValue0;
    usbMIDI.sendControlChange(1, map(sensorValue0, 0, 1023, 0, 100), 11);


  }


  if (abs(mappedPotValue1 - previousValue1) > 2) {
    Serial.println("changed in pot value");
    clearPixels(1);
    echoCounters[1] = 500;
    previousValue1 = mappedPotValue1;
    //    usbMIDI.sendControlChange(3, map(sensorValue0, 0, 1023, 0, 100), 12);


  }
  movementTimeDelta = currentMillis - previousMovementTime;
  //
  if (movementTimeDelta > 250) {
Serial.print("x axis value 1 is ; ");
  Serial.println(xAxisEchoOrigin[1]);
  Serial.print("previous ; ");
  Serial.println(previousValue1);
    if (abs(xAxisEchoOrigin[0] - previousValue0) > 2) {
      matrix->fillCircle( xAxisEchoOrigin[0], yAxisEchoOrigin[0], 1, LED_BLACK);
      drawEchoMovement(0, mappedPotValue0, currentMillis);

    }

    if (abs(xAxisEchoOrigin[1] - previousValue1) > 2) {
      matrix->fillCircle( xAxisEchoOrigin[1], yAxisEchoOrigin[1], 1, LED_BLACK);
      drawEchoMovement(1, mappedPotValue1, currentMillis);

    }

    previousMovementTime = currentMillis;

  }
  //

  ////
  ////
  ////
  if (abs(mappedPotValue3 - previousValue3) > 2) {
    clearPixels(3);
    yAxisEchoOrigin[3] = mappedPotValue3;

    previousValue3 = mappedPotValue3;
    //    usbMIDI.sendControlChange(2, map(sensorValue0, 0, 1023, 0, 100), 11);


  }
  //
  //
  //
  //  //  //  int mappedPotValue1 = 14;
  //  //  int  sensorValue1 = analogRead(A3);
  //  //  int mappedPotValue1 = map(sensorValue1, 0, 1023, 3, 16);
  //  //  int  sensorValue4 = analogRead(A1);
  //  //  int mappedPotValue4 = map(sensorValue4, 0, 1023, 11, 3);
  //  //  int mappedPotValue3 = 9;
  //  //  Serial.println("mapping - sensorValue1");
  //  //  Serial.println(mappedPotValue1);
  //  //  Serial.println(sensorValue1);
  //  //Serial.println(currentMillis);
  timeDelta = currentMillis - previousMillis;
  //
  //Play first tone
  if (timeDelta > 857 && !echoInMovement[0] && stageOne) {
    for (int index = 0; index < NUM_ECHOES; index++) {
      drawEchoAnimation(index);

    }
    matrix->show();
    stageOne = false;

  }
  //Play second tone
  if (timeDelta > 857 * 2  && stageTwo && !echoInMovement[0]) {
    for (int index = 0; index < NUM_ECHOES; index++) {
      drawEchoAnimation(index);

    }


    stageTwo = false;
    matrix->show();

  }

  //
  //Play third tone
  if (timeDelta > 857 * 3 && !echoInMovement[0] && stageThree) {
    for (int index = 0; index < NUM_ECHOES; index++) {
      drawEchoAnimation(index);

    }
    matrix->show();
    stageThree = false;


  }

  //
  //Play clear color and play last tone
  if (timeDelta > 857 * 4 && !echoInMovement[0] && stageFour) {

    for (int index = 0; index < NUM_ECHOES; index++) {
      drawEchoAnimation(index);

    }
    matrix->show();
    stageFour = false;
  }
  //  Serial.println("timeDelta before reset ");
  if (currentMillis - previousMillis > 857 * 4 && !echoInMovement[0]) {

    stageOne = true;
    stageTwo = true;
    stageThree = true;
    stageFour = true;
    matrix->show();
    //    checkForCollisions(0);
    //    checkForCollisions(3);
    previousMillis = currentMillis;

  }

}

void checkForCollisions(int echoLookupIndex) {
  int checkStartIndex;
  int checkEndIndex;
  int xIndex = xAxisEchoOrigin[echoLookupIndex];
  int yIndex = yAxisEchoOrigin[echoLookupIndex];
  int currentCounter = echoCounters[echoLookupIndex];
  //  Serial.print("comparing with main index");
  //  Serial.println(echoLookupIndex);
  //  Serial.print("x : ");
  //  Serial.println(xIndex);
  //  Serial.print("y : ");
  //  Serial.println(xIndex);


  if (echoDirectionLookup[echoLookupIndex] == 'x') {
    checkStartIndex = 0;
    checkEndIndex = 3;

  } else {
    checkStartIndex = 3;
    checkEndIndex = 6;

  }
  for (int index = checkStartIndex; index < checkEndIndex; index++) {

    if ( echoCounters[echoLookupIndex] == 100) {
      Serial.print("comparing with alt index");
      Serial.println(index);
      Serial.print("compare x : ");
      Serial.println(xAxisEchoOrigin[index]);
      Serial.print("compare y : ");
      Serial.println(yAxisEchoOrigin[index]);

      Serial.print("Distance : ");
      Serial.println(sqrt(pow(xAxisEchoOrigin[index] - xIndex, 2) + pow(yAxisEchoOrigin[index] - yIndex, 2)));
    }
    // find distance between origin of echoes

    double originDistance = sqrt(pow(xAxisEchoOrigin[index] - xIndex, 2) + pow(yAxisEchoOrigin[index] - yIndex, 2));
    int midpointX = (xAxisEchoOrigin[index] + xIndex) / 2;
    int midpointY = (yAxisEchoOrigin[index] + yIndex) / 2;
    if (originDistance < 4 && echoCounters[echoLookupIndex] != 100) {

      clearPixels(index);
      xAxisEchoOrigin[echoLookupIndex] = midpointX;
      yAxisEchoOrigin[echoLookupIndex] = midpointY;
      xAxisEchoOrigin[index] = midpointX;
      yAxisEchoOrigin[index] = midpointY;
      Serial.print("midpoint x ");
      Serial.println(midpointX);
      Serial.print("midpoint y ");
      Serial.println(midpointY);

      echoCounters[echoLookupIndex] = 100;
      echoCounters[index] = 100;
      collisionLookupMap[index] = echoLookupIndex;
      collisionLookupMap[echoLookupIndex] = index;
      matrix->fillCircle(midpointX, midpointY, 3, LED_COLORS[random(0, 6)]);

    } else if (originDistance < 6 && echoCounters[index] == 100) {
      xAxisEchoOrigin[echoLookupIndex] = midpointX;
      yAxisEchoOrigin[echoLookupIndex] = midpointY;
      xAxisEchoOrigin[index] = midpointX;
      yAxisEchoOrigin[index] = midpointY;
      xAxisEchoOrigin[collisionLookupMap[index]] = midpointX;
      yAxisEchoOrigin[collisionLookupMap[index]] = midpointY;
      echoCounters[echoLookupIndex] = 200;
      echoCounters[index] = 200;
      echoCounters[collisionLookupMap[index]] = 200;





    }

  }

}


// If counter == 1 that means were just started the echo
// Counter 2 level 2
// Counter 3 level 3
// Counter 100 means a collision of two echos
// Counter 200 means a collision of three
// counter of 500 means in movement

void drawEchoAnimation(int echoLookupIndex) {
  
  int counter = echoCounters[echoLookupIndex];
  int xIndex = xAxisEchoOrigin[echoLookupIndex];
  int yIndex = yAxisEchoOrigin[echoLookupIndex];
  if (!(xIndex == 0 || yIndex == 17)) {
    if (counter == 1) {
      matrix->drawPixel(xIndex, yIndex, LED_COLORS[random(0, 6)]);
      echoCounters[echoLookupIndex]++;
      usbMIDI.sendNoteOn(61, 155 - counter * 35 , echoLookupIndex + 1);
    } else if (counter <= 3 && counter > 1 ) {
      matrix->drawCircle(xIndex, yIndex, counter - 1, LED_COLORS[random(0, 6)]);
      echoCounters[echoLookupIndex]++;
      usbMIDI.sendNoteOn(61, 155 - counter * 35 , echoLookupIndex + 1);

    } else if (counter == 100) {
      //      matrix->fillCircle(xIndex, yIndex, 3, LED_BLACK);
      //      xAxisEchoOrigin[echoLookupIndex] += random(-1,1);
      //      yAxisEchoOrigin[echoLookupIndex] += random(-1,1);

      matrix->fillCircle(xIndex, yIndex, 3, LED_COLORS[random(0, 6)]);

      usbMIDI.sendNoteOn(61, 100 , 11);

    } else if (counter == 500) {
      //      matrix->fillCircle(xIndex, yIndex, 3, LED_BLACK);
      //      xAxisEchoOrigin[echoLookupIndex] += random(-1,1);
      //      yAxisEchoOrigin[echoLookupIndex] += random(-1,1);
      Serial.println("moving x axis");
      matrix->fillCircle(xIndex, yIndex, 1, LED_COLORS[random(0, 6)]);

      usbMIDI.sendNoteOn(61, 100 , 13);

    }

    if (counter > 3 && counter < 10) {
      //    Serial.println("resetting");
      matrix->drawCircle(xIndex, yIndex, 1, LED_BLACK);
      matrix->drawCircle( xIndex, yIndex, 2, LED_BLACK);
      matrix->drawPixel(xIndex, yIndex, LED_BLACK);
      echoCounters[echoLookupIndex] = 1;
      usbMIDI.sendNoteOn(61, 10 , echoLookupIndex + 1);
      matrix->show();

    }
  }
}

bool pixelHasValue(int pixel) {

  return  strip.getPixelColor(pixel) != 0;

}
int offset = 0;


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
  //
  //if (echoLookupIndex ==1) {
  //  Serial.println("1");
  //  Serial.println(counter);
  //  Serial.println(xIndex);
  //  Serial.println(yIndex);
  ////  delay(3000);
  //
  //}
  //
  if (echoCounters[echoLookupIndex] == 100) {

    matrix->fillCircle(xIndex, yIndex, 3, LED_BLACK);
    matrix->fillCircle(xIndex - 1, yIndex, 3, LED_BLACK);
    matrix->fillCircle(xIndex - +1, yIndex, 3, LED_BLACK);
    int relatedIndex = collisionLookupMap[echoLookupIndex];
    echoCounters[relatedIndex] = 1;
    matrix->fillCircle(xAxisEchoOrigin[relatedIndex], yAxisEchoOrigin[relatedIndex], 3, LED_BLACK);
    echoCounters[echoLookupIndex] = 1;

  } else if (counter == 500) {

    matrix->fillCircle( xIndex, yIndex, 2, LED_BLACK);

  } else {


    matrix->drawCircle(xIndex, yIndex, 1, LED_BLACK);
    matrix->drawCircle( xIndex, yIndex, 2, LED_BLACK);
    matrix->drawPixel(xIndex, yIndex, LED_BLACK);
    echoCounters[echoLookupIndex] = 1;
  }
  //
  //
  //  matrix->show();
  //
  //
  //
  //
  usbMIDI.sendNoteOff(61, 0, echoLookupIndex + 1); // 60 = C4

}
void drawEchoMovement(int echoIndex, int mappedPotValue, long currentMillis) {

  Serial.print("Analog value is ; ");
  Serial.println(mappedPotValue);
  Serial.print("X ; ");
  Serial.println(xAxisEchoOrigin[echoIndex]);
  Serial.print("Y ; ");
  Serial.println(yAxisEchoOrigin[echoIndex]);
  Serial.print("echoDirectionLookup ; ");
  Serial.println(echoDirectionLookup[echoIndex]);
  Serial.print("index ; ");
  Serial.println(echoIndex);
  if (echoDirectionLookup[echoIndex] == 'y' ) {
    if (xAxisEchoOrigin[echoIndex] < mappedPotValue) {
      Serial.println("in here helllloooooo");

      xAxisEchoOrigin[echoIndex] += 1;


    } else {
      xAxisEchoOrigin[echoIndex] -= 1;


    }
    matrix->fillCircle(xAxisEchoOrigin[echoIndex], yAxisEchoOrigin[echoIndex], 1, LED_COLORS[random(0, 6)]);
    //If you want a trail of echo comment this function call out
    //    clearPixels(echoIndex);
    matrix->show();

//     Serial.print("comparing analog value to determine if reached end of movement | analog : ");
//  Serial.println(mappedPotValue);
//  Serial.print("X ; ");
//  Serial.println(xAxisEchoOrigin[echoIndex]);

    if ((abs(xAxisEchoOrigin[echoIndex] - mappedPotValue) < 4)) {
      echoInMovement[echoIndex] = false;
      Serial.print("done with echo movement across board on x axis" );
      //        Serial.println(xAxisEchoOrigin[echoIndex]);
      //        delay(1000);
      clearPixels(echoIndex);
      echoCounters[echoIndex] = 1;

    }
  }

  //  } else if (yAxisEchoOrigin[echoIndex] > 0){
  //    yAxisEchoOrigin[echoIndex] -= 1;
  //    //      Serial.println(yAxisEchoOrigin[echoIndex]);
  //    if (yAxisEchoOrigin[echoIndex] <= mappedPotValue  ) {
  //      echoInMovement[echoIndex] = false;
  //      Serial.println("done with echo movement across board on y axis");
  //
  //
  //      clearPixels(echoIndex);
  //         echoCounters[echoIndex] = 1;
  //
  //
  //    }
  //
  //
  //  }
  Serial.println("Reseting this");
  //    clearPixels(echoIndex);


}




