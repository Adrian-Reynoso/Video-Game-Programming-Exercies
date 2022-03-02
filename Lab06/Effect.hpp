//
//  Effect.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/1/22.
//

#ifndef Effect_hpp
#define Effect_hpp

#include <stdio.h>
#include "Actor.h"
#include "SDL2/SDL_mixer.h"
#include <string>

class Effect : public Actor
{
    public:
        //Constructor
        Effect(class Game* game, Vector2 position, std::string animName, std::string soundName);
        class AnimatedSprite* animatedSprite;
        void OnUpdate(float deltaTime) override;
    
    private:
        float lifetime;
};
#endif /* Effect_hpp */
