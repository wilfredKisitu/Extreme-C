#ifndef PLAYER_HEADER_FILE
#define PLAYER_HEADER_FILE

struct player_t;

// Memory Allocator
struct player_t* player_new();

// Constructor
void player_ctor(struct player_t*, const char*);

// Destructor
void player_dtor(struct player_t*);

// Behavior functions
void player_pickup_gun(struct player_t*, struct gun_t*);
void player_shoot(struct player_t*);
void player_drop_gun(struct player_t*);

#endif