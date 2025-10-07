#include "roulette.h"
#include "../main.h"

extern void delay(int);
extern void set_leds(int led_mask);
extern void set_displays(int display_number, int value);
extern void reset_disp();
extern void print(const char*);
extern void print_dec(unsigned int);
extern int get_btn(void);
extern int get_sw(void);
extern int randFT(int from, int to);
extern int nextprime( int );

/*
  Roulette constants
*/
#define DECELERATION 1.07

/*
  Roulette globals
*/
static int betEven = 0;
static int betOdd = 0;
static int betNum = 10; // Default value anything > 9

/*
  Displays winning number on 7-segm-displays
*/
void showNumber(int n){
  for (int i = 0; i < 5; i++){
    set_displays(0, n);
    set_displays(1, 35);
    set_displays(2, n);
    set_displays(3, 35);
    set_displays(4, n);
    set_displays(5, 35);
    delay(200);
    reset_disp();
    delay(200);
  }
}

/*
  Displays start game routine on 7-segm-displays
*/
void showStartRound(){
  for (int i = 0; i < 3; i++){
    set_displays(5, 36);
    set_displays(4, 36);
    set_displays(3, 36);
    set_displays(2, 36);
    set_displays(1, 36);
    set_displays(0, 36);
    delay(200);
    reset_disp();
    delay(200);
  }
  set_displays(2, 3);
  delay(800);
  reset_disp();
  set_displays(1, 2);
  delay(800);
  reset_disp();
  set_displays(0, 1);
  delay(800);
  reset_disp();
}

/*
  Player picks bets; odd/even and a number
*/
void pickBets(){

  /*
    Odd or even
  */
  reset_disp();
  set_displays(5, 0);   // O (Odd)
  set_displays(4, 1);   // 1
  set_displays(1, 15);  // E (Even)
  set_displays(0, 2);   // 2
  delay(500);

  int choiceMade = 0;
  while(!choiceMade){
    if (get_btn()){
      int choice = get_sw();

      if (choice == 1){
        betOdd = 1;
        choiceMade = 1;
      }
      if (choice == 2){
        betEven = 1;
        choiceMade = 1;
      }
      if (choice == 0){
        choiceMade = 1;
      }
    }
  }

  print("You picked: ");
  if (betOdd == 1){
    print("odd\n");
  } else if (betEven == 1) {
    print("even\n");
  } else {
    print("none\n");
  }

  /*
    Pick a number 0-9
  */
  reset_disp();
  set_displays(5, 0);
  set_displays(4, 36);
  set_displays(3, 9);
  delay(500);

  choiceMade = 0;
  while(!choiceMade){
    if (get_btn()){
      if (get_sw() < 10){
        betNum = get_sw();

        print("You picked: ");
        print_dec(betNum);
        print("\n");
      }
      choiceMade = 1;
    }

    /*
      Show current switch number
    */
    if (get_sw() < 10){
      set_displays(0, get_sw());
    } else {
      set_displays(0, 35);
    }
  }
}

/*
  Spin wheel procedure
*/
int spinWheel(){

  int wheelSpins = 1;
  int lampBinV = 1;
  int delayT = randFT(10,50);
  int goRight = 0;
  int counter = 0;

  /*
    Display bets
  */
  if (betOdd == 1){
    set_displays(1, 0);
  } else if (betEven == 1) {
    set_displays(1, 15);
  } else {
    set_displays(1, 36);
  }
  if (betNum < 10) {
    set_displays(0, betNum);
  } else {
    set_displays(0, 36);
  }

  /*
    Reset seconds counter
  */
  set_seconds(0);

  /*
    Spin wheel
  */
  int delayMax = 210;
  set_leds(lampBinV);
  delay(delayT);
  while (wheelSpins){

    /*
      If delay time reaches delayMax -> stop wheel
    */
    if (delayT>delayMax){wheelSpins = 0;}

    /*
      Go left/right logic
    */
    if (!goRight) {
      lampBinV = lampBinV*2;
    } else {
      lampBinV = lampBinV/2;
    }
    if (lampBinV >= 512){
      goRight = 1;
    }
    if (goRight && lampBinV <= 1) {
      goRight = 0;
    }

    /*
      Switch to next lamp and wait delay ms
    */
    set_leds(lampBinV);
    delay(delayT);

    /*
      Decelerate ball speed (increase delay time)
    */
    delayT += DECELERATION*get_seconds();
  }

  /*
    Get led number
  */
  int ledN = 0;
  while (lampBinV > 1){
    lampBinV = lampBinV/2;
    ledN ++;
  }

  /*
    Return number of winning led
  */
  return ledN;
}

/*
  Round loop

  Takes a bet (int amount) as input
  Returns winning amount
*/
int playRound(int bet){

  /*
    Reset globals
  */
  betOdd = 0;
  betEven = 0;
  betNum = 10;

  /*
    Current turn variables
  */
  int currNum = 10; // Default value anything > 9
  int currEven = 0; // If ball lands on even number
  int currOdd = 0;  // If ball lands on odd number
  int win = 0;      // Amount won this round

  /*
    Put bets
  */
  pickBets();

  /*
    Start game
  */
  print("Let's play!\n");
  delay(500);
  showStartRound();

  /*
    Spin wheel
  */
  currNum = spinWheel();
  showNumber(currNum);
  print("Landed on: ");
  print_dec(currNum);
  print("\n");

  /*
    Evaluate current number
  */
  if (currNum%2 == 0 && currNum != 0){
    currEven = 1;
  } else {
    currOdd = 1;
  }

  /*
    Evaluate wins
  */
  if (currNum == betNum){
    win += bet*9;
  }
  if (betOdd && currOdd){
    win += bet*2;
  }
  if (betEven && currEven){
    win += bet*2;
  }

  return win;
}
