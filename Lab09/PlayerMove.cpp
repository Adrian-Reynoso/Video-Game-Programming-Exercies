//
//  PlayerMove.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/3/22.
//

#include "PlayerMove.hpp"
#include "Player.hpp"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "Random.h"
#include "CameraComponent.hpp"
#include "Block.hpp"

PlayerMove::PlayerMove(class Player* owner)
: MoveComponent(owner)
{
    mPlayer = owner;
    ChangeState(Falling);
}

void PlayerMove::Update(float deltaTime)
{
    //Check current state and call it's respective update function
    if (mCurrentState == OnGround)
    {
        UpdateOnGround(deltaTime);
    }
    else if (mCurrentState == Jump)
    {
        UpdateJump(deltaTime);
    }
    else
    {
        UpdateFalling(deltaTime);
    }
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
    //Forward/Backward
    if (keyState[SDL_SCANCODE_W])
    {
        SetForwardSpeed(forwardSpeed);
    }
    else if (keyState[SDL_SCANCODE_S])
    {
        SetForwardSpeed(forwardSpeed * -1);
    }
    else if ((keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_S]) ||(!keyState[SDL_SCANCODE_W] && !keyState[SDL_SCANCODE_S]))
    {
        SetForwardSpeed(0.0f);
    }
    
    //Strafing
    if (keyState[SDL_SCANCODE_D])
    {
        SetStrafeSpeed(forwardSpeed);
    }
    else if (keyState[SDL_SCANCODE_A])
    {
        SetStrafeSpeed(forwardSpeed * -1);
    }
    else if ((keyState[SDL_SCANCODE_D] && keyState[SDL_SCANCODE_A]) ||(!keyState[SDL_SCANCODE_D] && !keyState[SDL_SCANCODE_A]))
    {
        SetStrafeSpeed(0.0f);
    }
    
    //For jumping
    if (keyState[SDL_SCANCODE_SPACE] && !spaceWasPressed && mCurrentState == OnGround)
    {
        mZSpeed = JUMP_SPEED;
        ChangeState(Jump);
    }
    
    //Relative Mouse Movement
    int x, y;
    SDL_GetRelativeMouseState(&x, &y);
    
    //Calculate the angular speed and pass into MoveComponent
    float angularSpeed = (x / 500.0f) * Math::Pi * 10.0f;
    SetAngularSpeed(angularSpeed);
    
    //Calculate the angular speed and pass into MoveComponent
    float pitchSpeed = (y / 500.0f) * Math::Pi * 10.0f;
    mPlayer->cameraComponent->SetPitchSpeed(pitchSpeed);
    
    //For leading edge detection
    spaceWasPressed = keyState[SDL_SCANCODE_SPACE];
}

void PlayerMove::ChangeState(MoveState state)
{
    mCurrentState = state;
}

void PlayerMove::UpdateOnGround(float deltaTime)
{
    MoveComponent::Update(deltaTime);
    
    //Loop over all the blocks in the game and call FixCollision on each
    bool isOnBlock = false;
    for (Actor* obstacle : mOwner->GetGame()->GetBlockVector())
    {
        Block* otherBlock = (Block*) obstacle;
        CollSide onBlock = FixCollision(mPlayer->collisionComponent, otherBlock->collisionComponent);
        
        //If ANY of the FixCollision calls return Top, this means you landed
        if (onBlock == CollSide::Top)
        {
            isOnBlock = true;
        }
    }
    
    if (isOnBlock)
    {
        ChangeState(Falling);
    }
}

void PlayerMove::UpdateJump(float deltaTime)
{
    MoveComponent::Update(deltaTime);
    
    //Update mZSpeed based on gravity and delta time
    mZSpeed += GRAVITY * deltaTime;
    
    //Update the owner’s z position based on mZSpeed and delta time
    Vector3 tempPos = mOwner->GetPosition();
    tempPos.z += mZSpeed * deltaTime;
    mOwner->SetPosition(tempPos);
    
    //Loop over all the blocks in the game and call FixCollision on each
    for (Actor* obstacle : mOwner->GetGame()->GetBlockVector())
    {
        Block* otherBlock = (Block*) obstacle;
        CollSide onBlock = FixCollision(mPlayer->collisionComponent, otherBlock->collisionComponent);
        
        //If ANY of the FixCollision calls return Top, this means you landed
        if (onBlock == CollSide::Bottom)
        {
            mZSpeed = 0.0f;
            ChangeState(OnGround);
        }
    }
    
    //If the mZSpeed <= 0.0f. If it is, that means you reached the apex of your jump, and should start falling
    if (mZSpeed <= 0.0f)
    {
        ChangeState(Falling);
    }
}

void PlayerMove::UpdateFalling(float deltaTime)
{
    MoveComponent::Update(deltaTime);
    
    //Update mZSpeed based on gravity and delta time
    mZSpeed += GRAVITY * deltaTime;
    
    //Update the owner’s z position based on mZSpeed and delta time
    Vector3 tempPos = mOwner->GetPosition();
    tempPos.z += mZSpeed * deltaTime;
    mOwner->SetPosition(tempPos);
    
    //Loop over all the blocks in the game and call FixCollision on each
    for (Actor* obstacle : mOwner->GetGame()->GetBlockVector())
    {
        Block* otherBlock = (Block*) obstacle;
        CollSide onBlock = FixCollision(mPlayer->collisionComponent, otherBlock->collisionComponent);
        
        //If ANY of the FixCollision calls return Top, this means you landed
        if (onBlock == CollSide::Top)
        {
            mZSpeed = 0.0f;
            ChangeState(OnGround);
        }
    }
}

CollSide PlayerMove::FixCollision(class CollisionComponent* self, class CollisionComponent* block)
{
    //Get current position
    Vector3 tempPos = mOwner->GetPosition();
    
    //Call GetMinOverlap(), If side isn’t CollSide::None fix the position of the player based on offset.
        Vector3 offSet {0.0f, 0.0f, 0.0f};
    CollSide onBlock = mPlayer->collisionComponent->GetMinOverlap(block, offSet);

        if (onBlock != CollSide::None)
        {
            //Add offset to temPos
            tempPos += offSet;
            
            //Call setPosition
            mOwner->SetPosition(tempPos);
        }
    
    return onBlock;
}
