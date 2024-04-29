#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>

struct Player {
    string name;
    int time;
    bool isNew;

    Player(const string& name, int time, bool isNew = false) {
        this->name = name;
        this->time = time;
        this->isNew = isNew;
    }
};

struct Leaderboard {
    vector<Player> players;
    const int maxPlayers = 5;

    Leaderboard() {
        loadLeaderboard();
    }

    void loadLeaderboard() {
        ifstream file("files/leaderboard.txt");
        if (file.is_open()) {
            players.clear();
            string line;
            while (getline(file, line)) {
                istringstream playerEntry(line);
                string time, name;
                if (getline(playerEntry >> ws, time, ',') && getline(playerEntry >> ws, name)) {
                    int minutes, seconds;
                    char colon;
                    istringstream timeStream(time);
                    if (timeStream >> minutes >> colon >> seconds) {
                        int timeInSeconds = minutes * 60 + seconds;
                        players.emplace_back(name, timeInSeconds);
                    }
                }
            }
            file.close();
        }
    }

    void saveLeaderboard() {
        sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return a.time < b.time;
        });

        if (players.size() > maxPlayers) {
            players.pop_back();
        }

        ofstream file("files/leaderboard.txt");
        if (file.is_open()) {
            for (int i = 0; i < players.size(); i++) {
                int minutes = players[i].time / 60;
                int seconds = players[i].time % 60;
                file << setw(2) << setfill('0') << minutes << ":";
                file << setw(2) << setfill('0') << seconds << ", ";
                file << players[i].name << endl;
            }
            file.close();
        }
    }

    void updateLeaderboard(const string& enteredName, int timeInSeconds) {
        players.emplace_back(enteredName, timeInSeconds, true);
        saveLeaderboard();
        sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return a.time < b.time;
        });
        if (players.size() > maxPlayers) {
            players.pop_back();
        }
    }

    void displayLeaderboard(int colCount, int rowCount, bool playerWon, const string& playerName, int timeInSeconds) {
        sf::RenderWindow leaderboard(sf::VideoMode(colCount * 16, rowCount * 16 + 50), "Minesweeper");
        sf::Font font;
        if (!font.loadFromFile("files/font.ttf")) {
            cout << "Failed to load font file!" << endl;
            return;
        }

        sf::Text title("LEADERBOARD", font, 20);
        title.setFillColor(sf::Color::White);
        title.setStyle(sf::Text::Bold | sf::Text::Underlined);
        title.setPosition(colCount * 8, 20);
        setText(title, colCount * 8, 20);

        leaderboard.clear(sf::Color::Blue);
        leaderboard.draw(title);

        string leaderboardText;
        for (int i = 0; i < players.size() && i < maxPlayers; i++) {
            int minutes = players[i].time / 60;
            int seconds = players[i].time % 60;

            stringstream playerEntry;
            playerEntry << i + 1 << ".\t";
            playerEntry << setw(2) << setfill('0') << minutes << ":";
            playerEntry << setw(2) << setfill('0') << seconds << "\t";
            playerEntry << players[i].name;

            if (players[i].isNew) {
                playerEntry << "*";
                players[i].isNew = false;
            }

            leaderboardText += playerEntry.str();
            if (i < players.size() - 1) {
                leaderboardText += "\n\n";
            }
        }

        sf::Text leaderboardInfo(leaderboardText, font, 18);
        leaderboardInfo.setFillColor(sf::Color::White);
        leaderboardInfo.setStyle(sf::Text::Bold);
        setText(leaderboardInfo, colCount * 8, 140);
        leaderboard.draw(leaderboardInfo);

        leaderboard.display();

        while (leaderboard.isOpen()) {
            sf::Event event;
            leaderboard.requestFocus();
            while (leaderboard.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    leaderboard.close();
                }
            }
        }
    }

    void setText(sf::Text &text, float x, float y) {
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top  + textRect.height / 2.0f);
        text.setPosition(sf::Vector2f(x, y));
    }
};
