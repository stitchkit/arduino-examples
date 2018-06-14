/* StitchKit.io Rainbow Effect to Pulse Effect based on Proximity
 * -----------------------
 * This software switches a rainbow effect to a pulse effect based on the grove proximity sensor.
 * 
 * We are using the rainbow and pulse animations, but it can be switched to any other FastLED animation from 
 * FastLED, Tweaking4All or the samples we have on our github. Try them out! :-)
 * 
 * We have enabled the effects on both Pin 5 and 6 outputs on the StitchKit board.
 *  
 * Scroll down and look for the "CHANGE CODE HERE" text to adjust the value or effect
 *
 * Requires:
 * ---------
 *  MakeFashion controller board
 *  SeeedStudio Ultrasonic Ranger ( http://wiki.seeed.cc/Grove-Ultrasonic_Ranger/ )
 *  FastLED-compatible LEDs ( https://github.com/FastLED/FastLED/wiki/Overview#chipsets )
 *  
 * Dependencies:
 * -------------
 *  FastLED ( Available via Arduino package manager )
 *  SeeedStudio Ultrasonic library ( https://github.com/Seeed-Studio/Grove_Ultrasonic_Ranger/archive/master.zip )
 */

#include <Ultrasonic.h>
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

//Change the brightness of LEDs here
#define BRIGHTNESS          150

CRGB leds[NUM_LEDS];
#define FRAMES_PER_SECOND  120

//Variable for the pin on which the proximity sensor is located on
#define PIN_ULTRASONIC   0

//Distance for detecting the proximity sensor value (in centimeters)
#define DISTANCE 30 

Ultrasonic ultrasonic(PIN_ULTRASONIC);
long ultrasonic_range;

CRGB leds[NUM_LEDS];

//For rainbow pattern
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void setup() {
  
  delay(2000); // 2 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,PIN_LEDARRAY1,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,PIN_LEDARRAY2,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // Set master brightness control, setting it to 0 so it only goes on when button is pressed
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(9600);

}

void loop()
{
  
    ultrasonic_range = ultrasonic.MeasureInCentimeters();
    Serial.println(ultrasonic_range);

    //If far (the range), show rainbow
    if (ultrasonic_range > DISTANCE)
    {
      rainbow();
    }
    //Otherwise pulse red
    else
    {
      FadeInOut(0xff, 0x00, 0x00); // red
    }
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

//Pulse animation
void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;
      
  for(int k = 0; k < 256; k=k+1) { 
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
}

//Tweaked FastLED Rainbow
void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
  showStrip();
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
}

