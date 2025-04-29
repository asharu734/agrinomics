#ifndef CARDS_H
#define CARDS_H

typedef struct {
    int id;
    char *description;
} cardStruct;


cardStruct *create_cardStruct_array(int count);
void init_cardStruct (cardStruct *s, int id, const char *description);
void destroy_cardStruct_array(cardStruct *array, int count);
void insert_card_data(cardStruct *array, int count);

#endif