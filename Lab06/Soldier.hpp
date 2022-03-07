//
//  Soldier.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/22/22.
//

#ifndef Soldier_hpp
#define Soldier_hpp

#include <stdio.h>
#include "Actor.h"

class Soldier : public Actor
{
    public:
        //Constructor
        Soldier(class Game* game, class PathNode* start, class PathNode* end);
        class AnimatedSprite* spriteComponent = nullptr;
        class CollisionComponent* collisionComponent = nullptr;
        class SoldierAI* soldierAI = nullptr;
        class EnemyComponent* enemyComponent = nullptr;
        class Effect* effect = nullptr;
};

#endif /* Soldier_hpp */
