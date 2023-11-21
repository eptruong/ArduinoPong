#include <ESP32Lib.h>
#include <Ressources/Font6x8.h>

//pin configuration
const int redPin = 4;
const int greenPin = 2;
const int bluePin = 15;
const int hsyncPin = 14;
const int vsyncPin = 12;
   
//VGA Device
VGA3Bit vga;

void player_one() {
  
}


void player_two() {
  
}

void player_three() {
  
}

void player_four() {
  
}
void setup()
{
  //initializing vga at the specified pins
  vga.init(vga.MODE320x240, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
  //selecting the font
  vga.setFont(Font6x8);
  //displaying the text
  vga.println("Hello World! Testing Testing Testing Testing Testing Testing Testing Testing Testing Testing Testing Testing Testing Testing Testing Testing Testing ");
}
int a = 0;
void loop()
{
  vga.clear(vga.RGB(0));
  //text position
  delay(10);
  vga.setCursor(100, 100);
  vga.println(a++);
}
