//
//  Frog.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/27/22.
//

#include "Frog.hpp"
#include "SpriteComponent.h"
#include "Game.h"

Frog::Frog(class Game* game)
:Actor(game)
{
    // TODO: Add code here
    //Dynamically allocate a SpriteComponent and assign it to the member variables
    spriteComponent = new SpriteComponent(this);
    
    //Assign ship texture to sprite component
    spriteComponent->SetTexture(game->GetTexture("Assets/Frog.png"));
    
    //Initialize the map with the four arrow key movements
    leadingEdgesMap[SDL_SCANCODE_UP] = false;
    leadingEdgesMap[SDL_SCANCODE_DOWN] = false;
    leadingEdgesMap[SDL_SCANCODE_LEFT] = false;
    leadingEdgesMap[SDL_SCANCODE_RIGHT] = false;
}

//Override of OnProcessInput
void Frog::OnProcessInput(const Uint8* keyState)
{
    //Grab the state of the entire Keyboard
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    
    //Check If the UP key was pressed
    if (keyboardState[SDL_SCANCODE_UP])
    {
        
    }
}
