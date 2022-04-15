//
//  PlayerMove.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/3/22.
//

#ifndef PlayerMove_hpp
#define PlayerMove_hpp

#include <stdio.h>
#include "Math.h"
#include "MoveComponent.h"
#include <vector>
#include "SDL2/SDL_mixer.h"
#include "CollisionComponent.h"

class PlayerMove : public MoveComponent
{
    public:
    PlayerMove(class Player* owner);
    ~PlayerMove();
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void UpdateOnGround(float deltaTime);
    void UpdateJump(float deltaTime);
    void UpdateFalling(float deltaTime);
    void UpdateWallClimb(float deltaTime);
    void UpdateWallRun(float deltaTime);
    void PhysicsUpdate(float deltaTime);
    void FixXYVelocity();
    void AddForce(const Vector3& force)
    {
        mPendingForces += force;
    }
    
    enum MoveState
    {
        OnGround,
        Jump,
        Falling,
        WallClimb,
        WallRun
    };
    
    void ChangeState(MoveState state);
    bool CanWallClimb(CollSide collSide);
    bool CanWallRun(CollSide collSide);
    bool isWallRunning = false;
    CollSide isRunningOn = CollSide::None;
    
    protected:
    CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);
    
    private:
    bool oppositeSigns(int x, int y)
    {
        return ((x ^ y) < 0);
    }
    
    //Private functions for updating game
    class Player* mPlayer;
    float HDist = 60.0;
    float VDist = 70.0;
    float TargetDist = 50.0;
    std::vector<int> sideIndexPattern {0, 1, 2, 0};
    std::vector<int> topIndexPattern {6, 7};
    float forwardSpeed = 350.0f;
    MoveState mCurrentState;
    bool spaceWasPressed = false;
    Vector3 mVelocity;
    Vector3 mAcceleration;
    Vector3 mPendingForces;
    float mMass = 1.0f;
    Vector3 mGravity {0.0f, 0.0f, -980.0f};
    Vector3 mJumpForce{0.0f, 0.0f, 35000.0f};
    Vector3 wallClimbForce{0.0f, 0.0f, 1800.0f};
    float maxSpeed = 400.0f;
    float mWallClimbTimer = 0.0f;
    Vector3 mWallRunForce {0.0f, 0.0f, 1200.0f};
    float mWallRunTimer = 0.0f;
    int playerRunChannel = 0;
};
#endif /* PlayerMove_hpp */
