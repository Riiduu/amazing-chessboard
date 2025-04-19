#include <Arduino.h>
#include <pinouts.h>
#include <mcu-max.h>

mcumax_piece board_snapshot[64];
mcumax_piece new_snapshot[64];


// tracking the physical state of the reed switches
// 0 - filled; 1 - not filled
int reedSwitchStates[64];
int new_reedSwitchStates[64];


void selectChannel(int ch) {
  digitalWrite(S0, ch & 0x01);
  digitalWrite(S1, (ch >> 1) & 0x01);
  digitalWrite(S2, (ch >> 2) & 0x01);
  digitalWrite(S3, (ch >> 3) & 0x01);
}

void printBoard()
{
	// the new board print function
  if (reedSwitchStates != new_reedSwitchStates) {
    Serial.println("New Board State:");
    for (int i = 0; i < 64; i++) {
      Serial.print("|");
      Serial.print(new_reedSwitchStates[i]);
  
      // Print a new line after every 8 elements
      if ((i + 1) % 8 == 0) {
          Serial.println("|");
      }
    }
  } else {
    for (int i = 0; i < 64; i++) {
      Serial.print("|");
      Serial.print(reedSwitchStates[i]);
  
      // Print a new line after every 8 elements
      if ((i + 1) % 8 == 0) {
          Serial.println("|");
      }
    }
  }
  
}

bool readSwitch(int muxIndex, int channel) {
  selectChannel(channel); // S0–S3
  delayMicroseconds(10);  // Small delay to settle

  switch (muxIndex) {
    case 0: return digitalRead(muxOutAB) == HIGH;
    case 1: return digitalRead(muxOutCD) == HIGH;
    case 2: return digitalRead(muxOutEF) == HIGH;
    case 3: return digitalRead(muxOutGH) == HIGH;
    default: return false;
  }
}

void updateBoardSnapshot(int switchArray[]) {
  for (int i = 0; i < 64; i++) {
    int mux = i / 16;         // Which mux (0–3)
    int channel = i % 16;     // Which channel (0–15)

    bool occupied = readSwitch(mux, channel);
    switchArray[i] = occupied ? 1 : 0;
  }
}

void setupBoard()
{
  // Set the initial position using FEN
  mcumax_set_fen_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  updateBoardSnapshot(reedSwitchStates);
  updateBoardSnapshot(new_reedSwitchStates);
}

void saveBoard()
{
  updateBoardSnapshot(new_reedSwitchStates);
}

void compareBoard()
{
  for (int i = 0; i < 64; i++) {
    if (reedSwitchStates[i] != new_reedSwitchStates[i]) {
        printf("Change at square %d: %d -> %d\n", i, reedSwitchStates[i], new_reedSwitchStates[i]);
    }
  }
}

void moveBtnAction()
{
  saveBoard();
  delay(100);
  compareBoard();
}