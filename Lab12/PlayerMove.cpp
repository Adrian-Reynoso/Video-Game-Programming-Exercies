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
    
    //For Running
    mPlayer->GetGame()->GetSound("Assets/Sounds/Running.wav");
    Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Running.wav"), -1);
    Mix_Pause(playerRunChannel);
}

PlayerMove::~PlayerMove()
{
    Mix_HaltChannel(playerRunChannel);
}

void PlayerMove::Update(float deltaTime)
{
    //Check current state and call it's respective update function
    if (mCurrentState == OnGround)
    {
        UpdateOnGround(deltaTime);
        
        //Check if velocity length is > 50.0f to play sound
        if (mVelocity.Length() > 50.0f)
        {
            Mix_Resume(playerRunChannel);
        }
        else
        {
            Mix_Pause(playerRunChannel);
        }
    }
    else if (mCurrentState == Jump)
    {
        UpdateJump(deltaTime);
        Mix_Pause(playerRunChannel);
    }
    else if (mCurrentState == Falling)
    {
        UpdateFalling(deltaTime);
        Mix_Pause(playerRunChannel);
    }
    else if (mCurrentState == WallClimb)
    {
        UpdateWallClimb(deltaTime);
        Mix_Resume(playerRunChannel);
    }
    else
    {
        UpdateWallRun(deltaTime);
        Mix_Resume(playerRunChannel);
    }
    
    //Check if player needs to be respawned
    if (mOwner->GetPosition().z < -750.0f || mPlayer->GetToBeRespawned())
    {
        mPlayer->SetToBeRespawned(false);
        mOwner->SetPosition(mPlayer->GetRespawnPosition());
        mOwner->SetRotation(0.0f);
        mVelocity = Vector3::Zero;
        mPendingForces = Vector3::Zero;
        ChangeState(Falling);
    }
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
    //Forward/Backward
    if (keyState[SDL_SCANCODE_W])
    {
        AddForce(mOwner->GetForward() * 700.0f);
    }
    else if (keyState[SDL_SCANCODE_S])
    {
        AddForce(mOwner->GetForward() * -700.0f);
    }
    else if ((keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_S]) ||(!keyState[SDL_SCANCODE_W] && !keyState[SDL_SCANCODE_S]))
    {
        SetForwardSpeed(0.0f);
    }

    //Strafing
    if (keyState[SDL_SCANCODE_D])
    {
        AddForce(mOwner->GetRight() * 700.0f);
    }
    else if (keyState[SDL_SCANCODE_A])
    {
        AddForce(mOwner->GetRight() * -700.0f);
    }
    else if ((keyState[SDL_SCANCODE_D] && keyState[SDL_SCANCODE_A]) ||(!keyState[SDL_SCANCODE_D] && !keyState[SDL_SCANCODE_A]))
    {
        SetStrafeSpeed(0.0f);
    }

    //For jumping
    if (keyState[SDL_SCANCODE_SPACE] && !spaceWasPressed && mCurrentState == OnGround)
    {
        //Play Sound
        Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
        
        AddForce(mJumpForce);
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

        //Check if you can do a wall climb
        else if (onBlock == CollSide::Left || onBlock == CollSide::Right || onBlock == CollSide::Front || onBlock == CollSide::Back)
        {
            if (CanWallClimb(onBlock))
            {
                //Change state to wall climb and set timer to 0
                mWallClimbTimer = 0.0f;
                ChangeState(WallClimb);
                return;
            }
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

        //If ANY of the FixCollision calls return Top, this means you landed
        if (onBlock == CollSide::Bottom)
        {
            mVelocity.z = 0.0f;
            ChangeState(OnGround);
        }

        //Check if you can do a wall climb
        else if (onBlock == CollSide::Left || onBlock == CollSide::Right || onBlock == CollSide::Front || onBlock == CollSide::Back)
        {
            if (CanWallClimb(onBlock))
            {
                //Change state to wall climb and set timer to 0
                mWallClimbTimer = 0.0f;
                ChangeState(WallClimb);
                return;
            }
            else if (CanWallRun(onBlock))
            {
                //Change state to wall climb and set timer to 0
                isWallRunning = true;
                isRunningOn = onBlock;
                mWallRunTimer = 0.0f;
                ChangeState(WallRun);
                return;
            }
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
            //Play Sound
            Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Land.wav"), 0);
            
            mVelocity.z = 0.0f;
            ChangeState(OnGround);
        }
    }
}

void PlayerMove::UpdateWallClimb(float deltaTime)
{
    //Update the timer variable, and then only add the wall climb force if the timer is less than 0.4f
    mWallClimbTimer += deltaTime;
    AddForce(mGravity);
    if (mWallClimbTimer < 0.4f)
    {
        AddForce(wallClimbForce);
    }
    PhysicsUpdate(deltaTime);

    //Loop over all the blocks in the game and call FixCollision on each
    bool collidedWithWall = false;
    for (Actor* obstacle : mOwner->GetGame()->GetBlockVector())
    {
        Block* otherBlock = (Block*) obstacle;
        CollSide onBlock = FixCollision(mPlayer->collisionComponent, otherBlock->collisionComponent);

        if (onBlock == CollSide::Left || onBlock == CollSide::Right || onBlock == CollSide::Front || onBlock == CollSide::Back)
        {
            collidedWithWall = true;
        }
    }

    //If player did not collide with the Left/Right/Front/Back side of at least one block
    if (!collidedWithWall || mVelocity.z <= 0.0f)
    {
        //Then set mVelocity.z to 0.0f and change the state to Falling
        mVelocity.z = 0.0f;
        ChangeState(Falling);
    }
}

