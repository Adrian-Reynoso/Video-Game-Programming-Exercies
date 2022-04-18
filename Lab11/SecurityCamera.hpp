//
//  SecurityCamera.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 4/13/22.
//

#ifndef SecurityCamera_hpp
#define SecurityCamera_hpp

#include <stdio.h>
#include "Actor.h"

class SecurityCamera : public Actor
{
    public:
    SecurityCamera(class Game* game, Actor* parent);
    void OnUpdate(float deltaTime) override;
    class SecurityCone* securityCone;
    void SetStartQ(Quaternion q) {startQ = q;}
    void SetEndQ(Quaternion q) {endQ = q;}
    void SetInterpTime(float value) {interpTime = value;}
    void SetPauseTime(float value) {pauseTime = value;}
    
    private:
    Quaternion startQ;
    Quaternion endQ;
    Quaternion currentQ;
    float interpTime;
    float pauseTime;
    float counter = 0.0f;
    bool isPaused = false;
};
#endif /* SecurityCamera_hpp */
