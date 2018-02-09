#include "WProgram.h"
void setup();
void loop();
int photosensorPin = 0;
int ledPin = 13;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(photosensorPin));
  if(analogRead(photosensorPin) < 94){
      digitalWrite(ledPin,HIGH);
      Serial.println("DARK");
      delay(10);
      
    }else{
      Serial.println("LIGHT");
      digitalWrite(ledPin,LOW);
      delay(10);
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

