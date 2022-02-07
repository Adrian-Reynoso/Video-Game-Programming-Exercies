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
        Vehicle(class Game* game, char letter, float dir);
        void OnUpdate(float deltaTime);
        class SpriteComponent* getSpriteComponent()
        {
            return spriteComponent;
        }
        class CollisionComponent* getCollisionComponent()
        {
            return collisionComponent;
        }
    
    private:
        class WrappingMove* wrappingMove;
        class SpriteComponent* spriteComponent;
        class CollisionComponent* collisionComponent;
};

#endif /* Vehicle_hpp */
