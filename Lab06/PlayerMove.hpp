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
#include "Math.h"

class PlayerMove : public MoveComponent
{
    public:
        //Constructor
        PlayerMove(class Player* owner);
    
        // Update the PlayerMove
        void Update(float deltaTime) override;
        void ProcessInput(const Uint8* keyState) override;
        void setSwordPos();
    
    private:
        //Map to detect leading edges
        void Jump(const Uint8* keyState);
        std::unordered_map<SDL_Scancode, bool> lastFrame;
        class Player* mPlayer;
        float mVelocity = 150.0f;
        Vector2 playerDir {0.0f, 0.0f};
        bool isMoving;
        void SetAnim();
        class CollisionComponent* ownerCollisionComponent;
        
        //For storing directions
        enum Direction
        {
            up,
            down,
            left,
            right
        };
        Direction playerState = Direction::down;
        bool isSpacePressed = false;
        float attackCooldown = 1.0f;
        class Sword* sword;
        Vector2 posOfSword;
};

#endif /* PlayerMove_hpp */
