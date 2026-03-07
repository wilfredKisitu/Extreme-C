struct sample_t
{
    char first;
    char second;
    char third;
    char forth;
};

void print_size(struct sample_t* var) {
    printf("Size %lu\n", sizeof(*var));
}

void print_byte(struct sample_t* var) {
    unsigned char* ptr = (unsigned char*)&var;

    for(int i = 0; i < sizeof(*var); i++, ptr++) {
        printf("%d\n", (unsigned int)*ptr);
    }
    printf("\n");
}






