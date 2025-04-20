#include <Arduino.h>
#include <pinouts.h>
#include <mcu-max.h>


// trackingg the state of the game in FEN writing
char boardStateFEN[] = "rnbqkb1r/pppppppp/88888888/888888888/888888888/888888888/PPPPPPPP/RNBQKB1R w KQkq - 0 1";

int moveCount = 0;

// tracking the physical state of the reed switches
// 0 - filled; 1 - not filled
int reedSwitchStates[64];
int new_reedSwitchStates[64];

// Used by the multiplexers to select the channels
void selectChannel(int ch) {
  digitalWrite(S0, ch & 0x01);
  digitalWrite(S1, (ch >> 1) & 0x01);
  digitalWrite(S2, (ch >> 2) & 0x01);
  digitalWrite(S3, (ch >> 3) & 0x01);
}

// Prints the board. If It has changed, it will print the new state
// If it has not changed, it will print the old state
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
    for (int rank = 0; rank < 8; rank++) {
      Serial.print("|");
      for (int i = 0; i < 64; i++) {
        Serial.print("|");
        Serial.print(reedSwitchStates[i]);
    
        // Print a new line after every 8 elements
        if ((i + 1) % 8 == 0) {
            Serial.println("|");
        }
      }
      Serial.println();
    }
  }
  

  Serial.print("mcumax_get_current_side(): ");
  Serial.println(mcumax_get_current_side());
}

// Reads the state of the reed switches
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

// Updates the board snapshot
void updateBoardSnapshot(int switchArray[]) {
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      int flippedFile = 7 - file; // horizontal mirror

      int boardIndex = rank * 8 + file;
      int physicalIndex = rank * 8 + flippedFile;

      int mux = physicalIndex / 16;
      int channel = physicalIndex % 16;

      bool occupied = readSwitch(mux, channel);
      switchArray[boardIndex] = occupied ? 1 : 0;
    }
  }
}

// Sets up the board layout
// The initial position is set using FEN (Forsyth-Edwards Notation)
// This function initializes the board with the standard starting position
void setupBoard()
{
  mcumax_init();
  mcumax_set_fen_position(boardStateFEN);

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
        // printf("Change at square %d: %d -> %d\n", i, reedSwitchStates[i], new_reedSwitchStates[i]);

        Serial.print("Change at square ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(reedSwitchStates[i]);
        Serial.print(" -> ");
        Serial.println(new_reedSwitchStates[i]);
    }
  }

  for (int i = 0; i < 64; i++) {
    reedSwitchStates[i] = new_reedSwitchStates[i];
  }
}

void moveBtnAction()
{
  saveBoard();
  delay(100);
  compareBoard();

  moveCount++;
}