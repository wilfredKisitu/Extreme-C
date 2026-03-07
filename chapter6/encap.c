
// Variable Encapsulation
typedef struct {
    int x, y;
    int red, green, blue;
} pixel_t;

// Functionalities Encapsulation



int main(int argc, char const *argv[])
{
    pixel_t p1, p2;
    // Initialization of pixel p1
    p1.x = 56;
    p1.y = 34;
    p1.red = 123;
    p1.green = 37;
    p1.blue = 127;

    // Initialization of pixel p2
    p2.x = 212;
    p2.y = 994;
    p2.red = 127;
    p2.green = 127;
    p2.green = 0;

    return 0;
}
