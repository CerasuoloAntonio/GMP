#include <entity.cpp>

class player : public entity{
    private :
        
    public :

        player(){
            position = getPosition();
        }

        void moveLeft(){
            if(position.get_x() > 2){
                position.set_x(position.get_x() - 1);
            }
        }
        void moveRight(){
            if(position.get_x() < screenLenght){
                position.set_x(position.get_x() + 1);
            }
        }
        void moveUp(){
            if(position.get_y() > 2){
                position.set_y(position.get_y() - 1);
            }
        }
        void moveDown(){
            if(position.get_y() < screenHeight + 1){
                position.set_y(position.get_y() + 1);
            }
        }
};