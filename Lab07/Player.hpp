//
//  Player.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/3/22.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "Actor.h"

class Player : public Actor
{
    public:
    Player(class Game* game);
    class MeshComponent* meshComponent;
    class CollisionComponent* GetCollisionComponent()
    {
        return collisionComponent;
    }
    class PlayerMove* playerMove;
    int shieldLevel = 3;
    class HUD* hud;
    
    private:
    class CollisionComponent* collisionComponent;
};
#endif /* Player_hpp */
