//
//  Block.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/3/22.
//

#include "Block.hpp"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Game.h"

Block::Block(class Game* game, char letter)
:Actor(game)
{
    //Block needs to add itself to the game via AddActor
    game->AddBlock(this);
    
    //Dynamically allocate a SpriteComponent and assign it to the member variables
    spriteComponent = new SpriteComponent(this);
    
    //Dynamically allocate a collisionComponent
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(32.0f, 32.0f);
    
    //Judging by the character, see what texture we need to use
    if(letter == 'A')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/BlockA.png"));
    }
    
    else if(letter == 'B')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/BlockB.png"));
    }
    
    else if(letter == 'C')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/BlockC.png"));
    }
    
    else if(letter == 'D')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/BlockD.png"));
    }
    
    else if(letter == 'E')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/BlockE.png"));
    }
    
    else if(letter == 'F')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/BlockF.png"));
    }
    
    else if(letter == 'G')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/BlockG.png"));
    }
    
    else if(letter == 'H')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/BlockH.png"));
    }
    
    else if(letter == 'I')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/BlockI.png"));
    }
}

Block::~Block()
{
    //Use remove block function
    mGame->RemoveBlock(this);
}
