#include "Space.h"

Space::Space() {
	isRevealed = false;
	isFlagged = false;
	isMine = false;
	defaultTexture.setTexture(TextureManager::GetTexture("tile_hidden")); //Give the tile a default texture of hidden tile
}

void Space::DrawMine(sf::RenderWindow& window) {
	sf::Sprite mine(TextureManager::GetTexture("mine"));
	mine.setPosition(defaultTexture.getPosition());
	window.draw(mine);
}

void Space::DrawTileRevealed(sf::RenderWindow& window) {
	sf::Sprite revealedTile(TextureManager::GetTexture("tile_revealed"));
	revealedTile.setPosition(defaultTexture.getPosition());
	window.draw(revealedTile);
}

void Space::DrawMineCount(sf::RenderWindow& window) {
	std::string nearbyMines = "number_" + std::to_string(GetNearbyMineCount());
	sf::Sprite mineCount(TextureManager::GetTexture(nearbyMines));
	mineCount.setPosition(defaultTexture.getPosition());
	window.draw(mineCount);
}

void Space::DrawFlag(sf::RenderWindow& window) {
	sf::Sprite flag(TextureManager::GetTexture("flag"));
	flag.setPosition(defaultTexture.getPosition());
	window.draw(flag);
}

void Space::Update(sf::RenderWindow& window) {
	//Draw the base, a hidden tile.
	window.draw(defaultTexture);

	//Draw an opened tile.
	if (isRevealed) {
		DrawTileRevealed(window);

		//Draw a mine if it's a mine.
		if (isMine) {
			DrawMine(window);
		}
		//If it's not a mine nor having 0 neighbor mines, draw a number.
		else if (GetNearbyMineCount() > 0) {
			DrawMineCount(window);
		}
	}

	//Draw a flag over a closed tile.
	if (isFlagged && !isRevealed) {
		DrawFlag(window);
	}
}

sf::Sprite& Space::GetSprite() {
	return defaultTexture;
}

void Space::Reveal() {
	if (isRevealed) {
		return;
	}
	else if (!(isFlagged || isRevealed)) {
		isRevealed = true;
	}
	
	if (!isFlagged && GetNearbyMineCount() == 0) {
		RecursiveReveal();
	}
}

void Space::RecursiveReveal() {
	for (unsigned int i = 0; i < NUM_NEIGHBORS; i++) {
		if (neighborSpaces[i] != nullptr && !neighborSpaces[i]->isRevealed && !isMine) {
			neighborSpaces[i]->Reveal();
		}
	}
}

void Space::SwitchFlag() {
	if (!isRevealed) {
		isFlagged = !isFlagged;
	}
}

void Space::PlaceMine() {
	isMine = true;
}

void Space::AddNeighbor(int index, Space* space) {
	neighborSpaces[index] = space;
}

int Space::GetNearbyMineCount() const {
	int count = 0;
	for (unsigned int i = 0; i < NUM_NEIGHBORS; i++) {
		if (neighborSpaces[i] != nullptr && neighborSpaces[i]->isMine) {
			count++;
		}
	}
	return count;
}

bool Space::HasMine() const {
	return isMine;
}

bool Space::HasFlag() const {
	return isFlagged;
}

bool Space::IsRevealed() const {
	return isRevealed;
}