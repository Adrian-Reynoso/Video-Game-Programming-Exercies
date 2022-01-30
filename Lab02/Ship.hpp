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
    
        void OnProcessInput(const Uint8* keyState) override;
        void OnUpdate(float deltaTime) override;
    
    private:
        float forwardVelocity = 300.0;
        float angularVelocity = 4.0;
        float coolDown = 0.0f;

        void laserShot(const Uint8 *keyboardState);
};

#endif /* Ship_hpp */
