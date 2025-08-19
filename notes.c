#include "deck.h"
#include "player.h"
#include <stdio.h>

int main() {
    // Initialize the game
    printf("Welcome to Go Fish!\n");

    // Shuffle deck
    if (shuffle() != 0) {
        printf("Error shuffling the deck.\n");
        return 1;
    }

    // Deal cards to both players (user and computer)
    if (deal_player_cards(&user) != 0 || deal_player_cards(&computer) != 0) {
        printf("Error dealing cards.\n");
        return 1;
    }

    // Initialize players
    reset_player(&user);
    reset_player(&computer);

    // Display starting hands (for debugging purposes, usually hidden in real gameplay)
    printf("\nYour starting hand:\n");
    struct hand* current = user.card_list;
    while (current != NULL) {
        printf("Rank: %s Suit: %c\n", current->top.rank, current->top.suit);
        current = current->next;
    }

    // Game loop
    int turn = 1;  // 1 for user, 0 for computer
    while (1) {
        char chosen_rank;
        if (turn == 1) {
            // User's turn
            printf("\n--- Your turn! ---\n");
            chosen_rank = user_play(&user);

            // Check if the computer has the chosen rank
            if (search(&computer, chosen_rank)) {
                printf("You asked for %c, and the computer has it!\n", chosen_rank);
                transfer_cards(&computer, &user, chosen_rank);
            } else {
                printf("Go Fish! Drawing a card...\n");
                struct card* new_card = next_card();
                add_card(&user, new_card);
            }

            // Check if a book was completed
            char book = check_add_book(&user);
            if (book != 0) {
                printf("You completed a book of %c!\n", book);
            }

            // Check if the game is over
            if (game_over(&user)) {
                printf("Congratulations, you won!\n");
                break;
            }
        } else {
            // Computer's turn
            printf("\n--- Computer's turn! ---\n");
            chosen_rank = computer_play(&computer);
            printf("Computer asks: Do you have any %c's?\n", chosen_rank);

            // Check if the user has the chosen rank
            if (search(&user, chosen_rank)) {
                printf("You have %c, giving it to the computer.\n", chosen_rank);
                transfer_cards(&user, &computer, chosen_rank);
            } else {
                printf("The computer goes fishing!\n");
                struct card* new_card = next_card();
                add_card(&computer, new_card);
            }

            // Check if the computer completed a book
            char book = check_add_book(&computer);
            if (book != 0) {
                printf("The computer completed a book of %c.\n", book);
            }

            // Check if the game is over
            if (game_over(&computer)) {
                printf("The computer won the game.\n");
                break;
            }
        }

        // Alternate turns
        turn = !turn;
    }

    // End of game
    printf("Thanks for playing Go Fish!\n");
    return 0;
}
