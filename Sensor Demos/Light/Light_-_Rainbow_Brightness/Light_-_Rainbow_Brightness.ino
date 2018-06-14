/* StitchKit.io Light Sensor Example - Rainbow Brightening and Dimming based on Light Sensor
 * -----------------------
 * This software uses the light sensor to adjust the brightness of the rainbow animation 
 * 
 * 
 * We are using the rainbow animation, but it can be switched to any other FastLED animation from 
 * FastLED, Tweaking4All or the samples we have on our github. Try them out! :-)
 * 
 *  We have enabled the effects on both Pin 5 and 6 outputs on the StitchKit board.
 *  
 *  Scroll down and look for the "CHANGE CODE HERE" text to adjust brightness values to your environment
 * 
 * Requires:
 * ---------
 *  StitchKit controller board (http://stitchkit.io)
 *  SeeedStudio Light Sensor (http://wiki.seeedstudio.com/Grove-Light_Sensor/)
 *  FastLED-compatible LEDs ( https://github.com/FastLED/FastLED/wiki/Overview#chipsets )
 *  
 * Dependencies:
 * -------------
 *  FastLED ( Available via Arduino package manager )
 */


#include <Wire.h>
#include "FastLED.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif


//StitchKit LED output 1
#define PIN_LEDARRAY1    5

//StitchKit LED output 2
#define PIN_LEDARRAY2    6

#define LED_TYPE           WS2812B
#define COLOR_ORDER        GRB

//Change the number of LEDs here according to how many you have
#define NUM_LEDS          100

CRGB leds[NUM_LEDS];
#define FRAMES_PER_SECOND  120


//Change the brightness of the LEDs here according to how many you have
int brightness = 10;

void setup()
{
  delay(2000); // 2 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,PIN_LEDARRAY1,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,PIN_LEDARRAY2,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(10);

  Serial.begin(9600);
}



void loop()
{ 
  //Continuously run the rainbow animation
  rainbowCycle(20);
}


//This function reads the brightness from the sensor
//and returns it as a percentage value from the sensor
int getLightBrightness()
{
  int value = analogRead(A6);
  value = map(value, 0, 1023, 0, 100);   
  return value;
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

void rainbowCycle(int SpeedDelay) {

  byte *c;
  uint16_t i, j;
  
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }


  //-- CHANGE CODE HERE ---
  
  //This chunk of code is what you will typically paste into different LED pattern examples that
  //have some sort of change in color occurring
  //What is going on here is the sensor picks up the data, it is checked and then a value is adjusted
  //before showStrip() occurs, which shows the LED pattern
  
  //Get the brightness from the sensor
  int sensorBrightness = getLightBrightness();

  Serial.println(sensorBrightness);

  //The values below can be adjusted according to your own lighting conditions)

  //If there is a light shined on the sensor, set maximum brightness of the LEDs
  if (sensorBrightness > 75)
    FastLED.setBrightness(255);

  //If the sensor is covered, lowering the brightness of the LEDs to nearly 0
  else if (sensorBrightness < 20)
    FastLED.setBrightness(5);

  //Otherwise, set brightness of LEDs to 50 (basically a normal lighting condition)
  else
    FastLED.setBrightness(50);

    showStrip();
    delay(SpeedDelay);


  //-- CHANGE CODE HERE ---
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];

  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {

   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {

   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }
  return c;
}
