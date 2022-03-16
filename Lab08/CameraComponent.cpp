//
//  CameraComponent.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/15/22.
//

#include "CameraComponent.hpp"
#include "Player.hpp"
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
    Vector3 tempPos = mPlayer->GetPosition();
    
    //Calculate the damping constant
    dampeningConstant = 2.0f * sqrt(springConstant);
    
    //Do some numeric integration of the spring
    Vector3 displacement = cameraPosition - calcIdealPos();
    Vector3 springAccel = ((-1 * springConstant) * displacement) - (dampeningConstant * cameraVelocity);
    cameraVelocity += springAccel * deltaTime;
    cameraPosition += cameraVelocity * deltaTime;
    
    //Edit the eye position and then use that for the target position
    Vector3 targetPos = tempPos + (mPlayer->GetForward() * TargetDist);
    
    //create Matrix4::CreateLookAt and call on renderer
    Matrix4 lookAt = Matrix4::CreateLookAt(cameraPosition, targetPos, Vector3::UnitZ);
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

void CameraComponent::SnapToIdeal()
{
    //Calculate the ideal position of the camera and the target position. Then,  immediately set the view matrix to this ideal camera
    cameraPosition = calcIdealPos();
    
    Vector3 tempPos = mPlayer->GetPosition();
    Vector3 targetPos = tempPos + (mPlayer->GetForward() * TargetDist);
    
    Matrix4 lookAt = Matrix4::CreateLookAt(cameraPosition, targetPos, Vector3::UnitZ);
    mPlayer->GetGame()->GetRenderer()->SetViewMatrix(lookAt);
}
