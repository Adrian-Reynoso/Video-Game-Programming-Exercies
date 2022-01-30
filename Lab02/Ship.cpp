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
#include "Laser.hpp"

//Constructor implementation
Ship::Ship(class Game* game)
:Actor(game)
{
    //Dynamically allocate a SpriteComponent and a MoveComponent, and assign them to the member variables
    spriteComponent = new SpriteComponent(this);
    moveComponent = new MoveComponent(this);
    
    //Assign ship texture to sprite component
    spriteComponent->SetTexture(game->GetTexture("Assets/Ship.png"));
}

void Ship::OnProcessInput(const Uint8* keyState)
{
    //Grab the state of the entire Keyboard
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    
    //Check If the arrow keys were pressed, if so do the required actions
    if (keyboardState[SDL_SCANCODE_UP] && keyboardState[SDL_SCANCODE_DOWN])
    {
        //Nothing happens
        moveComponent->SetForwardSpeed(0.0f);
        
        //Call laser shot function
        laserShot(keyboardState);
        
        //Assign ship texture to sprite component
        spriteComponent->SetTexture(mGame->GetTexture("Assets/Ship.png"));
    }
    
    else if (keyboardState[SDL_SCANCODE_LEFT] && keyboardState[SDL_SCANCODE_RIGHT])
    {
        //Nothing happens
        moveComponent->SetAngularSpeed(0.0f);
        
        //Call laser shot function
        laserShot(keyboardState);
        
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
            
            //Call laser shot function
            laserShot(keyboardState);
            
        }
        else if (keyboardState[SDL_SCANCODE_LEFT])
        {
            //Rotate Left
            moveComponent->SetAngularSpeed(angularVelocity);
            
            //Call laser shot function
            laserShot(keyboardState);
        }
        else
        {
            moveComponent->SetAngularSpeed(0.0f);
            
            //Call laser shot function
            laserShot(keyboardState);
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
            
            //Call laser shot function
            laserShot(keyboardState);
        }
        else if (keyboardState[SDL_SCANCODE_LEFT])
        {
            //Rotate Left
            moveComponent->SetAngularSpeed(angularVelocity);
            
            //Call laser shot function
            laserShot(keyboardState);
        }
        else
        {
            moveComponent->SetAngularSpeed(0.0f);
            
            //Call laser shot function
            laserShot(keyboardState);
        }
        
        //Assign ship texture to sprite component
        spriteComponent->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
    }
    
    else if (keyboardState[SDL_SCANCODE_RIGHT])
    {
        //Rotate Right
        moveComponent->SetAngularSpeed(angularVelocity * -1);
        
        //Call laser shot function
        laserShot(keyboardState);
        
        //Assign ship texture to sprite component
        spriteComponent->SetTexture(mGame->GetTexture("Assets/Ship.png"));
    }
    
    else if (keyboardState[SDL_SCANCODE_LEFT])
    {
        //Rotate Left
        moveComponent->SetAngularSpeed(angularVelocity);
        
        //Call laser shot function
        laserShot(keyboardState); 
        
        //Assign ship texture to sprite component
        spriteComponent->SetTexture(mGame->GetTexture("Assets/Ship.png"));
    }
    
    else if (keyboardState[SDL_SCANCODE_SPACE])
    {
        //Call laser shot function
        laserShot(keyboardState);
        
        //Halt all other movements from the ship
        moveComponent->SetAngularSpeed(0.0f);
        moveComponent->SetForwardSpeed(0.0f);
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

//Laser function to allocate a new shot
void Ship::laserShot(const Uint8 *keyboardState)
{
    if (keyboardState[SDL_SCANCODE_SPACE])
    {
        //check cooldown
        if (coolDown <= 0.0f)
        {
            //Allocate a new laser
            Laser* laser = new Laser(mGame);
            
            //Set the position of the laser to the ship’s position
            laser->SetPosition(GetPosition());
            
            //Set the rotation of the laser to the ship’s rotation
            laser->SetRotation(GetRotation());
            
            //Set cooldown to 1.0f
            coolDown = 1.0f;
        }
    }
}

//OnUpdate implementation
void Ship::OnUpdate(float deltaTime)
{
    //Update coolDown by deltaTime
    coolDown -= deltaTime;
}
