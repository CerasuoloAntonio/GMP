#include <object.cpp>
#include <coordinates.cpp>

struct node4d{
    object* oggetto;
    coordinates posizione;
    node4d* down;
    node4d* up;
    node4d* left;
    node4d* right;
};

struct pointerlist4d{
    node4d* pointer;
    pointerlist4d* next;
};