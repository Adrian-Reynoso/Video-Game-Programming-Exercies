//
//  PlayerMove.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/3/22.
//

#ifndef PlayerMove_hpp
#define PlayerMove_hpp

#include <stdio.h>
#include "Math.h"
#include "MoveComponent.h"
#include <vector>
#include "SDL2/SDL_mixer.h"
#include "CollisionComponent.h"

class PlayerMove : public MoveComponent
{
    public:
    PlayerMove(class Player* owner);
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void UpdateOnGround(float deltaTime);
    void UpdateJump(float deltaTime);
    void UpdateFalling(float deltaTime);
    
    enum MoveState
    {
        OnGround,
        Jump,
        Falling
    };
    
    void ChangeState(MoveState state);
    
    protected:
    CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);
    
    private:
    //Private functions for updating game
    class Player* mPlayer;
    float HDist = 60.0;
    float VDist = 70.0;
    float TargetDist = 50.0;
    std::vector<int> sideIndexPattern {0, 1, 2, 0};
    std::vector<int> topIndexPattern {6, 7};
    float forwardSpeed = 350.0f;
    MoveState mCurrentState;
    float mZSpeed = 0.0f;
    const float GRAVITY = -980.0f;
    const float JUMP_SPEED = 500.0f;
    bool spaceWasPressed = false;
};
#endif /* PlayerMove_hpp */
