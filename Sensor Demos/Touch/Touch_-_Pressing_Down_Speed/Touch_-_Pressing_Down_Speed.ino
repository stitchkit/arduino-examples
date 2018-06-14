/* StitchKit.io Touch Sensor Example - Running Lights Speed based on Holding and Pressing Button
 * -----------------------
 * This software uses the touch sensor to turn speed up the effects in the Running lights LED example
 * Some of this code is modified from the Arduino website, on Hold Button, check it out here: 
 * https://playground.arduino.cc/Code/HoldButton
 * 
 * 
 * We are using the running lights animation, but it can be switched to any other FastLED animation from 
 * FastLED, Tweaking4All or the samples we have on our github. Try them out! :-)
 * 
 * We have enabled the effects on both Pin 5 and 6 outputs on the StitchKit board.
 *  
 * Scroll down and look for the "CHANGE CODE HERE" text to adjust the value or effect
 * 
 * Requires:
 * ---------
 *  StitchKit controller board (http://stitchkit.io)
 *  SeeedStudio Grove Button (http://wiki.seeedstudio.com/Grove-Touch_Sensor/)
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

//Location of the touch sensor pin on the StitchKit board (it will always be A6)
const int touchPin = A6;

// State of the touch sensor currently
int currentTouchState = 0;

// State of the touch sensor previously 
byte previousTouchState = LOW;

// How long the touch sensor was held (milliseconds)
long millis_held;  

// How long the touch sensor was held (seconds)
long secs_held;     

// How long the touch sensor was held previously (seconds)  
long prev_secs_held; 

// How long since the touch sensor was first pressed 
unsigned long firstTime; 

void setup()
{
  delay(2000); // 2 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,PIN_LEDARRAY1,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,PIN_LEDARRAY2,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // Set master brightness control, setting it to 0 so it only goes on when button is pressed
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);

  pinMode(touchPin, INPUT);
}


void loop() {

  RunningLights(0xff,0,0, 100);
  
}

boolean getTouchState()
{
  return digitalRead(touchPin);
}

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
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

       //-- CHANGE CODE HERE ---
  
      //This chunk of code is what you will typically paste into different LED pattern examples that
      //have some sort of change in color occurring
      //What is going on here is the sensor picks up whether the touch sensor is pressed, checks how long it is pressed and a value is adjusted
      //before showStrip() occurs, which shows the LED pattern
      
      // Get the current state of the button
      currentTouchState = getTouchState();
    
      // Get the time of the first button press (before it is released)
      if (currentTouchState == HIGH && previousTouchState == LOW && (millis() - firstTime) > 200) {
         firstTime = millis();
      }
    
      //Get the amount of time it is held
      millis_held = (millis() - firstTime);
      secs_held = millis_held / 1000;
  
  
      //Set a minimum of how long the button is held (to make sure its actually a button press)
      if (millis_held > 50) {
    
        //If the button is being pressed
        if (currentTouchState == HIGH && previousTouchState == HIGH) {

          //If the button is held for less than a second, set the speed to a slow amount
          //Note: the higher the amount, the slower it is
          if (secs_held <= 0) {
            WaveDelay = 100;
          }

          //If the button is held between 1-2 seconds, set the speed
          else if (secs_held >= 1 && secs_held < 3) {
            WaveDelay = 75;
          }

          //If the button is held between 3-6 seconds, set the speed          
          else if (secs_held >= 3 && secs_held < 7) {
            WaveDelay = 50;
          }
          
          //If the button is held for more than 7 seconds, set the speed          
          else if (secs_held >= 7) {
            WaveDelay = 20;
          }           
        }
      }
  
      //Assign the previous state of the button
      previousTouchState = currentTouchState;
    
      //Save how many seconds the button was held previously
      prev_secs_held = secs_held;
        
      delay(WaveDelay);

      //-- CHANGE CODE HERE ---
  }
}

