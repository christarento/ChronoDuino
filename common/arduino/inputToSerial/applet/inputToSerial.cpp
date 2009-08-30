#include "WProgram.h"
void setup();
void loop();
const int ledPin = 13;
const int sensorPin = 2;

int sensorState = 0;

void setup()
{
  Serial.begin(9600);
  
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
}

void loop()
{
  sensorState = digitalRead(sensorPin);
  if (sensorState == HIGH)
  {
    Serial.print(1, BYTE);
  }
  else
  {
    Serial.print(0, BYTE);
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

