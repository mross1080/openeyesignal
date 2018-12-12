
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
int desiredAxisValue[5];
int currentPotValues[5];

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
  desiredAxisValue[index] = 0;


}


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


void loop() {
  while (usbMIDI.read()) {
    // ignore incoming messages
  }

  //
  long timeDelta;
  unsigned long currentMillis = millis();
  long movementTimeDelta;
  //
  //  //    Serial.println( currentMillis);
  //
  int  sensorValue0 =  analogRead(A0);
  int mappedPotValue0 = map(sensorValue0, 0, 1023, 0, 47);
  desiredAxisValue[0] = mappedPotValue0;
  //
  //
  int  sensorValue1 =  analogRead(A1);
  int mappedPotValue1 = map(sensorValue1, 0, 1023, 0, 47);
  desiredAxisValue[1] = mappedPotValue1;

  int  sensorValue2 = analogRead(A3);
  int mappedPotValue2 = map(sensorValue2, 0, 1023, 13, 0);
  desiredAxisValue[2] = mappedPotValue2;

  int  sensorValue3 = analogRead(A4);
  int mappedPotValue3 = map(sensorValue3, 0, 720, 13, 2);
  desiredAxisValue[3] = mappedPotValue3;


  int  sensorValue4 = analogRead(A5);
  int mappedPotValue4 = map(sensorValue4, 0, 1023, 13, 0);
  desiredAxisValue[4] = mappedPotValue4;

  //    Serial.println("n pot value 4 ");
  //    Serial.print(mappedPotValue4);
  movementTimeDelta = currentMillis - previousMovementTime;
  //
  if (movementTimeDelta > 250) {

    //    // draw 3rd col
    //    Serial.print(" xAxisEchoOrigin 0 : ");
    //    Serial.println(xAxisEchoOrigin[0]);
    //    Serial.print(" counter 0 : ");
    //    Serial.println(echoCounters[0]);
    //    Serial.print(" counter 1 : ");
    //    Serial.println(echoCounters[1]);
    //    Serial.print(" counter 2 : ");
    //    Serial.println(echoCounters[2]);
    //    Serial.print(" counter  3 : ");
    //    Serial.println(echoCounters[3]);
    //    Serial.print(" counter  4 : ");
    //    Serial.println(echoCounters[4]);
    //    Serial.println("yAxisEchoOrigin 0 : ");
    //    Serial.println(yAxisEchoOrigin[0]);
    //    Serial.print(" desired x for  0 : ");
    //    Serial.println( desiredAxisValue[0]);


    //    // for each echo check if the potentiometer has moved
    for (int echoLookupIndex = 0; echoLookupIndex < NUM_ECHOES; echoLookupIndex++) {


      // if there has been a change in pot value

      //      Serial.print(" index  moved ! : ");
      //      Serial.println( echoLookupIndex );
      //      Serial.print(" index  : ");
      //      Serial.println( echoLookupIndex );
      //      Serial.print(" comparing  : ");
      //      Serial.println( currentPotValues[echoLookupIndex] );
      //      Serial.print(" with  : ");
      //      Serial.println( desiredAxisValue[echoLookupIndex] );
      if (echoDirectionLookup[echoLookupIndex] == 'y') {
        if (abs(xAxisEchoOrigin[echoLookupIndex] - desiredAxisValue[echoLookupIndex]) > 2) {
              Serial.print(" $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ");
                Serial.print(" index  moved ! : ");
            Serial.println( echoLookupIndex );
            Serial.print(" index  : ");
            Serial.println( echoLookupIndex );
            Serial.print(" comparing  : ");
            Serial.println( xAxisEchoOrigin[echoLookupIndex] );
            Serial.print(" with  : ");
            Serial.println( desiredAxisValue[echoLookupIndex] );
          Serial.print(" index  moved ! : ");
          Serial.println( echoLookupIndex );

          clearPixels(echoLookupIndex);
          resetWallCollision(echoLookupIndex);
          echoCounters[echoLookupIndex] = 500;


        }
      } else {
        if (abs(yAxisEchoOrigin[echoLookupIndex] - desiredAxisValue[echoLookupIndex]) > 2) {

          clearPixels(echoLookupIndex);
          resetWallCollision(echoLookupIndex);

          echoCounters[echoLookupIndex] = 500;

        }
      }

    }

    // for each echo check if the movement is complete
    for (int echoLookupIndex = 0; echoLookupIndex < NUM_ECHOES; echoLookupIndex++) {
      // if there has been a change in pot value
      if (echoDirectionLookup[echoLookupIndex] == 'y') {

              
        if (abs(xAxisEchoOrigin[echoLookupIndex] - desiredAxisValue[echoLookupIndex]) > 1) {
            Serial.print("abs diff :");

                Serial.println(abs(xAxisEchoOrigin[echoLookupIndex] - desiredAxisValue[echoLookupIndex]));

          Serial.print("moving echo : ");

          Serial.println(echoLookupIndex);
           Serial.print("checking echo : ");

          Serial.println(echoLookupIndex);
        Serial.print(" current y axis : ");
        Serial.print(xAxisEchoOrigin[echoLookupIndex]);
          Serial.print("moving y axis to : ");
        Serial.print(desiredAxisValue[echoLookupIndex]);


          matrix->fillCircle( xAxisEchoOrigin[echoLookupIndex], yAxisEchoOrigin[echoLookupIndex], 1, LED_BLACK);
          drawEchoMovement(echoLookupIndex, currentPotValues[echoLookupIndex], currentMillis);
        }

      } else {
         
        if (abs(yAxisEchoOrigin[echoLookupIndex] - desiredAxisValue[echoLookupIndex]) > 1) {
           Serial.print("checking echo : ");

          Serial.println(echoLookupIndex);
        Serial.print(" current y axis : ");
        Serial.print(yAxisEchoOrigin[echoLookupIndex]);
          Serial.print("moving y axis to : ");
        Serial.print(desiredAxisValue[echoLookupIndex]);

          Serial.print("moving echo : ");

          Serial.println(echoLookupIndex);


          matrix->fillCircle( xAxisEchoOrigin[echoLookupIndex], yAxisEchoOrigin[echoLookupIndex], 1, LED_BLACK);
          drawEchoMovement(echoLookupIndex, currentPotValues[echoLookupIndex], currentMillis);
        }
      }

    }
    previousMovementTime = currentMillis;
  }

  timeDelta = currentMillis - previousMillis;
  
  ////draw out animation cycle and check for collisions 
  if (timeDelta > 857) {
    for (int index = 0; index < NUM_ECHOES; index++) {
      drawEchoAnimation(index);

    }
    matrix->show();
    //    stageOne = false;
    previousMillis = currentMillis;

    checkForCollisions(0);
  }

}

