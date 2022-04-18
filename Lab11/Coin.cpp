//
//  Coin.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 4/12/22.
//

#include "Coin.hpp"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Checkpoint.hpp"
#include "Player.hpp"

Coin::Coin(class Game* game, Actor* parent)
: Actor(game, parent)
{
    MeshComponent* meshComponent = new MeshComponent(this);
    meshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Coin.gpmesh"));
    
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(100.0f, 100.0f, 100.0f);
}

void Coin::OnUpdate(float deltaTime)
{
    //Make coin rotate
    float tempRotation = GetRotation();
    tempRotation += (Math::Pi) * deltaTime;
    SetRotation(tempRotation);

    //If player collides with coin that is also active
    if (collisionComponent->Intersect(mGame->GetPlayer()->collisionComponent))
    {
        //Get by seting ActorState to destroy
        SetState(ActorState::Destroy);
    }
}
