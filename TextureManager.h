#pragma once
#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
using namespace std;

struct TextureManager {
    map<string, sf::Texture> textures;
    sf::Font font;

    bool loadTexture(const string& name, const string& filename) {
        if (textures.find(name) != textures.end()) {
            return true;
        } else {
            sf::Texture texture;
            if (texture.loadFromFile(filename)) {
                textures[name] = texture;
                return true;
            }
            return false;
        }
    }

    sf::Texture& getTexture(const string& name) {
        auto result = textures.find(name);
        if (result == textures.end()) {
            sf::Texture newTexture;
            newTexture.loadFromFile("files/images/" + name + ".png");
            textures[name] = newTexture;
            return textures[name];
        } else {
            return result->second;
        }
    }

    void loadAllTextures() {
        loadTexture("debug", "files/images/debug.png");
        loadTexture("digits", "files/images/digits.png");
        loadTexture("face_happy", "files/images/face_happy.png");
        loadTexture("face_win", "files/images/face_win.png");
        loadTexture("flag", "files/images/flag.png");
        loadTexture("leaderboard", "files/images/leaderboard.png");
        loadTexture("mine", "files/images/mine.png");
        loadTexture("number_1", "files/images/number_1.png");
        loadTexture("number_2", "files/images/number_2.png");
        loadTexture("number_3", "files/images/number_3.png");
        loadTexture("number_4", "files/images/number_4.png");
        loadTexture("number_5", "files/images/number_5.png");
        loadTexture("number_6", "files/images/number_6.png");
        loadTexture("number_7", "files/images/number_7.png");
        loadTexture("number_8", "files/images/number_8.png");
        loadTexture("pause", "files/images/pause.png");
        loadTexture("play", "files/images/play.png");
        loadTexture("tile_hidden", "files/images/tile_hidden.png");
        loadTexture("tile_revealed", "files/images/tile_revealed.png");
    }

};