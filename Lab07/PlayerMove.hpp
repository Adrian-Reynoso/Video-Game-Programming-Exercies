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

class PlayerMove : public MoveComponent
{
    public:
    PlayerMove(class Player* owner);
    void Update(float deltaTime) override;
    float speed = 400.0f;
    Vector3 velocity {speed, 0.0f, 0.0f};
    
    private:
    class Player* mPlayer;

};
#endif /* PlayerMove_hpp */
