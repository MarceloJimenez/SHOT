/* 
 *  SHOT V1
 *  Menu de configuración de parámetros para disparo automático de cámara
 *  
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

 *  Marcelo Jiménez 28-11-21
 */


// Include Libraries
#include "Arduino.h"
//#include "Keypad.h" 
#include "LiquidCrystal_PCF8574.h"
#include "Button.h"
#include "Relay.h"
#include <avr/wdt.h>

// Librerías incluidas para el adaptador I2C del Keyboard
/* Cuidado con tener 2 librerías idénticas en el Path, 
 *  arduino busca los include en el directorio del sketch y
 *  en la carpeta libraries. Sólo debe encontrar 1 de cada 1 *  
 */
#include <Keypad_I2C.h>
#include <Keypad.h>        // GDY120705
#include <Wire.h>



#define I2CADDR 0x20 // Dirección I2C del KeyBoard 
// Global variables and defines
//Use this 2D array to map the keys as you desire
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keypadmem4x4keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad_I2C keypadmem4x4( makeKeymap(keypadmem4x4keys), rowPins, colPins, ROWS, COLS, I2CADDR); 


// There are several different versions of the LCD I2C adapter, each might have a different address.
// Try the given addresses by Un/commenting the following rows until LCD works follow the serial monitor prints. 
// To find your LCD address go to: http://playground.arduino.cc/Main/I2cScanner and run example.
#define LCD_ADDRESS 0x27
// Define LCD characteristics
#define LCD_ROWS 2
#define LCD_COLUMNS 16
#define SCROLL_DELAY 150
#define BACKLIGHT 255

LiquidCrystal_PCF8574 lcdI2C;


// define vars for testing menu
int tiempo1;
int tiempo2;
int tiempoTotal;
char tiempoIngresado1;
char tiempoIngresado2;

int laserPin = 12;
int focusPin = 3;
int shootPin = 4;
bool first_time = true;
int pressed = 2;


// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
   wdt_disable(); // Disable WatchDogTimer
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    Wire.begin( );
    //Initialize the keypad with selected key map
    keypadmem4x4.begin();
    // initialize the lcd
    lcdI2C.begin(LCD_COLUMNS, LCD_ROWS, LCD_ADDRESS, BACKLIGHT); 

  // make the laserPin's pin an input:
  pinMode(laserPin, INPUT);

  //Led usage for visual detection
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(shootPin,OUTPUT);
  pinMode(focusPin, OUTPUT);
       
    wdt_enable(WDTO_4S);
  
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() {
   if (first_time){
    int pressed = menu();
    first_time = false;
    }
  
  // read the input pin:
  int laserState = digitalRead(laserPin);
  // print out the state of the button:
  Serial.println(laserState);
  delay(1);        // delay in between reads for stability
 
     if (laserState == LOW) {
       digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
       laserState == HIGH;
       wdt_disable();
      shoot(pressed);
      wdt_enable(WDTO_4S);
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    } 
}



/*Shoot(segundos_presionados)
Dispara la cámara y mantiene el disparador durante la cantidad de segundos especificada
Falta aun hacer un cálculo de cuantos segundos = cuantas fotos


*/
void shoot(int seconds){
      lcdI2C.clear();
      lcdI2C.print("Auto Disparo OK");
      lcdI2C.selectLine(2);
      lcdI2C.print("Apagando LCD");
      delay(2000);
      lcdI2C.noDisplay();
      lcdI2C.setBacklight(0);
  
  long s_to_millis = seconds*1000;
  digitalWrite(focusPin, HIGH); 
  delay(250);
  digitalWrite(shootPin, HIGH); 
  
  delay(s_to_millis);                       // wait for a second

  digitalWrite(focusPin, LOW);  
  digitalWrite(shootPin, LOW);  
  delay(250);                       // wait for a second/4
 
}

int menu(){  
 lcdI2C.clear();
      lcdI2C.print("Ingrese");
      lcdI2C.selectLine(2);
      lcdI2C.print("N");
      lcdI2C.print((char)223);           // (char)223 es °
      lcdI2C.print("de segundos: ");           
  
      tiempoIngresado1 = ' ';
      while (!isdigit(tiempoIngresado1) && tiempoIngresado1 != '#')
      { 
        wdt_disable();
        tiempoIngresado1 = keypadmem4x4.waitForKey();
        wdt_enable(WDTO_4S);
        switch ( tiempoIngresado1 )   // Segundo switchcase, un switchcase donde el default sea poner números, si se pone otra opción se especifica qué pasa
        {
          case '#':  break;
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':{                  
    
            lcdI2C.clear();
            lcdI2C.print(tiempoIngresado1);
            lcdI2C.print(" segundos");
            lcdI2C.selectLine(2);
            lcdI2C.print("* para iniciar");
            tiempo1 = tiempoIngresado1 - 48;
            tiempoTotal = tiempo1;
            delay(200);
    
            
            tiempoIngresado2 = ' ';
            while (!isdigit(tiempoIngresado2) && tiempoIngresado2 != '#' && tiempoIngresado2 != '*')
            {
              wdt_disable();
              tiempoIngresado2 = keypadmem4x4.waitForKey();
              wdt_enable(WDTO_4S);
              switch ( tiempoIngresado2 ) //tercer swichcase, para poner el segundo dígito
              {
                case '*': return tiempoTotal; break; // se desinfecta con los minutos del primer dígito ingresado
                case '#':  break; //se cancela
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':{
  
                
                  tiempo2 = tiempoIngresado2 - 48;
                  tiempoTotal = tiempo1*10 + tiempo2;
                  lcdI2C.clear();
                  lcdI2C.print(String(tiempoTotal)+String(" segundos"));   
                  lcdI2C.selectLine(2);
                  lcdI2C.print("* para iniciar");
      
                  char key2 = ' ';
                  while (key2 != '*' && key2 != '#')              //si se apreta algo que no sea * o #, se vuelve a pedir un input
                  {
                    wdt_disable();
                    key2 = keypadmem4x4.waitForKey();
                    wdt_enable(WDTO_4S);
                    switch ( key2 )                              //cuarto switchcase, nunca son suficientes switchcases
                    {
                      case '*': return tiempoTotal; break;
                      case '#': break; // se cancela el proceso
                      default : break;
                    }
                  }
                break;
                } //break de los dígitos del tercer swichcase
                //default: break;
                
              }
            }
            break;
            }
            
           // default: break; // break del default, es decir de cuando se ingresa * o alguna letra    
        }
      }
      
      
   wdt_reset();
  
  }
