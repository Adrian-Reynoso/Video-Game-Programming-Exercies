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
    class Arrow* arrow;
    class HUD* mHUD;
    class CollisionComponent* collisionComponent;
    void SetRespawnPosition(Vector3 pos) {respawnPos = pos;}
    Vector3 GetRespawnPosition() {return respawnPos;}
    void SetToBeRespawned(bool value) {toBeRespawned = value;}
    bool GetToBeRespawned() {return toBeRespawned;}
    
    private:
    Vector3 respawnPos;
    bool toBeRespawned = false;
    
};
#endif /* Player_hpp */
