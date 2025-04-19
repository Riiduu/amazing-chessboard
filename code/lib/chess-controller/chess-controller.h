#ifndef CHESS_CONTROLLER_H
#define CHESS_CONTROLLER_H

extern mcumax_piece board_snapshot[64];

void selectChannel(int ch);
void printBoard();
void saveBoard();
void compareBoard();
void moveBtnAction();
void setupBoard();

#endif