//
//  SideBlock.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/5/22.
//

#include "SideBlock.hpp"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.hpp"

SideBlock::SideBlock(class Game* game, size_t textureIndex)
: Actor(game)
{
    //Set the SideBlock’s scale to 500.0f (to make it big)
    SetScale(500.0f);
    
    //Create a MeshComponent that uses the "Assets/Cube.gpmesh" mesh
    meshComponent = new MeshComponent(this);
    meshComponent->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
    meshComponent->SetTextureIndex(textureIndex);
    
}

void SideBlock::OnUpdate(float deltaTime)
{
    //Check if the block is more than 2000 units behind the ship. If so, destroy the actor
    if (mGame->GetPlayer()->GetPosition().x - GetPosition().x >= 2000.0f)
    {
        SetState(ActorState::Destroy);
    }
}
