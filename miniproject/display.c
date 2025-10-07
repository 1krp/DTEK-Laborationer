#include "hw_regs.h"
#include "graphics/bgPxArrays.h"
#include "graphics/cardPxArrays.h"

void displayBgImage(unsigned char img[]){
  for (int i = 0; i < 320*480; i++){
    VGA_FB[i] = img[i];
  }
}

/*
void testAnimation(){ // 28*41

  int cardInd = 0;
  for (int i = 0; i < 320*480; i++){
    if (i > 40000 && i%320 > 50 && i%320 <= 78) {

      if (cardInd < 1148){
        VGA_FB[i] = SA[cardInd];
        cardInd ++;
      }
    }
    
  }
}*/