#include <stdbool.h>
#include "ai.h"

// AI selects the best playable card.
bool ai_try_select_card(const player* p, const card* top_card, card* selected_card)
{
    int best_index = 255;

    // Prioritize action cards.
    for (int i = 0; i < p->hand_size; i++)
    {
        if (card_is_playable(&p->hand[i], top_card))
        {
            // Immediately play an action card.
            if (p->hand[i].value == CARD_VALUE_SKIP ||
                p->hand[i].value == CARD_VALUE_REVERSE ||
                p->hand[i].value == CARD_VALUE_DRAW_TWO)
            {
                *selected_card = p->hand[i];
                return true;
            }

            // Store first valid card.
            if (best_index == 255)
            {
                best_index = i;
            }
        }
    }

    // Play regular number cards first, save Wild cards.
    if (best_index != 255)
    {
        *selected_card = p->hand[best_index];
        return true;
    }

    // Last resort: play Wild cards.
    for (int i = 0; i < p->hand_size; i++)
    {
        if (p->hand[i].value == CARD_VALUE_WILD || p->hand[i].value == CARD_VALUE_WILD_DRAW_FOUR)
        {
            *selected_card = p->hand[i];
            return true;
        }
    }

    return false;
}
