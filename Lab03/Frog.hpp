//
//  Frog.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/27/22.
//

#ifndef Frog_hpp
#define Frog_hpp

#include <stdio.h>
#include <unordered_map>
#include "SDL2/SDL.h"
#include "Actor.h"
#include "Math.h"

class Frog : public Actor
{
    public:
        //Constructor
        Frog(class Game* game);
        void OnProcessInput(const Uint8* keyState);
        class SpriteComponent* spriteComponent;
    
    private:
        //Map to detect leading edges
        std::unordered_map<SDL_Scancode, bool> lastFrame;
        float screenWidth;
        float screenHeight;
        float maxHeight;
        float minHeight = 128.0;

};

#endif /* Frog_hpp */
