# Go Fish (C Version)

Welcome to **Go Fish**, a two-player card game (You vs. Computer), implemented in **C**!  
This terminal-based game allows you to play Go Fish against a basic AI. Shuffle, deal, collect books — and try to win before the computer does!

---

## About the Project

This is a terminal-based version of the classic card game "Go Fish", written in **C** with structured programming practices. The game includes:

- Deck shuffling and card dealing
- Player and computer turn logic
- Card searching, transferring, and drawing
- Book creation (sets of four matching ranks)
- Game end conditions and replay prompts

---

## Getting Started

### Prerequisites

You’ll need a C compiler like `gcc` installed.

### File Structure

Make sure you have the following source/header files:

main.c - The game loop (this file)
deck.h/.c - Deck creation, shuffling, card dealing
card.h/.c - Card structures and logic
player.h/.c - Player logic, book management, card transfer

### Compile

```bash
gcc main.c deck.c card.c player.c -o gofish
```

### Run
```bash
./gofish
```


### Skills Used in This Code

This project showcases fundamental and intermediate C programming skills and demonstrates the design of a logical, stateful application.

Some skills I've used throughout this project are:

#### 1. Core C Programming & Software Engineering

* **Modularity and Code Organization:** The use of multiple header files (`deck.h`, `player.h`, `card.h`) shows a strong understanding of software engineering principles. By following the desing principle, separation of concerns, the code is made easier to read, maintain, and debug than if it were all in one giant file.
* **Header Guards (implied):** A production-ready version of these header files would use header guards (`#ifndef`, `#define`, `#endif`) to prevent multiple inclusion errors, which is a critical skill in C/C++.
* **Function Decomposition:** The logic is broken down into small, single-purpose functions like `shuffle()`, `deal_player_cards()`, `check_add_book()`, and `game_over()`.

#### 2. Data Structures and Memory Management

* **Structs:** The use of `struct` is central to this project. `struct card` and `struct player` are used to model real-world objects, bundling related data (like a card's rank and suit, or a player's hand and books) into single, manageable units.
* **Linked Lists:** The player's hand is implemented as a linked list (`struct hand* current = user.card_list; ... current = current->next;`). This is an excellent choice because a player's hand size changes dynamically. It demonstrates a solid grasp of pointers and dynamic data structures, which are more complex than simple arrays.
* **Arrays:** The `player.book` array is used to store the ranks of completed books. This is an efficient way to keep track of a fixed number of potential books (13 in a standard deck, but here sized at 7 to win).
* **Pointers:** The code is rich with pointers (`&user`, `char* chosen_rank`, `struct card* new_card`). This demonstrates proficiency in manipulating memory addresses, which is essential for working with complex data structures, passing data efficiently to functions (pass-by-reference), and handling dynamically allocated memory.

#### 3. Algorithmic Thinking & Logic

* **Game Loop Architecture:** The `while (play_again == 1)` loop forms the core engine of the game. It correctly manages player turns, checks for win conditions, and handles the "play again" logic.
* **State Management:** The program successfully tracks numerous states: the contents of the deck, each player's hand, each player's books, and whose turn it is. This is a fundamental skill in any application development.
* **Randomization:** Using `time.h` and `stdlib.h` (presumably for `srand(time(NULL))` and `rand()` inside the `shuffle()` function) to properly shuffle the deck is a classic and important algorithm.
* **Search Algorithms:** The `search()` function is a simple linear search algorithm to check if a card of a specific rank exists in a player's hand (which is a linked list).

#### 4. Standard Library Proficiency

* **Input/Output (`stdio.h`):** Extensive use of `printf()` for displaying game information and `scanf()` for capturing user input.
* **String Manipulation (`string.h`):** The use of `strcmp()` is critical for comparing card ranks, which are stored as strings (e.g., comparing the drawn card's rank to the requested rank).
* **System Calls (`unistd.h`):** The use of `sleep()` and `usleep()` is a thoughtful touch. It demonstrates an understanding of how to control the program's execution speed to improve the user experience, making the game feel more paced and interactive.
