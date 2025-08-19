#include "deck.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
/*
 * Variable: deck_instance
 * -----------------------
 *  
 * Go Fish uses a single deck
 */
struct deck deck_instance;
int shuffle(){
    // initalize deck
    char *ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};// array of pointers called ranks

    // ranks[9] = "10"  this is stored as ranks[9]={"1", "0", "/0"}
    char suits[] = {'H', 'D', 'C', 'S'};
    int card_index = 0;
    for (int i = 0; i < 5; i++){ // Looping through suits
        for (int j = 0; j < 13; j++){ // Assigning ranks per suit
            //deck_instance.list[card_index].rank = ranks[j];
            deck_instance.list[card_index].suit = suits[i];
            if (j == 9){
            deck_instance.list[card_index].rank[0] = ranks[j][0]; 
            deck_instance.list[card_index].rank[1] = ranks[j][1]; // could also use *(ranks+1)
            }
            else {
            deck_instance.list[card_index].rank[0] = *ranks[j];
            deck_instance.list[card_index].rank[1] = '\0';      // Add null terminator for single character ranks

            }
            card_index++;
        }
    }
    // initalizing deck complete
    //==========================
    // shuffle deck
    // 1 empty list of size 52 shuffled_deck_instance
    // assign each item in deck_instance to a random number rn
    // assign shuffled_deck_instance[rn] = deck_instance[index]
    if (NUM_CARDS != 52) {
        return 1;  // Error if deck size is not 52
    }
    srand(time(NULL));

    // Fisher-Yates shuffle algorithm
    for (int m = NUM_CARDS - 1; m > 0; m--){
        // Generate a random index between 0 and i
        int n = rand() % (m + 1);

        // Swap the cards at indices i and j
        struct card temp = deck_instance.list[m];
        deck_instance.list[m] = deck_instance.list[n];
        deck_instance.list[n] = temp;
    }

    // set index 1 to top card
    deck_instance.top_card = 0; //deck_instance.list[0];
    return 0;  // Return 0 if no errors
    //struct deck shuffled_deck_instance;
    

}

int deal_player_cards(struct player* target){
    struct hand* prev = NULL;  // To track the previous card
    struct hand* new_hand = NULL;  // Pointer to the new hand being created

    // Loop to deal NUM_CARDS cards from the deck
    for (int i = 0; i < HAND_SIZE; i++) {
        // Allocate memory for a new hand (a new node in the linked list)
        new_hand = (struct hand*)malloc(sizeof(struct hand));
        if (!new_hand) {
            printf("Memory allocation failed!\n");
            return 1;  // Exit on failure
        }
        if (deck_instance.top_card >= NUM_CARDS) {
            printf("No more cards left in the deck!\n");
            return 1;  // Exit if there are no cards left
    }
        // Assign the card from the deck to the hand
        new_hand->top = deck_instance.list[deck_instance.top_card];  // Copy the card from the deck to the player's hand
        new_hand->next = NULL;  // Initialize the next pointer to NULL
        deck_instance.top_card++;  // Move to the next card in the deck
        // If it's the first card, set it as the head of the player's hand
        if (target->card_list == NULL) {
            target->card_list = new_hand;  // Set the first node (card) in the hand
        } else {
            prev->next = new_hand;  // Link the previous card to the new one
        }
        // Update the previous pointer for the next iteration
        prev = new_hand;
    }
    return 0;
}
/*
 * Function: next_card
 * -------------------
 *  Return a pointer to the top card on the deck.
 *  Removes that card from the deck. 
 *
 *  returns: pointer to the top card on the deck.
 */
struct card* next_card( ){
    struct card* next_card = &deck_instance.list[deck_instance.top_card]; // integer to the top card
    deck_instance.top_card++;
    return next_card;
}
size_t deck_size( ){
    return NUM_CARDS - deck_instance.top_card;
}
