#include <stdio.h>
#include <string.h>
#include "player.h"
#include "random.h"

// Initializes a player.
void player_init(player* p, const char* name, const bool is_ai)
{
    strncpy(p->name, name, MAX_NAME_LEN - 1);
    
    p->name[MAX_NAME_LEN - 1] = '\0'; // Ensure null-termination.
    p->hand_size = 0;
    p->is_ai = is_ai;
}

// Swaps two players in the array.
static void swap_players(player* a, player* b)
{
    player temp = *a;
    *a = *b;
    *b = temp;
}

// Fisher-Yates Shuffle algorithm using random_int().
void players_shuffle(player* players, const int num_players)
{
    for (int i = num_players - 1; i > 0; i--)
    {
        int j = random_int(0, i);
        swap_players(&players[i], &players[j]);
    }
}

// Adds a card to the player's hand.
void player_add_card(player* p, const card new_card)
{
    if (p->hand_size < MAX_CARDS)
    {
        p->hand[p->hand_size++] = new_card;
    }
}

// Removes a card from the player's hand.
void player_remove_card(player* p, const int index)
{
    if (index >= 0 && index < p->hand_size)
    {
        for (int i = index; i < p->hand_size - 1; i++)
        {
            p->hand[i] = p->hand[i + 1];
        }

        p->hand_size--;
    }
}

// Prints the player's hand.
void player_print_hand(const player* p)
{
    printf("%s's hand:\n", p->name);

    for (int i = 0; i < p->hand_size; i++)
    {
        printf("  %d: ", i + 1);
        card_print(&p->hand[i]);
        printf("\n");
    }
}
