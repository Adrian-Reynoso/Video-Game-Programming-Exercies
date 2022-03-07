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
    float speed = 400.0f;
    Vector3 velocity {speed, 1.0f, 1.0f};
    Vector3 direction {1.0f, 0.0f, 0.0f};
    void DestroyExplodingBlock(class Block* block);
    
    
    private:
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
    bool gotDamaged = false;
    class Bullet* bullet;

};
#endif /* PlayerMove_hpp */
