//
//  Collider.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/22/22.
//

#ifndef Collider_hpp
#define Collider_hpp

#include <stdio.h>
#include "Actor.h"

class Collider : public Actor
{
    public:
        //Constructor
        Collider(class Game* game, float width, float height);
        class CollisionComponent* collisionComponent;
};

#endif /* Collider_hpp */
