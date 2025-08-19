#include "player.h"
#include <stdio.h>
#include <string.h> // For strcmp()
#include <stdbool.h> // For bool type
#include <time.h>

/*
 * Instance Variables: user, computer   
 * ----------------------------------
 *  
 *  We only support 2 users: a human and a computer
 */
struct player user;
struct player computer;

int add_card(struct player* target, struct card* new_card){
    // Allocate memory for a new hand node
    struct hand* new_hand = (struct hand*)malloc(sizeof(struct hand));
    // Check if memory allocation failed
    if (!new_hand) {
        printf("Memory allocation failed!\n");
        return 1;  // Return error code
    }
    // Copy the new card data into the new hand node
    new_hand->top = *new_card;
    new_hand->next = NULL;    
    // Add the new card to the player's hand (at the beginning of the list)
    if (target->card_list == NULL) {
        // If the hand is empty, the new card is the first card
        target->card_list = new_hand;
    } else {
        //================================
        struct hand* current = target->card_list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_hand;
        //================================
    }

    // Update the player's hand size
    target->hand_size++;

    return 0;  // Success
}
int remove_card(struct player* target, struct card* old_card) {
    if (target == NULL || target->card_list == NULL) {
        return 1;  
    }
    struct hand* current = target->card_list;
    struct hand* prev = NULL;
    // Loop through the player's hand to find the matching card
    while (current != NULL) {
        // Compare the card's rank and suit to find a match
        if (strcmp(current->top.rank, old_card->rank) == 0 && current->top.suit == old_card->suit) {
            // If the card to remove is the first card in the hand
            if (prev == NULL) {
                target->card_list = current->next;  // Update head of the list
            } else {
                prev->next = current->next;  // Skip the current card in the list
            }

            // Free the memory for the removed card
            free(current);

            // Update the player's hand size
            target->hand_size--;

            return 0;  // Success: card removed
        }

        // Move to the next card
        prev = current;
        current = current->next;
    }
    // If we reach here, the card was not found
    return 1;  // Error: card not found
}

char* check_add_book(struct player* target) {
    if (target == NULL || !target->card_list) {
        return NULL;
    }

    int rank_count[13] = {0};  // 13 possible ranks (A, 2-10, J, Q, K)
    char* ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    struct hand* current = target->card_list;

    while (current != NULL) {
        for (int i = 0; i < 13; i++) {
            if (strcmp(current->top.rank, ranks[i]) == 0) {
                rank_count[i]++;
                break;
            }
        }
        current = current->next;
    }

    for (int i = 0; i < 13; i++) {
        if (rank_count[i] == 4) {
            // Remove cards of that rank and add to book
            current = target->card_list;
            struct hand* prev = NULL;

            while (current != NULL) {
                if (strcmp(current->top.rank, ranks[i]) == 0) {
                    struct hand* temp = current;
                    if (prev == NULL) {
                        target->card_list = current->next;
                    } else {
                        prev->next = current->next;
                    }
                    current = current->next;
                    free(temp);
                    target->hand_size--;
                } else {
                    prev = current;
                    current = current->next;
                }
            }

            // Allocate memory for storing the rank in the book
            for (int j = 0; j < 7; j++) {
                if (target->book[j] == NULL) {
                    if (strcmp(ranks[i], "10") == 0) {
                        target->book[j] = malloc(3 * sizeof(char)); // Allocate for "10\0"
                        strcpy(target->book[j], "10");  // Copy "10" into the book
                    } else {
                        target->book[j] = malloc(2 * sizeof(char)); // Allocate for single character + '\0'
                        strcpy(target->book[j], ranks[i]);  // Copy the rank
                    }
                    break;
                }
            }
            return ranks[i];  // Return the rank of the completed book
        }
    }
    return NULL;  // No book added
}

/*
 * Function: check_add_book
 * ------------------------
 *  Check if a player has all 4 cards of the same rank.
 *  If so, remove those cards from the hand, and add the rank to the book.
 *  Returns after finding one matching set of 4, so should be called after adding each a new card.
 * 
 *  target: pointer to the player to check
 *  
 *  Return: a char that indicates the book that was added; return 0 if no book added.
 */

int search(struct player* target, char* rank) {
    // Ensure the player has a hand to search through
    if (!target || !target->card_list) {
        return 0;  // No cards in hand, so return 0
    }

    // Traverse the player's hand (linked list) to search for the rank
    struct hand* current = target->card_list;
    while (current != NULL) {
        if (strcmp(current->top.rank, rank) == 0) { // --top.rank[0]--
            return 1;  // Card with the requested rank found
        }
        current = current->next;  // Move to the next card in the hand
    }

    return 0;  // No card of the requested rank found
}

