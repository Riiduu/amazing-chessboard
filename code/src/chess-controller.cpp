#include <Arduino.h>
#include <pinouts.h>
#include <mcu-max.h>

int moveCount = 0;

char fenState[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// tracking the physical state of the reed switches
// 0 - filled; 1 - not filled
int reedSwitchStates[64];
int new_reedSwitchStates[64];

// tracks the changed switch states. Updates in the compareboard function after each move
int startMove;
int endMove;


// Used by the multiplexers to select the channels
void selectChannel(int ch) {
  digitalWrite(S0, ch & 0x01);
  digitalWrite(S1, (ch >> 1) & 0x01);
  digitalWrite(S2, (ch >> 2) & 0x01);
  digitalWrite(S3, (ch >> 3) & 0x01);
}


void generateFENFromState(int* state) {
  String fen = "";

  // for (int rank = 0; rank < 8; rank++) {
  //   int emptyCount = 0;

  //   for (int file = 0; file < 8; file++) {
  //     int index = rank * 8 + file;
  //     int square = state[index];

  //     if (square == 1) {
  //       emptyCount++;
  //     } else {
  //       if (emptyCount > 0) {
  //         fen += String(emptyCount);
  //         emptyCount = 0;
  //       }
  //       fen += "P"; // Placeholder for a piece
  //     }
  //   }

  //   if (emptyCount > 0) {
  //     fen += String(emptyCount);
  //   }

  //   if (rank < 7) {
  //     fen += "/";
  //   }
  // }

  // take the old fen
  int count = 0;
  char startMovePiece;

  // finding the starting piece
  for (size_t i = 0; i < sizeof(fenState); i++) {
    // range of numbers (val 1-8)
    // before you move on make sure to check if you haven't jumped over. The necessary position.
    for (int j = 0; j < 8; j++) {
      if (fenState[i] == char(j)) {
        if (count + j < startMove) {
          count += j;
        } else if (count + j > startMove) {
          for (int k = count + j; j >= startMove; k--) {
            Serial.print("Reducing from: ");
            Serial.println(k);
            if (k == startMove) {
              Serial.print("Found: ");
              Serial.println(k);
              startMovePiece = fenState[i];
            }
          }
        }
      }
    }

    // piece id (val 1)
    char piece = toLowerCase(fenState[i]);
    
    if ((piece == 'p' || piece == 'r' || piece == 'n' || 
        piece == 'b' || piece == 'q' || piece == 'k') && count != startMove) {
        count++;
    } else if ((piece == 'p' || piece == 'r' || piece == 'n' || 
                piece == 'b' || piece == 'q' || piece == 'k') && count == startMove) {
        startMovePiece += fenState[i];  // Keep original case
    }
    // "/" ( skip to the next; i++)



    if (count == 63) {
      startMovePiece = fenState[i];
    }



    
  }

  Serial.print("StartMovePiece Var: ");
  Serial.println(startMovePiece);

  Serial.print("startMove Var: ");
  Serial.println(startMove);

  // find the starting move pos. 
  // Save the piece id.
  // replace with an empty place
  // find the ending move pos
  // adjust the /..../ accordingly

  fen.toCharArray(fenState, sizeof(fenState));
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
  Serial.println("FEN piece placement:");
  Serial.println(fenState);
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
  // mcumax_set_fen_position(boardStateFEN);
  mcumax_set_fen_position(fenState);
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
        if (reedSwitchStates[i] == 0 && new_reedSwitchStates[i] == 1)
        {
          startMove = i;
        } else if (reedSwitchStates[i] == 1 && new_reedSwitchStates[i] == 0)
        {
          endMove = i;
        }
    }
  }

  for (int i = 0; i < 64; i++) {
    reedSwitchStates[i] = new_reedSwitchStates[i];
  }

  Serial.print(startMove);
  Serial.print(": -> ");
  Serial.println(endMove);
}

void moveBtnAction()
{
  saveBoard();
  delay(100);
  compareBoard();

  moveCount == 0 ? generateFENFromState(reedSwitchStates) : generateFENFromState(new_reedSwitchStates);

  moveCount++;
}