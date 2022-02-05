//
//  PlayerMove.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/3/22.
//

#include "PlayerMove.hpp"
#include "Actor.h"
#include "CollisionComponent.h"
#include "Block.hpp"
#include "Game.h"

PlayerMove::PlayerMove(class Actor* owner)
:MoveComponent(owner)
{
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
        onBlock = mOwner->GetComponent<CollisionComponent>()->GetMinOverlap(block->collisionComponent, offSet);

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
    
    //Check if player if less greater than 448.0f, if so cap it there
    if (tempPos.y > 448.0f)
    {
        tempPos.y = 448.0f;
        mInAir = false;
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
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
    //Grab the state of the entire Keyboard
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    
    //Check If the arrow keys were pressed, if so do the required actions
    if (keyboardState[SDL_SCANCODE_LEFT] && keyboardState[SDL_SCANCODE_RIGHT])
    {
        //Nothing happens
        SetForwardSpeed(0.0f);
        mSpacePressed = false;
    }
    
    else if (keyboardState[SDL_SCANCODE_LEFT])
    {
        SetForwardSpeed(-300.0f);
        mSpacePressed = false;
        
        jump(keyState);
    }
    
    else if (keyboardState[SDL_SCANCODE_RIGHT])
    {
        SetForwardSpeed(300.0f);
        mSpacePressed = false;
        
        jump(keyState);
    }
    
    else if (keyboardState[SDL_SCANCODE_SPACE] && mInAir == false && mSpacePressed == false)
    {
        jump(keyState);
    }
    
    else
    {
        //Revert speed back to zero
        SetForwardSpeed(0.0f);
        mSpacePressed = false;
    }
    
}

void PlayerMove::jump(const Uint8 *keyState)
{
    //Grab the state of the entire Keyboard
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    
    if (keyboardState[SDL_SCANCODE_SPACE] && mInAir == false && mSpacePressed == false)
    {
        mYSpeed = -700.0f;
        mInAir = true;
        mSpacePressed = true;
    }
}
