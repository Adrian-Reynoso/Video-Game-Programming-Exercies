//
//  Laser.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/23/22.
//

#include "Laser.hpp"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Math.h"
#include "Asteroid.hpp"

//Constructor implementation
Laser::Laser(class Game* game)
:Actor(game)
{
    //Dynamically allocate a SpriteComponent and a MoveComponent, and assign them to the member variables
    spriteComponent = new SpriteComponent(this);
    moveComponent = new MoveComponent(this);
    
    //Assign laser texture to sprite component
    spriteComponent->SetTexture(game->GetTexture("Assets/Laser.png"));
    
    //Make the moveComponent have a forward speed of 400
    moveComponent->SetForwardSpeed(400.0f);
}

//OnUpdate() override
void Laser::OnUpdate(float deltaTime)
{
    //Add to the laser lifetime
    lifeTime += deltaTime;
    
    //Use the asteroid vector getter to get the available asteroids
    std::vector<Asteroid*> asteroidTracker = mGame->GetAsteroidTracker();
    
    //Loop over the asteroid vector to see if a laser hit one
    for (Asteroid* asteroid : asteroidTracker)
    {
        //If distance between laser and asteroid is <= 70, then set both the laser and asteroid to ActorState::Destroy and break out of the loop
        if (Vector2::Distance(GetPosition(), asteroid->GetPosition()) <= 70.0f)
        {
            //For Laser
            SetState(ActorState::Destroy);
            
            //For Asteroid
            asteroid->SetState(ActorState::Destroy);
        }
    }
    
    //Check if lifetime is greater than 1s. If so, change actor state to destroy
    if (lifeTime >= 1.0f)
    {
        SetState(ActorState::Destroy);
    }
}
