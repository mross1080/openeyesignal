
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
//const int LED_COLORS[4] = {LED_WHITE_MEDIUM, LED_CYAN_MEDIUM, LED_PURPLE_MEDIUM, LED_BLUE_MEDIUM};
const int LED_COLORS[4] = {LED_WHITE_LOW, LED_CYAN_LOW, LED_PURPLE_LOW, LED_BLUE_LOW};

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
  createEcho( 0, 0, 2, 'y');
  // 1 is coming from the y axis and is closer to the bottom
  createEcho( 1, 0, 9, 'y');
  Serial.println("!!");

  // 2 is coming from x axis and is closest to the left
  createEcho( 2, 20, 14, 'x');
  // 3 is coming from the x axis and is in the middle
  createEcho( 3, 30, 14, 'x');

  // 4is coming from the x axis and is closest to the right
  createEcho( 4, 40, 14, 'x');

  // put your setup code here, to run once:
  int pixelHue = 200;
}

//long previousMillis = 0;

int previousValue0 = 0;
int previousValue1 = 0;
int previousValue2 = 0;
int previousValue3 = 0;
int previousValue4 = 0;
int mappedPotValue1;
int mappedPotValue3;


int collisionLookupMap[5];
int offsetLookup [5];
int echoCounters[5];
int counters[5];
bool echoCollisionTriggerLookup[5];
int xAxisEchoOrigin[5];
int xAxisEchoDefault[5];
int yAxisEchoDefault[5];
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
  while (usbMIDI.read()) {
    // ignore incoming messages
  }


  long timeDelta;
  unsigned long currentMillis = millis();
  long movementTimeDelta;

  //    Serial.println( currentMillis);

  int  sensorValue0 =  analogRead(A0);
  int mappedPotValue0 = map(sensorValue0, 0, 1023, 0, 47);
  //
  //
  int  sensorValue1 =  analogRead(A1);
  int mappedPotValue1 = map(sensorValue1, 0, 1023, 0, 47);
  int  sensorValue2 = analogRead(A3);
  int mappedPotValue2 = map(sensorValue2, 0, 1023, 13, 0);
  int  sensorValue3 = analogRead(A4);
  int mappedPotValue3 = map(sensorValue3, 0, 720, 13, 2);

  int  sensorValue4 = analogRead(A5);
  int mappedPotValue4 = map(sensorValue4, 0, 1023, 13, 0);
  //  Serial.println("n pot value 3 ");
  //  Serial.print(mappedPotValue3);
  movementTimeDelta = currentMillis - previousMovementTime;
  //
  if (movementTimeDelta > 250) {
    if (abs(mappedPotValue0 - previousValue0) > 1) {
      //    Serial.println("changed in pot value 0 moving");
      //    Serial.print(mappedPotValue0);
      //    Serial.println("old value of pot : ");
      //    Serial.print(previousValue0);
      clearPixels(0);
      echoCounters[0] = 500;
      yAxisEchoOrigin[0] = yAxisEchoDefault[0];
      // xAxisEchoOrigin[collisionLookupMap[0]] = xAxisEchoDefault[collisionLookupMap[0]];
      checkAndResetCollisions(0);
      checkAndResetCollisions(2);
      checkAndResetCollisions(3);
      checkAndResetCollisions(4);
      previousValue0 = mappedPotValue0;
      usbMIDI.sendControlChange(1, map(sensorValue0, 0, 1023, 0, 100), 11);

    }

    if (abs(mappedPotValue1 - previousValue1) > 2 ) {

      clearPixels(1);
      echoCounters[1] = 500;

      previousValue1 = mappedPotValue1;
      yAxisEchoOrigin[1] = yAxisEchoDefault[1];
      //    usbMIDI.sendControlChange(3, map(sensorValue0, 0, 1023, 0, 100), 12);
      checkAndResetCollisions(1);
      checkAndResetCollisions(2);
      checkAndResetCollisions(3);
      checkAndResetCollisions(4);


    }



    if (abs(mappedPotValue2 - previousValue2) > 2) {


      Serial.println("changed in pot value");
      clearPixels(2);
      echoCounters[2] = 500;
      previousValue2 = mappedPotValue2;
      //    usbMIDI.sendControlChange(2, map(sensorValue0, 0, 1023, 0, 100), 11);
      checkAndResetCollisions(0);
      checkAndResetCollisions(2);
      checkAndResetCollisions(1);

    }
    //    Serial.print(" sensor2 : ");
    //    //  Serial.println(calculateDistance(xAxisEchoOrigin[3], yAxisEchoOrigin[3], xAxisEchoOrigin[3], mappedPotValue3));
    //    Serial.println(sensorValue2);

    if (abs(mappedPotValue3 - previousValue3) > 2) {
      //if (calculateDistance(xAxisEchoOrigin[3],yAxisEchoOrigin[3],xAxisEchoOrigin[3],mappedPotValue3) > 3) {

      //           Serial.println("changed in pot value 0 moving");
      //        Serial.print(mappedPotValue0);
      //        Serial.println("old value of pot : ");
      //        Serial.print(previousValue0);
      //        Serial.println("!!!!!!");
      //        delay(5000);
      clearPixels(0);
      clearPixels(3);
      echoCounters[3] = 500;
      previousValue3 = mappedPotValue3;
      //      Serial.print("moved 3 the reference collision index is : " );
      //      Serial.println(collisionLookupMap[3]);
      //      Serial.print("the default y value for that one is : " );
      //      Serial.println(yAxisEchoDefault[collisionLookupMap[3]]);
      xAxisEchoOrigin[3] = xAxisEchoDefault[3];
      yAxisEchoOrigin[collisionLookupMap[3]] = yAxisEchoDefault[collisionLookupMap[3]];
      //    usbMIDI.sendControlChange(2, map(sensorValue0, 0, 1023, 0, 100), 11);

      checkAndResetCollisions(0);
      checkAndResetCollisions(3);
      checkAndResetCollisions(1);

    }

    if (abs(mappedPotValue4 - previousValue4) > 2) {
      Serial.println("changed in pot value");
      clearPixels(4);
      echoCounters[4] = 500;
      previousValue4 = mappedPotValue4;
      //    usbMIDI.sendControlChange(2, map(sensorValue0, 0, 1023, 0, 100), 11);
      checkAndResetCollisions(0);
      checkAndResetCollisions(4);
      checkAndResetCollisions(1);

    }


    if (xAxisEchoOrigin[0] != previousValue0) {
      matrix->fillCircle( xAxisEchoOrigin[0], yAxisEchoOrigin[0], 1, LED_BLACK);
      drawEchoMovement(0, mappedPotValue0, currentMillis);
      //      Serial.println("POT MOVE~~!~~~~~!!!!");
      //      Serial.print("x axis value 0 is ; ");
      //      Serial.println(xAxisEchoOrigin[0]);
      //      Serial.print("moving point to  ; ");
      //      Serial.println(previousValue0);
      //      Serial.print("Counter ");
      //      Serial.println(echoCounters[0]);
      //      Serial.print("abs diff ");
      //      Serial.println(abs(xAxisEchoOrigin[0] - previousValue0));

    }

    //    if (abs(xAxisEchoOrigin[1] - previousValue1) > 2) {
    if (xAxisEchoOrigin[1] != previousValue1) {

      matrix->fillCircle( xAxisEchoOrigin[1], yAxisEchoOrigin[1], 1, LED_BLACK);
      drawEchoMovement(1, mappedPotValue1, currentMillis);

    }

    if (yAxisEchoOrigin[2] != previousValue2) {
      matrix->fillCircle( xAxisEchoOrigin[2], yAxisEchoOrigin[2], 1, LED_BLACK);
      drawEchoMovement(2, mappedPotValue2, currentMillis);

    }
    if (yAxisEchoOrigin[3] != previousValue3) {
      matrix->fillCircle( xAxisEchoOrigin[3], yAxisEchoOrigin[3], 1, LED_BLACK);
      drawEchoMovement(3, mappedPotValue3, currentMillis);

    }
    if (yAxisEchoOrigin[4] != previousValue4) {
      matrix->fillCircle( xAxisEchoOrigin[4], yAxisEchoOrigin[4], 1, LED_BLACK);
      drawEchoMovement(4, mappedPotValue4, currentMillis);

    }

    previousMovementTime = currentMillis;

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
    checkForCollisions(0);
    checkForCollisions(1);
    //    checkForCollisions(3);
    previousMillis = currentMillis;

  }

}


