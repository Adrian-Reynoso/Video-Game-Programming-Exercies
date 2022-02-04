//
//  PlayerMove.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/3/22.
//

#ifndef PlayerMove_hpp
#define PlayerMove_hpp

#include <stdio.h>
#include "MoveComponent.h"

class PlayerMove : public MoveComponent
{
    public:
        //Constructor
        PlayerMove(class Actor* owner);
    
        // Update the PlayerMove
        void Update(float deltaTime) override;
        void ProcessInput(const Uint8* keyState) override;
    
};

#endif /* PlayerMove_hpp */
