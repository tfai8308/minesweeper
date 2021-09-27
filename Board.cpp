#include "Board.h"

vector<vector<Space>> Board::gridSpace;
unsigned int Board::safeSpaceCount;
unsigned int Board::columnCount;
unsigned int Board::rowCount;
unsigned int Board::mineCount;
unsigned int Board::flagCount;
unsigned int Board::tilesRevealedCount;
int Board::boardActive;

void Board::LoadConfigs(string path) {
	ifstream inFile(path);

	if (inFile.is_open()) {
		
		inFile >> columnCount;
		inFile >> rowCount;
		inFile >> mineCount;

		inFile.close();
	}
	else {
		cout << "Failed to open file at " << path << endl;
	}
}

void Board::LoadBoardFile(string path) {
	ifstream inFile(path);

	if (inFile.is_open()) {

		for (unsigned int row = 0; row < rowCount; row++) {
			for (unsigned int col = 0; col < columnCount; col++) {
				
				char bit;
				inFile.get(bit);

				if (bit == '\n') { //Don't do anything for newline characters.
					col--;
				}
				else if (bit == '1') { //Place a mine if there's a 1 in the file.
					gridSpace[row][col].PlaceMine();
				}
			}
		}
		inFile.close();
	}
	else {
		cout << "Failed to open file at " << path << endl;
	}
}

void Board::LoadSpaces() {
	//Create the 2D vector of Space objects
	gridSpace.resize(rowCount);
	for (unsigned int i = 0; i < gridSpace.size(); i++) {
		gridSpace[i].resize(columnCount);
	}

	//Give each tile an X and Y position relative to the application window
	//Where 32x32 are the tile dimensions 
	int yPos = 0;
	int xPos = 0;
	for (unsigned int row = 0; row < rowCount; row++) {
		yPos = row * TILE_SIZE;
		for (unsigned int col = 0; col < columnCount; col++) {
			xPos = col * TILE_SIZE;
			gridSpace[row][col].GetSprite().setPosition(xPos, yPos);
		}
	}
}

void Board::LoadMines() {
	for (unsigned int i = 0; i < mineCount; i++) {
		int randRow = Random::Int(0, rowCount - 1);
		int randCol = Random::Int(0, columnCount - 1);
		if (!gridSpace[randRow][randCol].HasMine()) {
			gridSpace[randRow][randCol].PlaceMine();
		}
		else {
			i--; //Find a different place to put a mine
		}
	}
}

void Board::LoadNeighbors() {
	for (unsigned int row = 0; row < rowCount; row++) {
		for(unsigned int col = 0; col < columnCount; col++) {
			//Want to check spaces at (row, col)
			// 
			//(-1, -1)	(-1, 0)	(-1, 1)	(-1, 2) (-1, 3)
			//(0, -1)	(0, 0)	(0, 1)	(0, 2)	(0, 3)
			//(1, -1)	(1, 0)	(1, 1)	(1, 2)	(1, 3)
			//(2, -1)	(2, 0)	(2, 1)	(2, 2)	(2, 3)
			//(3, -1)	(3, 0)	(3, 1)	(3, 2)	(3, 3)
			// 
			//Relative From (row, col)
			// 
			//neighborSpaces = {UL, UM, UR, ML, MR, DL, DM, DR}
			// 
			//ULft (row-1)(col-1), UMid (row-1)(col), URgt (row-1)(col+1)
			//MLft (row)(col-1),   Mid (current tile), MRgt (row)(col+1)
			//DLft (row+1)(col-1), DMid (row+1)(col), DRgt (row+1)(col+1)
			//
			//If (row = 0), POS 0, 1, 2 = nullptr
			//If (col = 0), POS 0, 3, 5 = nullptr
			//If (row = rowCount - 1), POS 5, 6, 7 = nullptr;
			//If (col = colCount - 1), POS 2, 4, 7 = nullptr;
			//Else, save the reference to space at [row][col]
			int index = 0;
			for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
				for (int colOffset = -1; colOffset <= 1; colOffset++) {

					if (rowOffset == 0 && colOffset == 0) {
						continue; //Don't add itself as a neighbor.
					}

					try {
						gridSpace[row][col].AddNeighbor(index, &gridSpace.at(row + rowOffset).at(col + colOffset));
					}
					catch (const out_of_range& excpt) {
						gridSpace[row][col].AddNeighbor(index, nullptr);
					}
					index++;
				}
			}
		}
	}
}

int Board::GetMineCount() {
	int count = 0;
	for (unsigned int row = 0; row < rowCount; row++) {
		for (unsigned int col = 0; col < columnCount; col++) {
			if (gridSpace[row][col].HasMine()) {
				count++;
			}
		}
	}
	return count;
}

