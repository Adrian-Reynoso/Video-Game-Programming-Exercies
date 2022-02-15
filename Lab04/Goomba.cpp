//
//  Goomba.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/4/22.
//

#include "Goomba.hpp"
#include "GoombaMove.hpp"
#include "SpriteComponent.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "Game.h"

Goomba::Goomba(class Game* game)
:Actor(game)
{
    //Block needs to add itself to the game via AddActor
    game->AddGoomba(this);
    
    //Dynamically allocate a SpriteComponent and assign it to the member variables
    spriteComponent = new AnimatedSprite(this, 150);
    spriteComponent->SetTexture(game->GetTexture("Assets/Goomba/Walk0.png"));
    
    //To make Goombas have a walking animation and dead animation
    std::vector<SDL_Texture*> walkAnim{
        GetGame()->GetTexture("Assets/Goomba/Walk0.png"),
        GetGame()->GetTexture("Assets/Goomba/Walk1.png")
    };
    std::vector<SDL_Texture*> deadAnim{
        GetGame()->GetTexture("Assets/Goomba/Dead.png")
    };
    spriteComponent->AddAnimation("walk", walkAnim);
    spriteComponent->AddAnimation("dead", deadAnim);
    
    //Set the current animation to walk
    spriteComponent->SetAnimation("walk");
    
    //Dynamically allocate a collisionComponent
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(32.0f, 32.0f);
    
    //Create a PlayerMove in the Player constructor
    goombaMove = new GoombaMove(this);
}

Goomba::~Goomba()
{
    //Use remove goomba function
    mGame->RemoveGoomba(this);
}
