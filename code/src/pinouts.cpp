#include "../lib/pinouts/pinouts.h"
#include <Arduino.h>

const int S0 = 2;
const int S1 = 3;
const int S2 = 4;
const int S3 = 5;
const int muxOutAB = A0;
const int muxOutCD = A1;
const int muxOutEF = A2;
const int muxOutGH = A3;
const int moveBtn = 7;

void setupPins()
{
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(muxOutAB, INPUT);
    pinMode(muxOutCD, INPUT);
    pinMode(muxOutEF, INPUT);
    pinMode(muxOutGH, INPUT);

    pinMode(moveBtn, INPUT_PULLUP);
}