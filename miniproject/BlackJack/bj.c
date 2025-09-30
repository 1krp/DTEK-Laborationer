#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*  
Global variable for the payroll of the game.
This variable is set when starting the program
*/
int GlobalPayroll = 0;




extern int randFT(int from, int to);

typedef struct card {
    char *suit;
    int value;
} Card;

Card create_card(char *suit, int value) {
    Card new_card;
    new_card.suit = suit;
    new_card.value = value;
    return new_card;
}

Card **init_deck() {
    static Card cards[52];      // array of cards
    static Card *deck[52];      // array of pointers

    for (int i = 0; i < 52; i++) {
        deck[i] = &cards[i];    
        deck[i]->suit = "";
        deck[i]->value = 0;
    }

    return deck;  
}
void fill_deck(Card **deck) {
    for (int i = 0; i < 52; i++) {
        int value = (i % 13) + 2;
        if (i % 4 == 0) deck[i]->suit = "Hearts";
        else if (i % 4 == 1) deck[i]->suit = "Diamonds";
        else if (i % 4 == 2) deck[i]->suit = "Clubs";
        else deck[i]->suit = "Spades";
        deck[i]->value = value;
    }
}
// fisher-yates-shuffling
void shuffle_deck(Card **deck) {
    int n = 52;
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card *temp = deck[j];
        deck[j] = deck[i];
        deck[i] = temp;
    }
}
void init_payroll(int n){
    GlobalPayroll = n;
}

Card *init_hand() {
    static Card deck[10];
}


int main(int argc, char *argv[]) { 
    srand(time(NULL));                          //Ta bort för brädan
    if(argc == 2)
    {
        init_payroll(atoi(argv[1]));
    }
  else {
    printf("Please state an integer number.\n");
    }




    Card **deck = init_deck();
    fill_deck(deck);

    for (int i = 0; i < 52; i++) {
        printf("Card %d: %s %d\n", i + 1, deck[i]->suit, deck[i]->value);
    }

    printf("\nShuffling...\n\n");
    shuffle_deck(deck);

    for (int i = 0; i < 52; i++) {
        printf("Card %d: %s %d\n", i + 1, deck[i]->suit, deck[i]->value);
    }



    return 0;
}