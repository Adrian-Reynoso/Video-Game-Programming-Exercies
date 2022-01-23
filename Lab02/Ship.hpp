//
//  Ship.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/22/22.
//

#ifndef Ship_hpp
#define Ship_hpp

#include <stdio.h>
#include "Actor.h"

class Ship : public Actor
{
    public:
        //Constructor
        Ship(class Game* game);
        class MoveComponent* moveComponent;
        class SpriteComponent* spriteComponent;
    
        void OnProcessInput(const Uint8* keyState);
    
    private:
        float forwardVelocity = 300.0;
        float angularVelocity = 4.0;
};

#endif /* Ship_hpp */
