//
//  PlayerMove.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/3/22.
//

#include "PlayerMove.hpp"
#include "Actor.h"
#include "Player.hpp"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "AnimatedSprite.h"
#include "Game.h"

PlayerMove::PlayerMove(class Player* owner)
:MoveComponent(owner)
{
    mPlayer = owner;
    ownerCollisionComponent = mOwner->GetComponent<CollisionComponent>();
}

void PlayerMove::Update(float deltaTime)
{

}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
    
}

void PlayerMove::SetAnim()
{
   
}
