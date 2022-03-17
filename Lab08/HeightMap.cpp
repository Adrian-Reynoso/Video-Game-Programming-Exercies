//
//  HeightMap.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/15/22.
//

#include "HeightMap.hpp"
#include <string>
#include <fstream>
#include "CSVHelper.h"
#include "SDL2/SDL.h"

HeightMap::HeightMap()
{
    //Create your ifstream
    std::string fileName = "Assets/HeightMap/HeightMap.csv";
    std::ifstream filein(fileName);
    
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
            for (std::string stringNum : temp)
            {
                temp2.push_back(std::stoi(stringNum));
            }
            
            //Push this vector into the intsFromCSV member variable vector
            intsFromCSV.push_back(temp2);
        }
    }
    
    SDL_Log("Vector Rows: %i", (int)intsFromCSV.size());
    SDL_Log("Vector Column[1]: %i", (int)intsFromCSV[0].size());
    SDL_Log("Vector Column[2]: %i", (int)intsFromCSV[1].size());
    SDL_Log("Value at [2][2]: %i", intsFromCSV[2][2]);
    
    SDL_Log("Is spot on track: %i", IsCellOnTrack(39, 58));
    Vector3 spot {gridTop - cellSize * 39, gridLeft + cellSize * 58, GetHeightFromCell(39, 58)};
    
    SDL_Log("X, Y, and Z Positions: %f, %f, %f", spot.x, spot.y, spot.z);
}

bool HeightMap::IsCellOnTrack(int row, int column)
{
    //Returns true if that cell is on the track and false if otherwise
    if (row >= (int)intsFromCSV.size() || row < 0)
    {
        return false;
    }
    else if (column >= (int)intsFromCSV[row].size() || column < 0)
    {
        return false;
    }
    else if (intsFromCSV[row][column] == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

float HeightMap::GetHeightFromCell(int row, int column)
{
    //Returns a float value corresponding to the height of the cell
    return -40.0f + intsFromCSV[row][column] * 5.0f;
}

const Vector3& HeightMap::CellToWorld(int row, int column)
{
    //If the cell is not on the track
    if (IsCellOnTrack(row, column) == false)
    {
        return ifFalse;
    }
//    else
//    {
//        //Returns the Vector3 (x, y, z) position corresponding to that cell
//        return Vector3 {gridTop - cellSize * row, gridLeft + cellSize * column, GetHeightFromCell(row, column)};
//    }
    
    returnVector.x = gridTop - cellSize * row;
    returnVector.y = gridLeft + cellSize * column;
    returnVector.z = GetHeightFromCell(row, column);
    return returnVector;
}
