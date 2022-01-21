#pragma once
#include "SDL2/SDL.h"
#include <vector>

// TODO
//Create a new Game class with respective public member functions
class Game
{
    public:
        Game();
        bool Initialize();
        void Shutdown();
        void RunLoop();
        //Functions for game loop
        void ProcessInput();
        void UpdateGame();
        void GenerateOuput();
        //Functions for Actors
        void AddActor(class Actor* actor);
        void RemoveActor(class Actor* actor);
    
    private:
        //Pointer to window and renderer
        SDL_Window* windowPtr;
        SDL_Renderer* rendererPtr;
        bool gameIsRunning = true;
        
        //For Update Game
        float previousFrameTime = 0.0;
        float deltaTime;
    
        //Actors
        std::vector<class Actor*> mActors;
    
        //LoadData and UnloadData private Functions
        void LoadData();
        void UnloadData();

};
