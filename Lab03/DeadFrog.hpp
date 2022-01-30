//
//  DeadFrog.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/29/22.
//

#ifndef DeadFrog_hpp
#define DeadFrog_hpp

#include <stdio.h>
#include "Actor.h"

class DeadFrog : public Actor
{
    public:
        //Constructor
        DeadFrog(class Game* game, float xPosition, float yPosition);
        class SpriteComponent* spriteComponent;
        void OnUpdate(float deltaTime);
    
    private:
        float screenWidth;
        float screenHeight;
        float maxHeight;
        float minHeight = 128.0;
        Game* mGame;
        float xPos;
        float yPos;
        float lifeTime = 0.0f;

};

#endif /* DeadFrog_hpp */
