/* StitchKit.io Touch Sensor Example - Meteor Brightness based on touch 
 * -----------------------
 * This software uses the touch sensor to set the brightness of the meteor animation 
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
 *  SeeedStudio Touch Sensor (http://wiki.seeedstudio.com/Grove-Touch_Sensor/)
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
const int touchPin = A6;     

// Variable for reading whether the touch sensor has been pressed or not
int touchState = 0;

void setup()
{
  delay(2000); // 2 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,PIN_LEDARRAY1,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,PIN_LEDARRAY2,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // Set master brightness control, setting it to 0 so it only goes on when button is pressed
  FastLED.setBrightness(50);

  Serial.begin(9600);

  //This sets the touch sensor pin on the StitchKit board to be input
  pinMode(touchPin, INPUT);
  
}

void loop()
{ 
  //Continuously run the rainbow animation
  meteorRain(0xff,0xff,0xff,10, 64, true, 50);
}

boolean getTouchState()
{
  return digitalRead(touchPin);
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

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
  
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
    
    
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
    
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        setPixel(i-j, red, green, blue);
      } 
    }
   
    //-- CHANGE CODE HERE ---
  
    //This chunk of code is what you will typically paste into different LED pattern examples that
    //have some sort of change in color occurring
    //What is going on here is the sensor picks up whether the touch sensor has been touched, it is checked and then a value is adjusted
    //before showStrip() occurs, which shows the LED pattern
  
    //Get whether the button is pressed or not from the sensor
    touchState = getTouchState();

    //If the button is pressed, set maximum brightness of the LEDs
    if (touchState == HIGH)
      FastLED.setBrightness(255);
  
    //Otherwise, turn off the lights from LED
    else
      FastLED.setBrightness(50);
  
    showStrip();
    delay(SpeedDelay);

    //-- CHANGE CODE HERE ---
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
    
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
    
    strip.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}

