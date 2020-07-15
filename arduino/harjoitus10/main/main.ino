#include <LedMatrix.h>
// Defines

#define ROW_1 2
#define ROW_2 3
#define ROW_3 4
#define ROW_4 5
#define ROW_5 6
#define ROW_6 7
#define ROW_7 8
#define ROW_8 9

#define COL_1 10
#define COL_2 11
#define COL_3 12
#define COL_4 13
#define COL_5 A0
#define COL_6 A1
#define COL_7 A2
#define COL_8 A3

byte rows[] = {
    ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8
};
byte col[] = {
  COL_1,COL_2, COL_3, COL_4, COL_5, COL_6, COL_7, COL_8
};

LedMatrix matrix(rows, col);

void setup() {
  matrix.clear();
  matrix.setDelay(200);
  matrix.setShape('u');
}

int count = 0;

void loop() {
  char word [] = {'s', 'u', 'l', 'a', 'r','i'};
  //matrix.drawWord(word, sizeof(word), 80);
  //matrix.drawAnimation(1);
  //matrix.drawAnimation(2);
  matrix.drawAnimation(3);
}
