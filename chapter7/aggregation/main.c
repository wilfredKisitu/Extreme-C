#include <stdio.h>
#include <stdlib.h>

#include "Gun.h"
#include "Player.h"

int main(int argc, char const *argv[])
{

    // Create and construct a gun
    struct gun_t *gun = gun_new();
    gun_ctor(gun, 3);

    // Create and construct a player
    struct player_t *player = player_new();
    player_ctor(player, "Billy");

    // Begin the aggregation relation
    player_pickup_gun(player, gun);

    // shoot until no bullets
    while (gun_has_bullets(gun))
    {
        player_shoot(player);
    }
    // Refill
    gun_refill(gun);

    // Shoot until no bullet is left
    while (gun_has_bullets(gun))
    {
        player_shoot(player);
    }

    // End the aggregation
    player_drop_gun(player);

    // Destruct and free the player object
    player_dtor(player);
    free(player);

    // Destruct and free the gun object
    gun_dtor(gun);
    free(gun);

    return 0;
}
