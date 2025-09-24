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


int hash(int seed) {
    seed = ((seed >> 16) ^ seed) * 0x45d9f3bu;
    seed = ((seed >> 16) ^ seed) * 0x45d9f3bu;
    seed = (seed >> 16) ^ seed;
    return seed;
}
int randFT(int from, int to, int seed) {
  int diff = to - from ; 

  int randomNum = (seed % diff)+from;

  return randomNum;
}
/* Your code goes into main as well as any needed functions. */
int main() {
  //labinit();
  int arr[11] = {0,0,0,0,0,0,0,0,0,0,0};
  int randomNum = 0;

  for(int i=1; i < 30000; i++){
    randomNum = randFT(1,11,hash(i));
    arr[randomNum]++;
  }
  for(int i = 1; i < 11; i++){
    printf("Number of %ds is %d\n",i, arr[i]);
  }

}


