#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

class Space {
private:
	bool isRevealed;
	bool isFlagged;
	bool isMine;
	const int NUM_NEIGHBORS = 8;
	Space* neighborSpaces[8];
	sf::Sprite defaultTexture; //A space has a default hidden tile texture

	void RecursiveReveal();

public:
	Space();
	void Update(sf::RenderWindow& window);
	sf::Sprite& GetSprite();
	void Reveal();
	void SwitchFlag();
	void PlaceMine();
	void AddNeighbor(int index, Space* space);

	int GetNearbyMineCount() const;
	bool HasMine() const;
	bool HasFlag() const;
	bool IsRevealed() const;

	void DrawMine(sf::RenderWindow& window);
	void DrawTileRevealed(sf::RenderWindow& window);
	void DrawMineCount(sf::RenderWindow& window);
	void DrawFlag(sf::RenderWindow& window);
};