void PlayerMove::UpdateWallRun(float deltaTime)
{
    //Update the timer variable, and then only add the wall run force if the timer is less than 0.4f
    mWallRunTimer += deltaTime;
    AddForce(mGravity);
    if (mWallRunTimer < 0.4f)
    {
        AddForce(mWallRunForce);
    }
    PhysicsUpdate(deltaTime);

    //Loop over all the blocks in the game and call FixCollision on each
    for (Actor* obstacle : mOwner->GetGame()->GetBlockVector())
    {
        Block* otherBlock = (Block*) obstacle;
        FixCollision(mPlayer->collisionComponent, otherBlock->collisionComponent);
    }

    //If player z velocity is negative OR 0, change state to falling
    if (mVelocity.z <= 0.0f)
    {
        //Then set mVelocity.z to 0.0f and change the state to Falling
        isWallRunning = false;
        isRunningOn = CollSide::None;
        mVelocity.z = 0.0f;
        ChangeState(Falling);
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
    
    if (onBlock == CollSide::Left || onBlock == CollSide::Right || onBlock == CollSide::Front || onBlock == CollSide::Back)
    {
        //Find the normal of the collSide
        Vector3 normal;
        if (onBlock == CollSide::Left)
        {
            normal.Set(0.0f, -1.0f, 0.0f);
        }
        else if (onBlock == CollSide::Right)
        {
            normal.Set(0.0f, 1.0f, 0.0f);
        }
        else if (onBlock == CollSide::Front)
        {
            normal.Set(1.0f, 0.0f, 0.0f);
        }
        else if (onBlock == CollSide::Back)
        {
            normal.Set(-1.0f, 0.0f, 0.0f);
        }
        
        //Multiply the normal by 700 and add that force
        normal *= 700.0f;
        AddForce(normal);
        
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
    float xComp = xyVelocity.x * xyVelocity.x;
    float yComp = xyVelocity.y * xyVelocity.y;
    float squareSum = xComp + yComp;
    squareSum = Math::Sqrt(squareSum);

    if (squareSum > maxSpeed)
    {
        xyVelocity = Vector2::Normalize(xyVelocity);
        xyVelocity *= maxSpeed;
    }

    //If the current state is OnGround OR WallClimb, you also need to apply braking
    if (mCurrentState == OnGround || mCurrentState == WallClimb)
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

bool PlayerMove::CanWallClimb(CollSide collSide)
{
    bool checker = true;

    //Find the normal of the collSide
    Vector3 normal;
    if (collSide == CollSide::Left)
    {
        normal.Set(0.0f, -1.0f, 0.0f);
    }
    else if (collSide == CollSide::Right)
    {
        normal.Set(0.0f, 1.0f, 0.0f);
    }
    else if (collSide == CollSide::Front)
    {
        normal.Set(1.0f, 0.0f, 0.0f);
    }
    else if (collSide == CollSide::Back)
    {
        normal.Set(-1.0f, 0.0f, 0.0f);
    }


    //Test for three things:
    //1) The player is “roughly” facing towards that side
    if (!(Vector3::Dot(normal, mPlayer->GetForward()) < -0.9f))
    {
        checker = false;
    }

    //2) The xy-velocity of the player is “roughly” moving towards that side
    Vector3 xyVelocity {mVelocity.x, mVelocity.y, 0.0f};
    if (!(Vector3::Dot(normal, Vector3::Normalize(xyVelocity)) < -0.9f))
    {
        checker = false;
    }

    //3) The xy-velocity of the player has a magnitude greater than 350.0f
    float compX = xyVelocity.x * xyVelocity.x;
    float compY = xyVelocity.y * xyVelocity.y;
    float sqrtSum = Math::Sqrt(compX + compY);
    if (sqrtSum <= 350.0f)
    {
        checker = false;
    }

    return checker;
}

bool PlayerMove::CanWallRun(CollSide collSide)
{
    bool checker = true;

    //Find the normal of the collSide
    Vector3 normal;
    if (collSide == CollSide::Left)
    {
        normal.Set(0.0f, -1.0f, 0.0f);
    }
    else if (collSide == CollSide::Right)
    {
        normal.Set(0.0f, 1.0f, 0.0f);
    }
    else if (collSide == CollSide::Front)
    {
        normal.Set(1.0f, 0.0f, 0.0f);
    }
    else if (collSide == CollSide::Back)
    {
        normal.Set(-1.0f, 0.0f, 0.0f);
    }
    
    //Test for three things:
    //1) The player is “roughly” perpendicular towards that side
    if (Math::Acos(Vector3::Dot(normal, mPlayer->GetForward())) > 2.3f)
    {
        checker = false;
    }

    //2) The xy-velocity of the player is “roughly” moving towards the forward direction of the player
    Vector3 xyVelocity {mVelocity.x, mVelocity.y, 0.0f};
    if (Vector3::Dot(mPlayer->GetForward(), Vector3::Normalize(xyVelocity)) < 0.0f)
    {
        checker = false;
    }

    //3) The xy-velocity of the player has a magnitude greater than 350.0f
    float compX = xyVelocity.x * xyVelocity.x;
    float compY = xyVelocity.y * xyVelocity.y;
    float sqrtSum = Math::Sqrt(compX + compY);
    if (sqrtSum <= 350.0f)
    {
        checker = false;
    }

    return checker;
}


