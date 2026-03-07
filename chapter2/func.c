#include <stdio.h>
#include "func.h"

void print_point(point_t* v) {
    printf("<%d, %d>\n", v->x, v->y);
}

void print_bytes(point_t* v) {
    unsigned char* ptr = (unsigned char*) &v;

    for(int i = 0; i < sizeof(*v); i++, ptr++){
        printf("%d", (unsigned int)*ptr);
    }
    printf("\n");
}

void say_hello(){
    printf("Hello, how is it going!");
}

double average(int* arrray, int length){
    if(length <= 0) {
        return 0;
    }
    double sum = 0.0;
    for(int i = 0; i < length; i++){
        sum += arrray[i];
    }
    return sum / length;

}

int main(int argc, char const *argv[])
{
    point_t v;
    v.x = 4.0;
    v.y = 6.0;

    print_bytes(&v);
    print_point(&v);

    return 0;
}
