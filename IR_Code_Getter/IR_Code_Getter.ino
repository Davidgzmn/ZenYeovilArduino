#include <IRremote.h>
#include <FastLED.h>

const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;
//FastLed variables
#define NUM_LEDS 16
#define DATA_PIN 3
#define CLOCK_PIN 8
CRGB leds[NUM_LEDS];
bool oneLight = true;
int lightPatter = 0;
CRGB lightColor = CRGB(255,0,0);

void setup(){
  delay(500);
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);

  
}

void CheckIrInput(){
  if (irrecv.decode(&results)){
        Serial.println(results.value, HEX);
        if (results.value == 0XFFFFFFFF)
          results.value = key_value;

          switch(results.value){
            case 0xFF02FD:
            oneLight = !oneLight;
            break;  
            case 0xFF1AE5:
            lightColor = CRGB(0,0,255);
            break;  
            case 0xFF9A65:
            lightColor = CRGB(0,255,0);
            break;  
            case 0xFFA25D:
            lightColor = CRGB(255,0,0);
            break;      
          }
        key_value = results.value;
        irrecv.resume(); 
  }
}

void loop(){
  CheckIrInput();
  if(oneLight == true){
    for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      // Turn our current led on to white, then show the leds
      leds[whiteLed] = lightColor;

      // Show the leds (only one of which is set to white, from above)
      FastLED.show();

      // Wait a little bit
      delay(10);

      // Turn our current led back to black for the next loop around
      leds[whiteLed] = CRGB::Black;
   }
  }
  if(oneLight == false){
    for(int i = 0; i < NUM_LEDS+1; i++){
      leds[i] = CRGB::Black;
      FastLED.show();
    }
  }
}
