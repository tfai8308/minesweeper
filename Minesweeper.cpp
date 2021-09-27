#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Random.h"
#include "UI.h"
#include "Board.h"

int main() {
    
    Board::InitializeBoard(false);
    UI::InitializeUIFunctions();
    sf::RenderWindow window(sf::VideoMode(Board::GetColumnCount() * Board::TILE_SIZE, (Board::GetRowCount() * Board::TILE_SIZE) + 88), "Minesweeper");
    
    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            //Mouse events
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                if (event.mouseButton.button == sf::Mouse::Left) {
                    UI::UIEvent(mousePosition);
                    Board::RevealEvent(mousePosition);
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    Board::FlagEvent(mousePosition);
                }
            }
        }
       
        window.clear();
        Board::DrawBoard(window);
        UI::DrawUI(window);
        window.display();
    }

    TextureManager::Clear();
    return 0;
}