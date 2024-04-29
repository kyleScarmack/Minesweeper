#pragma once
#include <iostream>
#include "TextureManager.h"
#include "GameWindow.h"
#include <SFML/Graphics.hpp>

struct WelcomeWindow {
    int colCount;
    int rowCount;
    int mineCount;
    int width;
    int height;
    string enteredName;
    sf::RenderWindow window;
    sf::Font font;

    WelcomeWindow(int colCount, int rowCount, int mineCount) {
        window.create(sf::VideoMode(colCount * 32, rowCount * 32 + 100), "Minesweeper");
        this->colCount = colCount;
        this->rowCount = rowCount;
        this->mineCount = mineCount;
        width = colCount * 32;
        height = rowCount * 32 + 100;
    }

    void runWindow() {
        sf::Event event;
        sf::Font font;
        if (!font.loadFromFile("files/font.ttf")) {
            cout << "Failed to load font file!" << endl;
        }
        while (window.isOpen()) {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                } else if (event.type == sf::Event::TextEntered) {
                    char typedChar = static_cast<char>(event.text.unicode);
                    if (isalpha(typedChar)) {
                        if (enteredName.size() < 10) {
                            enteredName += tolower(typedChar);
                            if (enteredName.size() == 1) {
                                enteredName[0] = toupper(enteredName[0]);
                            }
                        }
                    }
                } else if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::BackSpace && !enteredName.empty()) {
                        enteredName.pop_back();
                    } else if (event.key.code == sf::Keyboard::Enter && !enteredName.empty()) {
                        window.close();
                        GameWindow game(colCount, rowCount, mineCount, enteredName);
                        game.runWindow();
                    }
                }
            }
            window.clear(sf::Color::Blue);

            sf::Text welcomeToMinesweeper("WELCOME TO MINESWEEPER!", font, 24);
            welcomeToMinesweeper.setFillColor(sf::Color::White);
            welcomeToMinesweeper.setStyle(sf::Text::Bold | sf::Text::Underlined);
            welcomeToMinesweeper.setPosition(width / 2, height / 2 - 150);
            setText(welcomeToMinesweeper, width / 2, height / 2 - 150);
            window.draw(welcomeToMinesweeper);

            sf::Text enterName("Enter your name:", font, 20);
            enterName.setFillColor(sf::Color::White);
            enterName.setStyle(sf::Text::Bold);
            enterName.setPosition(width / 2, height / 2 - 75);
            setText(enterName, width / 2, height / 2 - 75);
            window.draw(enterName);

            sf::Text name(enteredName + "|", font, 18);
            name.setFillColor(sf::Color::Yellow);
            float nameWidth = name.getLocalBounds().width;
            name.setPosition((width - nameWidth) / 2, height / 2 - 45);
            window.draw(name);

            window.display();
        }
    }

    void setText(sf::Text &text, float x, float y) {
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top  + textRect.height / 2.0f);
        text.setPosition(sf::Vector2f(x, y));
    }
};