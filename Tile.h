#pragma once
#include <iostream>
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
using namespace std;

struct Tile {
    bool isMine;
    bool isRevealed;
    bool isFlagged;
    int numAdjacentMines;
    sf::Sprite sprite;
    sf::Texture hiddenTexture;
    sf::Texture revealedTexture;
    sf::Texture flagTexture;
    sf::Texture mineTexture;
    sf::Texture numberTextures[8];

    Tile() {
        isMine = false;
        isRevealed = false;
        isFlagged = false;
        numAdjacentMines = 0;

        if (!hiddenTexture.loadFromFile("files/images/tile_hidden.png") ||
            !revealedTexture.loadFromFile("files/images/tile_revealed.png") ||
            !flagTexture.loadFromFile("files/images/flag.png") ||
            !mineTexture.loadFromFile("files/images/mine.png")) {
            cout << "Could not load texture" << endl;
        }
        for (int i = 1; i < 8; i++) {
            string filename = "files/images/number_" + to_string(i) + ".png";
            if (!numberTextures[i].loadFromFile(filename)) {
                cout << "Could not load texture" << endl;
            }
        }

        sprite.setTexture(hiddenTexture);
    }

    void setMine() {
        isMine = true;
    }

    void setAdjacentMines(int count) {
        numAdjacentMines = count;
    }

    void reset() {
        isMine = false;
        isRevealed = false;
        isFlagged = false;
        numAdjacentMines = 0;
    }

    void reveal() {
        isRevealed = true;
        if (isMine) {
            sprite.setTexture(mineTexture);
        } else if (numAdjacentMines > 0) {
            sprite.setTexture(numberTextures[numAdjacentMines]);
        } else {
            sprite.setTexture(revealedTexture);
        }
    }

    bool toggleFlag() {
        bool flagAdded = !isFlagged;
        isFlagged = !isFlagged;
        if (isFlagged) {
            sprite.setTexture(flagTexture);
        } else {
            sprite.setTexture(hiddenTexture);
        }
        return flagAdded;
    }
};