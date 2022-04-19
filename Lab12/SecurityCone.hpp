//
//  SecurityCone.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 4/13/22.
//

#ifndef SecurityCone_hpp
#define SecurityCone_hpp

#include <stdio.h>
#include "Actor.h"
#include "Math.h"

class SecurityCone : public Actor
{
    public:
    SecurityCone(class Game* game, Actor* parent);
    void OnUpdate(float deltaTime) override;
    class MeshComponent* meshComponent;
    bool IfYellowCone()
    {
        if (isInFOV && isInProximity)
        {
            return true;
        }
        
        return false;
    }
    bool GetPlayerIsDetected() {return playerIsDetected;}
    
    private:
    float heightByScale;
    float halfAngleOfCone = Math::Pi / 6.0f;
    bool isInProximity = false;
    bool isInFOV = false;
    bool playerIsDetected = false;
    float cameraCountdown = 0.0f;
    float deathCountdown = 0.0f;
    int coneSoundChannel = -1;
};
#endif /* SecurityCone_hpp */
