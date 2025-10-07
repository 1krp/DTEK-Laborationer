#include "bj.h"
#include "../main.h"
#include "../display.h"
#include "../graphics/bgPxArrays.h"
#include "../graphics/cardPxArrays.h"

#define HAND_SIZE 10
#define DECK_SIZE 52

extern int randFT(int from, int to);
extern void print(const char*);
extern void print_dec(unsigned int);

// Card struct
typedef struct card {
    char *suit;
    int value;
    int imgIndex;
} Card;

void init_deck(Card *deck);
void fill_deck(Card *deck);
void shuffle_deck(Card *deck);
void init_hand(Card *hand);
void deal_card(Card *deck, Card *hand, int numOfCards);
void start_game(Card *pHand, Card *dHand, Card *deck);


// Global variables

//card to be dealt next
int topOfDeck = 0;

//total hand value
int pHandValue = 0;
int dHandValue = 0;

//cards in a hand
int pCardCounter = 0;
int dCardCounter = 0;

int pShownCardIndex = 0;
int dShownCardIndex = 0;

int roundStatus = 0; // 0 = ongoing, 1 = player win, 2 = dealer win, 3 = push
int Run = 0;
int pStand = 0;

//distance for the cards
int pY = 181;
int dY = 40;

int cardOffset[10] = {0,1,-1,2,-2,3,-3,4,-4,5};

Card deck[DECK_SIZE];
Card pHand[HAND_SIZE];
Card dHand[HAND_SIZE];

Card create_card(char *suit, int value) {
    Card new_card;
    new_card.suit = suit;
    new_card.value = value;
    new_card.imgIndex = 0;
    return new_card;
}

/*
Initializes a deck of DECK_SIZE empty cards
*/
void init_deck(Card *deck) {
    for (int i = 0; i < DECK_SIZE; i++) {
        deck[i].suit = "";
        deck[i].value = 0;
        deck[i].imgIndex = 0;
    }
}

/*
Initializes a hand of 3 empty cards
*/
void init_hand(Card *hand) {
    for (int i = 0; i < HAND_SIZE; i++) {
        hand[i].suit = "empty";
        hand[i].value = 0;
        hand[i].imgIndex = 0;
    }
}

/*
Assigns the values for each resepective card for all the empty cards
*/
void fill_deck(Card *deck) {
    for(int i = 0; i < 52; i++) {
        if(i < 13) {
            deck[i].suit = "H";
            deck[i].value = (i % 13) + 2;
            deck[i].imgIndex = i;
        } else if(i < 26) {
            deck[i].suit = "S";
            deck[i].value = (i % 13) + 2;
            deck[i].imgIndex = i;
        } else if(i < 39) {
            deck[i].suit = "C";
            deck[i].value = (i % 13) + 2;
            deck[i].imgIndex = i;
        } else if(i < 52) {
            deck[i].suit = "D";
            deck[i].value = (i % 13) + 2;
            deck[i].imgIndex = i;
        }
        print(deck[i].suit);
        print_dec(deck[i].value);
        print(": ");
        print_dec(deck[i].imgIndex);
        print("\n");

    }
    print("\n");
}

/*
Shuffels the Deck, using randFT, witch uses snaplo as seed
*/
void shuffle_deck(Card *deck) {
    int n = DECK_SIZE;
    Card temp;

    for (int i = n - 1; i > 0; --i) {
        int j = randFT(0, i);
        temp = deck[j];
        deck[j] = deck[i];
        deck[i] = temp;
    }

    print("Deck is shuffled!\n\n");
}
/*
All variabels are reset, initialises the hands and the deck with empty cards, shuffels, and deals and prints/displays the first four cards.
*/
void init_game(Card *pHand, Card *dHand, Card *deck) {

    print("Initializes game.\n\n");

    init_hand(pHand);
    init_hand(dHand);
    
    init_deck(deck);
    fill_deck(deck);
    shuffle_deck(deck);

    topOfDeck = 0;
    pHandValue = 0;
    dHandValue = 0;
    roundStatus = 0;

    print("Player start hand: \n");
    deal_card(deck, pHand, 2);
    pCardCounter = 2;

    displayCardImage(cardXOffset(1), pY, cardImageArr[pHand[0].imgIndex]);
    displayCardImage(cardXOffset(2), pY, cardImageArr[pHand[1].imgIndex]);

    print("Check card pixel value: ");
    print_dec(pHand[0].imgIndex);
    print(pHand[0].suit);
    print_dec(pHand[0].value);
    print("\n");
    print_dec(cardImageArr[pHand[0].imgIndex][0]);
    print("\n");

    print("Dealer start hand: \n");
    deal_card(deck, dHand, 2);
    print("\n");
    dCardCounter = 2;

    displayCardImage(cardXOffset(1), dY, cardImageArr[52]);
    displayCardImage(cardXOffset(2), dY, cardImageArr[52]);
}

