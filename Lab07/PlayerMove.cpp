//
//  PlayerMove.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/3/22.
//

#include "PlayerMove.hpp"
#include "Player.hpp"

PlayerMove::PlayerMove(class Player* owner)
: MoveComponent(owner)
{
    mPlayer = owner;
}

void PlayerMove::Update(float deltaTime)
{
    //Update the players movement
    mPlayer->SetPosition(mPlayer->GetPosition() + (velocity * deltaTime));
}
