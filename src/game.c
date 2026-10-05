#include <string.h>
#include <unistd.h>
#include "ai.h"
#include "ansicolor.h"
#include "game.h"
#include "random.h"

// Predefined names for bots.
static const char* bot_names[] =
{
    "James", "Mary", "John", "Jennifer", "Robert", "Linda", 
    "Michael", "Elizabeth", "David", "Susan", "William", "Jessica", 
    "Joseph", "Sarah", "Charles", "Karen", "Thomas", "Nancy", 
    "Christopher", "Betty"
};

// =============================================================================== //
// Returns the index of the next player considering the game direction.
static int game_get_next_player_index(const game_status* g)
{
    int next_index = g->current_player_index + g->direction;

    // Handle wrap-around logic.
    if (next_index >= g->num_players)
    {
        return 0; // Wrap back to the first player.
    }
    else if (next_index < 0)
    {
        return g->num_players - 1; // Wrap back to the last player.
    }

    return next_index;
}

// Switches to the next player using game_get_next_player_index.
static void game_next_turn(game_status* g)
{
    g->current_player_index = game_get_next_player_index(g);
}

// Asks the player to call UNO.
static void check_uno_call(game_status* g, player* p)
{
    if (p->hand_size != 1)
    {
        return;
    }

    if (p->is_ai)
    {
        printf("{ %s calls UNO! }\n", p->name);
        return;
    }

    char response[4];
    printf("> You have one card left! Type 'UNO' to call it: ");
    scanf("%3s", response);

    if (strcmp(response, "UNO") != 0)
    {
        puts("{ You forgot to call UNO! Drawing 2 penalty cards. }");
        player_add_card(p, deck_draw(&g->deck));
        player_add_card(p, deck_draw(&g->deck));
    }
}

// =============================================================================== //
// Handles the effect of a "Skip" card.
static void apply_skip_effect(game_status* g)
{
    puts("{ Next player is skipped! }");
    game_next_turn(g);
}

// Handles the effect of a "Reverse" card.
static void apply_reverse_effect(game_status* g)
{
    puts("{ Changing direction! }");
    g->direction *= -1;
}

// Handles the effect of a "Draw Two" card.
static void apply_draw_two_effect(game_status* g)
{
    puts("{ Next player draws 2 cards! }");
    int next_player_index = game_get_next_player_index(g);
    game_next_turn(g);

    for (int i = 0; i < 2; i++)
    {
        player_add_card(&g->players[next_player_index], deck_draw(&g->deck));
    }
}

// Prompts or randomly selects a new color for WILD cards.
static card_color choose_wild_color(const game_status* g)
{
    int chosen_color = 0;

    if (g->players[g->current_player_index].is_ai)
    {
        chosen_color = random_int(0, 3); // AI chooses a random color.
    }
    else
    {
        printf("> Choose a color (0: Red | 1: Blue | 2: Green | 3: Yellow): ");
        scanf("%d", &chosen_color);
    }

    return (card_color)chosen_color;
}

// Converts a card_color to a readable string.
static const char* card_color_to_string(card_color color)
{
    switch (color)
    {
        case CARD_COLOR_RED:    return ANSI_COLOR_RED "Red" ANSI_COLOR_RESET;
        case CARD_COLOR_BLUE:   return ANSI_COLOR_BLUE "Blue" ANSI_COLOR_RESET;
        case CARD_COLOR_GREEN:  return ANSI_COLOR_GREEN "Green" ANSI_COLOR_RESET;
        case CARD_COLOR_YELLOW: return ANSI_COLOR_YELLOW "Yellow" ANSI_COLOR_RESET;
        default:                return "Unknown";
    }
}

// Handles the effect of a "Wild" card.
static void apply_wild_effect(game_status* g)
{
    puts("{ Choosing a new color... }");
    g->discard_pile.color = choose_wild_color(g);
    printf("{ The chosen color was: %s. }\n", card_color_to_string(g->discard_pile.color));
}

