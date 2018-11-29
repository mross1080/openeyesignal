
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


class Echo {
  public:
    int originX;
    int originY;
    int echoLeds[kMatrixWidth][kMatrixHeight];
    int activatedLeds[49];
    int minimumX;
    int minimumY;

    Echo() {}

    Echo(int x, int y) {
      originX = x;
      originY = y;

      for (int x = 0; x < kMatrixWidth; x++) {
        for (int y = 0; y < kMatrixHeight; y++) {
          echoLeds[x][y] = NULL;

        }
      }
    }

    void clearPixels() {
      counter = 1;
      for (int x = 0; x < kMatrixWidth; x++) {
        for (int y = 0; y < kMatrixHeight; y++) {
          leds[XY(x, y)]  = CHSV( 0, 0, 0);
          echoLeds[x][y] = NULL;

        }
      }
    }

    int previousMillis = 0;
    int counter = 1;

    void drawEchoAnimation(long currentMillis) {


      long timeDelta = currentMillis - previousMillis;
      if (counter <= 3 ) {
        fillSquare(originX, originY, counter);
        counter++;

      } else {
        clearPixels();
      }

    }


    int xindex;
    int yindex;
    int offset = 0;

    void fillSquare(int xCoordinate, int  yCoordinate,  int level) {
      int pixelHue = random(0, 255);
      if (level == 1) {

        offset = 3;

      } else if (level == 2) {
        offset = 5;

      } else if (level == 3) {

        offset = 7;
      }

      for (int x = 0; x < offset; x++) {
        xindex = xCoordinate - level + x;
        Serial.print("X : ");
        Serial.println(xindex);
        for (int i = 0; i < offset; i++) {
          yindex = yCoordinate - level + i;
          Serial.print("Y : ");
          Serial.println(yindex);
          int ledLocation = XY(xindex, yindex);

          if (echoLeds[xindex][yindex] == NULL) {
            Serial.println("turning on lights");
            leds[ledLocation]  = CHSV(pixelHue, 200, 200);
            echoLeds[xindex][yindex] = 1;

          }

        }
      }

    }


    bool compareMinMax(int compareMin, int compareMax, int compareOffset) {

      return true;



    }




};
Echo echoLookup[7];
Echo *echo = new Echo(5, 3);
Echo *echo2 = new Echo(10, 7);
Echo *echo3 = new Echo(14,10);
//Echo *echo4 = new Echo(4,10);



void setup() {
  Serial.println(echo->originX);
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

  //
  //  fillSquare(3, 3, 1);
  //  delay(1000);
  //  fillSquare(3, 3, 2);
  //  delay(1000);
  //  fillSquare(3, 3, 3);
}

void loop() {
  unsigned long currentMillis = millis();
  //  Echo echoLookup[2];
  // put your main code here, to run repeatedly:
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);
  //   fillSquare(3, 3, 1,echo->echoLeds);
  //   fillSquare(8, 8, 1,echo2->echoLeds);
  //  delay(1000);
  //  fillSquare(3, 3, 2);
  //  fillSquare(8, 8  2,echo2);
  //  delay(1000);
  //  fillSquare(3, 3, 3);
  //  fillSquare(8, 8, 2, echo2);
  echo->drawEchoAnimation(currentMillis);
  echo2->drawEchoAnimation(currentMillis);
    echo3->drawEchoAnimation(currentMillis);
//    echo4->drawEchoAnimation(currentMillis);
  delay(400);
  Serial.println("going again");
  for (int index = 0; index < 7; index++) {
    for (int compareIndex = 0; compareIndex < 7; compareIndex++) {
      if (index != compareIndex) {
        if (echoLookup[index].compareMinMax(echoLookup[compareIndex].minimumX, echoLookup[compareIndex].minimumY, echoLookup[compareIndex].offset)) {
          Serial.println("!!!!!!!!!!!");


        }


      }
    }
  }

  FastLED.show();
  //  echo->clearPixels();
  //  echo2->clearPixels();

}


//int xindex;
//int yindex;
//void fillSquare(int xCoordinate, int  yCoordinate,  int level) {
//  int pixelHue = random(0,255);
//  int offset = 0;
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
//    xindex = xCoordinate - level + x;
//    Serial.print("X : ");
//    Serial.println(xindex);
//    for (int i = 0; i < offset; i++) {
//      yindex = yCoordinate - level + i;
//      Serial.print("Y : ");
//      Serial.println(yindex);
//      int ledLocation = XY(xindex, yindex);
//
//      if (echo->echoLeds[xindex][yindex] == NULL) {
//        Serial.println("turning on lights");
//        leds[ledLocation]  = CHSV(pixelHue, 200, 200);
//        echo->echoLeds[xindex][yindex] = 1;
//
//      }
//
//    }
//  }
//  FastLED.show();
//}
//