int calculateDistance(int xIndex1, int yIndex1, int xIndex2, int yIndex2) {
  return sqrt(pow(xIndex2 - xIndex1, 2) + pow(yIndex2 - yIndex1, 2));


}

void checkForCollisions(int echoLookupIndex) {
  //    Serial.println("!-----------------------------!");

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
    checkStartIndex = 2;
    checkEndIndex = 5;

  }
  for (int index = checkStartIndex; index < checkEndIndex; index++) {

    //  int index = 3;


    //    if ( echoCounters[echoLookupIndex] == 100) {
    //      Serial.print("comparing with alt index");
    //      Serial.println(index);
    //      Serial.print("compare x : ");
    //      Serial.println(xAxisEchoOrigin[index]);
    //      Serial.print("compare y : ");
    //      Serial.println(yAxisEchoOrigin[index]);
    //
    //      Serial.print("Distance : ");
    //      Serial.println(sqrt(pow(xAxisEchoOrigin[index] - xIndex, 2) + pow(yAxisEchoOrigin[index] - yIndex, 2)));
    //    }


    // find distance between origin of echoes
    int relatedLookupIndex = collisionLookupMap[index];
    double originDistance = sqrt(pow(xAxisEchoOrigin[index] - xIndex, 2) + pow(yAxisEchoOrigin[index] - yIndex, 2));
    Serial.println("----------------------------- ");

    Serial.print("index : ");
    Serial.print(echoLookupIndex);
    Serial.print(" x : ");
    Serial.print(xIndex);
    Serial.print(" y : ");
    Serial.println(xIndex);
    Serial.print(" index2 (from for loop): ");
    Serial.print(index);
    Serial.print(" x2: ");
    Serial.print(xAxisEchoOrigin[index]);
    Serial.print(" y2 : ");
    Serial.println(yAxisEchoOrigin[index]);
    Serial.print(" distance between ");
    Serial.println(originDistance);
    int midpointX = (xAxisEchoOrigin[index] + xIndex) / 2;
    int midpointY = (yAxisEchoOrigin[index] + yIndex) / 2;

    // Example
    // echo 0 and 2 are in a collision
    // echocollision lookup for  0 == 2
    // we want to check for echo 1
    //  so 0 and 2 should equal 200
    // 1 should be less than 3
    Serial.println("Related collision index ");
    Serial.println(relatedLookupIndex);
    Serial.print("0 counter  ");
    Serial.println(echoCounters[echoLookupIndex]);
    Serial.print("2 counter  ");
    Serial.println(echoCounters[relatedLookupIndex]);
    Serial.print("1 counter ");
    Serial.println(echoCounters[index]);
    Serial.println("----------------------------- ");

    if (originDistance < 5 && echoCounters[echoLookupIndex] != 100 && echoCounters[index] != 0  && echoCounters[index] != 100 && echoCounters[echoLookupIndex] != 200  && echoCounters[index] != 200 ) {

      clearPixels(index);
      xAxisEchoOrigin[echoLookupIndex] = midpointX;
      yAxisEchoOrigin[echoLookupIndex] = midpointY;
      xAxisEchoOrigin[index] = midpointX;
      yAxisEchoOrigin[index] = midpointY;

      echoCounters[echoLookupIndex] = 100;
      echoCounters[index] = 100;
      collisionLookupMap[index] = echoLookupIndex;
      collisionLookupMap[echoLookupIndex] = index;
      matrix->fillCircle(midpointX, midpointY, 3, LED_COLORS[random(0, 3)]);
      // if there is a collision and the distance is less than 6
      // register that new collision in absorbed and clear everything
      // set counter to a new number and create a visualization for that
      //    } else if (originDistance < 7 && echoCounters[0] == 100 && echoCounters[2] != 100 && index != collisionLookupMap[index]) {
    } else if (originDistance > 1 && originDistance < 7 && echoCounters[0] == 100 && echoCounters[2] == 100 && echoCounters[1] != 100 ) {
      //      Serial.println("3rd collision detected, initiating draw");

      //      // Set all the 3 collisions to the center
      echoCounters[echoLookupIndex] = 200;
      echoCounters[index] = 200;
      echoCounters[collisionLookupMap[index]] = 200;
      clearPixels(echoLookupIndex);
      clearPixels(index);
      clearPixels(collisionLookupMap[index]);
      xAxisEchoOrigin[echoLookupIndex] = midpointX;
      yAxisEchoOrigin[echoLookupIndex] = midpointY;
      xAxisEchoOrigin[index] = midpointX;
      yAxisEchoOrigin[index] = midpointY;
      xAxisEchoOrigin[collisionLookupMap[index]] = midpointX;
      yAxisEchoOrigin[collisionLookupMap[index]] = midpointY;





    }

  }
  //  Serial.println("!-----------------------------!");

}


