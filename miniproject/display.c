#include "hw_regs.h"
#include "graphics/bgPxArrays.h"
#include "graphics/cardPxArrays.h"

#define BGIMGSIZE 76800
#define CARDIMGSIZE 1148
#define ROULETTESIZE 4875

void displayBgImage(unsigned char* img){
  for (int i = 0; i < BGIMGSIZE; i++){
    VGA_FB[i] = img[i];
  }
}
//X is px from left
//Y is px from top
void displayCardImage(int x, int y, unsigned char* img){

  int cardInd = 0;
  int i = y*320+x;

  while (cardInd < CARDIMGSIZE) {
    if( (i%320) - x < 28){
        VGA_FB[i] = img[cardInd];
        cardInd ++;
        i++;
    }else{
        i += 320 - 28;
    }
  }
}

void displayRouletteImage(unsigned char* img) {

  int x = 72;
  int y = 52;
  int width = 180;

  int cardInd = 0;
  int i = y*320+x;

  while (cardInd < ROULETTESIZE) {
    if( (i%320) - x < width){
        VGA_FB[i] = img[cardInd];
        cardInd ++;
        i++;
    }else{
        i += 320 - width;
    }
  }

}