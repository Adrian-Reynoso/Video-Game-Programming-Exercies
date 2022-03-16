//
//  VehicleMove.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/15/22.
//

#include "VehicleMove.hpp"
#include "Actor.h"


VehicleMove::VehicleMove(class Actor* owner)
: Component(owner, 50)
{
}

void VehicleMove::Update(float deltaTime)
{
    if (isPedalPressed)
    {
        //Update Acceleration time
        accelerationTracker += deltaTime;
        
        //Find acceleration Magnitude and velocity through Euler integreation
        float temp = Math::Clamp(accelerationTracker / accelRampTime, 0.0f, 1.0f);
        float accelMagnitude = Math::Lerp(minLinAccelMag, maxLinAccelMag, temp);
        velocity += mOwner->GetForward() * accelMagnitude * deltaTime;
    }
    else
    {
        //Reset acceleration time
        accelerationTracker = 0.0f;
    }
    
    //Calculate the position
    Vector3 temp = mOwner->GetPosition();
    temp += velocity * deltaTime;
    mOwner->SetPosition(temp);
    
    //Apply linear drag to velocity
    if (isPedalPressed)
    {
        velocity *= linDragCoeff_Pressed;
    }
    else
    {
        velocity *= linDragCoeff_NotPressed;
    }
    
    //If the vehicle is turning, Euler integrate the angular velocity (negative for turning left, positive for turning right)
    if (mDirection == Direction::Left)
    {
        angularVelocity += angularAccel * deltaTime * -1;
    }
    else if (mDirection == Direction::Right)
    {
        angularVelocity += angularAccel * deltaTime;
    }
    
    //Update angle
    float angle = mOwner->GetRotation();
    angle += angularVelocity * deltaTime;
    mOwner->SetRotation(angle);
    
    //Apply Angular Drag
    angularVelocity *= angDragCoeff;
    
}
