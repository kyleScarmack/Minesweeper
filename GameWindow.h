#pragma once
#include <iostream>
#include "TextureManager.h"
#include "Board.h"
#include "Leaderboard.h"
#include "Tile.h"
#include <SFML/Graphics.hpp>

struct GameWindow {
    int colCount;
    int rowCount;
    int mineCount;
    string enteredName;
    bool debugMode = false;
    bool isWon = false;
    bool isLost = false;
    bool gameOver = false;
    bool firstClick = true;
    bool gameOverDisplayed = false;
    bool paused = false;
    bool pausePressed = false;
    map<int, sf::Sprite> digitsMap;
    sf::RenderWindow window;
    sf::Font font;
    TextureManager textureManager;
    Board board;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> pauseTime;
    long elapsed_paused_time;

    GameWindow(int colCount, int rowCount, int mineCount, const string& enteredName) : board(rowCount, colCount, mineCount) {
        window.create(sf::VideoMode(colCount * 32, rowCount * 32 + 100), "Minesweeper");
        this->colCount = colCount;
        this->rowCount = rowCount;
        this->mineCount = mineCount;
        this->enteredName = enteredName;
    }

    void drawBoard(map<string, sf::Sprite>& buttons) {
        for (int i = 0; i < rowCount; i++) {
            for (int j = 0; j < colCount; j++) {
                Tile &tile = board.getTile(i, j);

                sf::Sprite sprite;
                sprite.setPosition(j * 32, i * 32);
                if (!tile.isRevealed) {
                    sprite.setTexture(tile.hiddenTexture);
                    window.draw(sprite);
                } else {
                    if (tile.isMine) {
                        if (isLost) {
                            sprite.setTexture(tile.revealedTexture);
                        } else {
                            sprite.setTexture(tile.hiddenTexture);
                        }
                        window.draw(sprite);
                    } else {
                        sprite.setTexture(tile.revealedTexture);
                        window.draw(sprite);
                    }

                    if (tile.isMine && tile.isRevealed) {
                        sprite.setTexture(tile.mineTexture);
                        window.draw(sprite);
                    } else if (tile.numAdjacentMines != 0) {
                        sprite.setTexture(tile.numberTextures[tile.numAdjacentMines]);
                        window.draw(sprite);
                    }
                }

                if (tile.isFlagged) {
                    sprite.setTexture(tile.flagTexture);
                    window.draw(sprite);
                }
                if (isLost && tile.isMine) {
                    sprite.setTexture(tile.mineTexture);
                    window.draw(sprite);
                }

                if (paused) {
                    if (!gameOver) {
                        sprite.setTexture(tile.revealedTexture);
                        window.draw(sprite);
                    }
                }
            }
        }
        for (auto it = buttons.begin(); it != buttons.end(); it++) {
            window.draw(it->second);
        }
        window.display();
    }

    void resetBoard() {
        board.clearBoard();
        board.flagCount = 0;
        board.debugMode = false;
        debugMode = false;
        isWon = false;
        isLost = false;
        gameOver = false;
        firstClick = true;
        gameOverDisplayed = false;
        paused = false;
        pausePressed = false;
        start_time = std::chrono::high_resolution_clock::now();
        pauseTime = std::chrono::high_resolution_clock::now();
        elapsed_paused_time = 0;
    }

    map<int, sf::Sprite> parseDigits(sf::Sprite digits) {
        map<int, sf::Sprite> digitsMap;

        for (int i = 0; i < 11; i++) {
            sf::IntRect rect(i * 21, 0, 21, 32);
            digits.setTextureRect(rect);
            sf::Sprite sprite = digits;
            digitsMap.emplace(i, sprite);
        }
        return digitsMap;
    }

