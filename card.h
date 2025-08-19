#ifndef CARD_H
#define CARD_H

/*
  Valid suits: C, D, H, and S
  Valid ranks: 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K, A
*/
struct card
{
  char suit;
  char rank[3]; // an array of up to two characterss

};

/*
  Linked list of cards in hand.
    top: first card in hand
    next: pointer to next card in hand
*/
struct hand //linked list
{
  struct card top; //this is the head of the linked list
  struct hand* next; // declares a pointer named "next" of structure-type "hand" 
  // ?? Why do we want a pointer to another hand structure. Wouldn't we want it 
  //    to point to the next card?
};

#endif
