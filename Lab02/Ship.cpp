//
//  Ship.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/22/22.
//

#include "Ship.hpp"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"

//Constructor implementation
Ship::Ship(class Game* game)
:Actor(game)
{
    // TODO: Add code here
    //Dynamically allocate a SpriteComponent and a MoveComponent, and assign them to the member variables
    spriteComponent = new SpriteComponent(this);
    moveComponent = new MoveComponent(this);
    
    //Assign ship texture to sprite component
    spriteComponent->SetTexture(game->GetTexture("Assets/Ship.png"));
}

void Ship::OnProcessInput(const Uint8* keyState)
{
    //Grab the state of the entire Keyboard
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    
    //Check If the arrow keys were pressed, if so do the required actions
    if (keyboardState[SDL_SCANCODE_UP] && keyboardState[SDL_SCANCODE_DOWN])
    {
        //Nothing happens
        moveComponent->SetForwardSpeed(0.0f);
        
        //Assign ship texture to sprite component
        spriteComponent->SetTexture(mGame->GetTexture("Assets/Ship.png"));
    }
    
    else if(keyboardState[SDL_SCANCODE_UP])
    {
        //Move forward
        moveComponent->SetForwardSpeed(forwardVelocity);
        
        //Check if the left or rightkey is also pressed
        if (keyboardState[SDL_SCANCODE_RIGHT])
        {
            //Rotate Right
            moveComponent->SetAngularSpeed(angularVelocity * -1);
        }
        else if (keyboardState[SDL_SCANCODE_LEFT])
        {
            //Rotate Left
            moveComponent->SetAngularSpeed(angularVelocity);
        }
        else
        {
            moveComponent->SetAngularSpeed(0.0f);
        }
        
        //Assign ship texture to sprite component
        spriteComponent->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
    }
    
    else if(keyboardState[SDL_SCANCODE_DOWN])
    {
        //Move backward
        moveComponent->SetForwardSpeed(forwardVelocity * -1);
        
        //Check if the left or rightkey is also pressed
        if (keyboardState[SDL_SCANCODE_RIGHT])
        {
            //Rotate Right
            moveComponent->SetAngularSpeed(angularVelocity * -1);
        }
        else if (keyboardState[SDL_SCANCODE_LEFT])
        {
            //Rotate Left
            moveComponent->SetAngularSpeed(angularVelocity);
        }
        else
        {
            moveComponent->SetAngularSpeed(0.0f);
        }
        
        //Assign ship texture to sprite component
        spriteComponent->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
    }
    
    else if (keyboardState[SDL_SCANCODE_RIGHT])
    {
        //Rotate Right
        moveComponent->SetAngularSpeed(angularVelocity * -1);
        
        //Assign ship texture to sprite component
        spriteComponent->SetTexture(mGame->GetTexture("Assets/Ship.png"));
    }
    
    else if (keyboardState[SDL_SCANCODE_LEFT])
    {
        //Rotate Left
        moveComponent->SetAngularSpeed(angularVelocity);
        
        //Assign ship texture to sprite component
        spriteComponent->SetTexture(mGame->GetTexture("Assets/Ship.png"));
    }
    
    else
    {
        //You aren't pressing anything
        moveComponent->SetAngularSpeed(0.0f);
        moveComponent->SetForwardSpeed(0.0f);
        
        //Assign ship texture to sprite component
        spriteComponent->SetTexture(mGame->GetTexture("Assets/Ship.png"));
    }
        
}
