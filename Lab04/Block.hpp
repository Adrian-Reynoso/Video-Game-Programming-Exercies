//
//  Block.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/3/22.
//

#ifndef Block_hpp
#define Block_hpp

#include <stdio.h>
#include "Actor.h"

class Block : public Actor
{
    public:
        //Constructor
        Block(class Game* game, char letter);
        ~Block();
        class SpriteComponent* spriteComponent;
        class CollisionComponent* collisionComponent;
};

#endif /* Block_hpp */
