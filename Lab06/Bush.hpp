//
//  Bush.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/22/22.
//

#ifndef Bush_hpp
#define Bush_hpp

#include <stdio.h>
#include "Actor.h"

class Bush : public Actor
{
    public:
        //Constructor
        Bush(class Game* game);
        ~Bush();
        class SpriteComponent* spriteComponent;
        class CollisionComponent* collisionComponent;
        class EnemyComponent* enemyComponent;
};

#endif /* Bush_hpp */
