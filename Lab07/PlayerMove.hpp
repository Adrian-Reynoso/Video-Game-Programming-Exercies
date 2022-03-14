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

class PlayerMove : public MoveComponent
{
    public:
    PlayerMove(class Player* owner);
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    float GetSpeedMultiplier()
    {
        return speedMultiplier;
    }
    
    private:
    //Private functions for updating game
    void handlePeppyCooldown();
    void trackBarrelRoll(float deltaTime);
    void loadBlock(float pos1, float pos2, int indexPattern, bool isRotated);
    void isShipDamaged();
    
    class Player* mPlayer;
    float HDist = 300.0;
    float VDist = 0.0;
    float TargetDist = 20.0;
    float spawnCheckpoint = 0.0f;
    float lastBlockPosition = 3500.0f;
    std::vector<int> sideIndexPattern {0, 1, 2, 0};
    std::vector<int> topIndexPattern {6, 7};
    int indexCounter = 0;
    bool spaceIsPressed = false;
    bool qIsPressed = false;
    bool isRolling = false;
    bool gotDamaged = false;
    class Bullet* bullet;
    float speedCount = 0.0f;
    float rollCount = 0.0f;
    float peppyCooldown = 0.0f;
    
    float speed = 400.0f;
    Vector3 velocity {speed, 1.0f, 1.0f};
    Vector3 direction {1.0f, 0.0f, 0.0f};
    void DestroyExplodingBlock(class Block* block);
    float speedMultiplier = 1.0f;


};
#endif /* PlayerMove_hpp */
