#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Gun.h"

// Attribute structure
typedef struct
{
    char *name;
    struct gun_t *gun;
} player_t;

// Memory allocator
player_t *player_new()
{
    return (player_t *)malloc(sizeof(player_t));
}

// Constructor
void player_ctor(player_t *player, const char *name)
{
    player->name = (char *)malloc((strlen(name) + 1) * sizeof(char));
    strcmp(player->name, name);
    // Required to nullify aggregation pointers
    player->gun = NULL;
}

// Destructor
void player_dtor(player_t *player)
{
    free(player->name);
}

// Behavior functions
void player_pickup_gun(player_t *player, struct gun_t *gun)
{
    // After the following line agggregation relation begins
    player->gun = gun;
}

void player_shoot(player_t *player)
{
    // Check if the player has picked up the gun
    if (player->gun)
    {
        gun_trigger(player->gun);
    }
    else
    {
        printf("Player wants to shoot but he doesn't have a gun!");
        exit(1);
    }
}

void player_drop_gun(player_t *player)
{
    // After the following line the aggregation relation
    // ends between two objects
    // The object gun has to be freed since this object is not
    // its owner like composition
    player->gun = NULL;
}