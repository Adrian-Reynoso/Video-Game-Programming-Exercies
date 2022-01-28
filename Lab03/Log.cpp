//
//  Log.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/27/22.
//

#include "Log.hpp"
#include "SpriteComponent.h"
#include "WrappingMove.hpp"
#include "Game.h"

Log::Log(class Game* game, char letter, float dir)
:Actor(game)
{
    // TODO: Add code here
    //Dynamically allocate a SpriteComponent and WrappingComponentand assign it to the member variables
    spriteComponent = new SpriteComponent(this);
    wrappingMove = new WrappingMove(this, dir);
    
    //Set a forward speed for WrappingMove
    wrappingMove->SetForwardSpeed(75);
    
    //Judging by the character, see what texture we need to use
    if(letter == 'X')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/LogX.png"));
    }
    
    else if(letter == 'Y')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/LogY.png"));
    }
    
    else if(letter == 'Z')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/LogZ.png"));
    }
}
