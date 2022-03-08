#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
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
    
    //Getters/Setters
    class Player* GetPlayer()
    {
        return mPlayer;
    }
    const std::vector<class Block*> &GetBlockVector()
    {
        return blockVector;
    }
    void AddBlock(class Block* block)
    {
        blockVector.push_back(block);
    }
    void RemoveBlock(class Block* block);
    
    //For loading blocks
    void LoadBlocks(std::string fileName);
    
    int soundChannel1 = 0;
    int soundChannel2 = 0;
    
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// All the actors in the game
	std::vector<class Actor*> mActors;
    std::vector<class Block*> blockVector;

	class Renderer* mRenderer = nullptr;

	Uint32 mTicksCount = 0;
	bool mIsRunning;
    
    class Player* mPlayer;
    
    float blockXPos = 1000.0f;
    int blockFileNumber = 1;
    std::string stringBlockFileNumber;
    float spawnCheckpoint = 0.0f;
    bool randomize = false;
    
    
};
