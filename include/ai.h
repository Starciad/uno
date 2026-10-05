#ifndef AI_H
#define AI_H

#include "card.h"
#include "player.h"

extern bool ai_try_select_card(const player* p, const card* top_card, card* selected_card);

#endif // AI_H
