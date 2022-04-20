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
#include "HUD.hpp"

Coin::Coin(class Game* game, Actor* parent)
: Actor(game, parent)
{
    meshComponent = new MeshComponent(this);
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
    
    //Check if player collides with coin by using getMinOverlap
    Vector3 offSet {0.0f, 0.0f, 0.0f};
    CollSide collided = collisionComponent->GetMinOverlap(mGame->GetPlayer()->collisionComponent, offSet);

    //If player collides with coin that is also active
    if (collided != CollSide::None)
    {
        //Play Sound
        Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Coin.wav"), 0);
    
        //Increment coin counter
        GetGame()->coinCounter++;
        
        //Add a coin to the HUD
        mGame->GetPlayer()->mHUD->AddACoin();
        
        //Get by seting ActorState to destroy
        SetState(ActorState::Destroy);
    }
}
