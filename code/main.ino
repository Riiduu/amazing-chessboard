#include <Arduino.h>

#include "lib/mcu-max/mcu-max.h"
#include "lib/pinouts/pinouts.h"
#include "lib/chess-controller/chess-controller.h"

void setup() 
{
	Serial.begin(9600);
    setupPins();
    printBoard();
    
}

void loop() {}