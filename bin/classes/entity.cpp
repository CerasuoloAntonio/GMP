#include <object.cpp>
#include <coordinates.cpp>

class entity : public object{
    
    protected :
        const int minHealth = 0;
        coordinates position;
        char sprite = '@';
        int attack;
        int maxHealth;

    public :
        int health;
        int defense;

    void setPosition(coordinates newPosition){position = newPosition;}
    coordinates getPosition(){return position;}
    char getEntitySprite(){return sprite;}

    entity(){
        maxHealth = 100;
        health = maxHealth;
        attack = 10;
        position = coordinates(2,2);
    }

    void attackEnemy(entity &enemy){
        enemy.health -= attack;
    }
};