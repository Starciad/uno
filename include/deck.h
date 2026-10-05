#ifndef DECK_H
#define DECK_H

#include "card.h"

#define DECK_SIZE 108

typedef struct deck {
    card cards[DECK_SIZE];
    int top_index; // Tracks the next card to be drawn.
} deck;

extern void deck_init(deck* deck);
extern void deck_shuffle(deck* deck);
extern card deck_draw(deck* deck);

#endif // DECK_H
