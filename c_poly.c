#include <stdio.h>

//animal.h
struct animal_vtable {
    const char* (*sound)();
    //const int (*legs)();
};

struct animal {   // class name
    struct animal_vtable methods; //methods
    const char* name; //state variables
};

void describe(struct animal *a) {
    printf("%s makes \"%s\" sound.\n", a->name, a->methods.sound());
}



//cat.c
const char* cat_sound() {
    return "meow!";
}

struct animal cat = {{&cat_sound}, "cat"};

//dog.c
const char* dog_sound() {
    return "bark!";
}

struct animal dog = {{&dog_sound}, "dog"};


//main.c
int main() {
    describe(&cat);
    describe(&dog);

    return 0;
}