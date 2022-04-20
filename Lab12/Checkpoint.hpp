//
//  Checkpoint.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 4/12/22.
//

#ifndef Checkpoint_hpp
#define Checkpoint_hpp

#include <stdio.h>
#include "Actor.h"
#include <string>

class Checkpoint : public Actor
{
    public:
    Checkpoint(class Game* game, Actor* parent);
    void OnUpdate(float deltaTime) override;
    void SetLevelString(std::string name) { mLevelString = name;}
    void SetTextString(std::string name) { checkpointText = name;}
    class MeshComponent* meshComponent;
    class CollisionComponent* collisionComponent;
    bool isActive = false;
    
    private:
    std::string mLevelString = "";
    std::string checkpointText = "";
};
#endif /* Checkpoint_hpp */
