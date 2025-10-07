#include "hw_regs.h"
#include "graphics/bgPxArrays.h"
#include "graphics/cardPxArrays.h"

#define BGIMGSIZE 76800
#define CARDIMGSIZE 1148

void displayBgImage(unsigned char img[]){
  for (int i = 0; i < BGIMGSIZE; i++){
    VGA_FB[i] = img[i];
  }
}
//X is px from left
//Y is px from top
void displayCardImage(int x, int y, unsigned char img[]){

  int cardInd = 0;

  while (cardInd < CARDIMGSIZE) {
    int i = y*320+x;

    if(i % 320-x < 29){
        VGA_FB[i] = img[cardInd];
        cardInd ++;
        i++;
    }else{
        i += 320 - 28;
    }
  }
  
}