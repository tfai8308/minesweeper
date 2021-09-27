#include "UI.h"

unordered_map<string, sf::Sprite> UI::UIElements;
unordered_map<string, UI::Button> UI::UIButtons;
bool UI::debugToggled;

void UI::DrawButtons(sf::RenderWindow& window) {
	float yPos = Board::GetRowCount() * Board::TILE_SIZE;
	float xPos = (Board::GetColumnCount() % 2 == 1) //Attempting to center
		? Board::GetColumnCount() * Board::TILE_SIZE / 2 - (Board::TILE_SIZE / 2) //Odd number of columns
		: Board::GetColumnCount() * Board::TILE_SIZE / 2 - (Board::TILE_SIZE * 1); //Even number of columns

	sf::Sprite smiley;
	if (Board::GetBoardActive() == Board::GameState::ACTIVE) {
		smiley.setTexture(TextureManager::GetTexture("face_happy"));
	}
	else if (Board::GetBoardActive() == Board::GameState::LOSS) {
		smiley.setTexture(TextureManager::GetTexture("face_lose"));
	}
	else if (Board::GetBoardActive() == Board::GameState::WIN) {
		smiley.setTexture(TextureManager::GetTexture("face_win"));
	}
	smiley.setPosition(xPos, yPos);
	window.draw(smiley);

	sf::Sprite test3(TextureManager::GetTexture("test_3"));
	xPos = Board::GetColumnCount() * Board::TILE_SIZE - (Board::TILE_SIZE * 2); //Left align to window
	test3.setPosition(xPos, yPos);
	window.draw(test3);

	//Create other buttons relative to test3
	sf::Sprite test2(TextureManager::GetTexture("test_2"));
	xPos = test3.getPosition().x - (Board::TILE_SIZE * 2);
	test2.setPosition(xPos, yPos);
	window.draw(test2);

	sf::Sprite test1(TextureManager::GetTexture("test_1"));
	xPos = test3.getPosition().x - (Board::TILE_SIZE * 4);
	test1.setPosition(xPos, yPos);
	window.draw(test1);

	sf::Sprite debug(TextureManager::GetTexture("debug"));
	xPos = test3.getPosition().x - (Board::TILE_SIZE * 6);
	debug.setPosition(xPos, yPos);
	window.draw(debug);

	UIElements.emplace("smiley", smiley);
	UIElements.emplace("test3", test3);
	UIElements.emplace("test2", test2);
	UIElements.emplace("test1", test1);
	UIElements.emplace("debug", debug);
}

void UI::DrawCounter(sf::RenderWindow& window) {
	//Counter will display the number of mines - number of flags placed
	//If there are more flags than mines, the counter will display a negative sign
	int totalBoardSpaces = Board::GetRowCount() * Board::GetColumnCount();
	int counterSpaces = 0;
	while (totalBoardSpaces > 0) {
		counterSpaces++;
		totalBoardSpaces /= 10;
	}
	
	//Default positions
	int xPos = 0;
	int yPos = Board::GetRowCount() * Board::TILE_SIZE;

	sf::Sprite counter(TextureManager::GetTexture("digits"));
	int mineFlagDiff = Board::GetMineCount() - Board::GetFlagCount();

	if (mineFlagDiff < 0) {
		counter.setTextureRect(sf::IntRect(21 * 10, 0, 21, 32));
		counter.setPosition(xPos, yPos);
		window.draw(counter);
	}
	
	mineFlagDiff = abs(mineFlagDiff); //The difference's absolute value is displayed. If flags > mines, the counter increases.

	while (counterSpaces > 0) {
		counter.setTextureRect(sf::IntRect(21 * (mineFlagDiff % 10), 0, 21, 32)); //The digit Sprite TextureRect corresponds to the ones places of the diff
		counter.setPosition(xPos + (21 * counterSpaces), yPos); //Offset the xPos to draw the counter from Right to Left
		window.draw(counter);
		counterSpaces--;
		mineFlagDiff /= 10;
	}
}

void UI::InitializeUIFunctions() {
	//Give functionality to the UIElements
	Button smiley([]() {
		Board::InitializeBoard(false);
	});

	Button test3([]() {
		Board::InitializeBoard(true, "3");
	});

	Button test2([]() {
		Board::InitializeBoard(true, "2");
	});

	Button test1([]() {
		Board::InitializeBoard(true, "1");
	});

	Button debug([]() {
		if (Board::GetBoardActive() == Board::GameState::ACTIVE) {
			ToggleDebugMode();
		}
	});

	UIButtons.emplace("smiley", smiley);
	UIButtons.emplace("test3", test3);
	UIButtons.emplace("test2", test2);
	UIButtons.emplace("test1", test1);
	UIButtons.emplace("debug", debug);

	debugToggled = false;
}

void UI::UIEvent(sf::Vector2i& mousePosition) {
	auto iter = UIElements.begin();
	for (; iter != UIElements.end(); iter++) {
		if (iter->second.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
			UIButtons.at(iter->first).action();
		}
	}
}

void UI::DrawUI(sf::RenderWindow& window) {
	DrawButtons(window);
	DrawCounter(window);
}

void UI::ToggleDebugMode() {
	debugToggled = !debugToggled;
}

bool UI::GetDebugToggled() {
	return debugToggled;
}

UI::Button::Button(std::function<void(void)> fnc) {
	action = fnc;
}