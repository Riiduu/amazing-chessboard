#include <Arduino.h>
#include "../lib/pinouts/pinouts.h"

void selectChannel(int ch) {
  digitalWrite(S0, ch & 0x01);
  digitalWrite(S1, (ch >> 1) & 0x01);
  digitalWrite(S2, (ch >> 2) & 0x01);
  digitalWrite(S3, (ch >> 3) & 0x01);
}

void printBoard()
{
	for (int i = 0; i < 8; i++)
    {
        int j = i + 8;
        //AB Columns
        
        selectChannel(i);
        delay(20);

        Serial.print("|");

        int val = digitalRead(muxOutAB);
        Serial.print(val == HIGH ? "X: " : "O: ");
        Serial.print(i);
        Serial.print("|");

        selectChannel(j);
        delay(20);

        val = digitalRead(muxOutAB);
        Serial.print(val == HIGH ? "X: " : "O: ");
        Serial.print(j);

        //CD Columns
        
        selectChannel(i);
        delay(20);

        Serial.print("|");

        val = digitalRead(muxOutCD);
        Serial.print(val == HIGH ? "X: " : "O: ");
        Serial.print(i);
        Serial.print("|");

        selectChannel(j);
        delay(20);

        val = digitalRead(muxOutCD);
        Serial.print(val == HIGH ? "X: " : "O: ");
        Serial.print(j);


        //EF Columns
        
        selectChannel(i);
        delay(20);

        Serial.print("|");

        val = digitalRead(muxOutEF);
        Serial.print(val == HIGH ? "X: " : "O: ");
        Serial.print(i);
        Serial.print("|");

        selectChannel(j);
        delay(20);

        val = digitalRead(muxOutEF);
        Serial.print(val == HIGH ? "X: " : "O: ");
        Serial.print(j);


        //GH Columns
        
        selectChannel(i);
        delay(20);

        Serial.print("|");

        val = digitalRead(muxOutGH);
        Serial.print(val == HIGH ? "X: " : "O: ");
        Serial.print(i);
        Serial.print("|");

        selectChannel(j);
        delay(20);

        val = digitalRead(muxOutGH);
        Serial.print(val == HIGH ? "X: " : "O: ");
        Serial.print(j);


        Serial.println("|");    
        j += 1;
    }
}