
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HAND_SIZE 10
#define DECK_SIZE 52

/*
Holds index of the card to be dealt next in the deck
*/
int topOfDeck = 0;

int pHandValue = 0;
int dHandValue = 0;

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
/*
Initializes a deck of DECK_SIZE empty cards
*/
Card *init_deck() {
    static Card deck[DECK_SIZE];
    for (int i = 0; i < DECK_SIZE; i++) {
        deck[i].suit = "";
        deck[i].value = 0;
    }
    return deck;
}
/*
Initializes a hand of 3 empty cards
*/

void init_hand(Card *hand, int size) {
    for (int i = 0; i < size; i++) {
        hand[i].suit = "empty";
        hand[i].value = 0;
    }
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
void shuffle_deck(Card *deck) {
    int n = 52;
    Card temp;

    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        temp = deck[j];
        deck[j] = deck[i];
        deck[i] = temp;
    }
}

void deal_card(Card *deck, Card *hand, int numOfCards) {
    int dealtCardsCounter = 0;

        for(int i = 0; i < 10; i++){
            if(hand[i].value == 0 && dealtCardsCounter < numOfCards){
                hand[i].suit = deck[topOfDeck].suit;
                hand[i].value = deck[topOfDeck].value;
                topOfDeck++;
                dealtCardsCounter++;
            }
        }
}

void print_cards(Card *hand, int size) {
    for (int i = 0; i < size; i++) {
        if (hand[i].value !=0) {
            printf("Card %d: %s %d\n", i + 1, hand[i].suit, hand[i].value);
        }
    }
}

int count_hand_value(Card *hand, int size) {
    int handValue = 0;
    int aceCounter = 0;

    for(int i = 0; i < size; i++) {
        if (hand[i].value != 14) {
            if (hand[i].value < 10) {
                handValue += hand[i].value;
            }else{
                handValue += 10;
            }
        }else if (hand[i].value == 14) {
            aceCounter++;
        }
    }

    for(int i = 0; i < aceCounter; i++) {
        if (handValue + 11 <= 21) {
            handValue += 11;
        }else{
            handValue += 1;
        }
    }

    return handValue;
}

void dealer_turn(Card *deck, Card *dHand) {
    while (dHandValue < 17) {
        deal_card(deck, dHand, 1);
        dHandValue = count_hand_value(dHand, HAND_SIZE);
    }
}




int main() { 
    srand(time(NULL));              // Seed random number generator
    Card pHand[HAND_SIZE];
    Card dHand[HAND_SIZE];

    init_hand(pHand, HAND_SIZE);
    init_hand(dHand, HAND_SIZE);

    Card* deck = init_deck();
    fill_deck(deck);

    printf("\n\nShuffling deck...\n\n");
    shuffle_deck(deck);

    print_cards(deck, DECK_SIZE);

    deal_card(deck, pHand, 2);
    deal_card(deck, dHand, 2);
    
    print_cards(pHand, HAND_SIZE);
    print_cards(dHand, HAND_SIZE);

    pHandValue = count_hand_value(pHand, HAND_SIZE);
    dHandValue = count_hand_value(dHand, HAND_SIZE);

    printf("Player hand value: %d\n", pHandValue);
    printf("Dealer hand value: %d\n", dHandValue);

    for (int i = 0; i < 8; i++) {
        dealer_turn(deck, dHand);

        print_cards(dHand, HAND_SIZE);
        printf("Dealer hand value: %d\n", dHandValue);
    }
    

    return 0;
}
