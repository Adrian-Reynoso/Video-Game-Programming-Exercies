//
//  HUD.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 4/19/22.
//

#include "HUD.hpp"
#include "Font.h"
#include "Shader.h"
#include "Texture.h"
#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "Player.hpp"
#include "SecurityCamera.hpp"
#include "Math.h"

HUD::HUD(Actor* owner)
: UIComponent(owner)
{
    //Dynamically allocate a Font into mFont, and then load the font file
    mFont = new Font();
    mFont->Load("Assets/Inconsolata-Regular.ttf");
    
    //Load in textures
    mTimerText = mFont->RenderText("00:00.00");
    mCheckpointText = mFont->RenderText(" ");
    AddACoin();
    
    radarBackgroundTexture = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Radar.png");
    arrowTexture = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/RadarArrow.png");
    blipTexture = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Blip.png");
}

HUD::~HUD()
{
    mFont->Unload();
    delete mFont;
}

void HUD::Update(float deltaTime)
{
    //Increment the time by deltaTime
    mOwner->GetGame()->timerIncrement += deltaTime;
    float timerIncrement = mOwner->GetGame()->timerIncrement;
    
    //Check if checkpoint was hit. If so increment the text timer
    if (hitCheckpoint)
    {
        checkpointTextTimer += deltaTime;
        
        //Check if timer is greater than 5 seconds
        if (checkpointTextTimer > 5.0f)
        {
            //Reset checkpoint text and variables
            hitCheckpoint = false;
            checkpointTextTimer = 0.0f;
            mCheckpointText = mFont->RenderText(" ");
        }
    }
    
    //Call Unload() on mTimerText
    mTimerText->Unload();
    
    //delete mTimerText
    delete mTimerText;
    
    //Format the timer time into a string (it should be MM:SS.FF) where FF is fractional amount of seconds. Don’t forget the leading zeros, and don’t have more than 2 digits after the decimal
    int minutes = (int) (timerIncrement / 60);
    int seconds = (int) timerIncrement % 60;
    int fracSeconds = (int) (timerIncrement * 100) % 100;

    std::string sMin = std::to_string(minutes);
    std::string sSec = std::to_string(seconds);
    std::string fSec = std::to_string(fracSeconds);
    std::string finalTime;
    
    //Add to the finalTime string starting with minutes section
    if (minutes > 9)
    {
        finalTime += sMin;
    }
    else
    {
        finalTime += "0" + sMin;
    }
    
    //Seconds section
    if (seconds > 9)
    {
        finalTime += ":" + sSec;
    }
    else
    {
        finalTime += ":0" + sSec;
    }
    
    //Fractional seconds section
    if (fracSeconds > 9)
    {
        finalTime += "." + fSec;
    }
    else
    {
        finalTime += ".0" + fSec;
    }
    
    //Call mFont->RenderText, like you did in the constructor
    mTimerText = mFont->RenderText(finalTime);
}

void HUD::Draw(class Shader *shader)
{
    //Draw the mTimerText in the bottom left corner
    DrawTexture(shader, mTimerText, Vector2(-420.0f, -325.0f));
    
    //Draw the mCoin Text right above it
    DrawTexture(shader, mCoinText, Vector2(-443.0f, -285.0f));
    
    //Draw the Checkpoint text
    DrawTexture(shader, mCheckpointText, Vector2::Zero);
    
    //Draw Radar Texture Layers
    DrawTexture(shader, radarBackgroundTexture, Vector2(400.0f, -280.0f));
    DrawTexture(shader, arrowTexture, Vector2(400.0f, -280.0f));
    RadarCalculations(shader);
    
}

void HUD::AddACoin()
{
    //Get the coin counter from game
    int coinCounter = mOwner->GetGame()->coinCounter;
    
    std::string coins = std::to_string(coinCounter);
    std::string finalOutput;
    
    //Add to the finalTime string starting with minutes section
    if (coinCounter > 9)
    {
        finalOutput += coins + "/55";
    }
    else if (coinCounter == 0)
    {
        finalOutput += "00/55";
    }
    else
    {
        finalOutput += "0" + coins + "/55";
    }
    
    //Render the text
    mCoinText = mFont->RenderText(finalOutput);
}

void HUD::UpdateCheckpointText(std::string text)
{
    mCheckpointText = mFont->RenderText(text);
    hitCheckpoint = true;
    checkpointTextTimer = 0.0f;
}

void HUD::RadarCalculations(class Shader *shader)
{
    //For the players forward vector, convert (x,y,z) -> (x,y)
    Vector3 forward3D = mOwner->GetGame()->GetPlayer()->GetForward();
    Vector2 forward2D;
    forward2D.x = forward3D.x;
    forward2D.y = forward3D.y;
    
    //Create rototation matrix
    float rotation = Math::Atan2(forward2D.y, forward2D.x);
    Matrix3 rotationMatrix = Matrix3::CreateRotation(rotation);
    
    
    //For the players pos vector, convert (x,y,z) -> (x,y)
    Vector3 playerPos3D = mOwner->GetGame()->GetPlayer()->GetPosition();
    Vector2 playerPos2D;
    playerPos2D.x = playerPos3D.y;
    playerPos2D.y = playerPos3D.x;
    
    //For each security camera in the level
    for (SecurityCamera* cam : mOwner->GetGame()->GetCameraVector())
    {
        //Construct 2D location from 3D
        Vector3 objectPos3D = cam->GetPosition();
        Vector2 objectPos2D;
        objectPos2D.x = objectPos3D.y;
        objectPos2D.y = objectPos3D.x;
        
        //Get Vector from player to object
        Vector2 playerToObject = objectPos2D - playerPos2D;
        
        //If in range of radar
        if (playerToObject.Length() <= 1500.0f)
        {
            playerToObject.x /= 1500.0f;
            playerToObject.y /= 1500.0f;
            playerToObject = playerToObject * 92.0f;
            playerToObject = Vector2::Transform(playerToObject, rotationMatrix);
            playerToObject += radarCenter;
            
            //For the blips forward vector, convert (x,y,z) -> (x,y)
            Vector3 bforward3D = cam->GetWorldForward();
            Vector2 bforward2D;
            bforward2D.x = bforward3D.y;
            bforward2D.y = bforward3D.x;
            
            //Create rototation matrix
            float brotation = Math::Atan2(bforward2D.y, bforward2D.x);
            DrawTexture(shader, blipTexture, playerToObject, 1, brotation);
        }
    }
}
