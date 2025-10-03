#include "roulette.h"
#include "main.h"

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
  Roulette globals
*/
static int odd = 0;
static int even = 0;
static int num = 10; // Default value anything > 9

int roulette(int bet){

  /*
    Reset globals
  */
  odd = 0;
  even = 0;
  num = 10;

  /*
    Current turn variables
  */
  int currNum = 10; // Default value anything > 9
  int currOdd = 0;
  int currEven = 0;
  int currGreen = 0;
  int win = 0;

  /*
    Make bet
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
  if (currNum == 0){
    currGreen = 1;
  } else if (currNum%2 != 0){
    currOdd = 1;
  } else {
    currEven = 1;
  }

  /*
    Evaluate wins
  */
  if (currNum == num){
    win = bet*9;
  }
  if (odd && currOdd){
    win = bet*2;
  }
  if (even && currEven){
    win = bet*2;
  }

  return win;
}

void pickBets(){
  int choiceMade = 0;

  /*
    Odd or even
  */
  reset_disp();
  set_displays(5, 0);   // O (Odd)
  set_displays(4, 1);   // 1
  set_displays(1, 15);  // E (Even)
  set_displays(0, 2);   // 2
  delay(500);

  while(!choiceMade){
    if (get_btn()){
      int choice = get_sw();

      if (choice == 1){
        odd = 1;
        choiceMade = 1;
      }
      if (choice == 2){
        even = 1;
        choiceMade = 1;
      }
      if (choice == 0){
        choiceMade = 1;
      }
    }
  }

  print("You picked: ");
  if (odd == 1){
    print("odd\n");
  } else if (even == 1) {
    print("even\n");
  } else {
    print("none\n");
  }

  /*
    One number
  */
  reset_disp();
  set_displays(5, 1);
  set_displays(4, 36);
  set_displays(3, 9);
  delay(500);

  choiceMade = 0;

  while(!choiceMade){
    if (get_btn()){
      int choice = get_sw();

      if (choice < 10){
        num = choice;
        choiceMade = 1;
      }
      if (choice == 512){
        choiceMade = 1;
      }
    }

    /*
      Show current switch number
    */
    int currentChoice = get_sw()%10;
    set_displays(0, currentChoice);
  }

  if (num<10){
    print("You picked: ");
    print_dec(num);
    print("\n");
  }
  else {
    print("You picked no number\n");
  }
}

int spinWheel(){

  int wheelSpins = 1;
  int randModHi = nextprime(randFT(10,20));
  int randModLo = randFT(3,7);
  int lampV = 1;
  int delayT = 20;
  int goRight = 0;
  int counter = 0;

  /*
    Display bets
  */
  if (odd == 1){
    set_displays(1, 0);
  } else if (even == 1) {
    set_displays(1, 15);
  } else {
    set_displays(1, 36);
  }
  if (num < 10) {
    set_displays(0, num);
  } else {
    set_displays(0, 36);
  }

  set_leds(lampV);
  delay(delayT);

  while (wheelSpins){

    /*
      If delay time reaches 300 -> stop wheel
    */
    if (delayT>250){wheelSpins = 0;}

    /*
      Increase delay time on random modulo
    */
    if(counter > 0 && counter%randModHi == 0){
      delayT*=1.5;
    }

    /*
      Increase delay time faster on larger delay times
    */
    if(delayT > 150 && counter%randModHi == 0){
      delayT*=1.8;
    }
    if(delayT > 200 && counter%randModLo == 0){
      delayT*=2;
    }

    /*
      Go right/left logic
    */
    if (!goRight) {
      lampV = lampV*2;
    } else {
      lampV = lampV/2;
    }
    if (lampV >= 512){
      goRight = 1;
    }
    if (goRight && lampV <= 1) {
      goRight = 0;
    }

    /*
      Increase counter every loop
    */
    counter ++;

    /*
      Switch to next lamp and wait delay ms
    */
    set_leds(lampV);
    delay(delayT);
  }

  int sqrt_lv = 0;
  while (lampV > 1){
    lampV = lampV/2;
    sqrt_lv ++;
  }

  return sqrt_lv;
}

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