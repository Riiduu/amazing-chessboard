#include <Arduino.h>

#include <mcu-max.h>
#include <pinouts.h>
#include <chess-controller.h>

void setup() 
{
    Serial.begin(9600);
    setupPins();
    setupBoard();
    delay(100);
    printBoard();

}

void loop() {
    if (digitalRead(moveBtn) == LOW) {
        moveBtnAction();
        delay(100);
        printBoard();
    }
}