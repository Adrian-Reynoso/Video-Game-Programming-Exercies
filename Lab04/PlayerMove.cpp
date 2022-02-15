//
//  PlayerMove.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/3/22.
//

#include "PlayerMove.hpp"
#include "Actor.h"
#include "Player.hpp"
#include "Goomba.hpp"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "AnimatedSprite.h"
#include "Block.hpp"
#include "Game.h"

PlayerMove::PlayerMove(class Player* owner)
:MoveComponent(owner)
{
    mPlayer = owner;
    ownerCollisionComponent = mOwner->GetComponent<CollisionComponent>();
}

void PlayerMove::Update(float deltaTime)
{
    //Create temporary position
    Vector2 tempPos = mOwner->GetPosition();
    bool collisionChecker = false;
    
    //Updates the owning actor’s x position based on the owning actor’s forward vector
    tempPos.x += (mForwardSpeed * deltaTime);
    
    //Check if Mario collides with blocks
    CollSide onBlock = CollSide::None;
    for (Block* block : mOwner->GetGame()->GetBlockVector())
    {
        Vector2 offSet {0.0f, 0.0f};
        onBlock = ownerCollisionComponent->GetMinOverlap(block->collisionComponent, offSet);

        if (onBlock != CollSide::None)
        {
            //Add offset to temPos
            tempPos += offSet;
            
            //Call setPosition
            mOwner->SetPosition(tempPos);
            
            collisionChecker = true;
            
            if (onBlock == CollSide::Top && mYSpeed > 0.0f)
            {
                //Mario is landing on top of block
                mYSpeed = 0;
                mInAir = false;
            }
            else if (onBlock == CollSide::Bottom && mYSpeed < 0.0f)
            {
                //Mario hit his head on a block
                mYSpeed = 0.0f;
                
                //Play sound effect
                Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Bump.wav"), 0);
            }

        }
    }
    
    //Check if Mario stomps Goombas
    for (Goomba* goomba : mOwner->GetGame()->GetGoombaVector())
    {
        Vector2 offSet {0.0f, 0.0f};
        onBlock = ownerCollisionComponent->GetMinOverlap(goomba->collisionComponent, offSet);

        if (onBlock != CollSide::None && goomba->stomp == false)
        {
            //Add offset to temPos
            tempPos += offSet;
            
            //Call setPosition
            mOwner->SetPosition(tempPos);
            
            if (onBlock == CollSide::Top || ((onBlock == CollSide::Right || onBlock == CollSide::Left) && mInAir))
            {
                //Make stomp variable in Goomba true and give Mario a half jump
                goomba->stomp = true;
                mYSpeed = -350.0f;
                
                //Play sound effect
                Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Stomp.wav"), 0);
            }
            else
            {
                //Mario should die
                mPlayer->spriteComponent->SetTexture(mPlayer->GetGame()->GetTexture("Assets/Mario/Dead.png"));
                mPlayer->SetState(ActorState::Paused);
            }
        }
    }
    
    //Check if player has not collided with anything. If so, make mInAir true
    if (collisionChecker == false)
    {
        mInAir = true;
    }
    
    //Updates the owning actor’s y position based on the owning actor’s y speed (Using Euler Integration)
    tempPos.y += (mYSpeed * deltaTime);
    mYSpeed += mYAccel * deltaTime;
    
    //Check if player if less greater than 464.0f, if so cap it there
    if (tempPos.y > 464.0f)
    {
        //Mario Dies
        mPlayer->spriteComponent->SetTexture(mPlayer->GetGame()->GetTexture("Assets/Mario/Dead.png"));
        mPlayer->SetState(ActorState::Paused);
    }
    
    //Set the position to our tempPosition Vector2
    if (tempPos.x >= mOwner->GetGame()->cameraPosition.x + 16)
    {
        mOwner->SetPosition(tempPos);
    }
    else
    {
        mOwner->SetPosition(Vector2 {mOwner->GetGame()->cameraPosition.x + 16, tempPos.y});
    }
    
    //Checks for camera in reference to Mario
    if (tempPos.x - (mOwner->GetGame()->SCREENWIDTH/2) > mOwner->GetGame()->cameraPosition.x)
    {
        //Update camera position
        mOwner->GetGame()->cameraPosition.x = tempPos.x - (mOwner->GetGame()->SCREENWIDTH/2);
    }
    
    //Call animation function to set the right animations
    SetAnim();
    
    //Check if Mario made it to the finish. If so, play victory music
    if (tempPos.x > 6368.0f)
    {
        //Play sound effect
        Mix_HaltChannel(mPlayer->GetGame()->soundMusicLoop);
        Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/StageClear.wav"), 0);
        mPlayer->SetState(ActorState::Paused);
    }
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
    //Grab the state of the entire Keyboard
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    
    mSpacePressed = keyboardState[SDL_SCANCODE_DOWN];
    Jump(keyState);
    if (keyboardState[SDL_SCANCODE_LEFT] && !keyboardState[SDL_SCANCODE_RIGHT])
    {
        SetForwardSpeed(-300.0f);
    }
    
    else if (keyboardState[SDL_SCANCODE_RIGHT] && !keyboardState[SDL_SCANCODE_LEFT])
    {
        SetForwardSpeed(300.0f);
    }
    
    else
    {
        //Revert speed back to zero
        SetForwardSpeed(0.0f);
    }
    
}

void PlayerMove::Jump(const Uint8 *keyState)
{
    //Grab the state of the entire Keyboard
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    
    if (keyboardState[SDL_SCANCODE_SPACE] && mInAir == false && mSpacePressed == false)
    {
        mYSpeed = -700.0f;
        mInAir = true;
        mSpacePressed = true;
        
        //Play sound effect
        Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
    }
}

void PlayerMove::SetAnim()
{
    //For Ground animations
    if (mPlayer->GetState() == ActorState::Paused)
    {
        mPlayer->spriteComponent->SetAnimation("dead");
        
        //Play sound effect
        Mix_HaltChannel(mPlayer->GetGame()->soundMusicLoop);
        Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Dead.wav"), 0);
        
    }
    else if (GetForwardSpeed() < 0 && mInAir == false)
    {
        mPlayer->spriteComponent->SetAnimation("runLeft");
    }
    else if (GetForwardSpeed() > 0 && mInAir == false)
    {
        mPlayer->spriteComponent->SetAnimation("runRight");
    }
    else if (GetForwardSpeed() == 0 && mInAir == false)
    {
        mPlayer->spriteComponent->SetAnimation("idle");
    }
    
    //For Air animations
    else if (GetForwardSpeed() < 0 && mInAir == true)
    {
        mPlayer->spriteComponent->SetAnimation("jumpLeft");
    }
    else if (GetForwardSpeed() > 0 && mInAir == true)
    {
        mPlayer->spriteComponent->SetAnimation("jumpRight");
    }
    else if (GetForwardSpeed() == 0 && mInAir == true)
    {
        if (mPlayer->spriteComponent->GetAnimName() == "runRight" || mPlayer->spriteComponent->GetAnimName() == "jumpRight" || mPlayer->spriteComponent->GetAnimName() == "idle")
        {
            mPlayer->spriteComponent->SetAnimation("jumpRight");
        }
        else
        {
            mPlayer->spriteComponent->SetAnimation("jumpLeft");
        }
    }

}
