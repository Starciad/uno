#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "card.h"

#define MAX_CARDS 108 // Max hand size.
#define MAX_NAME_LEN 20

typedef struct player {
    char name[MAX_NAME_LEN];
    card hand[MAX_CARDS];
    int hand_size;
    bool is_ai;
} player;

extern void player_init(player* p, const char* name, const bool is_ai);
extern void players_shuffle(player* players, const int num_players);
extern void player_add_card(player* p, const card new_card);
extern void player_remove_card(player* p, const int index);
extern void player_print_hand(const player* p);

#endif // PLAYER_H
