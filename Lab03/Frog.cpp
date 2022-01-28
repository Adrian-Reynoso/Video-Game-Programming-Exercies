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

Frog::Frog(class Game* game)
:Actor(game)
{
    // TODO: Add code here
    //Dynamically allocate a SpriteComponent and assign it to the member variables
    spriteComponent = new SpriteComponent(this);
    
    //Assign ship texture to sprite component
    spriteComponent->SetTexture(game->GetTexture("Assets/Frog.png"));
    
    //Assign the screen dimensions
    screenWidth = game->SCREENWIDTH;
    screenHeight = game->SCREENHEIGHT;
    
    //Define max height so the frog doesn't go backward when game starts
    maxHeight = game->SCREENHEIGHT - 64;
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
