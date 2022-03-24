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
#include "VehicleMove.hpp"
#include <vector>
#include "SDL2/SDL_mixer.h"

class PlayerMove : public VehicleMove
{
    public:
    PlayerMove(class Player* owner);
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    virtual void OnLapChange(int newLap) override;
    
    private:
    //Private functions for updating game
    class Player* mPlayer;
    float HDist = 60.0;
    float VDist = 70.0;
    float TargetDist = 50.0;
    std::vector<int> sideIndexPattern {0, 1, 2, 0};
    std::vector<int> topIndexPattern {6, 7};
    
    float speed = 400.0f;
    Vector3 velocity {speed, 1.0f, 1.0f};
    Vector3 direction {1.0f, 0.0f, 0.0f};
    bool hasPlayedFinal = false;
};
#endif /* PlayerMove_hpp */
