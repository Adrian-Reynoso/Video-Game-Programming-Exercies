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
        AddForce(mOwner->GetForward() * 700.0f);
    }
    if (keyState[SDL_SCANCODE_S])
    {
        AddForce(mOwner->GetForward() * -700.0f);
    }
    
    //Strafing
    if (keyState[SDL_SCANCODE_D])
    {
        AddForce(mOwner->GetRight() * 700.0f);
    }
    if (keyState[SDL_SCANCODE_A])
    {
        AddForce(mOwner->GetRight() * -700.0f);
    } 
    
    //For jumping
    if ((keyState[SDL_SCANCODE_SPACE] && !spaceWasPressed))
    {
        if  (mCurrentState == OnGround)
        {
            AddForce(mJumpForce);
            ChangeState(Jump);
        }
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
    PhysicsUpdate(deltaTime);
    
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
    
    if (!isOnBlock)
    {
        ChangeState(Falling);
    }
}

void PlayerMove::UpdateJump(float deltaTime)
{
    AddForce(mGravity);
    PhysicsUpdate(deltaTime);
    
    //Loop over all the blocks in the game and call FixCollision on each
    for (Actor* obstacle : mOwner->GetGame()->GetBlockVector())
    {
        Block* otherBlock = (Block*) obstacle;
        CollSide onBlock = FixCollision(mPlayer->collisionComponent, otherBlock->collisionComponent);
        
        //If ANY of the FixCollision calls return Bottom, this means you hit your head
        if (onBlock == CollSide::Bottom)
        {
            mVelocity.z = 0.0f;
        }
    }
    
    //If the mZSpeed <= 0.0f. If it is, that means you reached the apex of your jump, and should start falling
    if (mVelocity.z <= 0.0f)
    {
        ChangeState(Falling);
    }
}

void PlayerMove::UpdateFalling(float deltaTime)
{
    AddForce(mGravity);
    PhysicsUpdate(deltaTime);
    
    //Loop over all the blocks in the game and call FixCollision on each
    for (Actor* obstacle : mOwner->GetGame()->GetBlockVector())
    {
        Block* otherBlock = (Block*) obstacle;
        CollSide onBlock = FixCollision(mPlayer->collisionComponent, otherBlock->collisionComponent);
        
        //If ANY of the FixCollision calls return Top, this means you landed
        if (onBlock == CollSide::Top)
        {
            mVelocity.z = 0.0f;
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

void PlayerMove::PhysicsUpdate(float deltaTime)
{
    //acceleration = Force / mass
    mAcceleration = mPendingForces * (1.0f / mMass);
    
    //Update velocity
    mVelocity += mAcceleration * deltaTime;
    
    //Fix x/y velocity
    FixXYVelocity();
    
    //Update position
    Vector3 tempPos = mOwner->GetPosition();
    tempPos += mVelocity * deltaTime;
    mOwner->SetPosition(tempPos);
    
    //Update rotation
    mOwner->SetRotation(mOwner->GetRotation() + (mAngularSpeed * deltaTime));
    
    //Set mPendingForces to Vector3::Zero
    mPendingForces = Vector3::Zero;
}

void PlayerMove::FixXYVelocity()
{
    Vector2 xyVelocity {mVelocity.x, mVelocity.y};
    
    //If the length of xyVelocity is greater than a max speed of 400.0f, you want to change the length of xyVelocity to be exactly max speed
    
    if (xyVelocity.Length() > maxSpeed)
    {
        xyVelocity = Vector2::Normalize(xyVelocity);
        xyVelocity *= maxSpeed;
    }
    
    //If the current state is OnGround, you also need to apply braking
    if (mCurrentState == OnGround)
    {
        //If the x-component of mAcceleration is near zero, multiply xyVelocity.x by a braking factor of 0.9f
        if (Math::NearZero(mAcceleration.x) || oppositeSigns((int) mAcceleration.x, (int) xyVelocity.x))
        {
            xyVelocity.x *= 0.9f;
        }
        
        //Same for y component
        if (Math::NearZero(mAcceleration.y) || oppositeSigns((int) mAcceleration.y, (int) xyVelocity.y))
        {
            xyVelocity.y *= 0.9f;
        }
    }
    
    //Finally, update mVelocity.x/y to be xyVelocity.x/y
    mVelocity.x = xyVelocity.x;
    mVelocity.y = xyVelocity.y;
}
