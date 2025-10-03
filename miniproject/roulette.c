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
    Pick bets
  */
  makeBet();

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

  //if (win)
  //set_payroll(get_payroll() + win);

  return win;
}

void makeBet(){
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
  set_displays(0, 0);
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

  int spin = 1;
  int led_v = 1;
  int del_v = 20;
  int decr = 0;
  int counter = 0;

  set_leds(led_v);
  delay(del_v);

  while (spin){
    if (del_v>300){spin = 0;}
    if(counter > 0 && counter%17 == 0){
      del_v*=1.8;
    }

    if (!decr) {
      led_v = led_v*2;
    } else {
      led_v = led_v/2;
    }

    if (led_v >= 512){
      decr = 1;
    }
    if (decr && led_v <= 1) {
      decr = 0;
    }
    counter ++;
    set_leds(led_v);
    delay(del_v);
  }

  int sqrt_lv = 0;
  while (led_v > 1){
    led_v = led_v/2;
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