/*
Deals card to a specific hand from the deck, it can deal multible cards in one call.
*/
void deal_card(Card *deck, Card *hand, int numOfCards) {
    int dealtCardsCounter = 0;

    for(int i = 0; i < 10; i++){
        if(hand[i].value == 0 && dealtCardsCounter < numOfCards){

            print("Top of deck: ");
            print_dec(topOfDeck);
            print("\n");


            print("Top card: ");
            print_dec(deck[topOfDeck].imgIndex);
            print(deck[topOfDeck].suit);
            print(" ");
            print_dec(deck[topOfDeck].value);
            print("\n");

            hand[i].suit = deck[topOfDeck].suit;
            hand[i].value = deck[topOfDeck].value;
            hand[i].imgIndex = deck[topOfDeck].imgIndex;
            topOfDeck++;
            dealtCardsCounter++;

            // Print drawn card
            print("Drawn card: ");
            print_dec(hand[i].imgIndex);
            print(hand[i].suit);
            print(" ");
            print_dec(hand[i].value);
            print("\n");

        }
    }
}
/*
Prints the cards from a card array to the terminal
*/
void print_cards(Card *hand, int size) {
    for (int i = 0; i < size; i++) {
        if (hand[i].value !=0) {
            print("Card ");
            print_dec(i + 1);
            print(": ");
            print(hand[i].suit);
            print_dec(hand[i].value);
            print("\n");
        }
    }
}
/*
Sums the value of the cards in a hand, Takes to account the opptimal value for a ACE (1/11)
*/
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
/*
Changes the roundStatus variable, if that variable is 0, game is ongoing else (1,2,3) some one has won/draw.
*/
void round_status(){
    if (pStand){
        if (pHandValue > dHandValue){
            roundStatus = 2; // player wins
        }
        if (dHandValue > pHandValue && dHandValue <= 21) {
            roundStatus = 2; // dealer win
        }
        if (dHandValue == pHandValue) {
            roundStatus = 3; // push
        }
    }
    if (pHandValue > 21) {
        roundStatus = 2; // dealer win
    } else if (dHandValue > 21) {
        roundStatus = 1; // player win
    } else if (dHandValue >= 17 && pHandValue == dHandValue) {
        roundStatus = 3; // push
    } else if (dHandValue >= 17 && pHandValue > dHandValue) {
        roundStatus = 1; // player win
    }
}
/*
Prints the winner to the terminal
*/
void print_winner(){
    if (roundStatus == 1) {
        print("Player wins!\n");
    } else if (roundStatus == 2) {
        print("Dealer wins!\n");
    } else if (roundStatus == 3) {
        print("It's a push!\n");
    }
}
/*
Returns the x cordinate to where the card should be printed, to a given handsize.
*/
int cardXOffset(int cardCounter) {
    return 132+29*cardOffset[cardCounter-1];
}



/*
"Flips" the dealers cards, a loop that Displays all the dealercards on the display.
*/
void displayDealersCards(){
    for (int i = 0; i < dCardCounter; i++){
        displayCardImage(cardXOffset(i), dY, cardImageArr[dHand[i].imgIndex]);
    }
}

/*
The main game loop, called from main.c
*/
int bjGameLoop() {

    print("Let's play!\n\n");

    /*
        Wait for player to start (push btn)
    */
    int started = 0;
    while (!started){
        if (get_btn()){
            delay(100);
            started = 1;
        }
    }

    init_game(pHand, dHand, deck);

    pHandValue = count_hand_value(pHand, HAND_SIZE);
    dHandValue = count_hand_value(dHand, HAND_SIZE);

    print_cards(pHand, HAND_SIZE);
    print("Player hand value: ");
    print_dec(pHandValue);
    print("\n\n");

    print_cards(dHand, HAND_SIZE);
    print("Dealer hand value: ");
    print_dec(dHandValue);
    print("\n\n");

    
    while (roundStatus == 0) {

        if (!pStand){

            print("Players turn: \n");

            int chosen = 0;
            int choice = 0;

            while (!chosen){
                if (get_btn()){
                    choice = get_sw()%2;
                    chosen = 1;
                }

                /*
                Show current switch number
                */
                if (get_sw()%2 == 1){
                    set_displays(5, 18);    // H
                    set_displays(4, 1);     // I
                    set_displays(3, 30);    // T
                    set_displays(2, 35);
                    set_displays(1, 35);
                } else if (get_sw()%2 == 0){
                    set_displays(5, 5);     // S
                    set_displays(4, 30);    // T
                    set_displays(3, 11);    // A
                    set_displays(2, 24);    // N
                    set_displays(1, 14);    // D
                }
            }

            if (choice == 1){ // Hit
                print("Player hits.\n");
                deal_card(deck,pHand,1);
                pCardCounter++;

                displayCardImage(cardXOffset(pCardCounter), pY, cardImageArr[pHand[pCardCounter-1].imgIndex]);

                pHandValue = count_hand_value(pHand, HAND_SIZE);

                print("Players new hand value: ");
                print_dec(pHandValue);
                print("\n\n");
            } else if (choice == 2){ // Stand
                print("Player stands.\n\n");
                pStand = 1;
            }
        }        
            
        if (dHandValue < 17){
            print("Dealers turn: \n");
            deal_card(deck, dHand, 1);
            dCardCounter++;
            displayCardImage(cardXOffset(dCardCounter), dY, cardImageArr[dHand[dCardCounter-1].imgIndex]);

            dHandValue = count_hand_value(dHand, HAND_SIZE);

            print("Dealers new hand value: ");
            print_dec(dHandValue);
            print("\n\n");
        }

        print_cards(pHand, HAND_SIZE);
        print("Player hand value: ");
        print_dec(pHandValue);
        print("\n\n");

        print_cards(dHand, HAND_SIZE);
        print("Dealer hand value: ");
        print_dec(dHandValue);
        print("\n\n");

        dHandValue = count_hand_value(dHand, HAND_SIZE);
        pHandValue = count_hand_value(pHand, HAND_SIZE);

        round_status();
    }

    print("\n--------------Round Over------------\n");


    displayDealersCards();

    print_cards(pHand, HAND_SIZE);
    print("Player hand value: ");
    print_dec(pHandValue);
    print("\n\n");

    print_cards(dHand, HAND_SIZE);
    print("Dealer hand value: ");
    print_dec(dHandValue);
    print("\n\n");

    print_winner();

    return roundStatus;
}