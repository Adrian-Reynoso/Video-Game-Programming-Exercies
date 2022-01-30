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
#include "Ship.hpp"
#include "MoveComponent.h"
#include "Random.h"
#include "Asteroid.hpp"

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
    
    //Create a window: 1024x768 pixels
    windowPtr = SDL_CreateWindow("Asteroids Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL);
    
    //Check if the window could open, if not return false
    if (windowPtr == nullptr)
    {
        return false;
    }
    
    //Create a renderer
    rendererPtr = SDL_CreateRenderer(windowPtr, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    //Check if the window could open, if not return false
    if (rendererPtr == nullptr)
    {
        return false;
    }
    
    int initted = IMG_Init(IMG_INIT_PNG);
    if ((initted & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        printf("IMG_Init: Failed to init required jpg and png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
    }
    
    //Call LoadData
    LoadData();
    
    //Past this point, we know initialization works
    return true;
}

//Implements Shutdown function
void Game::Shutdown()
{
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
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    
    //Make a copy of the actor vector.
    std::vector<Actor*> mActorsCopy = mActors;
    
    //Loop over the copy of all actors and call ProcessInput on each
    for (Actor* actor : mActorsCopy)
    {
        actor->ProcessInput(keyboardState);
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
    for (Actor* actor : mActorsCopy)
    {
        actor->Update(deltaTime);
    }
    
    //Make a temporary Actor* vector for actors to destroy
    std::vector<Actor*> tempActors;
    
    //Loop over the actor vector, and any actors which are in state ActorState::Destroy should be added to the temporary vector from step 3
    for (Actor* actor : mActorsCopy)
    {
        if (actor->GetState() == ActorState::Destroy)
        {
            //Add to the tempActors vector
            tempActors.push_back(actor);
        }
    }
    
    //Loop over the vector from step 3 and delete each actor in it (this will automatically call the Actor destructor, which then calls RemoveActor, which removes the actor from the Game’s vector).
    for (Actor* actor : tempActors)
    {
        delete actor;
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
    for (SpriteComponent* spriteComponent : spriteCompVector)
    {
        if (spriteComponent->IsVisible())
        {
            //Call Draw
            spriteComponent->Draw(rendererPtr);
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
    //Load in the individual sprites

    //For stars
    Actor* test4 = new Actor(this);
    SpriteComponent* sc4 = new SpriteComponent(test4, 5);
    sc4->SetTexture(GetTexture("Assets/Stars.png"));
    test4->SetPosition(Vector2(512.0f, 384.0f));
    
    //For Ship
    Ship* ship = new Ship(this);
    ship->SetPosition(Vector2(SCREENWIDTH/2, SCREENHEIGHT/2));
    
    //Create 10 asteroids
    Asteroid* asteroids[10];
    for (int i = 0; i < 10; i++)
    {
        asteroids[i] = new Asteroid(this);
    }
    
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


void Game::AddAsteroid(class Asteroid* actor)
{
    //Add asteroid to the asteroid vector
    asteroidTracker.push_back(actor);
}

void Game::RemoveAsteroid(class Asteroid* actor)
{
    //use std::find (in <algorithm>) to get an iterator of the Asteroid*
    auto it = std::find(asteroidTracker.begin(), asteroidTracker.end(), actor);
    
    //then erase to remove the element the iterator points to
    if (it != asteroidTracker.end())
    {
        asteroidTracker.erase(it);
    }
}
