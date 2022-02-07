//
//  Log.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/27/22.
//

#ifndef Log_hpp
#define Log_hpp

#include <stdio.h>
#include "Actor.h"

class Log : public Actor
{
    public:
        //Constructor
        Log(class Game* game, char letter, float dir);
        class SpriteComponent* getSpriteComponent()
        {
            return spriteComponent;
        }
        class CollisionComponent* getCollisionComponent()
        {
            return collisionComponent;
        }
        class WrappingMove* getWrappingMove()
        {
            return wrappingMove;
        }
    
    private:
        class SpriteComponent* spriteComponent;
        class CollisionComponent* collisionComponent;
        class WrappingMove* wrappingMove;
};

#endif /* Log_hpp */
