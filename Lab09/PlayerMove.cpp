//
//  PlayerMove.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/3/22.
//

#include "PlayerMove.hpp"
#include "Player.hpp"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "Random.h"

PlayerMove::PlayerMove(class Player* owner)
: MoveComponent(owner)
{
}

void PlayerMove::Update(float deltaTime)
{
    MoveComponent::Update(deltaTime);
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{


}

