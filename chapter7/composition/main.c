#include <stdio.h>
#include <stdlib.h>

#include "Car.h"

int main(int argc, char const *argv[])
{
    // Allocate memory for car object
    struct car_t* car = car_new();

    // Constructor
    car_ctor(car);

    printf("Engine temperature before starting the car: %f\n",
    car_get_engine_temperature(car));

    car_start(car);

    printf("Engine temperature after starting the car: %f\n",
    car_get_engine_temperature(car));

    car_stop(car);
    printf("Engine temperature after stopping the car: %f\n",
    car_get_engine_temperature(car));

    car_dtor(car);
    free(car);

    return 0;
}
