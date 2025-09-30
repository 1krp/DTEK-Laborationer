extern void delay(int);
extern void set_leds(int led_mask);
extern void set_displays(int display_number, int value);
extern void reset_disp();

void roulette(){

  int spin = 1;
  int lv =1;
  int dv = 20;
  int decr = 0;
  int counter = 0;

  set_leds(lv);
  delay(dv);

  while (spin){
    if (dv>300){spin = 0;}
    if(counter > 0 && counter%17 == 0){
      dv*=1.8;
    }

    if (!decr) {
      lv = lv*2;
    } else {
      lv = lv/2;
    }

    if (lv >= 512){
      decr = 1;
    }
    if (decr && lv <= 1) {
      decr = 0;
    }
    counter ++;
    set_leds(lv);
    delay(dv);
  }

  int sqrt_lv = 0;
  while (lv > 1){
    lv = lv/2;
    sqrt_lv ++;
  }

  for (int i = 0; i < 5; i++){
    set_displays(1, 11);
    set_displays(2, sqrt_lv);
    set_displays(3, 11);
    set_displays(4, sqrt_lv);
    set_displays(5, 11);
    set_displays(6, sqrt_lv);
    delay(200);
    reset_disp();
    delay(200);
  } 
}