// Handles the effect of a "Wild Draw Four" card.
static void apply_wild_draw_four_effect(game_status* g)
{
    puts("{ Choosing a new color, next player draws 4 cards! }");
    int next_player_index = game_get_next_player_index(g);

    g->discard_pile.color = choose_wild_color(g);
    printf("{ The chosen color was: %s. }\n", card_color_to_string(g->discard_pile.color));

    game_next_turn(g);

    for (int i = 0; i < 4; i++)
    {
        player_add_card(&g->players[next_player_index], deck_draw(&g->deck));
    }
}

// Applies the effects of special cards.
static void apply_card_effect(game_status* g, const card* played_card)
{
    switch (played_card->type)
    {
        case CARD_SKIP_TYPE:
            apply_skip_effect(g);
            break;

        case CARD_REVERSE_TYPE:
            apply_reverse_effect(g);
            break;

        case CARD_DRAW_TWO_TYPE:
            apply_draw_two_effect(g);
            break;

        case CARD_WILD_TYPE:
            apply_wild_effect(g);
            break;

        case CARD_WILD_DRAW_FOUR_TYPE:
            apply_wild_draw_four_effect(g);
            break;

        default:
            break;
    }
}

// =============================================================================== //
// Prints the player list.
static void game_print_players(const game_status* g)
{
    puts("\n\n----------{ Player List }----------\n");

    switch (g->direction)
    {
        case GAME_DIRECTION_NONE:
            puts("[ Game Direction is Empty! ]\n");
            break;

        case GAME_DIRECTION_LEFT:
            puts("[ Game Direction: LEFT. ]\n");
            break;

        case GAME_DIRECTION_RIGHT:
            puts("[ Game Direction: RIGHT. ]\n");
            break;

        default:
            puts("[ Game Direction is Unknown! ]\n");
            break;
    }

    for (int i = 0; i < g->num_players; i++)
    {
        const player* p = &g->players[i];

        // Indicate the current player.
        if (i == g->current_player_index)
        {
            printf(">  ");
        }
        else
        {
            printf("   ");
        }

        // Highlight players who only have one card.
        if (p->hand_size == 1)
        {
            printf("[%d] %s - %d card(s) [UNO!]\n", i + 1, p->name, p->hand_size);
        }
        else
        {
            printf("[%d] %s - %d card(s)\n", i + 1, p->name, p->hand_size);
        }
    }

    printf("\n-----------------------------------");
}


// Print the current game state.
static void game_print_state(const game_status* g)
{
    // Print the current player.
    puts("\n");
    printf("> Current player: %s.\n", g->players[g->current_player_index].name);
    printf("> Current card on the Table: ");
    card_print(&g->discard_pile);
    puts(".\n");
}

// Print the main user's HUD.
static void game_print_hud_state(const game_status* g)
{
    // Prints the user hand of cards.
    puts("YOUR HAND:\n");

    for (int i = 0; i < g->players[g->current_player_index].hand_size; i++) {
        printf("%d. ", i + 1);
        card_print(&g->players[g->current_player_index].hand[i]);
        printf("\n");
    }

    printf("\n");
}

// =============================================================================== //
// Method responsible for handling player turn protocols.
static void game_execute_user_turn(game_status* g, player* p)
{
    int selected_card = 0;

    game_print_hud_state(g);

    try_again_action_label:
    printf("> Your turn! Select a card (1-%d) or enter 0 to draw: ", p->hand_size);
    scanf("%d", &selected_card);
    
    if (selected_card == 0)
    {
        puts("{ You drawn a card... }");
        card drawn_card = deck_draw(&g->deck);
        player_add_card(p, drawn_card);
        return;
    }

    selected_card--;

    if (selected_card < 0 || selected_card >= p->hand_size || !card_is_playable(&p->hand[selected_card], &g->discard_pile))
    {
        puts("{ Invalid move! Try again. }");
        goto try_again_action_label;
    }

    g->discard_pile = p->hand[selected_card];
    apply_card_effect(g, &g->discard_pile);
    player_remove_card(p, selected_card);
    check_uno_call(g, p);
}

