//
//  VehicleMove.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/15/22.
//

#ifndef VehicleMove_hpp
#define VehicleMove_hpp

#include <stdio.h>
#include "Math.h"
#include "Component.h"
#include <vector>

class VehicleMove : public Component
{
public:
    enum Direction
    {
        Left,
        Right,
        None
    };
    
    VehicleMove(class Actor* owner);
    
    // Update the move component
    void Update(float deltaTime) override;
    
    // Getters/setters
    void SetPedal(bool setting)
    {
        isPedalPressed = setting;
    }
    void SetDirection(Direction direction)
    {
        mDirection = direction;
    }
    virtual void OnLapChange(int newLap) { }
    int GetCurrLap()
    {
        return currLap;
    }
    int GetLastCheckpoint()
    {
        return lastCheckpoint;
    }
    float DistNextCheckpoint();
    void AtNextCheckpoint();
    void MovingKart(float deltaTime);
    
protected:
    bool isPedalPressed = false;
    Vector3 velocity {0.0f, 0.0f, 0.0f};
    float angularVelocity = 0.0f;
    float accelerationTracker = 0.0f;
    float minLinAccelMag = 1000.0f;
    float maxLinAccelMag = 2500.0f;
    float accelRampTime = 2.0f;
    float angularAccel = 5.0f * Math::Pi;
    float linDragCoeff_Pressed = 0.9f;
    float linDragCoeff_NotPressed = 0.975f;
    float angDragCoeff = 0.9f;
    Direction mDirection = Direction::None;
    std::vector<std::vector<int>> checkpointInfo;
    int currLap = 0;
    int lastCheckpoint = -1;
    
};
#endif /* VehicleMove_hpp */