void checkAndResetCollisions(int echoLookupIndex) {
  int relatedIndex  = collisionLookupMap[echoLookupIndex];

  if (echoDirectionLookup[echoLookupIndex] == 'y') {
    yAxisEchoOrigin[echoLookupIndex] = yAxisEchoDefault[echoLookupIndex];

  } else {
    xAxisEchoOrigin[echoLookupIndex] = xAxisEchoDefault[echoLookupIndex];


  }

  if (echoCounters[echoLookupIndex] == 200) {

    int relatedIndex = collisionLookupMap[echoLookupIndex];
    echoCounters[echoLookupIndex] = 1;
    echoCounters[relatedIndex] = 1;
    echoCounters[collisionLookupMap[relatedIndex ]] = 1;

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
  if (xIndex < 2 || yIndex > 11) {
    counter = 0;
    echoCounters[echoLookupIndex] = counter;

  }
  int color = LED_COLORS[random(0, 3)];

  if (counter == 1) {
    // draw pixel at center of echo

    matrix->drawPixel(xIndex, yIndex, LED_COLORS[random(0, 3)]);

    echoCounters[echoLookupIndex]++;
    usbMIDI.sendNoteOn(61, 155 - counter * 35 , echoLookupIndex + 1);
  } else if (counter <= 3 && counter > 1 ) {
    matrix->drawCircle(xIndex, yIndex, counter - 1, LED_COLORS[random(0, 3)]);
    echoCounters[echoLookupIndex]++;
    usbMIDI.sendNoteOn(61, 155 - counter * 35 , echoLookupIndex + 1);

  } else if (counter == 100) {


    matrix->fillCircle(xIndex, yIndex, 3, LED_COLORS[random(0, 3)]);
    matrix->fillCircle(xIndex, yIndex, 2, LED_COLORS[random(0, 3)]);

    usbMIDI.sendNoteOn(61, 100 , 11);

  } else if (counter == 500) {
    // Draw echo movement and trail

    matrix->fillCircle(xIndex, yIndex, 1, color);

    usbMIDI.sendNoteOn(61, 100 , 13);

  } else if (counter == 200) {
    clearPixels(echoLookupIndex);
    matrix->fillCircle(xIndex, yIndex, 2, LED_COLORS[random(0, 3)]);
    matrix->drawCircle(xIndex, yIndex, 4, LED_GREEN_MEDIUM);


  }

  if (counter > 3 && counter < 10) {
    //    Serial.println("resetting");
    matrix->drawCircle(xIndex, yIndex, 1, LED_BLACK);
    matrix->drawCircle( xIndex, yIndex, 2, LED_BLACK);
    matrix->drawPixel(xIndex, yIndex, LED_BLACK);
    echoCounters[echoLookupIndex] = 1;

    usbMIDI.sendNoteOn(61, 10 , echoLookupIndex + 1);
    if (echoDirectionLookup[echoLookupIndex] == 'y') {
      Serial.print("in here drawing a line between 0 and :");
      Serial.println(xIndex);

      matrix->drawLine(0, yIndex, xIndex , yIndex, color);
    } else {
      matrix->drawLine(xIndex, yIndex, xIndex , 17, color);

    }

    matrix->show();


  }
}

bool pixelHasValue(int pixel) {

  return  strip.getPixelColor(pixel) != 0;

}
int offset = 0;


void createEcho(int index, int x, int y, char c) {

  xAxisEchoOrigin[index] = x;
  yAxisEchoOrigin[index] = y;
  xAxisEchoDefault[index] = x;
  yAxisEchoDefault[index] = y;
  echoCounters[index] = 0;
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


  // Clear collision 1
  if (echoCounters[echoLookupIndex] == 100) {

    matrix->fillCircle(xIndex, yIndex, 3, LED_BLACK);
    matrix->fillCircle(xIndex - 1, yIndex, 3, LED_BLACK);
    matrix->fillCircle(xIndex - +1, yIndex, 3, LED_BLACK);

    //    matrix->fillCircle(xIndex, yIndex, 3, LED_BLACK);
    //    matrix->fillCircle(xIndex - 1, yIndex, 3, LED_BLACK);
    //    matrix->fillCircle(xIndex - +1, yIndex, 3, LED_BLACK);
    int relatedIndex = collisionLookupMap[echoLookupIndex];
    echoCounters[relatedIndex] = 1;
    matrix->fillCircle(xAxisEchoOrigin[relatedIndex], yAxisEchoOrigin[relatedIndex], 3, LED_BLACK);
    echoCounters[echoLookupIndex] = 1;
    usbMIDI.sendNoteOff(61, 0 , 11);
    usbMIDI.sendNoteOff(61, 0 , 13);

    // Clear movement of pixels
  } else if (counter == 500) {

    matrix->fillCircle( xIndex, yIndex, 2, LED_BLACK);

    // not sure if this is the right place for this
  } else if (counter == 200) {
    matrix->fillCircle(xIndex, yIndex, 4, LED_BLACK);

    if (echoDirectionLookup[echoLookupIndex] == 'y') {
      matrix->drawLine(0, yIndex, xIndex , yIndex, LED_BLACK);
    } else {
      matrix->drawLine(xIndex, yIndex, xIndex , 17, LED_BLACK);

    }




  } else if (counter != 0 ) {
    int xIndex = xAxisEchoOrigin[echoLookupIndex];
    int yIndex = yAxisEchoOrigin[echoLookupIndex];
    int color = LED_COLORS[random(0, 3)];
    matrix->drawCircle(xIndex, yIndex, 1, LED_BLACK);
    matrix->drawCircle( xIndex, yIndex, 2, LED_BLACK);
    matrix->drawPixel(xIndex, yIndex, LED_BLACK);





    echoCounters[echoLookupIndex] = 1;
  }
  //
  //
  //    matrix->show();
  //
  //
  //
  //
  usbMIDI.sendNoteOff(61, 0, echoLookupIndex + 1); // 60 = C4

}
void drawEchoMovement(int echoIndex, int mappedPotValue, long currentMillis) {

  if (echoCounters[echoIndex] != 100 && echoCounters[echoIndex] != 200) {

    if (echoDirectionLookup[echoIndex] == 'y' ) {
      if (xAxisEchoOrigin[echoIndex] < mappedPotValue) {

        xAxisEchoOrigin[echoIndex] += 1;


      } else {
        xAxisEchoOrigin[echoIndex] -= 1;


      }
      int xIndex = xAxisEchoOrigin[echoIndex];
      int yIndex = yAxisEchoOrigin[echoIndex];
      int color = LED_COLORS[random(0, 3)];
      matrix->drawLine(0, yIndex, xIndex , yIndex, color);
      matrix->fillCircle(xAxisEchoOrigin[echoIndex], yAxisEchoOrigin[echoIndex], 1, LED_COLORS[random(0, 3)]);
      //If you want a trail of echo comment this function call out
      //    clearPixels(echoIndex);
      matrix->show();

      //     Serial.print("comparing analog value to determine if reached end of movement | analog : ");
      //  Serial.println(mappedPotValue);
      //  Serial.print("X ; ");
      //  Serial.println(xAxisEchoOrigin[echoIndex]);

      if (abs(xAxisEchoOrigin[echoIndex] - mappedPotValue) < 2) {
        echoInMovement[echoIndex] = false;
        //        Serial.println(xAxisEchoOrigin[echoIndex]);
        //        delay(1000);
        clearPixels(echoIndex);
        echoCounters[echoIndex] = 1;

      }
    }

    if (echoDirectionLookup[echoIndex] == 'x' ) {
      if (yAxisEchoOrigin[echoIndex] < mappedPotValue) {
        yAxisEchoOrigin[echoIndex] += 1;

      } else {
        yAxisEchoOrigin[echoIndex] -= 1;


      }
      int xIndex = xAxisEchoOrigin[echoIndex];
      int yIndex = yAxisEchoOrigin[echoIndex];
      int color = LED_COLORS[random(0, 3)];
      matrix->drawLine(xIndex, yIndex, xIndex , 14, color);
      matrix->fillCircle(xAxisEchoOrigin[echoIndex], yAxisEchoOrigin[echoIndex], 1, color);
      //If you want a trail of echo comment this function call out
      //          clearPixels(echoIndex);
      matrix->show();

      //      Serial.print("comparing analog value to determine if reached end of movement | analog : ");
      //      Serial.println(mappedPotValue);
      //      Serial.print("y ; ");
      //      Serial.println(yAxisEchoOrigin[echoIndex]);
      //      Serial.print("abs ; ");
      //      Serial.println(abs(yAxisEchoOrigin[echoIndex] - mappedPotValue));
      int distance = calculateDistance(xAxisEchoOrigin[3], yAxisEchoOrigin[3], xAxisEchoOrigin[3], mappedPotValue);
      //      Serial.print("dis : ");
      //      Serial.println(distance);
      // maybe make this work one day?
      //if (distance < 3) {
      if (abs(yAxisEchoOrigin[echoIndex] - mappedPotValue) < 2) {
        echoInMovement[echoIndex] = false;
        Serial.println("done with echo movement across board on y axis" );

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
    //    clearPixels(echoIndex);
  }

}




