#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include "TextureManager.h"
#include "Random.h"
#include "Space.h"
#include "UI.h"

using namespace std;

class Board {
private:
	static void LoadConfigs(string path); //Reads config file
	static void LoadBoardFile(string path);
	static void LoadSpaces(); //Creates tiles
	static void LoadMines(); //Randomly places mines
	static void LoadNeighbors(); //Populate each space's neighbor list
	static void ResetBoard();
	static void RevealAllMines(sf::RenderWindow& window);
	static void FlagAllMines(sf::RenderWindow& window);
	static void TerminateGame(bool victory);
	static void CheckVictory(unsigned int row, unsigned int col);

	static unsigned int safeSpaceCount;
	static int boardActive;
	static unsigned int columnCount;
	static unsigned int rowCount;
	static unsigned int mineCount;
	static unsigned int flagCount;
	static unsigned int tilesRevealedCount;

public:
	static vector<vector<Space>> gridSpace;

	enum GameState {ACTIVE = 0, LOSS = 1, WIN = 2};
	
	static const int TILE_SIZE = 32; //In pixels

	static int GetBoardActive();
	static unsigned int GetColumnCount();
	static unsigned int GetRowCount();
	static unsigned int GetFlagCount();
	static int GetRevealedTileCount();
	static int GetMineCount();

	static void InitializeBoard(bool fromFile, string boardFileNum = "nofile"); //Default file name
	static void DrawBoard(sf::RenderWindow& window);
	static void RevealEvent(sf::Vector2i& mousePosition);
	static void FlagEvent(sf::Vector2i& mousePosition);
};

