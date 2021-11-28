// Nikon Remote Emulator by Gough Lui (https://goughlui.com)
// A1 = IR +
// A0 = IR -

int count=0;
int count2=0;

void setup() { 
   pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0,OUTPUT);
  digitalWrite(A0,LOW);
  pinMode(A1,OUTPUT);
  digitalWrite(A1,LOW);
  Serial.begin(9600);
  
}

void loop() {

   while(count2<3) {
  shoot();    
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(4000);                       // wait for a second
  count2++;
  Serial.println("Shoot");
  }
}

void shoot(){
  // Send code three times, as the clone remote does
  // Delays are tuned to account for overhead of library code.
  while(count<3) {
    tone(A1,38000);
    delay(2);
    noTone(A1);
    delay(28);
    tone(A1,38000);
    delayMicroseconds(200);
    noTone(A1);
    delayMicroseconds(1500);
    tone(A1,38000);
    delayMicroseconds(200);
    noTone(A1);
    delayMicroseconds(3300);
    tone(A1,38000);
    delayMicroseconds(200);
    noTone(A1);
    delayMicroseconds(100);    
    delay(63);    
    count++;  
  } 
}
