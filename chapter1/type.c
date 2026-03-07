#include <stdio.h>

typedef struct 
{  
    float x;
    float y;
    float z;
} Point;


void print_point(Point* v) {
    printf("<%f, %f, %f>", v->x, v->y, v->z);
}

void print_size(Point* v) {
    printf("Size %lu\n", sizeof(*v));
}

int main(int argc, char const *argv[])
{
    Point u;
    u.x = 4.0;
    u.y = 6.0;
    u.z = 10.4;

    print_size(&u);
    print_point(&u);

    return 0;
}


