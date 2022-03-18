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
#include "HeightMap.hpp"

PlayerMove::PlayerMove(class Player* owner)
: VehicleMove(owner)
{
    mPlayer = owner;
    
    //Set the position of mOwner to the result of CellToWorld(39, 58)
    mOwner->SetPosition(mOwner->GetGame()->heightMap->CellToWorld(39, 58));
}

void PlayerMove::Update(float deltaTime)
{
    //Call the parent class Update function
    VehicleMove::Update(deltaTime);
    
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
    //For Pedal
    if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP])
    {
        SetPedal(true);
    }
    else
    {
        SetPedal(false);
    }
    
    //For Turning
    if ((keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) && (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]))
    {
        SetDirection(None);
    }
    else if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT])
    {
        SetDirection(Left);
    }
    else if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT])
    {
        SetDirection(Right);
    }
    else
    {
        SetDirection(None);
    }

}

