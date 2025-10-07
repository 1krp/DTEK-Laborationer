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

void displayCardImage(unsigned char img[], int cardNr){

  int cardInd = 0;

  for (int i = 0; i < 320*480; i++){
    
    if (i > 57919 && i%320 > 10 + cardNr + (cardNr-1)*28 && i%320 <= 10 + cardNr + (cardNr)*28 ){

      if (cardInd < CARDIMGSIZE){
        VGA_FB[i] = img[cardInd];
        cardInd ++;
      }
    }
  }
}