//
//  Sword.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/28/22.
//

#ifndef Sword_hpp
#define Sword_hpp

#include <stdio.h>
#include "Actor.h"

class Sword : public Actor
{
    public:
        //Constructor
        Sword(class Game* game);
        class CollisionComponent* collisionComponent;
};

#endif /* Sword_hpp */
