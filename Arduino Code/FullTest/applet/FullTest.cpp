/////////////////////////////
//VARS
/////////////////////////////

// pin settings
#include "WProgram.h"
void setup();
void loop();
void blinkLED(int pin, int timesToBlink, int speedOfBlink);
int ledPin = 13;         // the digial pin to control the LEDs to light up the stairs
int pirPin = 3;          // the digital pin connected to the PIR sensor's output
int pirPowerPin = 7;     // the digital pin to enable the PIR sensor when the light is low
int photosensorPin = 0;  // analog pin for photocell checking

// program constants
int calibrationTime = 30000;                    // the time (miliseonds) we give the sensor to calibrate (10-60 secs according to the datasheet)
long unsigned int TimeToWaitForDayDark = 5000;  // the amount of milliseconds of "darkness" to wait before switching on motion sensor
long unsigned int ledOnDelay = 15000;           // time to leave led lights on for illumination of stairs
int resistanceValueForDark = 94;                // value from photo cell at which point we should concur it's dark out

// misc vars
long unsigned int PhotoPause = TimeToWaitForDayDark;  // assign the time to wait for dark to this variable to count up or down
int i;                                                // generic for loop counter


/////////////////////////////
//SETUP
void setup(){
  // setup the sensor's trigger pin as input
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);
  
  // define sensor's power pin as output
  pinMode(pirPowerPin, OUTPUT);
  
  // define LEDs as output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop(){
  // if photo cell says it's dark out there....
  if(analogRead(photosensorPin) < resistanceValueForDark){
    // let check to see if we already have the motion detector on, if we do, check for motion
    if(digitalRead(pirPowerPin) == HIGH){
      // check for motion from sensor
      // reset the count up till we shut things down. This prevents repeated "light" values from adding up to incorrectly shut down the sensor.
      PhotoPause = 0; 
      // if motion detected
      if(digitalRead(pirPin) == HIGH){
         digitalWrite(ledPin, HIGH);   // turn on LEDs to stairs
         delay(ledOnDelay);            // leave LEDs on for the configured time
         digitalWrite(ledPin, LOW);    // turn LEDs off again.
       }
    }else{
      // decrease the count-down until time for enable of PIR sensor
      if(PhotoPause <= 0){
        
        // blink the LEDs once to let us know the system is starting up the sensor for motion dectection
        blinkLED(ledPin,1,500);
       
        // power-up the sensor
        digitalWrite(pirPowerPin,HIGH);
        
        // allow sensor to calibrate
        delay(calibrationTime);
        
        // blink the LEDs twice to let us know the sensor is active and calibrated 
        blinkLED(ledPin,2,500);
      }else{
       // decrease time till we turn on the sensor
       PhotoPause = PhotoPause - 1000;
       // delay 1 second so we can ~mimick 1 second loops. This gets us close but given the other delays in the program it's not exact.
       delay(1000);
      }
    }
  }else{
   // well, it must be light out there, let's determine if we need to shut down the sensor or just sit still till it gets dark.
   if(PhotoPause >= TimeToWaitForDayDark){
     // check to see if we already have the sensor off, it not shut it down now...
     if(digitalRead(pirPowerPin) == HIGH){
       // blink the LEDs three times to let us know that the motion sensor is shutting down
       blinkLED(ledPin, 3, 250);
       // shut off power to the sensor
       digitalWrite(pirPowerPin, LOW);
       // just in case we got to this point while the LEDs where on, let's make sure they are off as well
       digitalWrite(ledPin, LOW);
       // wait 250 miliseconds as we burned ~750 above
       delay(250);
     }else{
       // nothing to do, the sensor is off and it's light outside...
       // wait 1 second to mimick ~1 second loops
       delay(1000);
     }
   }else{
     // we need to increase the wait time till we shut the sensor off
     PhotoPause = PhotoPause + 1000;
     // wait 1 second to mimick ~1 second loops
     delay(1000);
   }
  } 
}

// function to blink led
// inputs: 3
// 1. Digital Pin to use for LED control: typeof INT
// 2. Number of times to blink: typeof INT
// 3. Speed (pause) of blink in milliseonds: typeof INT
void blinkLED(int pin, int timesToBlink, int speedOfBlink){
    for (i = 0; i < timesToBlink; i++){
      digitalWrite(pin, HIGH);
      delay(speedOfBlink);
      digitalWrite(pin, LOW);
      delay(speedOfBlink);
    }
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

