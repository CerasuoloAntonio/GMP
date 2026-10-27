#ifndef ENTITY_H
#define ENTITY_H

#include "includepath/util.h"

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

        void attackEnemy(entity &enemy);

        void setPosition(coordinates newPosition){position = newPosition;}
        coordinates getPosition(){return position;}
        char getEntitySprite(){return sprite;}

        entity();

};

#endif