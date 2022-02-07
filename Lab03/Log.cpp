//
//  Log.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/27/22.
//

#include "Log.hpp"
#include "SpriteComponent.h"
#include "WrappingMove.hpp"
#include "CollisionComponent.h"
#include "Game.h"

Log::Log(class Game* game, char letter, float dir)
:Actor(game)
{
    //Dynamically allocate a SpriteComponent and WrappingComponentand assign it to the member variables
    spriteComponent = new SpriteComponent(this);
    wrappingMove = new WrappingMove(this, dir);
    collisionComponent = new CollisionComponent(this);
    
    //Set a forward speed for WrappingMove
    wrappingMove->SetForwardSpeed(75);
    
    //Judging by the character, see what texture we need to use and set the size of the collisionComponent
    if(letter == 'X')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/LogX.png"));
        collisionComponent->SetSize(192.0f, 48.0f);
    }
    
    else if(letter == 'Y')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/LogY.png"));
        collisionComponent->SetSize(256.0f, 48.0f);
    }
    
    else if(letter == 'Z')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/LogZ.png"));
        collisionComponent->SetSize(384.0f, 48.0f);
    }
}
