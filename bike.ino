#include <Keyboard.h>

int sensorPin2 = A1;    // select the input pin for the potentiometer

int sensorValue1 = 0;  // variable to store the value coming from the sensor
int sensorValue2 = 0;  // variable to store the value coming from the sensor

volatile  int press1 = 0;
int press2 = 0;

String turn;

void setup() {
  pinMode(4, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  Serial.begin(9600);

  pinMode(2, INPUT_PULLUP);
  EIFR = bit(INTF1); // clear INT0 interrupt flag
  attachInterrupt(1, magnet_detect, RISING);
  Keyboard.begin();
}

void loop() {
  // read the value from the sensor:
  sensorValue2 = analogRead(sensorPin2);

  //Serial.print(" S2 ");
  //Serial.println(sensorValue2);


  //----------------- forward -------------------

  if (press1 > 0) {
    press1--;
  } else {
    Keyboard.release(KEY_UP_ARROW);
  }

  //----------------------- left - right --------------
  if (sensorValue2 > 600 ) {
    Keyboard.press(KEY_RIGHT_ARROW);
    Keyboard.release(KEY_LEFT_ARROW);
    press2 = (sensorValue2 - 550) / 1;
    delay (20 + press2);
    Keyboard.release(KEY_RIGHT_ARROW);
    delay (abs(180 - press2));
  }
  if (sensorValue2 < 400 ) {
    Keyboard.press(KEY_LEFT_ARROW);
    Keyboard.release(KEY_RIGHT_ARROW);
    press2 = (450 - sensorValue2) / 1;
    delay (20 + press2);
    Keyboard.release(KEY_LEFT_ARROW);
    delay (abs(180 - press2));
  }
  if (sensorValue2 >= 400 and sensorValue2 <= 600) {
    Keyboard.release(KEY_LEFT_ARROW);
    Keyboard.release(KEY_RIGHT_ARROW);
    delay (20);
  }

  // ---------------------------- rescue --------------------
  if (digitalRead(4) == LOW) {
    Keyboard.releaseAll();
    delay (10);
    Keyboard.press(KEY_BACKSPACE);
    delay (100);
    Keyboard.releaseAll();
    delay (10);
  }

  // ---------------------------- fire --------------------
  if (digitalRead(6) == LOW) {
    Keyboard.press(KEY_LEFT_CTRL);
    delay (100);
    Keyboard.release(KEY_LEFT_CTRL);
    delay (10);
  }

  delay (20);
  Serial.println(press1);
}

void magnet_detect()//This function is called whenever a magnet/interrupt is detected by the arduino
{
  Keyboard.press(KEY_UP_ARROW);
  press1 = 10;
}
