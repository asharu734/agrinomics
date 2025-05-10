#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

typedef struct {
    int money;
    int seed_price;
    int crop_price;
    int plots_owned;
} GameState;

// File and Dialogue
char* read_file(const char* filename);
char* run_dialogue(cJSON* root, const char* start_node, GameState* state);
GameState run_game(const char* filename);

// Serialization
char* serialize_game_state(GameState state);

#endif
