#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cards.h"

cardStruct *create_cardStruct_array(count) {
    cardStruct *array = malloc(sizeof(cardStruct) * count);
    if(!array){
        perror("Failed to allocate cardStruct");
        return NULL;
    }

    for(int i = 0; i < count; i++){
        array[i].description = NULL;
    }

    return array;
}

void init_cardStruct (cardStruct *s, int id, const char *description) {
    s->id = id;
    s->description = malloc(strlen(description) + 1);
    if(s->description){
        perror("Failed to allocate description");
        free(s);
        return NULL;
    }

    strcpy(s->description, description);
}

void destroy_cardStruct_array(cardStruct *array, int count) {
    for(int i = 0; i < count; i++){
        free(array[i].description);
    }
    free(array);
}