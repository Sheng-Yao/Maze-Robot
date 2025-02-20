#include <Arduino.h>
#include <EEPROM.h>


void memoryReset(){
    for(int i = 0; i < int(EEPROM.length()); i++){
        EEPROM.write(i, 0);
    }
}

void memoryWrite(String input){
    for(int i = 0; i < int(input.length()); i++){
        EEPROM.write(i, input[i]);
    }
}

String memoryRead(){
    String buffer = "";
    for(int i = 0; i < int(EEPROM.length()); i++){
        if(EEPROM.read(i) != 0){
            buffer += EEPROM.read(i);
        }else{
            break;
        }
    }
    return buffer;
}