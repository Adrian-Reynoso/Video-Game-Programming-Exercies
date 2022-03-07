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
#include <fstream>
#include "Renderer.h"
#include "Random.h"
#include "Player.hpp"
#include "SideBlock.hpp"
#include "Block.hpp"

Game::Game()
:mIsRunning(true)
{
	
}

bool Game::Initialize()
{
	Random::Init();

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// TODO: Create renderer
    mRenderer = new Renderer(this);
    if (!mRenderer->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        SDL_Log("Renderer Failed to Initialize");
        return false;
    }
    
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	LoadData();

	mTicksCount = SDL_GetTicks();
	
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.01667f)
	{
		deltaTime = 0.01667f;
	}
	mTicksCount = SDL_GetTicks();
	
	// Make copy of actor vector
	// (iterate over this in case new actors are created)
	std::vector<Actor*> copy = mActors;
	// Update all actors
	for (auto actor : copy)
	{
		actor->Update(deltaTime);
	}

	// Add any actors to destroy to a temp vector
	std::vector<Actor*> destroyActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			destroyActors.emplace_back(actor);
		}
	}

	// Delete the destroyed actors (which will
	// remove them from mActors)
	for (auto actor : destroyActors)
	{
		delete actor;
	}
    
    //For continusously generating obstacles, check if player position is passed spawnCheckpoint. If so, increment the chekpoint and call loadBlocks
    if (mPlayer->GetPosition().x >= spawnCheckpoint)
    {
        //Call load data with a correct fileName
        stringBlockFileNumber = std::to_string(blockFileNumber);
        blockFileNumber++;
        LoadBlocks("Assets/Blocks/"+ stringBlockFileNumber +".txt");
        
        //Increment spawn checkpoint by 1000
        spawnCheckpoint += 1000.0f;
        
        //If blockFileNumber is greater than 20, that means you went through the whole file, so therefore set randomize to true
        if (blockFileNumber > 20)
        {
            randomize = true;
        }
        
        //If randomize is true, do the random calculations here for the next spawned obstacle
        if (randomize)
        {
            blockFileNumber = Random::GetIntRange(1, 20);
        }
    }
}

void Game::GenerateOutput()
{
	// TODO: tell renderer to draw
    mRenderer->Draw();
}

void Game::LoadData()
{
    //Create a player
    mPlayer = new Player(this);
    
    //Initialize the projection matrix and use it in renderer
    Matrix4 projection = Matrix4::CreatePerspectiveFOV(1.22f, WINDOW_WIDTH, WINDOW_HEIGHT, 10.0f, 10000.0f);
    mRenderer->SetProjectionMatrix(projection);
    
    //Initialize the view matrix and use it in renderer
    Matrix4 view = Matrix4::CreateLookAt(Vector3{-300, 0, 0}, Vector3{20, 0, 0}, Vector3::UnitZ);
    mRenderer->SetViewMatrix(view);
    
    //Load the first few block obstacles
    for (int i = 0; i < 3; i++)
    {
        stringBlockFileNumber = std::to_string(blockFileNumber);
        blockFileNumber++;
        LoadBlocks("Assets/Blocks/"+ stringBlockFileNumber +".txt");
    }
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy sounds
	for (auto s : mSounds)
	{
		Mix_FreeChunk(s.second);
	}
	mSounds.clear();
}

Mix_Chunk* Game::GetSound(const std::string& fileName)
{
	Mix_Chunk* chunk = nullptr;
	auto iter = mSounds.find(fileName);
	if (iter != mSounds.end())
	{
		chunk = iter->second;
	}
	else
	{
		chunk = Mix_LoadWAV(fileName.c_str());
		if (!chunk)
		{
			SDL_Log("Failed to load sound file %s", fileName.c_str());
			return nullptr;
		}

		mSounds.emplace(fileName, chunk);
	}
	return chunk;
}

void Game::Shutdown()
{
	UnloadData();
	Mix_CloseAudio();
	// TODO: Delete renderer
    mRenderer->Shutdown();
    delete mRenderer;
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mActors.end() - 1;
		std::iter_swap(iter, iter2);
		mActors.pop_back();
	}
}

void Game::RemoveBlock(class Block* block)
{
    //use std::find (in <algorithm>) to get an iterator of the Actor*
    auto it = std::find(blockVector.begin(), blockVector.end(), block);
    
    //then erase to remove the element the iterator points to
    if (it != blockVector.end())
    {
        blockVector.erase(it);
    }
}

//loadBlocks() function implementation
void Game::LoadBlocks(std::string fileName)
{
    //Create your ifstream
    std::ifstream filein(fileName);

    //Create z position variable
    float zPos = 237.5 + 25.0f;

    //Make a for-loop that goes through the file and takes each line
    for (std::string line; std::getline(filein, line); )
    {
        //y position for variable
        float yPos = -237.5 - 25.0f;

        //Add 25 to z position
        zPos -= 25.0f;

        //goes through the string and, if there is an actor needed to be created, create it
        for (unsigned long i = 0; i < line.size(); i++)
        {
            //Index through line and with if statements see if an actor needs to be created

            //Add 25 to y position
            yPos += 25.0f;

            //For spawning regular blocks
            if ((char)line[i] == 'A')
            {
                //Call block constructor
                Block* block = new Block(this, (char)line[i]);
                block->SetPosition(Vector3(blockXPos, yPos, zPos));
            }

            //For Spawning Mario
            if ((char)line[i] == 'B')
            {
                //Call block constructor
                Block* block = new Block(this, (char)line[i]);
                block->SetPosition(Vector3(blockXPos, yPos, zPos));
            }

            //If not above, that means its a period
        }
    }
    
    //Increment xPos by 1000 units to load next obstacle correctly
    blockXPos += 1000.0f;

}
