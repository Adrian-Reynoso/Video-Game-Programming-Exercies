//
//  PlayerMove.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/3/22.
//

#ifndef PlayerMove_hpp
#define PlayerMove_hpp

#include <stdio.h>
#include <vector>
#include "MoveComponent.h"
#include <unordered_map>
#include <SDL2/SDL_image.h>
#include "SDL2/SDL_mixer.h"

class PlayerMove : public MoveComponent
{
    public:
        //Constructor
        PlayerMove(class Player* owner);
    
        // Update the PlayerMove
        void Update(float deltaTime) override;
        void ProcessInput(const Uint8* keyState) override;
    
    private:
        //Map to detect leading edges
        void Jump(const Uint8* keyState);
        std::unordered_map<SDL_Scancode, bool> lastFrame;
        class Player* mPlayer;
        void SetAnim();
        class CollisionComponent* ownerCollisionComponent;
};

#endif /* PlayerMove_hpp */
