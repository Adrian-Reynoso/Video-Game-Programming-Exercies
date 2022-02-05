//
//  Spawner.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/4/22.
//

#include "Spawner.hpp"
#include "Player.hpp"
#include "Goomba.hpp"
#include "Game.h"

Spawner::Spawner(Game* game)
: Actor(game)
{
}

void Spawner::OnUpdate(float deltaTime)
{
    //Spawn a Goomba if the x-distance between the spawner and the player is less than 600
    distance = GetPosition().x - mGame->GetPlayer()->GetPosition().x;
    
    if (distance <= 600.0f)
    {
        goomba = new Goomba(mGame);
        goomba->SetPosition(GetPosition());
        
        //Set the Spawner’s state to ActorState::Destroy (so it’ll be destroyed and can’t spawn anything anymore)
        SetState(ActorState::Destroy);
    }
    
}
