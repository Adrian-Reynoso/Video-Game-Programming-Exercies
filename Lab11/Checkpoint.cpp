//
//  Checkpoint.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 4/12/22.
//

#include "Checkpoint.hpp"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "Player.hpp"

Checkpoint::Checkpoint(class Game* game, Actor* parent)
: Actor(game, parent)
{
    meshComponent = new MeshComponent(this);
    meshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Checkpoint.gpmesh"));
    
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(25.0f, 25.0f, 25.0f);
}

void Checkpoint::OnUpdate(float deltaTime)
{
    //Check if this is the next checkpoint in the queue. If so, set active variable to true
    if (mGame->checkpointQueue.front() == this && !isActive)
    {
        isActive = true;
        meshComponent->SetTextureIndex(0);
    }
    else if (!isActive)
    {
        meshComponent->SetTextureIndex(1);
    }
    
    //Check if player collides with this checkpoint by using getMinOverlap    
    if (collisionComponent->Intersect(mGame->GetPlayer()->collisionComponent) && isActive)
    {
        //Get rid of checkpoint in the queue and set ActorState to destroy
        mGame->checkpointQueue.pop();
        
        //Set respawn position of player to this checkpoint
        mGame->GetPlayer()->SetRespawnPosition(GetPosition());
        
        //Check if string level is set. If so, set mNextLevel in game to this string
        if (mLevelString != "")
        {
            mGame->SetNextLevel(mLevelString);
        }
        
        SetState(ActorState::Destroy);
    }
}
