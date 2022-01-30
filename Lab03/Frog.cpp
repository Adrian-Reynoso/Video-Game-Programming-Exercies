//
//  Frog.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/27/22.
//

#include "Frog.hpp"
#include "SpriteComponent.h"
#include "Game.h"
#include "Math.h"
#include "CollisionComponent.h"
#include "Vehicle.hpp"
#include "DeadFrog.hpp"

Frog::Frog(class Game* game, float xPosition, float yPosition)
:Actor(game)
{
    // TODO: Add code here
    //Dynamically allocate a SpriteComponent and assign it to the member variables
    spriteComponent = new SpriteComponent(this);
    
    //Dynamically allocate a collisionComponent and set width/height to 50
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(50.0f, 50.0f);
    
    //Assign ship texture to sprite component
    spriteComponent->SetTexture(game->GetTexture("Assets/Frog.png"));
    
    //Assign the screen dimensions
    screenWidth = game->SCREENWIDTH;
    screenHeight = game->SCREENHEIGHT;
    
    //Define max height so the frog doesn't go backward when game starts
    maxHeight = game->SCREENHEIGHT - 64;
    
    //Assign the game to the private member variable mGame
    mGame = game;
    xPos = xPosition;
    yPos = yPosition;
}

//Override of OnProcessInput
void Frog::OnProcessInput(const Uint8* keyState)
{
    //Grab the state of the entire Keyboard
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    
    //Create a temporary vector2
    Vector2 temp (GetPosition());
    
    //Check If the UP key was pressed
    if (keyboardState[SDL_SCANCODE_UP] && lastFrame[SDL_SCANCODE_UP] == false)
    {
        //Edit the current position if the move doesn't go off screen
        if (temp.y - 64 >= minHeight)
        {
            temp.y -= 64;

            //move position 64 pixels
            SetPosition(temp);
        }
        
    }
    
    //Check If the DOWN key was pressed
    else if (keyboardState[SDL_SCANCODE_DOWN] && lastFrame[SDL_SCANCODE_DOWN] == false)
    {
        //Edit the current position if the move doesn't go off screen
        if (temp.y + 64 <= maxHeight)
        {
            temp.y += 64;

            //move position 64 pixels
            SetPosition(temp);
        }
        
    }
    
    //Check If the RIGHT key was pressed
    else if (keyboardState[SDL_SCANCODE_RIGHT] && lastFrame[SDL_SCANCODE_RIGHT] == false)
    {
        //Edit the current position if the move doesn't go off screen
        if (temp.x + 64 < screenWidth)
        {
            temp.x += 64;
            
            //move position 64 pixels
            SetPosition(temp);
        }
        
    }
    
    //Check If the LEFT key was pressed
    else if (keyboardState[SDL_SCANCODE_LEFT] && lastFrame[SDL_SCANCODE_LEFT] == false)
    {
        //Edit the current position if the move doesn't go off screen
        if (temp.x - 64 > 0)
        {
            temp.x -= 64;
            
            //move position 64 pixels
            SetPosition(temp);
        }
        
    }
    
    //Save the currentstate of input to last frame
    lastFrame[SDL_SCANCODE_UP] = keyboardState[SDL_SCANCODE_UP];
    lastFrame[SDL_SCANCODE_DOWN] = keyboardState[SDL_SCANCODE_DOWN];
    lastFrame[SDL_SCANCODE_LEFT] = keyboardState[SDL_SCANCODE_LEFT];
    lastFrame[SDL_SCANCODE_RIGHT] = keyboardState[SDL_SCANCODE_RIGHT];
}

void Frog::OnUpdate(float deltaTime)
{
    //Check if frog collides with a vehicle
    for (unsigned long i = 0; i < mGame->vehicles.size(); i++)
    {
        bool collides = collisionComponent->Intersect(mGame->vehicles[i]->collisionComponent);
        
        //check if the frog collides, if so move the frog back to starting position
        if (collides == true)
        {
            //Create a DeadFrog actor at the spot where this frog died
            deadFrog = new DeadFrog(mGame, GetPosition().x, GetPosition().y);
            
            //Reset the position of the frog
            SetPosition(Vector2(xPos, yPos));
        }
    }
}
