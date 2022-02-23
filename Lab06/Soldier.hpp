//
//  Soldier.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/22/22.
//

#ifndef Soldier_hpp
#define Soldier_hpp

#include <stdio.h>
#include "Actor.h"

class Soldier : public Actor
{
    public:
        //Constructor
        Soldier(class Game* game);
        class AnimatedSprite* spriteComponent;
        class CollisionComponent* collisionComponent;
};

#endif /* Soldier_hpp */
