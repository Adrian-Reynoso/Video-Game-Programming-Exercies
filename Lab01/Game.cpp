//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"

// TODO
//Implementation for the functions in our Game class
Game::Game()
{
    
}

//Implements Initialize function
bool Game::Initialize()
{
    //First, call SDL function to see if game can initialize
    int initializeChecker = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    
    //Return false if there was an error with Initialization
    if (initializeChecker != 0)
    {
        return false;
    }
    
    //Create a window: 1024x768 pixels
    windowPtr = SDL_CreateWindow("Pong Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL);
    
    //Check if the window could open, if not return false
    if (windowPtr == NULL)
    {
        return false;
    }
    
    //Create a renderer
    rendererPtr = SDL_CreateRenderer(windowPtr, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    //Check if the window could open, if not return false
    if (rendererPtr == NULL)
    {
        return false;
    }
    
    //Past this point, we know initialization works
    return true;
}

//Implements Shutdown function
void Game::Shutdown()
{
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
                gameIsRunning = false;
                break;
        }
    }
    
}

//Implements UpdateGame function
void Game::UpdateGame()
{
    
}

//Implements GenerateOuput function
void Game::GenerateOuput()
{
    
}

