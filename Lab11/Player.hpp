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
    Player(class Game* game, Actor* parent);
    class PlayerMove* playerMove;
    class CameraComponent* cameraComponent;
    class CollisionComponent* collisionComponent;
    
};
#endif /* Player_hpp */
