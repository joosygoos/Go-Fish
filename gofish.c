#include "deck.h"
#include "player.h"
#include "card.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    // Initialize the game
    printf("Ready to play Go Fish?\n");
    int play_again = 1;
    for (int play = 1; play < 2; play++){
    sleep(1);
    // Shuffle deck
    if (shuffle() != 0) {
        printf("Error\n");
        return 1;
    }
    printf("Shuffling the deck...\n");
    sleep(1);
    printf("Shuffling complete!\n");
    sleep(1);
    // Deal cards to both players (user and computer)
    if (deal_player_cards(&user) != 0 || deal_player_cards(&computer) != 0) {
        printf("Error\n");
        return 1;
    }

    // Display starting hands (for debugging purposes, usually hidden in real gameplay)
    printf("\nYour starting hand:\n");
    struct hand* current = user.card_list;
    while (current != NULL) {
        printf("%s %c\n", current->top.rank, current->top.suit); //********************
        current = current->next;
    }

    // Game loop
    int turn = 1;  // 1 for user, 0 for computer
    while (play_again == 1) {
        //char chosen_rank;
        if (turn == 1) {
            // User's turn
            printf("\n--- Your turn! ---\n");
            usleep(800000);

            printf("Your current hand: \n");
            struct hand* update_hand;
            update_hand = user.card_list;
            while (update_hand!=NULL) {
                printf("%s %c\n", update_hand->top.rank, update_hand->top.suit);
                update_hand = update_hand->next;
            }
            printf("Your Book List:\n");
            for (int i = 0; i < 7; i++) {
                if (user.book[i] != NULL) {  // Check for valid entries
                    if (user.book[i][0] == '1') {  // Check for "10"
                        printf("10 ");  // Print "10" as a special case
                        //i++;  // Skip the next character since we printed "10"
                    } else {
                        printf("%c ", user.book[i][0]);  // Print single character rank
                    }
                }
            }
            printf("\n");
            char* chosen_rank = user_play(&user);
            // Check if the computer has the chosen rank
            usleep(800000);
            if (chosen_rank != NULL) {

                // Proceed with the game logic
                if (search(&computer, chosen_rank)) {
                    printf("You asked for %s, and the computer has it!\n", chosen_rank);
                    printf("Go again!\n");
                    transfer_cards(&computer, &user, chosen_rank);
                    char *book = check_add_book(&user);
                    turn = 0;
                    if (book != NULL) {
                        printf("You completed a book of %s.\n", book);
                        usleep(500000);
                        turn = 0;
                    }
                } else {
                    printf("Go Fish! Drawing a card...\n");
                    struct card* new_card = next_card();
                    add_card(&user, new_card);
                    printf("You drew a %s\n", new_card->rank);
                    if(strcmp(new_card->rank, chosen_rank) == 0){
                        printf(" Go again!");
                        turn = 0;
                    }
                    char *book = check_add_book(&user);
                    if (book != NULL) {
                        printf("You drew and completed a book of %s.\n", book);
                        turn=0;
                        usleep(500000);
                    }
                } 
            } else {
                printf("Choose a valid rank (case sensitive).\n");
            }
            sleep(1);

            // Check if the game is over
            if (game_over(&user)) {
                printf("Congratulations, you won!\nPlay again? [Y/N]: ");
                
                char yes_no;
                scanf(" %c", &yes_no);  // The space before %c helps to ignore any preceding whitespace (like newlines)
                
                if (yes_no == 'Y' || yes_no == 'y') {
                    play_again = 1;
                    play--;
                    reset_player(&user);
                    reset_player(&computer);
                } else {
                    play_again = 0;
                    printf("Thanks for playing Go Fish!\n");
                    //break;
                }
                
                break;
            }

            // }
            }


            else {
            // Computer's turn
                printf("\n--- Computer's turn! ---\n");
                usleep(800000);

                printf("Computer's Book List:\n");
                for (int i = 0; i < 7; i++) {
                    if (computer.book[i] != NULL) {  // Check for valid entries
                        if (computer.book[i][0] == '1') {  // Check for "10"
                            printf("10 ");  // Print "10" as a special case
                            //i++;  // Skip the next character since we printed "10"
                        } else {
                            printf("%c ", computer.book[i][0]);  // Print single character rank
                        }
                    }
                }
                printf("\n");
                sleep(1);

                char* chosen_rank = computer_play(&computer);
                if (chosen_rank == NULL) {
                    printf("Error: Opposing player has no cards.\n");//************ "Game end"
                    //exit(1);  // Exit or handle error gracefully
                }
                else{
                printf("Computer asks: Do you have any %s's?\n", chosen_rank);
                usleep(1500000);

                // Check if the user has the chosen rank
                if (search(&user, chosen_rank)) {
                    printf("You have %s, giving it to the computer.\n", chosen_rank);
                    transfer_cards(&user, &computer, chosen_rank);
                    turn = 1;

                    // Check if the computer completed a book
                    char *book = check_add_book(&computer);
                    if (book != NULL) {
                        printf("The computer completed a book of %s.\n", book);
                        usleep(500000);
                        turn = 1;
                    }

                }else{
                    printf("The computer goes fishing!\n");
                    struct card* new_card = next_card();
                    add_card(&computer, new_card);
                    if (strcmp(new_card->rank, chosen_rank) == 0) {
                        printf("Computer drew card %s! Going again!\n", chosen_rank);
                        turn = 1;
                    }
                    // Check if the computer completed a book
                    char *book = check_add_book(&computer);
                    if (book != NULL) {
                        printf("The computer drew and completed a book of %s.\n", book);
                        turn=1;
                        usleep(500000);
                    }
                }
                }
                sleep(1);

                // Check if the game is over
                if (game_over(&computer)) {
                    printf("The computer completed 7 books! Sorry, You lost!\nPlay again? [Y/N]: ");
                    char yes_no;
                    scanf(" %c", &yes_no);  // The space before %c helps to ignore any preceding whitespace (like newlines)
                    
                    if (yes_no == 'Y' || yes_no == 'y') {
                        play_again = 1;
                        play--;
                        // Initialize players
                        reset_player(&user);
                        reset_player(&computer);
                    } else {
                        play_again = 0;
                        printf("Thanks for playing Go Fish!\n");
                        //break;
                    }
                break;
                }
        }

        // Alternate turns
        turn = !turn;
        sleep(1);
        }
    }
    // End of game
// ************************** loop if char is Y, break if N

    // Initialize players
    reset_player(&user);
    reset_player(&computer);

    return 0;
    
}


