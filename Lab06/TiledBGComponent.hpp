//
//  TiledBGComponent.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/17/22.
//

#ifndef TiledBGComponent_hpp
#define TiledBGComponent_hpp

#include <stdio.h>
#include <vector>
#include "SpriteComponent.h"

class TiledBGComponent : public SpriteComponent
{
public:
    // (Lower draw order corresponds with further back)
    TiledBGComponent(class Actor* owner, int drawOrder = 50);
    
    void Draw(SDL_Renderer* renderer) override;
    
    void LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight);
    
private:
    int m_tileWidth;
    int m_tileHeight;
    std::vector<std::vector<int>> intsFromCSV;
};
#endif /* TiledBGComponent_hpp */
