#include <object.cpp>
#include <gamemath.cpp>
#include <iostream>

class coordinates : public object{
    private :

        int x;
        int y;

    public :

        coordinates(){
            x = 1;
            y = 1;
        }
        coordinates(int x_coordinate, int y_coordinate){
            x = x_coordinate;
            y = y_coordinate;
        }

        coordinates(enum alignment position, int y_coordinate, int textLenght){
            y = y_coordinate; 
            x = calculateAlignmentX(position, textLenght);
        }

        void printCoordinates(){
            std::cout << "x = " << x << " y = " << y << std::endl;
        }

        void set_x(int i){x=i;}
        void set_y(int i){y=i;}
        int get_x(){return x;}
        int get_y(){return y;}
};