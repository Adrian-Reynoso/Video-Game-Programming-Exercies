//
//  TiledBGComponent.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/17/22.
//

#include "TiledBGComponent.hpp"
#include "CSVHelper.h"
#include "Actor.h"
#include "Game.h"
#include "Math.h"
#include <fstream>
#include <string>

TiledBGComponent::TiledBGComponent(Actor* owner, int drawOrder)
    :SpriteComponent(owner)
{

}

void TiledBGComponent::Draw(SDL_Renderer* renderer)
{
    if (mTexture)
    {
        for (unsigned long i = 0; i < intsFromCSV.size(); i++)
        {
            for (unsigned long j = 0; j < intsFromCSV[i].size(); j++)
            {
                SDL_Rect r;
                r.w = static_cast<int>(m_tileWidth);
                r.h = static_cast<int>(m_tileHeight);
                // Center the rectangle around the position of the owner
                r.x = static_cast<int>(j * m_tileWidth) -  (int)mOwner->GetGame()->cameraPosition.x;
                r.y = static_cast<int>(i * m_tileHeight) - (int)mOwner->GetGame()->cameraPosition.y;
                
                SDL_Rect source;
                source.w = m_tileWidth;
                source.h = m_tileHeight;
                source.x = (intsFromCSV[i][j] % (GetTexWidth() / m_tileWidth)) * m_tileWidth;
                source.y = (intsFromCSV[i][j] / (GetTexWidth() / m_tileHeight)) * m_tileHeight;
                
                //Don't print anything if value from vector is -1
                if (intsFromCSV[i][j] != -1)
                {
                    // Draw (have to convert angle from radians to degrees, and clockwise to counter)
                    SDL_RenderCopyEx(renderer,
                        mTexture,
                        &source,
                        &r,
                        0.0,
                        nullptr,
                        SDL_FLIP_NONE);
                }
            }
        }
    }
}

void TiledBGComponent::LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight)
{
    //Create your ifstream
    std::ifstream filein(fileName);
    
    //Assign tileWidth and Height to the member variables
    m_tileWidth = tileWidth;
    m_tileHeight = tileHeight;
    
    //Make a for-loop that goes through the file and takes each line
    while (filein)
    {
        //Use the CSVHelper::split function to turn the
        std::string line;
        std::getline(filein, line);
        std::vector<std::string> temp;
        
        //Check if string is empty, if so don't try to split it
        if (!line.empty())
        {
            temp = CSVHelper::Split(line);
            std::vector<int> temp2;
            
            //Turn the string in each vector to an integer in the new temp2 vector
            for (std::string line : temp)
            {
                temp2.push_back(std::stoi(line));
            }
            
            //Push this vector into the intsFromCSV member variable vector
            intsFromCSV.push_back(temp2);
        }
    }
}
