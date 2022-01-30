//
//  DeadFrog.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/29/22.
//

#include "DeadFrog.hpp"
#include "Frog.hpp"
#include "SpriteComponent.h"
#include "Game.h"
#include "Math.h"
#include "CollisionComponent.h"
#include "Vehicle.hpp"

DeadFrog::DeadFrog(class Game* game, float xPosition, float yPosition)
:Actor(game)
{
    // TODO: Add code here
    //Dynamically allocate a SpriteComponent and assign it to the member variables
    spriteComponent = new SpriteComponent(this);
    
    //Assign deadFrog texture to sprite component
    spriteComponent->SetTexture(game->GetTexture("Assets/Dead.png"));
    
    //Assign the screen dimensions
    screenWidth = game->SCREENWIDTH;
    screenHeight = game->SCREENHEIGHT;
    
    //Define max height so the frog doesn't go backward when game starts
    maxHeight = game->SCREENHEIGHT - 64;
    
    //Assign the game to the private member variable mGame
    mGame = game;
    xPos = xPosition;
    yPos = yPosition;
    
    //Set the deadFrog position to the frog's death spot
    SetPosition(Vector2(xPosition, yPosition));
}

void DeadFrog::OnUpdate(float deltaTime)
{
    //Add to the sprite's lifetime
    lifeTime += deltaTime;
    
    //Check if lifetime is greater than 1s. If so, change actor state to destroy
    if (lifeTime >= 0.5f)
    {
        SetState(ActorState::Destroy);
    }
}