int transfer_cards(struct player* src, struct player* dest, char* rank) {
    if (!src || !dest || !src->card_list) {
        return -1;  // Error: invalid player or no cards in source's hand
    }

    int transferred_count = 0;
    struct hand* current = src->card_list;
    struct hand* prev = NULL;

    while (current != NULL) {
        // Check if the card rank matches the requested rank
        if (strcmp(current->top.rank, rank) == 0) {
            // Transfer this card to the destination player's hand
            struct card* transferred_card = &current->top;
            add_card(dest, transferred_card); 

            // Remove this card from the source player's hand
            if (prev == NULL) {
                // Card is at the head of the list
                src->card_list = current->next;
            } else {
                // Bypass the current card
                prev->next = current->next;
            }

            // Free the memory for the removed card
            struct hand* temp = current;
            current = current->next;  // Move to the next card
            free(temp);

            transferred_count++;  // Increment the transferred cards count
        } else {
            // Move to the next card, keeping track of the previous card
            prev = current;
            current = current->next;
        }
    }

    return transferred_count;  // Return number of cards transferred
}

int game_over(struct player* target) {
    if (!target) {
        return 0;  // Return 0 if the target player is NULL, meaning the game cannot be over
    }

    // Check if the player has 7 books
    // Assuming that the book array has space for at least 7 books
    for (size_t i = 0; i < sizeof(target->book) / sizeof(target->book[0]); i++) {
        if (target->book[i] == NULL) {
            return 0;  // If there's an empty slot in the book, the game is not over
        }
    }

    // If all slots are filled, the player has 7 books
    return 1;  // Game is over
}

int reset_player(struct player* target) {
    if (!target) {
        return -1;  // Return an error code if the target player is NULL
    }

    // Free the memory for the player's hand
    struct hand* current = target->card_list;
    struct hand* next;

    // Iterate through the linked list and free each node
    while (current != NULL) {
        next = current->next;  
        free(current);         
        current = next;        
    }

    // Reinitialize the player's card_list to NULL
    target->card_list = NULL;

    // Reset the book - assuming the book array can hold 7 cards
    for (size_t i = 0; i < sizeof(target->book) / sizeof(target->book[0]); i++) {
        target->book[i] = NULL;  // Reset each book slot to NULL character
    }

    // Reset the hand_size to 0
    target->hand_size = 0;

    return 0;  // Return 0 to indicate success
}

char* computer_play(struct player* target) {
    if (!target || target->card_list == NULL) {
        return NULL;  // Return NULL if the target is NULL or has no cards
    }
    // Create an array to hold valid ranks the computer can choose from
    char* valid_ranks[14];  // There are 13 ranks (A, 2-10, J, Q, K) + 1 for the null terminator
    int count = 0;
    // Traverse the player's hand to find valid ranks
    struct hand* current = target->card_list;
    while (current != NULL) {
        char* rank = current->top.rank;  // Assuming rank is stored as a string
        int exists = 0;
        // Check if the rank is already in valid_ranks
        for (int i = 0; i < count; i++) {
            if (strcmp(valid_ranks[i], rank) == 0) {
                exists = 1; // Rank already exists in the array
                break;
            }
        }
        // If it doesn't exist, add it to valid_ranks
        if (!exists) {
            valid_ranks[count++] = rank;  // Add the rank and increment count
        }
        current = current->next;  // Move to the next card
    }
    // If no valid ranks were found, return a null character
    if (count == 0) {
        return NULL;  
    }
    // Select a random index
    srand(time(NULL));  // Seed random number generator
    int random_index = rand() % count;

    return valid_ranks[random_index];  // Return the randomly selected rank
}


char* user_play(struct player* target) {
    if (!target || target->card_list == NULL) {
        printf("Error: Invalid player or empty hand.\n");
        return NULL;  // Return null character for an invalid player
    }

    //char input_rank[3]; // Buffer to store user input (e.g., "10", "J", "Q", etc.)
    char* input_rank = malloc(3 * sizeof(char)); // Allocate memory for input_rank
    if (!input_rank) {
        printf("Error: Memory allocation failed.\n");
        return NULL;  // Handle memory allocation failure
    }

    while (true) {
        printf("Enter the rank you wish to play (A, 2-10, J, Q, K): ");
        scanf("%s", input_rank); // Read user input

        // Check if the input is valid and of the correct format
        if (strlen(input_rank) > 2 || (input_rank[0] != '1' && 
            input_rank[0] != 'A' && input_rank[0] != 'J' && 
            input_rank[0] != 'Q' && input_rank[0] != 'K' &&
            !(input_rank[0] >= '2' && input_rank[0] <= '9'))) {
            printf("Error - Invalid rank. Please enter a valid rank.\n");
            continue; // Re-prompt user
        }

        // Check if the player has the requested rank in their hand
        bool has_card = false;
        struct hand* current = target->card_list;
        while (current != NULL) {
            if (strcmp(current->top.rank, input_rank) == 0) { // Compare ranks
                has_card = true; // Found a matching rank
                break;
            }
            current = current->next; // Move to the next card
        }

        if (!has_card) {
            printf("Error - must have at least one card from rank %s to play.\n", input_rank);
        } else {
            return input_rank; // Return the valid rank (convert to char)
        }
    }

}