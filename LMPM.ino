/* usage
  1st press - start calibration
  2nd press - set lower temperature
  3rd press = set uppper temperature
  4th press = confirm values
*/

// libraries
//#include "PinNames.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// variables
// temperature
bool buttonPressed = false;
int upper, lower, threshold;
char *state[] = {"off", "on"};

// display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

// solenoid
int solenoidPin = 2;

void setup() {
  pinMode(solenoidPin,OUTPUT);  
  
  Serial.begin(9600);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.drawBitmap(0, 0, logo_bmp, 32, 32, WHITE);
  display.display();
  delay(10000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  display.setTextSize(2.5);             
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println("");
  display.println("  L.M.P.M");
  display.display();
  delay(2000);
}

void loop() {
  while(1){
    digitalWrite(solenoidPin,HIGH);
    delay(1000);
    digitalWrite(solenoidPin,LOW);
    delay(1000);
  }
  //STATE 0 - Calibration
  buttonPressed = true;   //mock  
  if (buttonPressed){
    calabration();
  }

  //STATE 1 - Operation
  buttonPressed = true;   //mock
  if (buttonPressed){
    operation();
  }
}

void calabration(){
  // get range
  int temp = tempRead(0);
  buttonPressed = false;   //mock
  for (int i=0; i<=1; i++) {
    while (!buttonPressed){
      temp = tempRead(i);
      Serial.print(state[i]);
      Serial.print(" temp: ");
      Serial.println(temp);
      delay(1000);
      buttonPressed = true;   //mock
    }
    if (i == 0){
      lower = tempRead(i);
      buttonPressed = false; //mock                        
    }
    else if (i == 1) {
      upper = tempRead(i);     
    }
  }

  // calculate threshold
  threshold = (upper + lower)*0.5;

  // confirm values
  Serial.print("off = ");
  Serial.println(lower);
  Serial.print("on = ");
  Serial.println(upper);
  Serial.print("threshold = ");
  Serial.println(threshold);
}

void operation() {
  while(1) {    
    for (int i=0; i<=1; i++){
      int temp = tempRead(i);
      showDisplay(temp, i);
      if (temp < threshold) {
        pressButton();        
      }
      delay(750);
      
    }
  }
}

int tempRead(int state){  
  if (state == 0){
    return 21;
  }

  else {
    return 37;
  }
}

void pressButton() {
  Serial.println("button pressed!");
  delay(5000);
}

void showDisplay(int reading, int i) {
  Serial.print("temp: ");
  Serial.print(reading);
  Serial.print("\tthreshold: ");
  Serial.print(threshold);
  Serial.print("\theater: ");
  Serial.println(state[i]);
}
