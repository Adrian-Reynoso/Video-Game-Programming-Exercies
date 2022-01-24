//
//  Asteroid.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/23/22.
//

#include "Asteroid.hpp"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Random.h"
#include "Game.h"

//Constructor implementation
Asteroid::Asteroid(class Game* game)
:Actor(game)
{
    // TODO: Add code here
    
    //Asteroid needs to add itself to the game via AddActor
    game->AddAsteroid(this);
    
    //Dynamically allocate a SpriteComponent and a MoveComponent, and assign them to the member variables
    spriteComponent = new SpriteComponent(this);
    moveComponent = new MoveComponent(this);
    
    //Assign asteroid texture to sprite component
    spriteComponent->SetTexture(game->GetTexture("Assets/Asteroid.png"));
    
    //Hardcode the forward speed of the moveComponent to 150.0
    moveComponent->SetForwardSpeed(150.0f);
    
    //Set mRotation to a Random::GetFloatRange between 0.0f and Math::TwoPi
    mRotation = Random::GetFloatRange(0.0f, Math::TwoPi);
    
    //Set botRight vector to window dimesnions from game
    botRight.x = game->SCREENWIDTH - 1;
    botRight.y = game->SCREENHEIGHT - 1;
    
    //Get the width and height of window
    screenWidth = game->SCREENWIDTH;
    screenHeight = game->SCREENHEIGHT;
    
    //Set mPosition to a Random::GetVector
    mPosition = Random::GetVector(topLeft, botRight);
}

Asteroid::~Asteroid()
{
    mGame->RemoveAsteroid(this);
}

void Asteroid::OnUpdate(float deltaTime)
{
    //Check position and adjust accordingly for screen wrapping
    
    //If it moves off left of screen
    if (mPosition.x < 0)
    {
        //Set x direction to right of screen
        mPosition.x = screenWidth - 1;
    }
    
    //If it moves off right of screen
    else if (mPosition.x >= screenWidth)
    {
        //Set x direction to right of screen
        mPosition.x = 0;
    }
    
    //If it moves off top of screen
    else if (mPosition.y < 0)
    {
        //Set y direction to bottom of screen
        mPosition.y = screenHeight - 1;
    }
    
    //If it moves off bottom of screen
    else if (mPosition.y >= screenHeight)
    {
        //Set y direction to top of screen
        mPosition.y = 0;
    }
}
