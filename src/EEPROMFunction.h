#include <Arduino.h>
#include <EEPROM.h>


void memoryReset(){
    for(int i = 0; i < 255; i++){
        if(EEPROM.read(i) == ' '){
            continue;
        }else{
            EEPROM.write(i, ' ');
        }
    }
}

void memoryWrite(String input){
    for(int i = 0; i < input.length(); i++){
        EEPROM.write(i, input[i]);
    }
}

String memoryWrite(){
    String buffer = "";
    for(int i = 0; i < 255; i++){
        if(EEPROM.read(i) != ' '){
            buffer += EEPROM.read(i);
        }else{
            break;
        }
    }
    return buffer;
}