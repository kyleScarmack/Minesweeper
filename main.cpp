#include <iostream>
#include <fstream>
#include <string>
#include "WelcomeWindow.h"
using namespace std;

void readConfig(int& colCount, int& rowCount, int& mineCount) {
    ifstream configFile("files/config.cfg");
    if (!configFile.is_open()) {
        cout << "Failed to open config file." << endl;
        return;
    }
    string line;
    getline(configFile, line);
    colCount = stoi(line);
    getline(configFile, line);
    rowCount = stoi(line);
    getline(configFile, line);
    mineCount = stoi(line);
    configFile.close();
}

int main() {
    int colCount, rowCount, mineCount;
    readConfig(colCount, rowCount, mineCount);

    WelcomeWindow welcome(colCount, rowCount, mineCount);

    while (welcome.window.isOpen()) {
        welcome.runWindow();
    }

    return 0;
};
