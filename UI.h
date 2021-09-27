#pragma once
#include <functional>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Board.h"

class UI {
public:
	struct Button {
		Button(std::function<void(void)> fnc);
		std::function<void(void)> action;
	};

	static void InitializeUIFunctions();
	static void UIEvent(sf::Vector2i& mousePosition);
	static void DrawUI(sf::RenderWindow& window);
	static bool GetDebugToggled();

private:
	static unordered_map<string, sf::Sprite> UIElements;
	static unordered_map<string, Button> UIButtons;
	static bool debugToggled;

	static void DrawButtons(sf::RenderWindow& window);
	static void DrawCounter(sf::RenderWindow& window);
	static void ToggleDebugMode();
};