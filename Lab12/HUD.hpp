//
//  HUD.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 4/19/22.
//

#ifndef HUD_hpp
#define HUD_hpp

#include <stdio.h>
#include "UIComponent.h"
#include <string>

class HUD : public UIComponent
{
    public:
    HUD(Actor* owner);
    ~HUD();
    void Update(float deltaTime) override;
    void Draw(class Shader* shader) override;
    void AddACoin();
    void UpdateCheckpointText(std::string text);
    class Font* mFont;
    class Texture* mTimerText;
    class Texture* mCoinText;
    class Texture* mCheckpointText;
    
    private:
    float checkpointTextTimer = 0.0f;
    bool hitCheckpoint = false;

};
#endif /* HUD_hpp */
