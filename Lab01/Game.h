#pragma once
#include "SDL2/SDL.h"

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
    
    private:
        //Pointer to window and renderer
        SDL_Window* windowPtr;
        SDL_Renderer* rendererPtr;
        bool gameIsRunning = true;
        SDL_Point gamePaddle {20, 384}; //Center coordinates
        SDL_Point gameBall {512, 384}; //Center coordinates
    
        //Drawing constants
        const int wallThickness = 15;
        const int paddleHeight = 110;
        const int paddleWidth = 15;
        const int ballWidth = 15;
};