void resetWallCollision(int echoLookupIndex) {
  // THIS IS A HACK AND WILL ONLY ALLOW FOR ONE 3RD COLLISION, NEEDS TO BE FIXED AT SOME POINT
  //   Serial.println("WALL WALL WALL WALL WALL");
  //   Serial.println(echoLookupIndex);
  //   Serial.println(echoCounters[echoLookupIndex]);

  int  index = collisionLookupMap[echoLookupIndex];



  for (int index = 0; index < 4; index++) {
    if (echoCounters[index] == 200) {

      echoCounters[index] = 1;
    }

  }


  if (echoCounters[echoLookupIndex] == 100) {
    int relatedIndex = collisionLookupMap[echoLookupIndex];
    if (echoDirectionLookup[echoLookupIndex] == 'x') {
      xAxisEchoOrigin[echoLookupIndex] = xAxisEchoDefault[echoLookupIndex];
      yAxisEchoOrigin[relatedIndex] = yAxisEchoDefault[relatedIndex];


    } else {
      yAxisEchoOrigin[echoLookupIndex] = yAxisEchoDefault[echoLookupIndex];
      xAxisEchoOrigin[relatedIndex] = xAxisEchoDefault[relatedIndex];


    }

  }



  //        if (echoCounters[echoLookupIndex] == 200 || echoCounters[index] == 200 ||  echoCounters[collisionLookupMap[index]] == 200)
  if (echoCounters[echoLookupIndex] == 200 ) {

    Serial.println("broken the wall changing these 3 ");
    Serial.println(echoLookupIndex);
    Serial.println(index);
    Serial.println(collisionLookupMap[index]);
    echoCounters[echoLookupIndex] = 1;
    echoCounters[index] = 1;
    echoCounters[collisionLookupMap[index]] = 1;
  }

}


