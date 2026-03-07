#include <stdio.h>
#include <stdlib.h>

typedef int bool_t;

struct Point {
    float x;
    float y;
    float z;
};

void manulate_x(struct Point* v, float newValue) {
    (*v).x = newValue;
}


void print_point(struct Point* v) {
    printf("<%f, %f, %f>\n", (*v).x, (*v).y, (*v).z);
}

void modify(int* ptr) {
    int b = 500;
    *ptr = b;
}


int* add(int a, int target_sum) {
    int *b = (int*)malloc(sizeof(int));
    *b = a + target_sum;
    return b;
}


int main(int argc, char const *argv[])
{
    int var = 100;
    int* var_ptr = NULL;
    var_ptr = &var;
    
    printf("Before changeing value: %d\n", var);

    *var_ptr = 400;
    printf("After pointer changes: %d\n", var);

    modify(var_ptr);
    printf("After functional pointer reference: %d\n", var);

    struct Point v;
    v.x = 4.0;
    v.y = 5.0;
    v.z = 7.0;

    struct Point* u = &v;

    print_point(u);

    void (*manipulate_x_ptr)(struct Point*, float);
    void (*print_point_ptr)(struct Point*);

    manipulate_x_ptr = &manulate_x;
    print_point_ptr = &print_point;

    manipulate_x_ptr(u, 8);
    print_point_ptr(u);

    // int Pointer;
    int* int_ptr = NULL;
    int a = 10;
    int_ptr = add(a, 20);
    printf("Value of a: %d\n", *int_ptr);
    printf("Test passed\n");

    free(int_ptr);
    
    return 0;
}
