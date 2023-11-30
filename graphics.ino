#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 lcd(128, 64); // create display object

void setup() {
 lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C); // init
 lcd.clearDisplay();         // clear software buffer
 lcd.setTextColor(WHITE);    // set text color
 lcd.setCursor(0,32); // set cursor to pixel (0,0)
 lcd.print(0); // print text
 lcd.setCursor(120,32); // set cursor to pixel (0,0)
 lcd.print(0);
 lcd.display();       // push software buffer to oled 
}

void loop() {
 lcd.drawLine(18, 0, 18, 64, WHITE);
 lcd.drawLine(110, 0, 110, 64, WHITE);

 for (int i = 0; i < 64; i += 4) {
    lcd.drawFastVLine(64, i, 1, WHITE);
 }
  
 lcd.display();
 delay(100); 
}
