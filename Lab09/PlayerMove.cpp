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
#include "CameraComponent.hpp"

PlayerMove::PlayerMove(class Player* owner)
: MoveComponent(owner)
{
    mPlayer = owner;
}

void PlayerMove::Update(float deltaTime)
{
    MoveComponent::Update(deltaTime);
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
    //Forward/Backward
    if (keyState[SDL_SCANCODE_W])
    {
        SetForwardSpeed(forwardSpeed);
    }
    else if (keyState[SDL_SCANCODE_S])
    {
        SetForwardSpeed(forwardSpeed * -1);
    }
    else if ((keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_S]) ||(!keyState[SDL_SCANCODE_W] && !keyState[SDL_SCANCODE_S]))
    {
        SetForwardSpeed(0.0f);
    }
    
    //Strafing
    if (keyState[SDL_SCANCODE_D])
    {
        SetStrafeSpeed(forwardSpeed);
    }
    else if (keyState[SDL_SCANCODE_A])
    {
        SetStrafeSpeed(forwardSpeed * -1);
    }
    else if ((keyState[SDL_SCANCODE_D] && keyState[SDL_SCANCODE_A]) ||(!keyState[SDL_SCANCODE_D] && !keyState[SDL_SCANCODE_A]))
    {
        SetStrafeSpeed(0.0f);
    }
    
    //Relative Mouse Movement
    int x, y;
    SDL_GetRelativeMouseState(&x, &y);
    
    //Calculate the angular speed and pass into MoveComponent
    float angularSpeed = (x / 500.0f) * Math::Pi * 10.0f;
    SetAngularSpeed(angularSpeed);
    
    //Calculate the angular speed and pass into MoveComponent
    float pitchSpeed = (y / 500.0f) * Math::Pi * 10.0f;
    mPlayer->cameraComponent->SetPitchSpeed(pitchSpeed);
}

