//
//  WrappingMove.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/27/22.
//

#ifndef WrappingMove_hpp
#define WrappingMove_hpp

#include <stdio.h>
#include "MoveComponent.h"
#include "Math.h"

class WrappingMove : public MoveComponent
{
    public:
        //Constructor
        WrappingMove(class Actor* owner, float dir);
        void Update(float deltaTime) override;
        //Direction owner should move in
        Vector2 direction{1.0, 1.0};
    
    private:
        float screenWidth = 896.0;
        
};
    
#endif /* WrappingMove_hpp */
