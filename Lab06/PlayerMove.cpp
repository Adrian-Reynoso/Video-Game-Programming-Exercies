//
//  PlayerMove.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/3/22.
//

#include "PlayerMove.hpp"
#include "Actor.h"
#include "Player.hpp"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "AnimatedSprite.h"
#include "Game.h"
#include "Collider.hpp"

PlayerMove::PlayerMove(class Player* owner)
:MoveComponent(owner)
{
    mPlayer = owner;
    ownerCollisionComponent = mOwner->GetComponent<CollisionComponent>();
}

void PlayerMove::Update(float deltaTime)
{
    //Set camera position to player position plus an offset of (-256.0f, -224.0f)
    mOwner->GetGame()->cameraPosition = mPlayer->GetPosition() + Vector2{-256.0f, -224.0f};
    
    //Update the players movement
    mPlayer->SetPosition(mPlayer->GetPosition() + (playerDir * mVelocity * deltaTime));
    
    //Check if Link collides with objects
    Vector2 tempPos = mPlayer->GetPosition();
    CollSide onObject = CollSide::None;
    for (Collider* collider : mOwner->GetGame()->GetColliderVector())
    {
        Vector2 offSet {0.0f, 0.0f};
        onObject = ownerCollisionComponent->GetMinOverlap(collider->collisionComponent, offSet);

        if (onObject != CollSide::None)
        {
            //Add offset to temPos
            tempPos += offSet;
            
            //Call setPosition
            mOwner->SetPosition(tempPos);
        }
    }
    
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
    //Based off the keystate, determine the direction of the our member variable which stores direction
    //Grab the state of the entire Keyboard
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    
    //Assume player is moving for right now (If one of the movement keys aren't pressed, then set to false)
    isMoving = true;

    if (keyboardState[SDL_SCANCODE_DOWN])
    {
        playerDir.x = 0.0f;
        playerDir.y = 1.0f;
        playerState = Direction::down;
    }
    else if (keyboardState[SDL_SCANCODE_UP])
    {
        playerDir.x = 0.0f;
        playerDir.y = -1.0f;
        playerState = Direction::up;
    }
    else if (keyboardState[SDL_SCANCODE_LEFT])
    {
        playerDir.x = -1.0f;
        playerDir.y = 0.0f;
        playerState = Direction::left;
    }
    else if (keyboardState[SDL_SCANCODE_RIGHT])
    {
        playerDir.x = 1.0f;
        playerDir.y = 0.0f;
        playerState = Direction::right;
    }
    else
    {
        playerDir.x = 0.0f;
        playerDir.y = 0.0f;
        isMoving = false;
    }
    
    //Call SetAnim() to play the corresponding animation
    SetAnim();
}

void PlayerMove::SetAnim()
{
    //Based off the velocity and playerState, play the right animation
    //first determine if player is moving or not
    if (playerDir.x == 0.0f && playerDir.y == 0.0f)
    {
        //Determine the right animation based off the direction link is facing
        if (playerState == Direction::up && mPlayer->spriteComponent->GetAnimName() != "StandUp")
        {
            mPlayer->spriteComponent->SetAnimation("StandUp");
        }
        else if (playerState == Direction::down && mPlayer->spriteComponent->GetAnimName() != "StandDown")
        {
            mPlayer->spriteComponent->SetAnimation("StandDown");
        }
        else if (playerState == Direction::left && mPlayer->spriteComponent->GetAnimName() != "StandLeft")
        {
            mPlayer->spriteComponent->SetAnimation("StandLeft");
        }
        else if (playerState == Direction::right && mPlayer->spriteComponent->GetAnimName() != "StandRight")
        {
            mPlayer->spriteComponent->SetAnimation("StandRight");
        }
    }
    else
    {
        //Determine the right animation based off the direction link is facing
        if (playerState == Direction::up && mPlayer->spriteComponent->GetAnimName() != "WalkUp")
        {
            mPlayer->spriteComponent->SetAnimation("WalkUp");
        }
        else if (playerState == Direction::down && mPlayer->spriteComponent->GetAnimName() != "WalkDown")
        {
            mPlayer->spriteComponent->SetAnimation("WalkDown");
        }
        else if (playerState == Direction::left && mPlayer->spriteComponent->GetAnimName() != "WalkLeft")
        {
            mPlayer->spriteComponent->SetAnimation("WalkLeft");
        }
        else if (playerState == Direction::right && mPlayer->spriteComponent->GetAnimName() != "WalkRight")
        {
            mPlayer->spriteComponent->SetAnimation("WalkRight");
        }
    }
}
