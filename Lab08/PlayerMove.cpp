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
    mPlayer = owner;
}

void PlayerMove::Update(float deltaTime)
{
    //Update Position
    Vector3 tempPos = mPlayer->GetPosition();
    
    //Edit the eye position and then use that for the target position
    Vector3 eyePos = tempPos - (mPlayer->GetForward() * HDist) + (Vector3::UnitZ * VDist);
    Vector3 targetPos = tempPos + (mPlayer->GetForward() * TargetDist);
    
    //create Matrix4::CreateLookAt and call on renderer
    Matrix4 lookAt = Matrix4::CreateLookAt(eyePos, targetPos, Vector3::UnitZ);
    mPlayer->GetGame()->GetRenderer()->SetViewMatrix(lookAt);
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{

}