void Board::ResetBoard() {
	flagCount = 0;
	tilesRevealedCount = 0;
	gridSpace.clear();
}

void Board::InitializeBoard(bool fromFile, string boardFileNum) {
	ResetBoard(); //Clears current game state variables.
	LoadConfigs("boards/config.cfg");
	LoadSpaces();

	if (fromFile) {
		string path = "boards/testboard" + boardFileNum + ".brd";
		LoadBoardFile(path);
		mineCount = GetMineCount(); //Test board dimensions correspond to config.cfg, but mine count does not follow the config.
	}
	else {
		LoadMines();
	}

	LoadNeighbors();
	safeSpaceCount = rowCount * columnCount - mineCount; //Find out how many safe tiles there are.
	boardActive = GameState::ACTIVE;
}

void Board::TerminateGame(bool victory) {
	boardActive = victory ? GameState::WIN : GameState::LOSS;
}

void Board::DrawBoard(sf::RenderWindow& window) {
	for (unsigned int row = 0; row < rowCount; row++) {
		for (unsigned int col = 0; col < columnCount; col++) {
			gridSpace[row][col].Update(window);
		}
	}

	if (UI::GetDebugToggled() || boardActive == GameState::LOSS) { //All mines are revealed if debug mode is on, or the game is lost.
		RevealAllMines(window);
	}
	if (boardActive == GameState::WIN) { //All mines are flagged if the game is won.
		FlagAllMines(window);
	}
}

void Board::RevealAllMines(sf::RenderWindow& window) {
	for (unsigned int row = 0; row < rowCount; row++) {
		for (unsigned int col = 0; col < columnCount; col++) {
			if (gridSpace[row][col].HasMine()) {

				//The Board will handle drawing states related to ALL spaces.
				if (boardActive == GameState::LOSS) { //If the game is lost, all mine spaces are opened.
					gridSpace[row][col].DrawTileRevealed(window);
				}
				if (gridSpace[row][col].HasFlag()) {  //A flag is always drawn if the space is flagged.
					gridSpace[row][col].DrawFlag(window);
				}
				gridSpace[row][col].DrawMine(window);
			}
		}
	}
}

void Board::FlagAllMines(sf::RenderWindow& window) {
	for (unsigned int row = 0; row < rowCount; row++) {
		for (unsigned int col = 0; col < columnCount; col++) {
			if (gridSpace[row][col].HasMine()) {
				gridSpace[row][col].DrawFlag(window);
			}
		}
	}
	flagCount = mineCount; //If the player wins, all mines are flagged. The counters are set equal.
}

void Board::RevealEvent(sf::Vector2i& mousePosition) {
	if (boardActive == GameState::ACTIVE) {
		for (unsigned int row = 0; row < rowCount; row++) {
			for (unsigned int col = 0; col < columnCount; col++) {
				if (gridSpace[row][col].GetSprite().getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
					gridSpace[row][col].Reveal();

					CheckVictory(row, col);
				}
			}
		}
	}
}

void Board::CheckVictory(unsigned int row, unsigned int col) {
	//End conditions
	if (GetRevealedTileCount() == safeSpaceCount) { //All safe tiles have been opened.
		TerminateGame(true);
	}
	if (!gridSpace[row][col].HasFlag() && gridSpace[row][col].HasMine()) { //A mine was clicked.
		TerminateGame(false);
	}
}

int Board::GetRevealedTileCount() {
	int count = 0; //Start counting from 0 each time.
	for (unsigned int row = 0; row < rowCount; row++) {
		for (unsigned int col = 0; col < columnCount; col++) {
			if (gridSpace[row][col].IsRevealed()) {
				count++;
			}
		}
	}
	return count;
}

void Board::FlagEvent(sf::Vector2i& mousePosition) {
	if (boardActive == GameState::ACTIVE) {
		for (unsigned int row = 0; row < rowCount; row++) {
			for (unsigned int col = 0; col < columnCount; col++) {
				if (!gridSpace[row][col].IsRevealed() && gridSpace[row][col].GetSprite().getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {

					gridSpace[row][col].SwitchFlag();

					if (gridSpace[row][col].HasFlag()) { //Track the number of flags on the board.
						flagCount++;
					}
					else{
						flagCount--;
					}
				}
			}
		}
	}
}

int Board::GetBoardActive() {
	return boardActive;
}

unsigned int Board::GetColumnCount() {
	return columnCount;
}

unsigned int Board::GetRowCount() {
	return rowCount;
}

unsigned int Board::GetFlagCount() {
	return flagCount;
}