    map<string, sf::Sprite> runUI() {
        textureManager.loadAllTextures();

        map<string, sf::Sprite> buttonSprites;

        buttonSprites["pause"].setTexture(textureManager.getTexture("pause"));
        buttonSprites["face_happy"].setTexture(textureManager.getTexture("face_happy"));
        buttonSprites["debug"].setTexture(textureManager.getTexture("debug"));
        buttonSprites["leaderboard"].setTexture(textureManager.getTexture("leaderboard"));

        return buttonSprites;
    }

    void runWindow() {
        bool leaderboardOpen = false;
        start_time = std::chrono::high_resolution_clock::now();
        pauseTime = std::chrono::high_resolution_clock::now();
        elapsed_paused_time = 0;

        sf::Texture &digitsText = textureManager.getTexture("digits");
        sf::Sprite digits;
        digits.setTexture(digitsText);

        map<int, sf::Sprite> digitsMap = parseDigits(digits);

        map<string, sf::Sprite> uiButtons = runUI();

        sf::Sprite& pauseBttn = uiButtons["pause"];
        pauseBttn.setPosition((colCount * 32) - 240, 32 * (rowCount + 0.5));
        sf::Sprite& face_happyBttn = uiButtons["face_happy"];
        face_happyBttn.setPosition((colCount / 2) * 32 - 32, 32 * (rowCount + 0.5));
        sf::Sprite& debugBttn = uiButtons["debug"];
        debugBttn.setPosition((colCount * 32) - 304, 32 * (rowCount + 0.5));
        sf::Sprite& leaderboardBttn = uiButtons["leaderboard"];
        leaderboardBttn.setPosition((colCount * 32) - 176, 32 * (rowCount + 0.5));

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                } else if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int row = mousePos.y / 32;
                    int col = mousePos.x / 32;

                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (face_happyBttn.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            resetBoard();
                            face_happyBttn.setTexture(textureManager.getTexture("face_happy"));
                            pauseBttn.setTexture(textureManager.getTexture("pause"));
                        } else if (pauseBttn.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            if (!gameOver) {
                                paused = !paused;
                                if (paused) {
                                    pauseBttn.setTexture(textureManager.getTexture("play"));
                                    pauseTime = chrono::high_resolution_clock::now();
                                    pausePressed = true;

                                } else {
                                    pausePressed = false;
                                    pauseBttn.setTexture(textureManager.getTexture("pause"));
                                    auto unPausedTime = chrono::steady_clock::now();
                                    elapsed_paused_time += chrono::duration_cast<chrono::seconds>(
                                            unPausedTime - pauseTime).count();
                                }
                            }
                        } else if (debugBttn.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            if (!gameOver && !paused && !firstClick) {
                                board.toggleDebug();
                                debugMode = !debugMode;
                            }
                        } else if (leaderboardBttn.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            if (!gameOver) {
                                paused = true;
                                if (!pausePressed) {
                                    pauseTime = chrono::high_resolution_clock::now();
                                }
                                pauseBttn.setTexture(textureManager.getTexture("play"));
                            }
                            leaderboardOpen = true;
                        } else {
                            if (paused) {
                                pauseBttn.setTexture(textureManager.getTexture("play"));
                            }
                            if (!paused && !gameOver) {
                                auto current_time = std::chrono::high_resolution_clock::now();
                                auto duration = std::chrono::duration_cast<std::chrono::seconds>(
                                        current_time - start_time);
                                int total_time = duration.count() - elapsed_paused_time;
                                pauseBttn.setTexture(textureManager.getTexture("pause"));
                                if (board.getTile(row, col).isFlagged) {
                                    break;
                                }
                                if (firstClick) {
                                    if (!board.getTile(row, col).isMine) {
                                        board.placeMines(row, col);
                                        firstClick = false;
                                    }
                                }
                                if (board.getTile(row, col).isMine) {
                                    gameOver = true;
                                    isLost = true;
                                    face_happyBttn.setTexture(textureManager.getTexture("face_lose"));
                                    pauseBttn.setTexture(textureManager.getTexture("play"));
                                    board.revealMines();
                                } else {
                                    board.revealAdjacentTiles(row, col);
                                    if (board.allTilesRevealed()) {
                                        board.flagAllMines();
                                        isWon = true;
                                        gameOver = true;
                                        face_happyBttn.setTexture(textureManager.getTexture("face_win"));
                                        pauseBttn.setTexture(textureManager.getTexture("play"));
                                    }
                                }
                            }
                        }
                    } else if (event.mouseButton.button == sf::Mouse::Right) {
                        if (!paused && !gameOver) {
                            if (!board.getTile(row, col).isRevealed || debugMode) {
                                board.toggleFlag(row, col);
                            }
                        }
                    }
                }
            }
            window.clear(sf::Color::White);

            int counter = (mineCount - board.flagCount);
            bool isNegative = counter < 0;
            int counter0;
            int counter1 = abs(counter) / 100 % 10;
            int counter2 = abs(counter) / 10 % 10;
            int counter3 = abs(counter) % 10;
            if (isNegative) {
                counter0 = 10;
                digitsMap[counter0].setPosition(12, 32 * (rowCount + 0.5) + 16);
                window.draw(digitsMap[counter0]);
            } else {
                digitsMap[counter0].setPosition(12, 32 * (rowCount + 0.5) + 16);
            }
            digitsMap[counter1].setPosition(33, 32 * (rowCount + 0.5) + 16);
            window.draw(digitsMap[counter1]);
            digitsMap[counter2].setPosition(54, 32 * (rowCount + 0.5) + 16);
            window.draw(digitsMap[counter2]);
            digitsMap[counter3].setPosition(75, 32 * (rowCount + 0.5) + 16);
            window.draw(digitsMap[counter3]);

            auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::high_resolution_clock::now() - start_time);
            int total_time = game_duration.count();

            int minutes;
            int seconds;

            if (!paused) {
                total_time = total_time - elapsed_paused_time;
                minutes = total_time / 60;
                seconds = total_time % 60;
            }

            int minutes0 = minutes / 10 % 10;
            int minutes1 = minutes % 10;
            int seconds0 = seconds / 10 % 10;
            int seconds1 = seconds % 10;

            digitsMap[minutes0].setPosition((colCount * 32) - 97, 32 * (rowCount + 0.5) + 16);
            window.draw(digitsMap[minutes0]);

            digitsMap[minutes1].setPosition((colCount * 32) - 76, 32 * (rowCount + 0.5) + 16);
            window.draw(digitsMap[minutes1]);

            digitsMap[seconds0].setPosition((colCount * 32) - 54, 32 * (rowCount + 0.5) + 16);
            window.draw(digitsMap[seconds0]);

            digitsMap[seconds1].setPosition((colCount * 32) - 33, 32 * (rowCount + 0.5) + 16);
            window.draw(digitsMap[seconds1]);
            drawBoard(uiButtons);
            if (isLost) {
                if (!gameOverDisplayed) {
                    gameOverDisplayed = true;
                    gameOver = true;
                    paused = true;
                }
            } else if (isWon) {
                if (!gameOverDisplayed) {
                    gameOverDisplayed = true;
                    gameOver = true;
                    paused = true;
                    Leaderboard leaderboard;
                    leaderboard.updateLeaderboard(enteredName, total_time);
                    leaderboard.displayLeaderboard(colCount, rowCount, isWon, enteredName, total_time);
                }
            }
            if (leaderboardOpen) {
                Leaderboard leaderboard;
                leaderboard.displayLeaderboard(colCount, rowCount, isWon, enteredName, total_time);

                if (!pausePressed && !gameOver) {
                    paused = false;
                    pauseBttn.setTexture(textureManager.getTexture("pause"));

                    if (!paused) {
                        auto unPausedTime = std::chrono::high_resolution_clock::now();
                        elapsed_paused_time += std::chrono::duration_cast<std::chrono::seconds>(unPausedTime - pauseTime).count();
                    }
                }
                leaderboardOpen = false;
                drawBoard(uiButtons);
                window.clear(sf::Color::White);
            }
        }
    }
};