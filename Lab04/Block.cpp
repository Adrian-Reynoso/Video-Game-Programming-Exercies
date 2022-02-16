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
    
    std::string stringLetter(1, letter);
    
    //Judging by the character, see what texture we need to use
    if(letter == 'A' || letter == 'B' || letter == 'C'|| letter == 'D'|| letter == 'E'|| letter == 'F'|| letter == 'G'|| letter == 'H'|| letter == 'I')
    {
        //Assign texture to sprite component
        spriteComponent->SetTexture(game->GetTexture("Assets/Block" + stringLetter + ".png"));
    }
}

Block::~Block()
{
    //Use remove block function
    mGame->RemoveBlock(this);
}
