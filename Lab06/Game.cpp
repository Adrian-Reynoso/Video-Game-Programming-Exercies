//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <algorithm>
#include "Actor.h"
#include <SDL2/SDL_image.h>
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Random.h"
#include <fstream>
#include "CollisionComponent.h"
#include "Player.hpp"
#include "TiledBGComponent.hpp"
#include "CSVHelper.h"
#include "Collider.hpp"
#include "Bush.hpp"
#include "Soldier.hpp"
#include "PathFinder.h"

//Implementation for the functions in our Game class
//Constructor
Game::Game()
{
}

//Implements Initialize function
bool Game::Initialize()
{
    //Initialize the basic random library
    Random::Init();
    
    //First, call SDL function to see if game can initialize
    int initializeChecker = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    
    //Return false if there was an error with Initialization
    if (initializeChecker != 0)
    {
        return false;
    }
    
    //Create a window:
    windowPtr = SDL_CreateWindow("Zelda Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)SCREENWIDTH, (int)SCREENHEIGHT, SDL_WINDOW_OPENGL);
    
    //Check if the window could open, if not return false
    if (windowPtr == NULL)
    {
        return false;
    }
    
    //Create a renderer
    rendererPtr = SDL_CreateRenderer(windowPtr, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    //To make the window a smaller size
//    SDL_RenderSetLogicalSize(rendererPtr, (int)SCREENWIDTH, (int)SCREENHEIGHT);
    
    //Check if the window could open, if not return false
    if (rendererPtr == NULL)
    {
        return false;
    }
    
    int initted = IMG_Init(IMG_INIT_PNG);
    if ((initted & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        printf("IMG_Init: Failed to init required jpg and png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
    }
    
    //For sound effects
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    
    //Call LoadData
    LoadData();
    
    //Past this point, we know initialization works
    return true;
}

//Implements Shutdown function
void Game::Shutdown()
{
    //To close sound effects
    Mix_CloseAudio();
    
    //Delete all the cached sounds with Mix_FreeChunk
    std::unordered_map<std::string, Mix_Chunk*>::iterator it;

    for (it = mSoundMap.begin(); it != mSoundMap.end(); it++)
    {
        Mix_FreeChunk(it->second);
        it->second = nullptr;
    }
    
    //Call UnloadData
    UnloadData();
    
    //Call Image Quit
    IMG_Quit();
    
    //Destroy renderer, window, and quit SDL
    SDL_DestroyRenderer(rendererPtr);
    SDL_DestroyWindow(windowPtr);
    atexit(SDL_Quit);
}

//Implements RunLoop function
void Game::RunLoop()
{
    //Run an inifnite loop, calling the respective functions needed IN ORDER
    while (gameIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOuput();
    }
}

//Implements ProcessInput function
void Game::ProcessInput()
{
    //Create an event variable
    SDL_Event event;
    
    //Poll for events
    while (SDL_PollEvent(&event))
    {
        
        //Check what type of event it is
        switch (event.type)
        {
            //If the user wants to quit
            case SDL_QUIT:
                //Switch the gameIsRunnig bool value to false
                gameIsRunning = false;
                break;
        }
    }
    
    //Grab the state of the entire Keyboard
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    
    //Make a copy of the actor vector.
    std::vector<Actor*> mActorsCopy = mActors;
    
    //Loop over the copy of all actors and call ProcessInput on each
    for (unsigned long i = 0; i < mActorsCopy.size(); i++)
    {
        mActorsCopy[i]->ProcessInput(keyboardState);
    }
    
    //Check if escape, up, or down was pressed. If so, set gameIsRunning to false or its respective changes
    if (keyboardState[SDL_SCANCODE_ESCAPE])
    {
        gameIsRunning = false;
    }
    
}

//Implements UpdateGame function
void Game::UpdateGame()
{
    //1) Frame Limit with while loop
    while (previousFrameTime + 16 > SDL_GetTicks())
    {
    }
    
    //2) Calculate delta time (as a float in seconds)
    float currentTime = (float) SDL_GetTicks();
    deltaTime = currentTime - previousFrameTime;
    //Update previousFrameTime
    previousFrameTime = currentTime;
    //Convert the deltaTime milliseconds to seconds
    deltaTime /= 1000;
    
    
    //3) Cap to maximum delta time to no more than 0.033 seconds
    if (deltaTime > 0.033)
    {
        deltaTime = float(0.033);
    }
    //SDL_Log("My deltaTime is %f", deltaTime);
    
    //4) Update all Actors by delta time
    //Make a copy of the actors Vector
    std::vector<Actor*> mActorsCopy = mActors;
    
    //Loop over the copy and call Update on each actor
    for (unsigned long i = 0; i < mActorsCopy.size(); i++)
    {
        mActorsCopy[i]->Update(deltaTime);
    }
    
    //Make a temporary Actor* vector for actors to destroy
    std::vector<Actor*> tempActors;
    
    //Loop over the actor vector, and any actors which are in state ActorState::Destroy should be added to the temporary vector from step 3
    for (unsigned long i = 0; i < mActors.size(); i++)
    {
        if (mActors[i]->GetState() == ActorState::Destroy)
        {
            //Add to the tempActors vector
            tempActors.push_back(mActors[i]);
        }
    }
    
    //Loop over the vector from step 3 and delete each actor in it (this will automatically call the Actor destructor, which then calls RemoveActor, which removes the actor from the Game’s vector).
    for (unsigned long i = 0; i < tempActors.size(); i++)
    {
        delete tempActors[i];
    }
    
    //Check to see if intro music has finished, if it has, then play the looping music
    if (!Mix_Playing(soundMusicLoop1) && !Mix_Playing(soundMusicLoop2))
    {
        soundMusicLoop2 = Mix_PlayChannel(-1, GetSound("Assets/Sounds/MusicLoop.ogg"), -1);
    }

}

//Implements GenerateOuput function
void Game::GenerateOuput()
{
    //Set the render draw color to blue
    SDL_SetRenderDrawColor(rendererPtr, 0, 0, 0, 255);
    
    //Clear the backbuffer
    SDL_RenderClear(rendererPtr);
    
    //DRAW YOUR GAME OBJECTS:
    //loop over the sprite component vector. If visible, call Draw on it
    for (unsigned long i = 0; i < spriteCompVector.size(); i++)
    {
        if (spriteCompVector[i]->IsVisible())
        {
            //Call Draw
            spriteCompVector[i]->Draw(rendererPtr);
        }
    }
    
    //Present the render
    SDL_RenderPresent(rendererPtr);
}

//ACTOR FUNCTIONS

//Takes in an Actor* and adds it to the vector in Game
void Game::AddActor(Actor* actor)
{
    mActors.push_back(actor);
}

//Takes in an Actor*, removes the actor from the vector
void Game::RemoveActor(Actor* actor)
{
    //use std::find (in <algorithm>) to get an iterator of the Actor*
    auto it = std::find(mActors.begin(), mActors.end(), actor);
    
    //then erase to remove the element the iterator points to
    if (it != mActors.end())
    {
        mActors.erase(it);
    }
}

//DATA FUNCTIONS
void Game::LoadData()
{
    //Create an actor that has a TiledBGComponent
    Actor* background = new Actor(this);
    TiledBGComponent* component = new TiledBGComponent(background);
    
    //Call LoadTileCSV on it, passing in the "Assets/Map/Tiles.csv" file and a tile width/height of 32
    component->LoadTileCSV("Assets/Map/Tiles.csv", 32, 32);
    component->SetTexture(GetTexture("Assets/Map/Tiles.png"));
    
    //Allocate an instance of the PathFinder actor
    pathFinder = new PathFinder(this);
    
    //Load in objects with the respective function
    LoadInObjects();
    
    //"Assets/Sounds/MusicStart.ogg" should play initially (not looping)
    soundMusicLoop1 = Mix_PlayChannel(-1, GetSound("Assets/Sounds/MusicStart.ogg"), 0);
}

void Game::UnloadData()
{
    //Call delete on all the actors in the vector
    while(mActors.size() > 0)
    {
        delete mActors.back();
    }
    
    //call SDL_DestroyTexture on every texture in the textures map, and then clear the map.
    for (auto it = mHashmap.begin(); it != mHashmap.end(); it++)
    {
        SDL_DestroyTexture(it->second);
    }
    mHashmap.clear();
}

//SPRITE FUNCTIONS
//takes in a file name and returns an SDL_Texture*
SDL_Texture* Game::GetTexture(std::string fileName)
{
    //first check if a file by that name is in the hash map
    std::unordered_map<std::string,SDL_Texture*>::iterator it;
    it = mHashmap.find(fileName);
    
    //If the texture IS in the Hashmap
    if (it != mHashmap.end())
    {
        return mHashmap.find(fileName)->second;
    }
    
    //If the texture IS NOT in the Hashmap
    //load the texture with that file name.
    SDL_Surface *image;
    image = IMG_Load(fileName.c_str());
    
    //If the image failed to load
    if (image == nullptr)
    {
        //SDL_Log message that says which texture file it tried to load, and that it failed to load it
        SDL_Log("Tried to but failed to load %s ", fileName.c_str());
        return 0;
    }
    //If it didn't fail to load
    else
    {
        //to convert the SDL_Surface* to an SDL_Texture*
        SDL_Texture *texture = SDL_CreateTextureFromSurface(rendererPtr, image);
        
        //Free the SDL_Surface* using SDL_FreeSurface
        SDL_FreeSurface(image);
        
        //Add an entry to the map for that file, so subsequent calls to GetTexture will find the file in the map
        mHashmap.insert(std::pair<std::string,SDL_Texture*>(fileName, texture));
        
        //Return the texture pointer variable
        return texture;
    }
}

//Adds the sprite to the vector and then sorts the vector by draw order
void Game::AddSprite(class SpriteComponent* sprite)
{
    //Add sprite to vector
    spriteCompVector.push_back(sprite);
    
    //Sort the vector in draw order
    std::sort(spriteCompVector.begin(), spriteCompVector.end(),
        [](SpriteComponent* a, SpriteComponent* b) {
            return a->GetDrawOrder() < b->GetDrawOrder();
    });
}

void Game::RemoveSprite(class SpriteComponent* sprite)
{
    //use std::find (in <algorithm>) to get an iterator of the Actor*
    auto it = std::find(spriteCompVector.begin(), spriteCompVector.end(), sprite);
    
    //then erase to remove the element the iterator points to
    if (it != spriteCompVector.end())
    {
        spriteCompVector.erase(it);
    }
}

Mix_Chunk* Game::GetSound(const std::string& filename)
{
    //Check if sound is already in the map
    if (mSoundMap.find(filename) != mSoundMap.end())
    {
        return mSoundMap.find(filename)->second;
    }
    
    //If it's not in the map, add it in
    mSoundMap[filename] = Mix_LoadWAV(filename.c_str());
    return mSoundMap.find(filename)->second;
}

void Game::LoadInObjects()
{
    //Create your ifstream
    std::ifstream filein("Assets/Map/Objects.csv");
    
    //Make a variable that will skip the Header line of the CSV file
    int lineNumber = 0;
    
    //Make a for-loop that goes through the file and takes each line
    while (filein)
    {
        //Use the CSVHelper::split function to turn the
        std::string line;
        std::getline(filein, line);
        std::vector<std::string> temp;
        
        //Update lineNumber
        lineNumber++;
        
        //Check if string is empty, if so don't try to split it
        if (!line.empty())
        {
            temp = CSVHelper::Split(line);
            
            //Declare position variables
            float tempX;
            float tempY;
            float tempWidth;
            float tempHeight;
            
            if (lineNumber != 1)
            {
                //Calculate the position
                tempX = (float)std::stoi(temp[1]) + (float)(std::stoi(temp[3])/2);
                tempY = (float)std::stoi(temp[2]) + (float)(std::stoi(temp[4])/2);
                tempWidth = (float)std::stoi(temp[3]);
                tempHeight = (float)std::stoi(temp[4]);
            }
            
            //If the object is of type player
            if (temp[0] == "Player")
            {
                //Create an instance of type player and set it to the right position
                player = new Player(this);
                player->SetPosition(Vector2{tempX, tempY});
            }
            
            //If the object is of type Collider
            else if (temp[0] == "Collider")
            {
                //Create an instance of type collider and set it to the right position
                Collider* tempCollider = new Collider(this, tempWidth, tempHeight);
                tempCollider->SetPosition(Vector2{tempX, tempY});
                
                //Add this collider to our collider vector
                colliderVector.push_back(tempCollider);
            }
            
            //If the object is of type Bush
            else if (temp[0] == "Bush")
            {
                //Create an instance of type Bush and set it to the right position
                Bush* bush = new Bush(this);
                bush->SetPosition(Vector2{tempX, tempY});
            }
            
            //If the object is of type Soldier
            else if (temp[0] == "Soldier")
            {
                //Create an instance of type Bush and set it to the right position
                //Use Pathfinder member function to get the two PathNodes* corresponding (rowStart, colStart) and (rowEnd, colEnd)
                PathNode* start = pathFinder->GetPathNode(std::stoi(temp[5]), std::stoi(temp[6]));
                PathNode* end = pathFinder->GetPathNode(std::stoi(temp[7]), std::stoi(temp[8]));
                Soldier* soldier = new Soldier(this, start, end);
                
                //Fix positioning of the soldier
                soldier->SetPosition(Vector2{tempX, tempY});
            }

        }
    }
}

void Game::RemoveEnemyComponent(class EnemyComponent *enemy)
{
    //use std::find (in <algorithm>) to get an iterator of the Actor*
    auto it = std::find(enemyComponentVector.begin(), enemyComponentVector.end(), enemy);
    
    //then erase to remove the element the iterator points to
    if (it != enemyComponentVector.end())
    {
        enemyComponentVector.erase(it);
    }
}
