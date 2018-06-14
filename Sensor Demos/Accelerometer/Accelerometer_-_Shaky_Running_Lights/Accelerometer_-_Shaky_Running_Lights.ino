/* StitchKit.io Accelerometer Example - Rainbow Effect to Pulse Effect based on Proximity
 * -----------------------
 * 
 * This software switches a rainbow effect to a pulse effect based on the grove proximity sensor.
 * 
 * Requires:
 * ---------
 *  MakeFashion controller board
 *  SeeedStudio MMA7660 3-Axis Digital Accelerometer (http://wiki.seeedstudio.com/Grove-3-Axis_Digital_Accelerometer-1.5g/ )
 *  FastLED-compatible LEDs ( https://github.com/FastLED/FastLED/wiki/Overview#chipsets )
 *  
 * Dependencies:
 * -------------
 *  FastLED ( Available via Arduino package manager )
 *  SeeedStudio MMA7660 library (https://github.com/Seeed-Studio/Accelerometer_MMA7660 )
 */


#include <Wire.h>
#include "MMA7660.h"
#include "FastLED.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define PIN_LED1    5

#define LED_TYPE           WS2812B
#define COLOR_ORDER        GRB
#define NUM_LEDS           200
#define BRIGHTNESS         150
#define FRAMES_PER_SECOND  30


int LEDSPEED = 50;
#define SHAKYNESS  2



CRGB leds[NUM_LEDS];
MMA7660 accelemeter;


void setup()
{

  delay(3000); // 3 second delay for recovery

  accelemeter.init();
    
  FastLED.addLeds<LED_TYPE, 5, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, 6, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(9600);
}



void loop()
{
  //red, blue, green, speed of leds...the higher the number, the slower it is
  RunningLights(0xff,0x00,0xff, LEDSPEED); 
}

float getAccelerometer()
{
  float ax,ay,az;
  accelemeter.getAcceleration(&ax,&ay,&az);
  float asq = ax*ax + ay*ay + az*az;

  Serial.println(asq);
  return asq;
}

void showStrip() {
   FastLED.show();
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}

void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
  
  for(int i=0; i<NUM_LEDS*2; i++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<NUM_LEDS; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+Position) * 127 + 128;
        setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
      
      showStrip();

      float asq = getAccelerometer();
      
      if (asq < SHAKYNESS)
      {
        WaveDelay = 50;
      }
      if (asq > SHAKYNESS)
      {
        WaveDelay = 10;
      }
  
      delay(WaveDelay);
  }
}

