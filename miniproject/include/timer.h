#ifndef TIMER_H
#define TIMER_H

extern volatile unsigned short *TMR1_SNAPLO;

int getsnap(void);
int hash(int seed);
int randFT(int from, int to);

#endif