int calculateDistance(int xIndex1, int yIndex1, int xIndex2, int yIndex2) {
  return sqrt(pow(xIndex2 - xIndex1, 2) + pow(yIndex2 - yIndex1, 2));


}

void checkForCollisions(int echoLookupIndex) {
  Serial.println("!-----------------------------!");


  for (int echoLookupIndex = 0; echoLookupIndex < 2; echoLookupIndex++) {

    int checkStartIndex;
    int checkEndIndex;
    int xIndex = xAxisEchoOrigin[echoLookupIndex];
    int yIndex = yAxisEchoOrigin[echoLookupIndex];
    int currentCounter = echoCounters[echoLookupIndex];

    if (echoDirectionLookup[echoLookupIndex] == 'x') {
      checkStartIndex = 0;
      checkEndIndex = 3;

    } else {
      checkStartIndex = 2;
      checkEndIndex = 5;

    }
    for (int index = checkStartIndex; index < checkEndIndex; index++) {


      // find distance between origin of echoes
      int relatedLookupIndex = collisionLookupMap[index];
      double originDistance = sqrt(pow(xAxisEchoOrigin[index] - xIndex, 2) + pow(yAxisEchoOrigin[index] - yIndex, 2));
            int midpointX = (xAxisEchoOrigin[index] + xIndex) / 2;
      int midpointY = (yAxisEchoOrigin[index] + yIndex) / 2;
//      Serial.println("----------------------------- ");
//      Serial.print(" counter 1 : ");
//      Serial.println(echoCounters[1]);
//      Serial.print(" counter 2 : ");
//      Serial.println(echoCounters[2]);
//
//
//
//
//      Serial.print("index : ");
//      Serial.print(echoLookupIndex);
//      Serial.print(" x : ");
//      Serial.print(xIndex);
//      Serial.print(" y : ");
//      Serial.println(xIndex);
//      Serial.print(" counter : ");
//      Serial.println(echoCounters[echoLookupIndex]);
//
//
//
//
//      Serial.print(" index2 (from for loop): ");
//      Serial.print(index);
//      Serial.print(" x2: ");
//      Serial.print(xAxisEchoOrigin[index]);
//      Serial.print(" y2 : ");
//      Serial.println(yAxisEchoOrigin[index]);
//      Serial.print(" distance between ");
//      Serial.println(originDistance);
//      Serial.print(" counter : ");
//      Serial.println(echoCounters[index]);

//
//      // Example
//      // echo 0 and 2 are in a collision
//      // echocollision lookup for  0 == 2
//      // we want to check for echo 1
//      //  so 0 and 2 should equal 200
//      // 1 should be less than 3
//      Serial.println("Related collision index ");
//      Serial.println(relatedLookupIndex);
//      Serial.print(echoLookupIndex);
//      Serial.print(" counter  ");
//      Serial.println(echoCounters[echoLookupIndex]);
//      Serial.print(relatedLookupIndex);
//
//      Serial.print(" counter  ");
//      Serial.println(echoCounters[relatedLookupIndex]);
//      Serial.print(index);
//      Serial.print(" counter ");
//      Serial.println(echoCounters[index]);
//      Serial.println("---------------------------- - ");


      if (originDistance < 5 && echoCounters[echoLookupIndex] != 100 && echoCounters[index] != 0  &&
          echoCounters[echoLookupIndex] != 200  && echoCounters[index] != 200 ) {
        Serial.println("triggering collision  ");

        if (echoDirectionLookup[echoLookupIndex] == 'y' ) {

          desiredAxisValue[echoLookupIndex] = midpointX ;
          
        } else {
          desiredAxisValue[echoLookupIndex] = midpointY ;

        }
         Serial.print(" comparing  : ");
            Serial.println( desiredAxisValue[echoLookupIndex] );

        Serial.println("calling clear pixels from col function");
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
                      Serial.print(" ^^^^^^^^^^^^^^ ");


        // if there is a collision and the distance is less than 6
        // register that new collision in absorbed and clear everything
        // set counter to a new number and create a visualization for that
        //    } else if (originDistance < 7 && echoCounters[0] == 100 && echoCounters[2] != 100 && index != collisionLookupMap[index]) {
        //    } else if (originDistance > 1 && originDistance < 7 && echoCounters[0] == 100 && echoCounters[2] == 100 && echoCounters[1] != 100 ) {
      } else if (originDistance > 1 && originDistance < 7 && echoCounters[echoLookupIndex] != 100 && echoCounters[relatedLookupIndex] == 100 && echoCounters[index] == 100 ) {

        //      Serial.println("3rd collision detected, initiating draw");

        //      // Set all the 3 collisions to the center
        echoCounters[echoLookupIndex] = 200;
        echoCounters[index] = 200;
        echoCounters[collisionLookupMap[index]] = 200;
        collisionLookupMap[collisionLookupMap[index]] = echoLookupIndex;
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
    //  Serial.println("!---------------------------- -!");
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
  //  checkForCollisions(0);
  if (xIndex < 2 || yIndex > 11) {
    counter = 0;
    echoCounters[echoLookupIndex] = counter;

  }
  int color = LED_COLORS[random(0, 3)];

  if (counter == 1) {
    // draw pixel at center of echo

    matrix->drawPixel(xIndex, yIndex, LED_COLORS[random(0, 3)]);
    //    matrix->drawLine(xIndex, yIndex, xIndex+3, yIndex-3,LED_COLORS[random(0, 3)]);

    echoCounters[echoLookupIndex]++;
    usbMIDI.sendNoteOn(61, 155 - counter * 35 , echoLookupIndex + 1);
  } else if (counter <= 3 && counter > 1 ) {
    Serial.println("here i am turning this old thing on");
    matrix->drawCircle(xIndex, yIndex, counter - 1, LED_COLORS[random(0, 3)]);
    echoCounters[echoLookupIndex]++;
    usbMIDI.sendNoteOn(61, 155 - counter * 35 , echoLookupIndex + 1);

  } else if (counter == 100) {
    // collision 1

    matrix->fillCircle(xIndex, yIndex, 3, LED_COLORS[random(0, 3)]);
    matrix->fillCircle(xIndex, yIndex, 2, LED_COLORS[random(0, 3)]);

    usbMIDI.sendNoteOn(61, 100 , 11);

  } else if (counter == 500) {
    // Draw echo movement and trail

    usbMIDI.sendNoteOn(61, 100 , 13);

  } else if (counter == 200) {
    // collision 2
    clearPixels(echoLookupIndex);
    int color = LED_COLORS[random(0, 3)];
    //    matrix->fillCircle(xIndex, yIndex, 2, LED_COLORS[random(0, 3)]);
    matrix->fillRect(xIndex - 4, 0, 10, 14, LED_BLACK);
    //fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
    int start = 3;
    int triangleYAxisStart = yIndex - 3;

    for (int offset = 0; offset < 4; offset++) {
      matrix->drawLine(xIndex + offset, triangleYAxisStart + offset, (xIndex + 6) - offset , triangleYAxisStart + offset, color);
      matrix->drawLine(xIndex + offset, (triangleYAxisStart + 6) - offset, (xIndex + 6) - offset , (triangleYAxisStart + 6) - offset, color);


    }

    matrix->fillRect(xIndex, 0, 7, triangleYAxisStart, color);
    matrix->fillRect(xIndex, triangleYAxisStart + 6, 7, 14 - (triangleYAxisStart + 6), color);



  }

  if (counter > 3 && counter < 10) {
    Serial.println("resetting");
    matrix->drawCircle(xIndex, yIndex, 1, LED_BLACK);
    matrix->drawCircle( xIndex, yIndex, 2, LED_BLACK);
    matrix->drawPixel(xIndex, yIndex, LED_BLACK);
    echoCounters[echoLookupIndex] = 1;

    usbMIDI.sendNoteOn(61, 10 , echoLookupIndex + 1);


    matrix->show();


  }
}

bool pixelHasValue(int pixel) {

  return  strip.getPixelColor(pixel) != 0;

}
//int offset = 0;



//
void clearPixels(int echoLookupIndex) {
  int counter = echoCounters[echoLookupIndex];
  int xIndex = xAxisEchoOrigin[echoLookupIndex];
  int yIndex = yAxisEchoOrigin[echoLookupIndex];

  Serial.println("in clear pixels");
  // Clear collision 1
  if (echoCounters[echoLookupIndex] == 100) {
    Serial.println("in clear pixels 100");

    matrix->fillCircle(xIndex, yIndex, 4, LED_BLACK);
    matrix->fillCircle(xIndex - 1, yIndex, 3, LED_BLACK);
    matrix->fillCircle(xIndex - +1, yIndex, 3, LED_BLACK);

    //    matrix->fillCircle(xIndex, yIndex, 3, LED_BLACK);
    //    matrix->fillCircle(xIndex - 1, yIndex, 3, LED_BLACK);
    //    matrix->fillCircle(xIndex - +1, yIndex, 3, LED_BLACK);
    int relatedIndex = collisionLookupMap[echoLookupIndex];
    // not sure about this line 
//    echoCounters[relatedIndex] = 1;
    matrix->fillCircle(xAxisEchoOrigin[relatedIndex], yAxisEchoOrigin[relatedIndex], 3, LED_BLACK);
    echoCounters[echoLookupIndex] = 1;
    usbMIDI.sendNoteOff(61, 0 , 11);
    usbMIDI.sendNoteOff(61, 0 , 13);

    // Clear movement of pixels
  } else if (counter == 500) {
    Serial.println("in clear pixels 500");

    matrix->fillCircle( xIndex, yIndex, 2, LED_BLACK);
    if (echoDirectionLookup[echoLookupIndex] == 'y') {
      matrix->drawLine(xIndex - 6, yIndex, xIndex , yIndex, LED_BLACK);
    } else {
      matrix->drawLine(xIndex, yIndex, xIndex , yIndex + 6, LED_BLACK);

    }


    // not sure if this is the right place for this
  } else if (counter == 200) {
    //    matrix->fillCircle(xIndex, yIndex, 4, LED_BLACK);

    //    for (int echoLookupIndex = 0; echoLookupIndex < 4; echoLookupIndex++) {
    //      if (echoCounters[echoLookupIndex] == 200) {
    //
    ////        echoCounters[echoLookupIndex] = 1;
    //      }
    //
    //    }
    matrix->fillRect(xIndex, 0, 7, 14, LED_BLACK);


  } else if (counter < 5 ) {
    Serial.println("hurrr");
    // clear normal echo animation sequence
    int xIndex = xAxisEchoOrigin[echoLookupIndex];
    int yIndex = yAxisEchoOrigin[echoLookupIndex];
    //    matrix->drawCircle(xIndex, yIndex, 1, LED_BLACK);
    matrix->fillCircle( xIndex, yIndex, 3, LED_BLACK);
    //    matrix->drawPixel(xIndex, yIndex, LED_BLACK);
    echoCounters[echoLookupIndex] = 1;
  }

  usbMIDI.sendNoteOff(61, 0, echoLookupIndex + 1); // 60 = C4
  Serial.println("done clearing pixels");

}



void drawEchoMovement(int echoIndex, int mappedPotValue, long currentMillis) {
  Serial.println("in draw echo movement");

  // only draw movement if we're not in a collision
  //  if (echoCounters[echoIndex] != 100 && echoCounters[echoIndex] != 200 && ) {
  if (echoCounters[echoIndex] == 500 ) {
    Serial.println("in draw echo movement with 500 ");
    int relatedIndex = collisionLookupMap[echoIndex];
    if (echoDirectionLookup[echoIndex] == 'x') {
      xAxisEchoOrigin[echoIndex] = xAxisEchoDefault[echoIndex];

    } else {
      yAxisEchoOrigin[echoIndex] = yAxisEchoDefault[echoIndex];

    }



    Serial.println("in draw echo movement2222");

    // if the echo is coming from the left side
    if (echoDirectionLookup[echoIndex] == 'y' ) {
      int xIndex = xAxisEchoOrigin[echoIndex];
      int yIndex = yAxisEchoOrigin[echoIndex];
      int color = LED_COLORS[random(0, 3)];

      // if we're moving towards the right side
      if (xAxisEchoOrigin[echoIndex] < desiredAxisValue[echoIndex]) {
        matrix->drawLine(0, yIndex, xIndex - 5 , yIndex, LED_BLACK);
        xAxisEchoOrigin[echoIndex] += 1;
        matrix->drawLine(xIndex - 2, yIndex, xIndex , yIndex, color);

      } else {
        // moving to the left side closer to 0
        matrix->drawLine(0, yIndex, xIndex +   7 , yIndex, LED_BLACK);

        xAxisEchoOrigin[echoIndex] -= 1;
        matrix->drawLine(xIndex + 2, yIndex, xIndex , yIndex, color);
      }

      //      matrix->drawLine(0, yIndex, xIndex , yIndex, LED_BLACK);
      Serial.println("making circles happen1");
      matrix->fillCircle(xAxisEchoOrigin[echoIndex], yAxisEchoOrigin[echoIndex], 1, LED_COLORS[random(0, 3)]);
      if (abs(xAxisEchoOrigin[echoIndex] - desiredAxisValue[echoIndex]) < 3) {
        echoInMovement[echoIndex] = false;
        matrix->drawLine(0, yIndex, xIndex + 4 , yIndex, LED_BLACK);
        clearPixels(echoIndex);
        echoCounters[echoIndex] = 1;
        xAxisEchoOrigin[echoIndex] = desiredAxisValue[echoIndex];

      }
      matrix->show();

    }

    if (echoDirectionLookup[echoIndex] == 'x') {
      int color = LED_COLORS[random(0, 3)];
      int xIndex = xAxisEchoOrigin[echoIndex];
      int yIndex = yAxisEchoOrigin[echoIndex];


      if (yAxisEchoOrigin[echoIndex] < desiredAxisValue[echoIndex]) {
        Serial.println("moving echo down y axis");

        //move echo down the screen
        matrix->drawLine(xIndex, 0, xIndex , yIndex - 4, LED_BLACK);

        yAxisEchoOrigin[echoIndex] += 1;
        //        matrix->drawLine(xIndex, 13, xIndex , yIndex - 2, color);
        matrix->drawLine(xIndex, yIndex, xIndex , yIndex - 2, color);


      } else {
        // move echo up the screen
        Serial.println("moving echo up y axis");

        matrix->drawLine(xIndex, 13, xIndex , yIndex + 3, LED_BLACK);

        yAxisEchoOrigin[echoIndex] -= 1;
        matrix->drawLine(xIndex, yIndex, xIndex , yIndex + 2, color);



      }
      Serial.println("making circles happen2");

      matrix->fillCircle(xAxisEchoOrigin[echoIndex], yAxisEchoOrigin[echoIndex], 1, color);

      if (abs(yAxisEchoOrigin[echoIndex] - desiredAxisValue[echoIndex]) < 2) {
        //      if (yAxisEchoOrigin[echoIndex] == mappedPotValue) {
        yAxisEchoOrigin[echoIndex] = desiredAxisValue[echoIndex];
        echoInMovement[echoIndex] = false;
        Serial.println("done with echo movement across board on y axis" );
        Serial.println("AHHHHHHHHHHHHHHHHHHH");

        matrix->drawLine(xIndex, 0, xIndex , 14, LED_BLACK);

        //        matrix->drawLine(xIndex, yIndex, xIndex , 14, LED_BLACK);
        clearPixels(echoIndex);
        echoCounters[echoIndex] = 1;

      }
      matrix->show();
    }


  }


}




