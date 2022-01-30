//
//  Asteroid.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/23/22.
//

#ifndef Asteroid_hpp
#define Asteroid_hpp

#include <stdio.h>
#include "Actor.h"

class Asteroid : public Actor
{
    public:
        //Constructor
        Asteroid(class Game* game);
        ~Asteroid();
        class MoveComponent* moveComponent;
        class SpriteComponent* spriteComponent;
    
    protected:
        void OnUpdate(float deltaTime) override;
        
    private:
        Vector2 topLeft{0.0f, 0.0f};
        //We will know the dimensions of the window when game is passed in the constructor
        Vector2 botRight;
        float screenWidth;
        float screenHeight;
};

#endif /* Asteroid_hpp */
