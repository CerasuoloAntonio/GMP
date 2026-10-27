#ifndef COORDINATES_H
#define COORDINATES_H

#include "includepath/util.h"
#include <iostream>

class coordinates : public object{
    private :

        int x;
        int y;

    public :

        coordinates();
        coordinates(int x_coordinate, int y_coordinate);
        coordinates(enum alignment position, int y_coordinate, int textLenght);

        void printCoordinates();

        void set_x(int i){x=i;}
        void set_y(int i){y=i;}
        int get_x(){return x;}
        int get_y(){return y;}
};

#endif