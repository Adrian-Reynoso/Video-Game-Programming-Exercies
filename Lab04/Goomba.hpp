//
//  Goomba.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/4/22.
//

#ifndef Goomba_hpp
#define Goomba_hpp

#include <stdio.h>
#include "Actor.h"

class Goomba : public Actor
{
    public:
        //Constructor
        Goomba(class Game* game);
        ~Goomba();
        class SpriteComponent* spriteComponent;
        class CollisionComponent* collisionComponent;
        class GoombaMove* goombaMove;
        bool stomp = false;
};

#endif /* Goomba_hpp */
