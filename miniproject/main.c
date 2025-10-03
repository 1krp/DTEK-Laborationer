/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */

#include "main.h"
#include "hw_regs.h"
#include "roulette.h"

extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void delay(int);
extern int nextprime( int );
extern int randFT(int from, int to);

static int payroll = 0;
static int resetGame = 0;

void set_payroll(int v) { payroll = v; }

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
  if (value == 20){ // J
    sevSegVal = 0xE1;
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

void makePayment(){

  int paymentDone = 0;

  reset_disp();
  set_displays(5, 26);  // P
  set_displays(4, 11);  // A
  set_displays(3, 34);  // Y
  set_displays(0, 1);

  while (!paymentDone) {

    if (get_btn()){
      delay(100);

      while(!paymentDone){
        if (get_btn()){
          payroll = get_sw();
          paymentDone = 1;
        }
      }
      
      reset_disp();
      showPayroll();
      print("Payment received: ");
      print_dec(payroll);
      print("\n");

      delay(1000);
    }

    /*
      Show current switch number
    */
    int currentPayment = get_sw();
    int currOnes = currentPayment%10;
    int currTens = ((currentPayment-currOnes)%100)/10;
    int currHundr = ((currentPayment-currOnes-currTens)%1000)/100;
    set_displays(2, currHundr);
    set_displays(1, currTens);
    set_displays(0, currOnes);
  }
}

void displayBet(){
  reset_disp();
  set_displays(5, 8);   // B
  set_displays(4, 15);  // E
  set_displays(3, 30);  // T
}

int makeBet(){

  int newBet = 0;

  displayBet();

  while (1) {

    if (get_btn()){
      newBet = get_sw();

      if (newBet <= payroll){

        reset_disp();
        print("Your bet: ");
        print_dec(newBet);
        print("\n");
        delay(1000);

        payroll -= newBet;
        return newBet;
      } else {
        print("Not enough money");
      }
    }

    /*
      Show current switch number
    */
    int currentBet = get_sw();
    int currOnes = currentBet%10;
    int currTens = ((currentBet-currOnes)%100)/10;
    int currHundr = ((currentBet-currOnes-currTens)%1000)/100;
    set_displays(2, currHundr);
    set_displays(1, currTens);
    set_displays(0, currOnes);
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

void rouletteGameRun(){

  print("Time for roulette!\n");

  while(!resetGame){

    print("New round!\n");
    delay(1000);
    int currBet = 0;

    print("Make bet");
    currBet = makeBet();

    int win = roulette(currBet);

    if (win > 0){
      showPlus(win);
      payroll += win;
      print("You win: ");
      print_dec(win);
      print("\n");
    } else {
      print("No wins..\n");
    }

    /*
      Reset leds
    */
    set_leds(0x0);
    reset_disp();
    print("Rond finished\n");

    int playAgain = 0;
    while (!playAgain){

      /*
        Show payroll
      */
      showPayroll();

      if (get_btn()){
        print("Play again\n");
        playAgain = 1;
      }
    }
  }
}

void letsPlay(){

  while (1) {

    /*
      Choose game
    */
    reset_disp();
    set_displays(5, 1);
    set_displays(0, 2);

    if (get_btn()){
      delay(100);

      if (get_sw() == 1){ // Roulette choosed
        rouletteGameRun();
      }
      if (get_sw() == 2){ // Black jack choosed

      }
      resetGame = 0; // Unreset reset button
    }
  }
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
    if (edge & 0x200) {           // check if switch #10 (bit10) caused it
      print("Reset game");
      resetGame = 1;
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
  SW_IRQ     = 0x200;     // enable switch IRQ for switch # 10
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

  makePayment();
  letsPlay();
}