#ifndef GAME_H
#define GAME_H

#include "deck.h"
#include "player.h"

#define MIN_PLAYERS_SHIFTING_COUNT 10
#define MAX_PLAYERS_SHIFTING_COUNT 1000

#define MIN_DECK_SHIFTING_COUNT 10
#define MAX_DECK_SHIFTING_COUNT 1000

#define MAX_PLAYERS 10

typedef enum game_direction {
    GAME_DIRECTION_NONE = 0,
    GAME_DIRECTION_LEFT = -1,
    GAME_DIRECTION_RIGHT = 1
} game_direction;

typedef struct game_status {
    deck deck;
    player players[MAX_PLAYERS];
    int num_players;
    int current_player_index;
    card discard_pile;
    game_direction direction;
} game_status;

extern void game_init(game_status* g, const int num_players);
extern void game_start(game_status* g);

#endif // GAME_H
