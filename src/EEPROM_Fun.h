#include <Arduino.h>
#include <EEPROM.h>

// Reset memory in EEPROM (Reset the value to '0')
void memoryReset(){
    for(int i = 0; i < int(EEPROM.length()); i++){
        EEPROM.write(i, 0);
    }
}

// Write string into EEPROM
void memoryWrite(String input){
    for(int i = 0; i < int(input.length()); i++){
        EEPROM.write(i, input[i]);
    }
}

// Read memory in EEPROM until the end indicator '0'
String memoryRead(){
    // Temperary message holder
    String buffer = "";
    for(int i = 0; i < int(EEPROM.length()); i++){
        char character = char(EEPROM.read(i));
        if(character != 0){
            buffer += character;
        }else{
            break;
        }
    }
    return buffer;
}