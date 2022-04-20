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
    void RadarCalculations(class Shader *shader);
    class Font* mFont;
    class Texture* mTimerText;
    class Texture* mCoinText;
    class Texture* mCheckpointText;
    class Texture* radarBackgroundTexture;
    class Texture* arrowTexture;
    class Texture* blipTexture;
    
    private:
    float checkpointTextTimer = 0.0f;
    bool hitCheckpoint = false;
    Vector2 radarCenter {400.0f, -280.0f};

};
#endif /* HUD_hpp */
