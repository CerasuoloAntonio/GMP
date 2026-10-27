#include "coordinates.h"
#include "gamenum.cpp"
#include "func.cpp"

coordinates::coordinates(){
    x = 1;
    y = 1;
}
coordinates::coordinates(int x_coordinate, int y_coordinate){
    x = x_coordinate;
    y = y_coordinate;
}

coordinates::coordinates(enum alignment position, int y_coordinate, int textLenght){
    y = y_coordinate; 
    x = calculateAlignmentX(position, textLenght);
}

void coordinates::printCoordinates(){
    std::cout << "x = " << x << " y = " << y << std::endl;
}

void coordinates::set_x(int i){x=i;}
void coordinates::set_y(int i){y=i;}
int coordianets::get_x(){return x;}
int coordianets::get_y(){return y;}