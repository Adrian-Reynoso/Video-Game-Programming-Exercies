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
#include "PlayerUI.h"
#include "Enemy.hpp"
#include "EnemyMove.hpp"

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
    
    //If it's the final lap
    if (currLap == 4 && !hasPlayedFinal)
    {
        Mix_FadeOutChannel(mPlayer->GetGame()->beginningSound, 250);
        Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/FinalLap.wav"), 0);
        mPlayer->GetGame()->beginningSound = Mix_FadeInChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/MusicFast.ogg"), -1, 4000);
        
        hasPlayedFinal = true;
    }
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

void PlayerMove::OnLapChange(int newLap)
{
    Enemy* enemy = mOwner->GetGame()->GetEnemy();

    if (newLap == 5)
    {
        Mix_FadeOutChannel(mPlayer->GetGame()->beginningSound, 250);
        
        //Determine if player won or enemy won
        if (newLap > enemy->enemyMove->GetCurrLap())
        {
            mPlayer->playerUI->SetRaceState(PlayerUI::Won);
            Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Won.wav"), 0);
        }
        else
        {
            mPlayer->playerUI->SetRaceState(PlayerUI::Lost);
            Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Lost.wav"), 0);
        }
        
        mPlayer->SetState(ActorState::Paused);
        enemy->SetState(ActorState::Paused);
    }
    else
    {
        mPlayer->playerUI->OnLapChange(newLap);
    }
}