// Method responsible for handling AI shift routines.
static void game_execute_ai_turn(game_status* g, player* p)
{
    card selected_card;

    if (!ai_try_select_card(p, &g->discard_pile, &selected_card))
    {
        printf("{ %s draws a card. }\n", p->name);
        card drawn_card = deck_draw(&g->deck);
        player_add_card(p, drawn_card);
        return;
    }

    // Find the index of the selected card in your hand.
    int selected_index = -1;
    for (int i = 0; i < p->hand_size; i++)
    {
        if (p->hand[i].color == selected_card.color &&
            p->hand[i].value == selected_card.value)
        {
            selected_index = i;
            break;
        }
    }

    if (selected_index == -1)
    {
        fprintf(stderr, "{ Error: AI selected a card that does not exist in hand. }\n");
        return;
    }

    printf("{ %s plays ", p->name);
    card_print(&selected_card);
    printf(". }\n");

    g->discard_pile = selected_card;
    apply_card_effect(g, &g->discard_pile);
    player_remove_card(p, selected_index);
    check_uno_call(g, p);
}

// =============================================================================== //

// Initializes the game, dealing the cards and setting up the players.
void game_init(game_status* g, const int num_players)
{
    g->num_players = num_players;
    g->current_player_index = 0;
    g->direction = GAME_DIRECTION_RIGHT;

    deck_init(&g->deck); // Create the deck of cards.

    // Shuffle the deck.
    int deck_shifting_count = random_int(MIN_DECK_SHIFTING_COUNT, MAX_DECK_SHIFTING_COUNT);
    
    for (int i = 0; i < deck_shifting_count; i++)
    {
        deck_shuffle(&g->deck); 
    }

    // Allows the player to choose their name.
    char player_name[50];
    printf("> Enter your name: ");
    scanf("%49s", player_name); // Limit input to prevent overflow.
    player_init(&g->players[0], player_name, false); // The first player is the human.

    // Deals cards to the player.
    for (int j = 0; j < 7; j++)
    {
        player_add_card(&g->players[0], deck_draw(&g->deck));
    }

    // Assigns names and distributes cards to bots.
    int bot_index = 1;
    for (int i = 1; i < num_players; i++)
    {
        player_init(&g->players[i], bot_names[bot_index - 1], true); // Name the bot

        for (int j = 0; j < 7; j++)
        {
            player_add_card(&g->players[i], deck_draw(&g->deck)); // Deals cards to bots
        }

        bot_index++;
    }

    // Shuffle players.
    int players_shifting_count = random_int(MIN_PLAYERS_SHIFTING_COUNT, MAX_PLAYERS_SHIFTING_COUNT);
    
    for (int i = 0; i < players_shifting_count; i++)
    {
        players_shuffle(g->players, num_players);
    }

    // Put a card in the discard pile.
    g->discard_pile = deck_draw(&g->deck);
    
    // Ensures the starting card is playable.
    while (!card_is_playable(&g->discard_pile, &g->discard_pile))
    {
        g->discard_pile = deck_draw(&g->deck);
    }

    puts("Your competitors are:\n");
    for (int i = 0; i < num_players; i++)
    {
        printf("%i. %s\n", i + 1, g->players[i].name);
    }
    puts("\nThe game has started! Good luck!");
    sleep(5);
}

// Method responsible for waiting for the user to press a key.
static void wait_for_exit(void)
{
    puts("{ Press any key to finish... }");
    getchar(); // Capture the previous ENTER.
    getchar(); // Waits for any key press.
}

// Updates the game loop to include UNO call.
void game_start(game_status* g)
{
    while (true)
    {
        game_print_players(g);
        game_print_state(g);

        player* p = &g->players[g->current_player_index];

        if (p->is_ai)
        {
            game_execute_ai_turn(g, p);
        }
        else
        {
            game_execute_user_turn(g, p);
        }

        if (p->hand_size == 0)
        {
            printf("{ %s WINS! }\n\n", p->name);
            wait_for_exit(); // Wait for the user before exiting.
            break;
        }

        sleep(2);
        game_next_turn(g);
    }
}
