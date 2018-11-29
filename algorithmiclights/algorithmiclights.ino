
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
    int originX = 1;
    int originY;
    int echoLeds[kMatrixWidth][kMatrixHeight];
    int activatedLeds[49];

    Echo() {

      for (int x = 0; x < kMatrixWidth; x++) {
        for (int y = 0; y < kMatrixHeight; y++) {
          echoLeds[x][y] = NULL;

        }
      }
    }

    void clearPixels() {
       for (int x = 0; x < kMatrixWidth; x++) {
        for (int y = 0; y < kMatrixHeight; y++) {
          leds[XY(x,y)]  = CHSV( 0, 0, 0);
          echoLeds[x][y] = NULL;

        }
      }
    }
      
      
};

Echo *echo = new Echo();



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


  fillSquare(3, 3, 1);
  delay(1000);
  fillSquare(3, 3, 2);
  delay(1000);
  fillSquare(3, 3, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);
   fillSquare(3, 3, 1);
  delay(1000);
  fillSquare(3, 3, 2);
  delay(1000);
  fillSquare(3, 3, 3);
  delay(1000);
  Serial.println("going again");
  echo->clearPixels();

}


int xindex;
int yindex;
void fillSquare(int xCoordinate, int  yCoordinate,  int level) {

  int offset = 0;
  if (level == 1) {

    offset = 3;

  } else if (level == 2) {
    offset = 5;

  } else if (level == 3) {

    offset = 7;
  }

  for (int x = 0; x < offset; x++) {
    xindex = xCoordinate - level + x;
//    Serial.print("X : ");
//    Serial.println(xindex);
    for (int i = 0; i < offset; i++) {
      yindex = yCoordinate - level + i;
//      Serial.print("Y : ");
//      Serial.println(yindex);
      int ledLocation = XY(xindex, yindex);

      if (echo->echoLeds[xindex][yindex] == NULL) {
        Serial.println("turning on lights");
        leds[ledLocation]  = CHSV( random(0, 255), 200, 200);
        echo->echoLeds[xindex][yindex] = 1;

      }

    }
  }
  FastLED.show();
}
//
//
//            if (state == 1 && !squares[xindex][yindex].enabled) {
//                squares[xindex][yindex].enabled = true;
//                squares[xindex][yindex].level = level;
//                //         stroke(0);
//                //            fill(100, 200, 255);
//                // rect(210 + (xindex * w), yindex * w, w - 1, w - 1);
//
//            }

//    }


//    Serial.println("filling");
//    Serial.println(XY(xCoordinate - 1, yCoordinate));
//    Serial.println(XY(xCoordinate - offset, yCoordinate - offset));
//    //  int ledLocation = XY(xCoordinate, yCoordinate);
//    //  leds[XY(xCoordinate - offset, yCoordinate)]  = CHSV( 200, 200, 200);
//    //  leds[XY(xCoordinate - offset, yCoordinate + offset)]  = CHSV(200, 200, 200);
//    //  leds[XY(xCoordinate - offset, yCoordinate - offset)]  = CHSV( 200, 200, 200);
//    //  leds[XY(xCoordinate, yCoordinate - offset)]  = CHSV(200, 200, 200);
//    //  leds[XY(xCoordinate, yCoordinate + offset)]  = CHSV( random(0, 255), 200, 200);
//    //  leds[XY(xCoordinate + offset, yCoordinate - offset)]  = CHSV( random(0, 255), 200, 200);
//    //  leds[XY(xCoordinate + offset, yCoordinate)]  = CHSV( random(0, 255), 200, 200);
//    //  leds[XY(xCoordinate + offset, yCoordinate + offset)]  = CHSV( random(0, 255), 200, 200);
//  }
//}

