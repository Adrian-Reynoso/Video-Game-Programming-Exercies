//
//  PlayerMove.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/3/22.
//

#include "PlayerMove.hpp"
#include "Actor.h"
#include "Game.h"

PlayerMove::PlayerMove(class Actor* owner)
:MoveComponent(owner)
{
}

void PlayerMove::Update(float deltaTime)
{
    //Updates the owning actor’s position based on the owning actor’s forward vector
    Vector2 velocity = mOwner->GetForward() * mForwardSpeed;
    mOwner->SetPosition(mOwner->GetPosition() + (velocity * deltaTime));
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
    }
    
    else if (keyboardState[SDL_SCANCODE_LEFT])
    {
        SetForwardSpeed(-300.0f);
    }
    
    else if (keyboardState[SDL_SCANCODE_RIGHT])
    {
        SetForwardSpeed(300.0f);
    }
    
    else
    {
        //Revert speed back to zero
        SetForwardSpeed(0.0f);
    }
}
