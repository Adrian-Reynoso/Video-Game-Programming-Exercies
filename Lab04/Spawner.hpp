//
//  Spawner.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/4/22.
//

#ifndef Spawner_hpp
#define Spawner_hpp

#include <stdio.h>
#include "Actor.h"
class Spawner : public Actor
{
    public:
    Spawner(class Game* game);
    void OnUpdate(float deltaTime) override;
    
    private:
        float distance;
        class Goomba* goomba;
    
};
#endif /* Spawner_hpp */
