//
//  CameraComponent.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/15/22.
//

#ifndef CameraComponent_hpp
#define CameraComponent_hpp

#include <stdio.h>
#include "Math.h"
#include "Component.h"
#include "CollisionComponent.h"

class CameraComponent : public Component
{
public:
    CameraComponent(class Player* owner);
    void Update(float deltaTime) override;
    Vector3 calcIdealPos();
    float GetPitchSpeed() const { return mPitchSpeed; }
    void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
    
private:
    class Player* mPlayer;
    float HDist = 60.0;
    float VDist = 70.0;
    float TargetDist = 50.0;
    float horizDistance = 60.0f;
    float targetOffset = 50.0f;
    float springConstant = 256.0f;
    float dampeningConstant = 0.0f;
    Vector3 cameraPosition {0.0f, 0.0f, 0.0f};
    Vector3 cameraVelocity {0.0f, 0.0f, 0.0f};
    float mPitchAngle = 0.0f;
    float mPitchSpeed = 0.0f;
    Vector3 upVector = Vector3::UnitZ;
    float upVectorAngle = 0.0f;
    float upVectorSpeed = 3.0f;
    CollSide runningOn = CollSide::None;
};

#endif /* CameraComponent_hpp */
