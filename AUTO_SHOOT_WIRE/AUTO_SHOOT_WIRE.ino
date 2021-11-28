/*
  Auto Shoot Wire

  Reads a digital input on pin 7, executes  the shoot function to shoot the camera 
  and prints the result to the Serial Monitor

  FFCC female conector:
  Pin 1 -> Nada
  Pin 2 -> Runner
  Pin 3 -> VCC 12 V
  Pin 4 -> Data 3.3 V
  Pin 5 -> GND

  Camera Jack PINOUT:
 
  TIP: Green ->
  RING(middle): Red -> 
  SLEEVE(closer to cable): Black -> GND

  Author: Marcelo Jimenez 19/11

*/


// digital pin 7 has a FFCC attached to it.
int laserPin = 12;
int focusPin = 3;
int shootPin = 4;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the laserPin's pin an input:
  pinMode(laserPin, INPUT);

  //Led usage for visual detection
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(shootPin,OUTPUT);
  pinMode(focusPin, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  int laserState = digitalRead(laserPin);
  // print out the state of the button:
  Serial.println(laserState);
  delay(1);        // delay in between reads for stability
 
     if (laserState == LOW) {
       digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
       laserState == HIGH;
      shoot();
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    } 
}

/**
 * SHOOT: Disparo de la cámara, saca como 7 fotos
 */

void shoot(){
  
  digitalWrite(focusPin, HIGH); 
  delay(250);
  digitalWrite(shootPin, HIGH); 
  
  delay(2000);                       // wait for a second

  digitalWrite(focusPin, LOW);  
  digitalWrite(shootPin, LOW);  
  delay(1000);                       // wait for a second
 
}
