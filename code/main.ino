#include "lib/mcu-max/mcu-max.h"
#include "src/chess-controller.cpp"

void setup() 
{
    setupPins();
    Serial.begin(9600);
    printBoard();
}

void loop() {}