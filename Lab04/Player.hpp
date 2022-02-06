//
//  Player.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/3/22.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "Actor.h"

class Player : public Actor
{
    public:
        //Constructor
        Player(class Game* game);
        class AnimatedSprite* spriteComponent;
        class CollisionComponent* collisionComponent;
        class PlayerMove* playerMove;
};

#endif /* Player_hpp */
