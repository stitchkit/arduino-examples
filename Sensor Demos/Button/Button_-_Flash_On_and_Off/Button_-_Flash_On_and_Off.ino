/* StitchKit.io Button Example - Rainbow On and Off based on pressing Button
 * -----------------------
 * This software uses the button sensor to turn on and off the rainbow animation 
 * 
 * 
 * We are using the rainbow animation, but it can be switched to any other FastLED animation from 
 * FastLED, Tweaking4All or the samples we have on our github. Try them out! :-)
 * 
 *  We have enabled the effects on both Pin 5 and 6 outputs on the StitchKit board.
 *  
 *  Scroll down and look for the "CHANGE CODE HERE" text to adjust the value or effect
 * 
 * Requires:
 * ---------
 *  StitchKit controller board (http://stitchkit.io)
 *  SeeedStudio Grove Button (http://wiki.seeedstudio.com/Grove-Button/)
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

//Change the brightness of LEDs here
#define BRIGHTNESS          150

CRGB leds[NUM_LEDS];
#define FRAMES_PER_SECOND  120

//Variable for the pin in which the button is located on
const int buttonPin = A6;     

// Variable for reading whether the button has been pressed or not
int buttonState = 0;

void setup()
{
  delay(2000); // 2 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,PIN_LEDARRAY1,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,PIN_LEDARRAY2,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // Set master brightness control, setting it to 0 so it only goes on when button is pressed
  FastLED.setBrightness(0);

  Serial.begin(9600);

  //This sets the button pin on the StitchKit board to be input
  pinMode(buttonPin, INPUT);
  
}

void loop()
{ 
  //Continuously run the rainbow animation
  rainbowCycle(20);
}

boolean checkButtonState()
{
  return digitalRead(buttonPin);
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
  //What is going on here is the sensor picks up whether the button is pressed, it is checked and then a value is adjusted
  //before showStrip() occurs, which shows the LED pattern
  
  //Get whether the button is pressed or not from the sensor
    buttonState = checkButtonState();

  //If the button is pressed, set maximum brightness of the LEDs
  if (buttonState == HIGH)
    FastLED.setBrightness(150);

  //Otherwise, turn off the lights from LED
  else
    FastLED.setBrightness(0);

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



