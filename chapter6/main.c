#include <stdio.h>
#include "car.h"

int main(int argc, char const *argv[])
{
    //  Create the object variable
    car_t car;

    // Construct object
    car_construct(&car, "Renault");

    // Main algorithm
    car_refuel(&car, 100.0);
    printf("Car is refueled, the cirrect fuel level is level: %f\n", car.fuel);

    while (car.fuel > 0) {
        printf("Car fuel level: %f\n", car.fuel);
        if(car.speed < 80) {
            car_accelerate(&car);
            printf("Car has been accelerated to the speed: %f\n", car.speed);
        } else {
            car_brake(&car);
            printf("Car has been slowed down to the speed: %f\n", car.speed);
        }
    }

    printf("Car ran out of fuel! Slowing down...\n");
    while (car.speed > 0) {
        car_brake(&car);
        printf("Car has been slowed down to the speed: %f\n", car.speed);
    }
    
    // Destruct the object
    car_destruct(&car);

    return 0;
}
