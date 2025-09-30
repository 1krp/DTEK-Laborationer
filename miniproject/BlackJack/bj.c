/*
#include <stdio.h>
#include <stdlib.h>

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

Card *init_deck() {
    static Card deck[52];
    for (int i = 0; i < 52; i++) {
        deck[i].suit = "";
        deck[i].value = 0;
    }
    return deck;
}
void fill_deck(Card *deck) {
    for(int i = 0; i < 52; i++) {
        if(i % 4 == 0) {
            deck[i].suit = "Hearts";
            deck[i].value = (i % 13) + 2;
        } else if(i % 4 == 1) {
            deck[i].suit = "Diamonds";
            deck[i].value = (i % 13) + 2;
        } else if(i % 4 == 2) {
            deck[i].suit = "Clubs";
            deck[i].value = (i % 13) + 2;
        } else if(i % 4 == 3) {
            deck[i].suit = "Spades";
            deck[i].value = (i % 13) + 2;
        }
    }
}
// fisher-yates-shuffling
void suffle_deck(Card *deck) {
    int n = 52;
    int j = 0;
    Card temp = create_card("", 0);

    for(int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        temp = deck[j];
        deck[j] = deck[i];
        deck[i] = temp;
    }
}




int main() { 
    Card* deck = init_deck();
    fill_deck(deck);

    for(int i = 0; i < 52; i++) {
        printf("Card %d: %s %d\n", i + 1, deck[i].suit, deck[i].value);
    }
    printf("\n\nShuffling deck...\n\n");
    suffle_deck(deck);
    for(int i = 0; i < 52; i++) {
        printf("Card %d: %s %d\n", i + 1, deck[i].suit, deck[i].value);
    }
    return 0;
}
*/