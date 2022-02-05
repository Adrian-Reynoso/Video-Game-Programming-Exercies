//
//  GoombaMove.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/4/22.
//

#ifndef GoombaMove_hpp
#define GoombaMove_hpp

#include <stdio.h>
#include <vector>
#include "MoveComponent.h"
#include <unordered_map>
#include <SDL2/SDL_image.h>

class GoombaMove : public MoveComponent
{
    public:
        //Constructor
        GoombaMove(class Goomba* owner);
        // Update the PlayerMove
        void Update(float deltaTime) override;
        class Goomba* mGoomba;
    
    private:
        float speed = -100.0f;
        float mYSpeed = 0.0f;
        float mYAccel = 2000.0f;
        float lifetime = 0.0f;
    
};

#endif /* GoombaMove_hpp */
