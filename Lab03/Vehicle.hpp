//
//  Vehicle.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/27/22.
//

#ifndef Vehicle_hpp
#define Vehicle_hpp

#include <stdio.h>
#include "Actor.h"

class Vehicle : public Actor
{
    public:
        //Constructor
        Vehicle(class Game* game, char letter);
        class SpriteComponent* spriteComponent;
};

#endif /* Vehicle_hpp */
