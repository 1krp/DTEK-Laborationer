/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */

#include "hw_regs.h"

extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );
extern int randFT(int from, int to);
extern void roulette();

int mytime = 0x0;
char textstring[] = "text, more text, and even more text!";

void set_leds(int mask){
  LEDS = mask;
}

void reset_disp(){
  for (int i = 0; i < 6;i++){
    REG16(SEVSEG_BASE + i * 0x10) = 0xFFFF;
  }
}

void set_displays(int display_number, int value){
  
  int sevSegVal = 0xFF;

  if (value == 0){
    sevSegVal = 0xC0;
  }
  if (value == 1){
    sevSegVal = 0xF9;
  }
  if (value == 2){
    sevSegVal = 0xA4;
  }
  if (value == 3){
    sevSegVal = 0xB0;
  }
  if (value == 4){
    sevSegVal = 0x99;
  }
  if (value == 5){
    sevSegVal = 0x92;
  }
  if (value == 6){
    sevSegVal = 0x82;
  }
  if (value == 7){
    sevSegVal = 0xF8;
  }
  if (value == 8){
    sevSegVal = 0x80;
  }
  if (value == 9){
    sevSegVal = 0x90;
  }

  /*
    Set value to chosen display
  */
  REG16(SEVSEG_BASE + display_number * 0x10) = sevSegVal;

}

int get_sw(){
  unsigned int retVal = SW;
  return retVal;
}

int get_btn(){
  return 0x1 & BTN;
}

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{
  if (cause & 0x10){            // check if timer interrupt
    TMR1_SR = TMR1_SR & 0xE; 
  }
  if (cause == 0x11){           // check if switch interrupt

    int edge = SW_EDGE ;      // read which switch caused the edge
    SW_EDGE  = edge;          // clear it by writing 1s back
    

    delay(100);

    if (edge & 0x2) {           // check if switch #1 (bit1) caused it
    }
  }
  if (cause == 0x12){            // check if btn1 interrupt

    int inpEdge = BTN_EDGE;  // read which input caused the edge
    BTN_EDGE = inpEdge;      // clear it by writing 1s back

    delay(100);

    if (inpEdge & 0x1) {        // check if btn1 (bit1) caused it
    }
  }
}

/* Add your code here for initializing interrupts. */
void init(void) {
  TMR1_PERLO = (29999999/10) & 0xFFFF;
  TMR1_PERHI = (29999999/10) >> 16;
  TMR1_CR    = 0x7;     // start clock, CONTINUE,enables ITO
  SW_IRQ     = 0x2;     // enable switch IRQ
  BTN_IRQ    = 0x1;     // enable button IRQ
  enable_interrupts();
}

/* Your code goes into main as well as any needed functions. */
int main() {
  init();
  reset_disp();

  while (1) {
    
    int showInt = randFT(0,9);

    if (get_btn()){
      BTN = 0xFFFE & BTN;
      delay(50);
      //*btnPtr = 0xFFFE & *btnPtr;
      //set_leds(0x2);
      //set_displays(0, showInt);
      
      set_displays(0, showInt);


    }
    
  }
}