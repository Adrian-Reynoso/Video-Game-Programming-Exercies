//
//  EnemyMove.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/20/22.
//

#include "EnemyMove.hpp"
#include "Actor.h"
#include <string>
#include <fstream>
#include "CSVHelper.h"
#include "HeightMap.hpp"
#include "Math.h"

EnemyMove::EnemyMove(Actor* owner)
: VehicleMove(owner)
{
    //For storing the points of route from the CSV file
    //Create your ifstream
    std::string fileName = "Assets/HeightMap/Path.csv";
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
        if (!line.empty() && temp[0] == "Node")
        {
            //Convert the strings to integer values
            std::vector<int> temp2;
            temp2.push_back(std::stoi(temp[1]));
            temp2.push_back(std::stoi(temp[2]));
            
            //Convert the (cell row, cell col) to world space with the height map’s CellToWorld
            HeightMap heightMap;
            Vector3 worldSpace = heightMap.CellToWorld(temp2[0], temp2[1]);
            
            //Push the worldspace from the height map to the pointsOnRoute vector
            pointsOnRoute.push_back(worldSpace);
        }
    }
    
    //Set the position of the owner to the first point in the path
    mOwner->SetPosition(pointsOnRoute[0]);
    
    //Modify tunable parameters to make Enemy more challenging
    minLinAccelMag = 2750.0f;
    maxLinAccelMag = 3000.0f;
//    accelRampTime = 1.5f;
//    angDragCoeff = 0.9f;
//    linDragCoeff_NotPressed = 0.9f;
}

void EnemyMove::Update(float deltaTime)
{
    //Figure out if you’re “close enough” to the next target point, and if you are, advance your “next target point” index
    if (Vector3::Distance(mOwner->GetPosition(), pointsOnRoute[nextTargetIndex]) <= pointThreshold)
    {
        nextTargetIndex++;
        
        if (nextTargetIndex == (int) pointsOnRoute.size())
        {
            nextTargetIndex = 0;
        }
    }
    
    //Figure out if the next target point is “reasonably” in front of you, and if it is, set the pedal to true
    //Make a vector v from enemy to target and normalize
    Vector3 v = pointsOnRoute[nextTargetIndex] - mOwner->GetPosition();
    v = Vector3::Normalize(v);
    //If dot product b/w my forward and normalized v is close enough to 1, press the pedal
    if (Vector3::Dot(mOwner->GetForward(), v) >= 1.0f-turnThreshold)
    {
        SetPedal(true);
    }
    else
    {
        SetPedal(false);
    }
    
    //Figure out if you think you need to turn, and if so, whether it’s left or right, and set the turn state based on this
    if (Vector3::Dot(mOwner->GetForward(), v) >= 1.0f-turnThreshold)
    {
        SetDirection(None);
    }
    else
    {
        //Do the cross product to determine which direction the kart needs to turn
        Vector3 c = Vector3::Cross(mOwner->GetForward(), v);
        
        if (c.z >= 0.0f)
        {
            SetDirection(Right);
        }
        else
        {
            SetDirection(Left);
        }
    }
    
    //Call update from Vehicle Move
    VehicleMove::Update(deltaTime);
}
