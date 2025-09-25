/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */
#include <stdio.h>
#include <stdlib.h>

extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );

volatile unsigned short *TMR1_SR = (unsigned short*) 0x04000020;
volatile unsigned short *TMR1_CR = (unsigned short*) 0x04000024;
volatile unsigned short *TMR1_PERLO = (unsigned short*) 0x04000028;
volatile unsigned short *TMR1_PERHI = (unsigned short*) 0x0400002c;

volatile unsigned short *TMR1_SNAPLO = (unsigned short*) 0x04000030;
volatile unsigned short *TMR1_SNAPHI = (unsigned short*) 0x04000034;


/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{
  if (cause & 0x10){ // check if timer interrupt

    *TMR1_SR = *TMR1_SR & 0xE; // clear TO bit
  }
}

/* Add your code here for initializing interrupts. */
void labinit(void)
{ 
  *(TMR1_PERLO) = (29999999/10) & 0xFFFF;
  *(TMR1_PERHI) = (29999999/10) >> 16;

  *(TMR1_CR) = 0x7; // start clock, enables ITO
  enable_interrupts();
}

int getsnap(volatile unsigned short *SNAPLO){
  return (int)(*SNAPLO);
}

int hash(int seed) {
    seed = ((seed >> 16) ^ seed) * 0x45d9f3bu;
    seed = ((seed >> 16) ^ seed) * 0x45d9f3bu;
    seed = (seed >> 16) ^ seed;
    return seed;
}

int randFT(int from, int to) {
  int diff = to - from ;
  int seed = getsnap(TMR1_SNAPLO);
  int hash = hash(seed);  

  int randomNum = (seed % diff)+from;

  return randomNum;
}

int main() {
  //labinit();
  randFT(1, 100); 
}


