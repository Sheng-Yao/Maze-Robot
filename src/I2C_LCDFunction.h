#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setupLCDModule(){
    lcd.init();                      // initialize the lcd 
    lcd.backlight();
}

void clearLCDModule(){
    lcd.clear();
}

void printLCDModule(String input, byte cursorX, byte cursorY){
    lcd.setCursor(cursorX,cursorY);
    lcd.print(input);
}