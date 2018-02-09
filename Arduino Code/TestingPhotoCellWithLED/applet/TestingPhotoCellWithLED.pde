int photosensorPin = 0;
int ledPin = 13;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if(analogRead(photosensorPin) < 94){
      digitalWrite(ledPin,HIGH);
      delay(10);
    }else{
      digitalWrite(ledPin,LOW);
      delay(10);
    }
}
