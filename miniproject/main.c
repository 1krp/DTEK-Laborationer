/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */

extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );
extern int randFT(int from, int to);
extern int returnSumOf(int n);
extern void roulette();

int mytime = 0x0;
char textstring[] = "text, more text, and even more text!";

/*
  Timer registers
*/
volatile unsigned short *TMR1_SR = (unsigned short*) 0x04000020;
volatile unsigned short *TMR1_CR = (unsigned short*) 0x04000024;
volatile unsigned short *TMR1_PERLO = (unsigned short*) 0x04000028;
volatile unsigned short *TMR1_PERHI = (unsigned short*) 0x0400002c;
volatile unsigned short *TMR1_SNAPLO = (unsigned short*) 0x04000030;
volatile unsigned short *TMR1_SNAPHI = (unsigned short*) 0x04000034;

/*
  Led registers
*/
volatile int* ledPtr = 0x04000000;

/*
  7-seg registers
*/
volatile char* sevSegPtr = (volatile char*)0x04000050;

/*
  Switch registers
*/
volatile int* swPtr = (volatile int*)0x04000010;

/*
  Btn1 registers
*/
volatile int* btnPtr = (volatile int*)0x040000d0;


void set_leds(int led_mask){
  *ledPtr = led_mask;
}

void reset_disp(){
  for (int i = 0; i < 6;i++){
    *sevSegPtr = 0xFFFF;
    sevSegPtr += 0x10;
  }
}

void set_displays(int display_number, int value){

  /*
    Pick chosen display
  */
  for (int i = 1; i < display_number;i++){
    sevSegPtr += 0x10;
  }

  if (value == 0){
    *sevSegPtr = 0xC0;
  }
  if (value == 1){
    *sevSegPtr = 0xF9;
  }
  if (value == 2){
    *sevSegPtr = 0xA4;
  }
  if (value == 3){
    *sevSegPtr = 0xB0;
  }
  if (value == 4){
    *sevSegPtr = 0x99;
  }
  if (value == 5){
    *sevSegPtr = 0x92;
  }
  if (value == 6){
    *sevSegPtr = 0x82;
  }
  if (value == 7){
    *sevSegPtr = 0xF8;
  }
  if (value == 8){
    *sevSegPtr = 0x80;
  }
  if (value == 9){
    *sevSegPtr = 0x90;
  }
  if (value == 10){
    *sevSegPtr = 0xBF;
  }
  if (value == 11){
    *sevSegPtr = 0xFF;
  }

  /*
    When clock display: Show dots on display 5 and 3
  */
  /*
  if (display_number == 5 || display_number == 3){
    *sevSegPtr -= 0x80;
  } 
  */
}

int get_sw(){
  unsigned int retVal = *swPtr;
  return retVal;
}

int get_btn(){
  return 0x1 & *btnPtr;
}

int getsnap(){
  *TMR1_SNAPLO = 0;
  int low = *TMR1_SNAPLO;
  return low;
}

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{
  if (cause & 0x10){ // check if timer interrupt

    *TMR1_SR = *TMR1_SR & 0xE; // clear TO bit
    *TMR1_SNAPLO = *TMR1_SNAPLO + 0x1;

    globalCounter ++;

    if (globalCounter==10){
      globalCounter = 0;
      clock_counter();
      tick (&mytime);
    }
  }
}

/* Add your code here for initializing interrupts. */
void labinit(void)
{
  *(TMR1_PERLO) = (29999999/10) & 0xFFFF;
  *(TMR1_PERHI) = (29999999/10) >> 16;

  *(TMR1_CR) = 0x7; // start clock, CONTINUE ,enables ITO
  enable_interrupts();
}

/* Your code goes into main as well as any needed functions. */
int main() {
  labinit();
  reset_disp();

  /*
  while(1){
  
    if (get_btn()){
      delay(110);
      *btnPtr = 0x0 & *btnPtr;
      print_dec(returnSumOf(3));
    }
  }*/

  

  while (1) {
    if (get_btn()){
      roulette();
    }
  }
  

}