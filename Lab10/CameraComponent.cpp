//
//  CameraComponent.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/15/22.
//

#include "CameraComponent.hpp"
#include "Player.hpp"
#include "PlayerMove.hpp"
#include "Game.h"
#include "Renderer.h"

CameraComponent::CameraComponent(class Player* owner)
: Component(owner)
{
    mPlayer = owner;
}

void CameraComponent::Update(float deltaTime)
{    
    //Update Position
    Vector3 tempPos = mOwner->GetPosition();

    //Update pitchAngle
    mPitchAngle += mPitchSpeed * deltaTime;
    mPitchAngle = Math::Clamp(mPitchAngle, -Math::Pi / 4.0f, Math::Pi / 4.0f);
    
    //Edit the eye position and then use that for the target position
    Vector3 camForward = Vector3::Transform(Vector3(1.0f, 0.0f, 0.0f), Matrix4::CreateRotationY(mPitchAngle) * Matrix4::CreateRotationZ(mOwner->GetRotation()));
    
    Vector3 target = mOwner->GetPosition() + camForward * 10.0f;
    
    //Create the up vector as the third parameter for our lookAt Matrix (When wall running)
    Vector3 upVector;
    if (mPlayer->playerMove->isWallRunning)
    {
        //Check which CollSide player is running on
        if (mPlayer->playerMove->isRunningOn == CollSide::Front || mPlayer->playerMove->isRunningOn == CollSide::Back)
        {
            camForward = Vector3::Transform(Vector3(1.0f, 0.0f, 1.0f), Matrix4::CreateRotationY(Math::PiOver2));
        }
        else if (mPlayer->playerMove->isRunningOn == CollSide::Left || mPlayer->playerMove->isRunningOn == CollSide::Right)
        {
            camForward = Vector3::Transform(Vector3(0.0f, 1.0f, 1.0f), Matrix4::CreateRotationX(Math::PiOver2));
        }
    }
    else
    {
        upVector = Vector3::UnitZ;
    }
    
    //create Matrix4::CreateLookAt and call on renderer
    Matrix4 lookAt = Matrix4::CreateLookAt(tempPos, target, upVector);
    mPlayer->GetGame()->GetRenderer()->SetViewMatrix(lookAt);
}

Vector3 CameraComponent::calcIdealPos()
{
    //Update Position
    Vector3 tempPos = mPlayer->GetPosition();
    
    //Edit the eye position and then use that for the target position
    Vector3 eyePos = tempPos - (mPlayer->GetForward() * HDist) + (Vector3::UnitZ * VDist);
    eyePos.z = 70.0f;
    
    return eyePos;
}
