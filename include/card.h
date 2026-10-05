#ifndef CARD_H
#define CARD_H

#include <stdbool.h>

typedef enum card_color {
    CARD_COLOR_RED = 0,
    CARD_COLOR_BLUE = 1,
    CARD_COLOR_GREEN = 2,
    CARD_COLOR_YELLOW = 3,
    CARD_COLOR_BLACK = 4 // Wild cards.
} card_color;

typedef enum card_value {
    CARD_VALUE_NONE = -1,
    CARD_VALUE_0 = 0,
    CARD_VALUE_1 = 1,
    CARD_VALUE_2 = 2,
    CARD_VALUE_3 = 3,
    CARD_VALUE_4 = 4,
    CARD_VALUE_5 = 5,
    CARD_VALUE_6 = 6,
    CARD_VALUE_7 = 7,
    CARD_VALUE_8 = 8,
    CARD_VALUE_9 = 9,
    CARD_VALUE_SKIP = 10,
    CARD_VALUE_REVERSE = 11,
    CARD_VALUE_DRAW_TWO = 12,
    CARD_VALUE_WILD = 13,
    CARD_VALUE_WILD_DRAW_FOUR = 14
} card_value;

typedef enum card_type {
    CARD_NUMBER_TYPE = 0,
    CARD_SKIP_TYPE = 1,
    CARD_REVERSE_TYPE = 2,
    CARD_DRAW_TWO_TYPE = 3,
    CARD_WILD_TYPE = 4,
    CARD_WILD_DRAW_FOUR_TYPE = 5
} card_type;

typedef struct card {
    card_color color;
    card_type type;
    card_value value;
} card;

extern void card_print(const card* c);
extern bool card_is_playable(const card* c, const card* top_card);

#endif // CARD_H
