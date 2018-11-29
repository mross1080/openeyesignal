
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
  createEcho( 0, 6, 4);
  createEcho( 1, 12, 7);
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

void loop() {
  unsigned long currentMillis = millis();
  //  Echo echoLookup[2];
  // put your main code here, to run repeatedly:
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);


  drawEchoAnimation(0);
  drawEchoAnimation(1);
  //  drawEchoAnimation(2);

  delay(400);
  Serial.println("starting compare loop");

  if (compareMinMax(0, 1)) {
     Serial.println("!!!!!!!!!!!");
        
    
    }
//  for (int index = 0; index < 2; index++) {
//    for (int compareIndex = 0; compareIndex < 2; compareIndex++) {
//      
//      if (index != compareIndex) {
//        Serial.println("comparing again");
//        if (compareMinMax(index, compareIndex)) {
//          
//          //                    if (echoLookup[index].compareMinMax(echoLookup[compareIndex].minimumX, echoLookup[compareIndex].minimumY, echoLookup[compareIndex].offset)) {
//          Serial.println("!!!!!!!!!!!");
//          //          for (int x = 0; x < kMatrixWidth; x++) {
//          //            for (int y = 0; y < kMatrixHeight; y++) {
//          //              leds[XY(x, y)]  = CHSV( 220, 220, 220);
//          //            }
//          //
//          //
//          //
//          //          }
//
//
//        }
//
//
//      }
//    }
//  }

  FastLED.show();
  //  echo->clearPixels();
  //  echo2->clearPixels();

}


int offsetLookup [3];
int echoCounters[3];
int xAxisEchoOrigin[3];
int xAxisEchoMinimum[3];
int yAxisEchoOrigin[3];
int pixelsInEcho[3][49];
//int xAxisEchoMinimum[3];

bool compareMinMax(int echo1Index, int echo2Index) {


  // Retrieve the origin of both echos 
  int echo1x = xAxisEchoOrigin[echo1Index];
  int echo1y = yAxisEchoOrigin[echo1Index];
  int echo2x = xAxisEchoOrigin[echo2Index];
  int echo2y = yAxisEchoOrigin[echo2Index];

    
  int offset1 = offsetLookup[echo1Index];
  int offset2 = offsetLookup[echo2Index];


  if( offset1 > 1 && offset2 > 1) {







  
  int offset1 = offsetLookup[echo1Index];
  int offset2 = offsetLookup[echo2Index];

  int echo1xMax = echo1x + offset1;
  int echo1yMax = echo1y + offset1;
  int echo2xMin = echo2x - offset2;
  int echo2yMin = echo2y - offset2;


    Serial.print("X1 : ");
  Serial.println(echo1x);
  Serial.print("Y1 : ");
  Serial.println(echo1y);
      Serial.print("X2: ");
  Serial.println(echo2x);
  Serial.print("Y2 : ");
  Serial.println(echo2y);
//
  Serial.print("E1 offset : ");
  Serial.println(offset1);
  Serial.print("E2 offset : ");
  Serial.println(offset2);

  Serial.print("X1 max : ");
  Serial.println(echo1xMax);
  Serial.print("Y1 max : ");
  Serial.println(echo1yMax);
  Serial.print("X2 min : ");
  Serial.println(echo2xMin);
  Serial.print("Y2 min : ");
  Serial.println(echo2yMin);

  if (echo2xMin <= echo1xMax && echo2yMin <= echo1yMax) {


     leds[XY(echo1x, echo1y)]  = CHSV( 80, 220, 220);
     leds[XY(echo2x, echo2y)]  = CHSV( 80, 220, 220);

//     delay(10000);


    return true;
  }
  }

  return false;

}

void drawEchoAnimation(int echoLookupIndex) {
  int counter = echoCounters[echoLookupIndex];

  if (counter <= 3 ) {
    fillSquare(echoLookupIndex, xAxisEchoOrigin[echoLookupIndex], yAxisEchoOrigin[echoLookupIndex], counter);
    echoCounters[echoLookupIndex]++;

  } else {
//        clearPixels(echoLookupIndex);
  }

}

bool pixelHasValue(int pixel) {

  return  leds[pixel].red != 0 ||  leds[pixel].blue != 0 ||  leds[pixel].green != 0;

}
int offset = 0;

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




void createEcho(int index, int x, int y) {
  xAxisEchoOrigin[index] = x;
  yAxisEchoOrigin[index] = y;
  echoCounters[index] = 1;
  offsetLookup[index] = 0;

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

