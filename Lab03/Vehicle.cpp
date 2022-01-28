//
//  Vehicle.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/27/22.
//

#include "Vehicle.hpp"
#include "SpriteComponent.h"
#include "WrappingMove.hpp"
#include "Game.h"

Vehicle::Vehicle(class Game* game, char letter, float dir)
:Actor(game)
{
    // TODO: Add code here
    //Dynamically allocate a SpriteComponent and assign it to the member variables
    spriteComponent = new SpriteComponent(this);
    wrappingMove = new WrappingMove(this, dir);
    
    //Set forward speed
    wrappingMove->SetForwardSpeed(100.0);
    
    //Judging by the character, see what texture we need to use
    if(letter == 'A')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/CarA.png"));
    }
    
    else if(letter == 'B')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/CarB.png"));
    }
    
    else if(letter == 'C')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/CarC.png"));
    }
    
    else if(letter == 'D')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/CarD.png"));
    }
    
    else if(letter == 'T')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/Truck.png"));
    }
}
