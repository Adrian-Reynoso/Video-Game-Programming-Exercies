//
//  VehicleMove.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/15/22.
//

#include "VehicleMove.hpp"
#include "Actor.h"
#include "HeightMap.hpp"
#include "Game.h"
#include <fstream>
#include "CSVHelper.h"


VehicleMove::VehicleMove(class Actor* owner)
: Component(owner, 50)
{
    //Load in checkpoints
    //Create your ifstream
    std::string fileName = "Assets/HeightMap/Checkpoints.csv";
    std::ifstream filein(fileName);
    
    //Make a for-loop that goes through the file and takes each line
    while (filein)
    {
        //Use the CSVHelper::split function to turn the
        std::string line;
        std::getline(filein, line);
        std::vector<std::string> temp;
        temp = CSVHelper::Split(line);
        
        //Check if string is empty and it's of type Node, if not don't continue
        if (!line.empty() && temp[0] == "Checkpoint")
        {
            //Convert the strings to integer values
            std::vector<int> temp2;
            temp2.push_back(std::stoi(temp[1]));
            temp2.push_back(std::stoi(temp[2]));
            temp2.push_back(std::stoi(temp[3]));
            temp2.push_back(std::stoi(temp[4]));
            
            //Push the worldspace from the height map to the pointsOnRoute vector
            checkpointInfo.push_back(temp2);
            
        }
    }
}

void VehicleMove::Update(float deltaTime)
{
    MovingKart(deltaTime);
    
    //Calculate the position
    Vector3 temp = mOwner->GetPosition();
    temp += velocity * deltaTime;
    mOwner->SetPosition(temp);
    
    //Check if your (x, y) coordinate is on the track, and if so, set the z-position to the result of GetHeight
    Vector2 temp2 {temp.x, temp.y};
    if (mOwner->GetGame()->heightMap->IsOnTrack(temp2))
    {
        Vector3 newZPos = temp;
        newZPos.z = (float) mOwner->GetGame()->heightMap->GetHeight(temp2);
        
        mOwner->SetPosition(Vector3::Lerp(temp, newZPos, 0.1f));
    }
    
    //Apply linear drag to velocity
    if (isPedalPressed)
    {
        velocity *= linDragCoeff_Pressed;
    }
    else
    {
        velocity *= linDragCoeff_NotPressed;
    }
    
    //If the vehicle is turning, Euler integrate the angular velocity (negative for turning left, positive for turning right)
    if (mDirection == Direction::Left)
    {
        angularVelocity += angularAccel * deltaTime * -1;
    }
    else if (mDirection == Direction::Right)
    {
        angularVelocity += angularAccel * deltaTime;
    }
    
    //Update angle
    float angle = mOwner->GetRotation();
    angle += angularVelocity * deltaTime;
    mOwner->SetRotation(angle);
    
    //Apply Angular Drag
    angularVelocity *= angDragCoeff;
    
    //Figure out if you made it to the “next” checkpoint
    AtNextCheckpoint();

}

float VehicleMove::DistNextCheckpoint()
{
    Vector2 minPointCell {(float)checkpointInfo[lastCheckpoint + 1][0], (float) checkpointInfo[lastCheckpoint + 1][2]};
    Vector2 maxPointCell {(float)checkpointInfo[lastCheckpoint + 1][1], (float)checkpointInfo[lastCheckpoint + 1][3]};
    
    //Convert one of them to world position
    HeightMap heightMap;
    Vector3 checkpoint = heightMap.CellToWorld((int)minPointCell.x, (int)minPointCell.y);
    
    return Vector3::Distance(mOwner->GetPosition(), checkpoint);
}

void VehicleMove::AtNextCheckpoint()
{
    HeightMap heightMap;
    
    Vector3 pPos = mOwner->GetPosition();
    Vector2 pPosCell = heightMap.WorldToCell(Vector2{pPos.x, pPos.y});
    
    Vector2 minPointCell {(float)checkpointInfo[lastCheckpoint + 1][0], (float) checkpointInfo[lastCheckpoint + 1][2]};
    Vector2 maxPointCell {(float)checkpointInfo[lastCheckpoint + 1][1], (float)checkpointInfo[lastCheckpoint + 1][3]};
    
    if (((int)pPosCell.y <= (int)maxPointCell.y && (int)pPosCell.y >= (int)minPointCell.y) && ((int)pPosCell.x <= (int)maxPointCell.x && (int)pPosCell.x >= (int)minPointCell.x))
    {
        lastCheckpoint++;
        
        //For wrap around
        if (lastCheckpoint == (int)checkpointInfo.size() - 1)
        {
            lastCheckpoint = -1;
        }
        
        if (lastCheckpoint == 0)
        {
            //Increment lap
            currLap++;
            OnLapChange(currLap);
        }
    }
}

void VehicleMove::MovingKart(float deltaTime)
{
    if (isPedalPressed)
    {
        //Update Acceleration time
        accelerationTracker += deltaTime;
        
        //Find acceleration Magnitude and velocity through Euler integreation
        float temp = Math::Clamp(accelerationTracker / accelRampTime, 0.0f, 1.0f);
        float accelMagnitude = Math::Lerp(minLinAccelMag, maxLinAccelMag, temp);
        velocity += mOwner->GetForward() * accelMagnitude * deltaTime;
    }
    else
    {
        //Reset acceleration time
        accelerationTracker = 0.0f;
    }
}
