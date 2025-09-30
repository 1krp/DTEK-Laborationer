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

int mytime = 0x0;
char textstring[] = "text, more text, and even more text!";

int prime = 1234567;

volatile unsigned short *TMR1_SR = (unsigned short*) 0x04000020;
volatile unsigned short *TMR1_CR = (unsigned short*) 0x04000024;
volatile unsigned short *TMR1_PERLO = (unsigned short*) 0x04000028;
volatile unsigned short *TMR1_PERHI = (unsigned short*) 0x0400002c;

volatile int* swPtr = (volatile int*)0x04000010;
volatile int* swIRQPt = (volatile int*)0x04000018;

void set_leds(int led_mask){
  volatile int* ledPtr = 0x04000000;
  *ledPtr = led_mask;
}

void sec_leds(int led_mask){
  volatile int* ledPtr = 0x04000000;

  for (int i = 0; i < led_mask;i++){

    delay(1100);

    *ledPtr = i;
  }
}

void reset_disp(){

  volatile char* sevSegPtr = (volatile char*)0x04000050;

  for (int i = 0; i < 6;i++){
    *sevSegPtr = 0xFFFF;
    sevSegPtr += 0x10;
  }
}

void set_displays(int display_number, int value){

  volatile char* sevSegPtr = (volatile char*)0x04000050;

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
    *sevSegPtr = 0xC0;
  }

  /*
    Show dots on display 5 and 3
  */
  if (display_number == 5 || display_number == 3){
    *sevSegPtr -= 0x80;
  } 
}

int globalCounter = 0;
int sec_counter = 0;
int min_counter = 0;
int hr_counter = 0;

void clock_counter(){

  //delay(200);

  sec_counter++;

  if (sec_counter >= 60){
    min_counter ++;
    sec_counter = 0;
  }
  if (min_counter >= 60){
    hr_counter ++;
    min_counter = 0;
  }
}

void display_time_tens(){
  int sec_ones = 0;
  int sec_tens = 0;
  int min_ones = 0;
  int min_tens = 0;
  int hr_ones = 0;
  int hr_tens = 0;

  sec_ones = globalCounter;
  sec_tens = 0;
  min_ones = sec_counter%10;
  min_tens = sec_counter / 10;
  hr_ones = min_counter%10;
  hr_tens = min_counter / 10;

  set_displays(1, sec_ones);
  set_displays(2, sec_tens);
  set_displays(3, min_ones);
  set_displays(4, min_tens);
  set_displays(5, hr_ones);
  set_displays(6, hr_tens);
}

void display_time(){
  int sec_ones = 0;
  int sec_tens = 0;
  int min_ones = 0;
  int min_tens = 0;
  int hr_ones = 0;
  int hr_tens = 0;

  sec_ones = sec_counter%10;
  sec_tens = sec_counter / 10;
  min_ones = min_counter%10;
  min_tens = min_counter / 10;
  hr_ones = hr_counter%10;
  hr_tens = hr_counter / 10;

  set_displays(1, sec_ones);
  set_displays(2, sec_tens);
  set_displays(3, min_ones);
  set_displays(4, min_tens);
  set_displays(5, hr_ones);
  set_displays(6, hr_tens);
}

int get_sw(){
  volatile int* swPtr = (volatile int*)0x04000010;
  unsigned int retVal = *swPtr;
  return retVal;
}

int get_btn(){
  volatile int* btnPtr = (volatile int*)0x040000d0;
  return 0x1 & *btnPtr;
}

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) {
  if ((cause & 0x1F) == 0x10){ // check if timer interrupt

    *TMR1_SR = *TMR1_SR & 0xFFFE; // clear TO bit
    globalCounter ++;

    if (globalCounter==10){
      globalCounter = 0;

      clock_counter();
      tick (&mytime);

      display_time();
    }
  }

  if((cause & 0x1F)==0x11){

    delay(100);

    tick(&mytime);

  }
}

/* Add your code here for initializing interrupts. */
void labinit(void)
{ 
  *(TMR1_PERLO) = (29999999/10) & 0xFFFF;
  *(TMR1_PERHI) = (29999999/10) >> 16;

  *(TMR1_CR) = 0x7; // start clock, enables ITO

  *(swIRQPt) = 0x2; // enabels interupt for switch #1

  enable_interrupts();
}

/* Your code goes into main as well as any needed functions. */
int main() {
  labinit();
  while (1) {
    print ("Prime: ");
    prime = nextprime( prime );
    print_dec( prime );
    print("\n");
  }
}


