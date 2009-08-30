const int led_pin = 13;         // LED connected to digital pin 13
const long interval = 1000;     // interval at which to blink (milliseconds)

int value = LOW;                // previous value of the LED
long previous_millis = 0;       // will store last time LED was updated
int incoming_byte;              // wille store received data

void setup()
{
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  if (millis() - previous_millis > interval)
  {
    previous_millis = millis();   // remember the last time we blinked the LED

    // if the LED is off turn it on and vice-versa.
    if (value == LOW)
    {
      value = HIGH;
      Serial.print('H', BYTE);
    }
    else//value is HIGH
    {
      value = LOW;
      Serial.print('L', BYTE);
    }

    digitalWrite(led_pin, value);
  }
  
  if (Serial.available() > 0)
  {
    incoming_byte = Serial.read();
    if (incoming_byte == 'T')
      Serial.print('T', BYTE);
  }
}
