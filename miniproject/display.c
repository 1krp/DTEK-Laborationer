#include "hw_regs.h"
#include "graphics/tabBG.h"
#include "graphics/SA.h"

void testDisplay(){
  //volatile char* VGA = (volatile char*) 0x08000000;

  for (int i = 0; i < 320*480; i++){
    VGA_FB[i] = tabBG[i];
  }

  /*
  unsigned int y_ofs = 0;

  volatile int* VGA_CTRL = (volatile int*) 0x04000100;
  
  
  while(1){
    *(VGA_CTRL+1) = (unsigned int) (VGA+y_ofs*320);
    *(VGA_CTRL+0) = 0;
    y_ofs = (y_ofs+1) % 240;
    delay(100);
  }*/
}

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
}