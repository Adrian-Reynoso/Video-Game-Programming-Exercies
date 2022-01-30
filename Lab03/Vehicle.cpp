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
#include "CollisionComponent.h"
#include "Frog.hpp"

Vehicle::Vehicle(class Game* game, char letter, float dir)
:Actor(game)
{
    // TODO: Add code here
    //Dynamically allocate a SpriteComponent and assign it to the member variables
    spriteComponent = new SpriteComponent(this);
    wrappingMove = new WrappingMove(this, dir);
    
    //Dynamically allocate a collisionComponent
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(50.0f, 50.0f);
    
    //A car should be 64x64 while a truck should be 128x48
    if (letter == 'T')
    {
        collisionComponent->SetSize(128.0f, 48.0f);
    }
    else
    {
        collisionComponent->SetSize(64.0f, 64.0f);
    }
    
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

void Vehicle::OnUpdate(float deltaTime)
{
    //Figure out if the frog is within an angle in front of the vehicle. If it is, the vehicle should move at half of its original speed, otherwise the vehicle should move at normal speed
    Vector2 vehicleToFrog = mGame->mFrog->GetPosition() - GetPosition();
    
    //Calculate the angle from the vehicle direction to frog using dot product
    float angle = Math::Acos((Vector2::Dot(wrappingMove->direction, vehicleToFrog)) / (Math::Sqrt(Vector2::Dot(wrappingMove->direction, wrappingMove->direction)) * Math::Sqrt(Vector2::Dot(vehicleToFrog, vehicleToFrog))));
    
    //See if angle is less than PI/6, if so slow down the car speed by half
    if (angle <= Math::Pi/6)
    {
        wrappingMove->SetForwardSpeed(50.0f);
    }
    else
    {
        wrappingMove->SetForwardSpeed(100.0f);
    }
}
