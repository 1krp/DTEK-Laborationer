#include "hw_regs.h"

int getsnap(){
  TMR1_SNAPLO = 0;
  int low = TMR1_SNAPLO;
  return low;
}

int hash(int seed) {
    seed = ((seed >> 16) ^ seed) * 0x45d9f3bu;
    seed = ((seed >> 16) ^ seed) * 0x45d9f3bu;
    seed = (seed >> 16) ^ seed;
    return seed;
}

int randFT(int from, int to) {
  int seed = 0;
  int diff = to - from ;
  int snap = getsnap();
  seed = hash(snap);

  int randomNum = (seed % diff)+from;

  return randomNum;
}