#pragma once
#include "SDL2/SDL.h"
#include <vector>
#include <unordered_map>
#include <string>


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
        void AddSprite(class SpriteComponent* sprite);
        void RemoveSprite(class SpriteComponent* sprite);
        SDL_Texture* GetTexture(std::string fileName);
        const float SCREENHEIGHT = 768.0;
        const float SCREENWIDTH = 1024.0;
        
        //Functions for Asteroids
        //Keep track of asteroids
        std::vector<class Asteroid*> asteroidTracker;
        void AddAsteroid(class Asteroid* actor);
        void RemoveAsteroid(class Asteroid* actor);
    
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
    
        //Sprite Functions
        std::unordered_map<std::string, SDL_Texture*> mHashmap;
        std::vector<class SpriteComponent*> spriteCompVector;

};
