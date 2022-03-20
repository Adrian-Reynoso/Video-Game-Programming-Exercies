//
//  Enemy.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/20/22.
//

#ifndef Enemy_hpp
#define Enemy_hpp

#include <stdio.h>
#include "Actor.h"

class Enemy : public Actor
{
public:
    Enemy(class Game* game);
    class MeshComponent* meshComponent;
    class EnemyMove* enemyMove;
};
#endif /* Enemy_hpp */
