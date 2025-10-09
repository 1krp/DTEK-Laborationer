#include "bgPxArrays2.h"
#include "smallRoulettePxArrays1.h"
#include "hw_regs.h"


extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );

int imageIndex = 0;

static int payroll = 0;
static int timerTOCounter = 0;
static int secondCounter = 0;
static int continueGame = 1;
int choiceMade = 0;
volatile int request_reset = 0;

#define IRQ_TIMER   0x10
#define IRQ_SWITCH  0x11
#define IRQ_BTN     0x12
#define BTN_BIT     0x1
#define PRESSED_IS_1 0 

volatile int last_level = 0;
volatile int buttonIsPushed = 0;

#define BGIMGSIZE 76800
#define CARDIMGSIZE 1148
#define ROULETTESIZE 32760

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

void rouletteImage(){
  displayRouletteImage(rouletteImageArr[imageIndex%11]);
  imageIndex++;
}


void handle_interrupt(unsigned cause) 
{
  if (cause & IRQ_TIMER){            // check if timer interrupt
    TMR1_SR = TMR1_SR & 0xE; 
    timerTOCounter ++;

    if (timerTOCounter >= 10){
      timerTOCounter = 0;
      secondCounter ++;
    }
  }

  if (cause & IRQ_SWITCH){     // check if switch interrupt
    int edges = SW_EDGE ;      // read which switch caused the edge
    SW_EDGE  = edges;          // clear it by writing 1s back
        
    if (edges & (1 << 9)) {   // check if switch #10 (bit10) caused it
      request_reset = 1;
    }
  }

  if (cause & IRQ_BTN){
        int edges = BTN_EDGE;
        BTN_EDGE = edges;                 // write-1-to-clear

        if (edges & BTN_BIT){
            int level = (BTN & BTN_BIT) ? 1 : 0;
            if (level == 1 && last_level == 0) {    // active-high press only
                buttonIsPushed = 1;
            }
            last_level = level;
        }
    }
}




int main() {
  displayBgImage(RouletteEmpty_);
  while(1){
    rouletteImage();
    delay(100);
  }
}


