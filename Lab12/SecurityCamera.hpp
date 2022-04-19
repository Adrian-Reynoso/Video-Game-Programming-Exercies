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
    ~SecurityCamera();
    void OnUpdate(float deltaTime) override;
    class MeshComponent* meshComponent;
    class SecurityCone* securityCone;
    void SetStartQ(Quaternion q) {startQ = q;}
    void SetEndQ(Quaternion q) {endQ = q;}
    void SetInterpTime(float value) {interpTime = value;}
    void SetPauseTime(float value) {pauseTime = value;}
    int VolumeForPlayerDist();
    
    private:
    Quaternion startQ;
    Quaternion endQ;
    Quaternion currentQ;
    float interpTime;
    float pauseTime;
    float counter = 0.0f;
    bool isPaused = false;
    int camSoundChannel = -1;
    int camStopSoundChannel = -1;
    bool soundWhenYellow = false;
};
#endif /* SecurityCamera_hpp */
