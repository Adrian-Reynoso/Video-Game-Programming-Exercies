#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
#include "Math.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	Mix_Chunk* GetSound(const std::string& fileName);

	class Renderer* GetRenderer() {	return mRenderer; }
    
    const float WINDOW_WIDTH = 1024.0f;
    const float WINDOW_HEIGHT = 768.0f;
    
    class HeightMap* heightMap;
    
    //Getters/Setters
    class Player* GetPlayer()
    {
        return mPlayer;
    }
    const std::vector<class Actor*> &GetBlockVector()
    {
        return blockVector;
    }
    void SetPlayer(class Player* player)
    {
        mPlayer = player;
    }
    void SetNextLevel(std::string name)
    {
        mNextLevel = name;
    }
    void RemoveBlock(class Actor* block);
    void AddBlock(class Actor* block);
    void RemoveCamera(class SecurityCamera* cam);
    void AddCamera(class SecurityCamera* cam);
    const std::vector<class SecurityCamera*> &GetCameraVector()
    {
        return securityCamVector;
    }
    
    
    void LoadNextLevel();
    
    std::queue<class Checkpoint*> checkpointQueue;
    
    float timerIncrement = 0.0f;
    int coinCounter = 0;
    
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// All the actors in the game
	std::vector<class Actor*> mActors;
    std::vector<class Actor*> blockVector;

	class Renderer* mRenderer = nullptr;

	Uint32 mTicksCount = 0;
	bool mIsRunning;
    
    class Player* mPlayer;
    class Actor* track;
    
    std::string mNextLevel = "";
    
    std::vector<class SecurityCamera*> securityCamVector;
};
