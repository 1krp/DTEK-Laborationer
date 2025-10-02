/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */

#include "hw_regs.h"
#include "roulette.h"

extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void delay(int);
extern int nextprime( int );
extern int randFT(int from, int to);
//extern int roulette(int);

int payroll = 0;

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

  /*
    Numbers
  */
  if ((value == 0) || (value == 10)){
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
    Letters
  */
  if (value == 11){ // A
    sevSegVal = 0x88;
  }
  if (value == 15){ // E
    sevSegVal = 0x86;
  }
  if (value == 18){ // H
    sevSegVal = 0x89;
  }
  if (value == 22){ // L
    sevSegVal = 0xC7;
  }
  if (value == 26){ // P
    sevSegVal = 0x8C;
  }
  if (value == 30){ // T
    sevSegVal = 0xCE;
  }
  if (value == 34){ // Y
    sevSegVal = 0x8D;
  }

  /*
    Symbols
  */
  if (value == 35){ // EMPTY
      sevSegVal = 0xFF;
    }
  if (value == 36){ // "-"
    sevSegVal = 0xBF;
  }
  if (value == 37){ // "+"
    sevSegVal = 0xB9;
  }

  /*
    Set value to chosen display
  */
  REG16(SEVSEG_BASE + display_number * 0x10) = sevSegVal;

}

int get_sw(){
  unsigned int retVal = SW;
  return 0x000002FF & retVal;
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

int makePayment(){
  while(1){
    if (get_btn()){
      return get_sw();
    }
  }
}

void showPayroll(){

  int ones = payroll%10;
  int tens = ((payroll-ones)%100)/10;
  int hundr = ((payroll-ones-tens)%1000)/100;
  int thousn = ((payroll-ones-tens-hundr)%10000)/1000;

  set_displays(2, ones);
  set_displays(3, tens);
  set_displays(4, hundr);
  set_displays(5, thousn);
}

void showPlus(int n){

  int ones = n%10;
  int tens = ((n-ones)%100)/10;
  int hundr = ((n-ones-tens)%1000)/100;
  int thousn = ((n-ones-tens-hundr)%10000)/1000;

  for (int i = 0; i < 3; i++){

    delay(400);
    reset_disp();
    delay(400);
    
    set_displays(0, ones);
    set_displays(1, tens);
    set_displays(2, hundr);
    set_displays(3, thousn);

    set_displays(4, 36);
    set_displays(5, 37);
  }
  delay(1000);
}

void showMinus(int n){

  int ones = n%10;
  int tens = ((n-ones)%100)/10;
  int hundr = ((n-ones-tens)%1000)/100;
  int thousn = ((n-ones-tens-hundr)%10000)/1000;

  for (int i = 0; i < 3; i++){

    delay(400);
    reset_disp();
    delay(400);

    set_displays(0, ones);
    set_displays(1, tens);
    set_displays(2, hundr);
    set_displays(3, thousn);

    set_displays(4, 36);
  }
  delay(1000);
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

  /*
    Start screen
  */

  set_displays(0, 36);
  set_displays(1, 36);
  set_displays(2, 1);
  set_displays(3, 18);
  set_displays(4, 36);
  set_displays(5, 36);

  int cont = 0;

  while (!cont) {
    if (get_btn()){
      delay(100);
      cont = 1;
    }
  }

  /*
    Pay-screen
  */
  reset_disp();
  set_displays(0, 1);
  set_displays(3, 34);
  set_displays(4, 11);
  set_displays(5, 26);
  cont = 0;

  while (!cont) {

    if (get_btn()){
      delay(100);

      payroll = makePayment();
      
      reset_disp();
      showPayroll();
      print_dec(payroll);

      delay(1000);

      cont = 1;
    }
  }

  reset_disp();
  set_displays(5, 8);   // B
  set_displays(4, 15);  // E
  set_displays(3, 30);  // T
  delay(500);

  while(1){

    if (get_btn()){
      int newBet = get_sw();
      print("Time for roulette!\n");
      print("Your bet: ");
      print_dec(newBet);
      print("\n");
      int win = roulette(newBet);

      if (win > 0){
        showPlus(win);
        print("You win: ");
        print_dec(win);
        print("\n");
        payroll += win;
      } else {
        showMinus(newBet);
        print("You loose: ");
        print_dec(newBet);
        print("\n");
        if (payroll - newBet > 0){
          payroll -= newBet;
        } else {
          payroll = 0;
        }
        
      }
      
      /*
        Show payroll
      */
      reset_disp();
      showPayroll();
      delay(2000);

      /*
        Place new bets
      */
      reset_disp();
      set_displays(5, 8);   // B
      set_displays(4, 15);  // E
      set_displays(3, 30);  // T
      delay(500);
    }
  }
}