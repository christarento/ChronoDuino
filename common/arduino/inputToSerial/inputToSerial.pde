const int led_pin = 13;      // LED connected to digital pin 13
const int sensor_pin = 2;    // sensor connected to digital pin 2

int previous_sensor_state = LOW;
int current_sensor_state;
int incoming_byte;

void setup()
{
  pinMode(led_pin, OUTPUT);
  pinMode(sensor_pin, INPUT);
  
  Serial.begin(9600);
}

void loop()
{
  current_sensor_state = digitalRead(sensor_pin);
  if (current_sensor_state != previous_sensor_state)
  {
    previous_sensor_state = current_sensor_state;
    
    if (current_sensor_state == HIGH)
      Serial.print('H', BYTE);
    else//LOW
      Serial.print('L', BYTE);
    
    digitalWrite(led_pin, current_sensor_state);
  }
  
  if (Serial.available() > 0)
  {
    incoming_byte = Serial.read();
    if (incoming_byte == 'T')
      Serial.print('T', BYTE);
  }
}
