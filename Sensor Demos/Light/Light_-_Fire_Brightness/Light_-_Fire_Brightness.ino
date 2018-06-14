/* StitchKit.io Light Sensor Example - Fire Effect Brightening and Dimming based on Light Sensor
 * -----------------------
 * This software uses the light sensor to adjust the brightness of the fire animation 
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
 *  MakeFashion controller board
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
  //Continuously run the fire animation
  //and remember...we didn't start the fire...
  Fire(55,120,15);
  
}


//This function reads the brightness from the sensor
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

void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[NUM_LEDS];
  int cooldown;
 
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
   
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
 
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
   
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(j, heat[j] );
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

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}
