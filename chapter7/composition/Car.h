#ifndef CAR_HEADER_FILE
#define CAR_HEADER_FILE

// Forward declaration
struct car_t;

// Memory Allocator
struct car_t* car_new();

// Construtor
void car_ctor(struct car_t*);

// Destuctor 
void car_dtor(struct car_t*);

// Behavior function
void car_start(struct car_t*);
void car_stop(struct car_t*);
double car_get_engine_temperature(struct car_t*);

#endif