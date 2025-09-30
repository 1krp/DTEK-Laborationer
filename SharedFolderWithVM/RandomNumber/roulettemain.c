/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */
#include <stdio.h>
#include <stdlib.h>


// Thomas Wang's hash
int hash(int seed) {
    seed = ((seed >> 16) ^ seed) * 0x45d9f3bu;
    seed = ((seed >> 16) ^ seed) * 0x45d9f3bu;
    seed = (seed >> 16) ^ seed;
    return seed;
}

int randomGen(int from, int to, int seed) {
  int diff = to - from ;

  int randomNum = (seed % diff)+from;
  return randomNum;
}

/* Your code goes into main as well as any needed functions. */
int main() {
  //labinit();
  int numberList[10] = {0,0,0,0,0,0,0,0,0,0};
  for(int i = 1; i < 3000000; i++){
    numberList[randomGen(1, 11, hash(i))-1]++;
  }
  for(int i = 0; i < 10; i++){
    printf("Number %d was generated %d times\n", i+1, numberList[i]);
  }
}


