#pragma once
#include <iostream>
#include "Tile.h"
#include <vector>
#include <SFML/Graphics.hpp>

struct Board {
    int rowCount;
    int colCount;
    int mineCount;
    int flagCount = 0;
    bool debugMode = false;
    vector<vector<Tile>> tiles;

    Board(int rowCount, int colCount, int mineCount) {
        this->rowCount = rowCount;
        this->colCount = colCount;
        this->mineCount = mineCount;

        srand(time(nullptr));

        tiles.resize(rowCount);
        for (int i = 0; i < rowCount; i++) {
            tiles[i].resize(colCount);
            for (int j = 0; j < colCount; j++) {
                tiles[i][j] = Tile();
            }
        }
    }

    void clearBoard() {
        for (int i = 0; i < rowCount; i++) {
            for (int j = 0; j < colCount; j++) {
                tiles[i][j].reset();
            }
        }
    }

    void placeMines(int firstRow, int firstCol) {
        int minesPlaced = 0;
        while (minesPlaced < mineCount) {
            int row = rand() % rowCount;
            int col = rand() % colCount;
            if (row != firstRow || col != firstCol) {
                if (!tiles[row][col].isMine) {
                    tiles[row][col].setMine();
                    minesPlaced++;
                }
            }
        }
        setAdjacentMines();
    }

    void revealMines() {
        for (int i = 0; i < rowCount; i++) {
            for (int j = 0; j < colCount; j++) {
                if (tiles[i][j].isMine) {
                    tiles[i][j].isRevealed = true;
                }
            }
        }
    }

    void setAdjacentMines() {
        for (int i = 0; i < rowCount; i++) {
            for (int j = 0; j < colCount; j++) {
                if (tiles[i][j].isMine) {
                    continue;
                }

                int adjacentMines = 0;
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        int ni = i + di;
                        int nj = j + dj;
                        if (ni >= 0 && ni < rowCount && nj >= 0 && nj < colCount) {
                            if (tiles[ni][nj].isMine) {
                                adjacentMines++;
                            }
                        }
                    }
                }
                tiles[i][j].setAdjacentMines(adjacentMines);
            }
        }
    }

    Tile& getTile(int row, int col) {
        return tiles[row][col];
    }

    void revealAdjacentTiles(int row, int col) {
        if (row < 0 || row >= rowCount || col < 0 || col >= colCount) {
            return;
        }

        Tile& tile = tiles[row][col];

        if (!tile.isRevealed && !tile.isFlagged) {
            if (tile.numAdjacentMines != 0) {
                tile.reveal();
                return;
            }

            tile.reveal();

            for (int i = row - 1; i <= row + 1; i++) {
                for (int j = col - 1; j <= col + 1; j++) {
                    if (i != row || j != col) {
                        revealAdjacentTiles(i, j);
                    }
                }
            }
        }
    }

    void toggleFlag(int row, int col) {
        if (tiles[row][col].toggleFlag()) {
            flagCount++;
        } else {
            flagCount--;
        }
    }

    void toggleDebug() {
        debugMode = !debugMode;
        if (debugMode) {
            revealDebugMines();
        } else {
            hideMines();
        }
    }

    void hideMines() {
        for (int i = 0; i < rowCount; i++) {
            for (int j = 0; j < colCount; j++) {
                if (tiles[i][j].isMine) {
                    tiles[i][j].isRevealed = false;
                }
            }
        }
    }

    void revealDebugMines() {
        for (int i = 0; i < rowCount; i++) {
            for (int j = 0; j < colCount; j++) {
                Tile &currentTile = tiles[i][j];
                if (currentTile.isMine) {
                    currentTile.isRevealed = true;
                }
            }
        }
    }

    bool allTilesRevealed() {
        for (int i = 0; i < rowCount; i++) {
            for (int j = 0; j < colCount; j++) {
                if (!tiles[i][j].isRevealed && !tiles[i][j].isMine) {
                    return false;
                }
            }
        }
        return true;
    }

    void flagAllMines() {
        for (int i = 0; i < rowCount; i++) {
            for (int j = 0; j < colCount; j++) {
                Tile& tile = tiles[i][j];
                if (tile.isMine && !tile.isFlagged) {
                    tile.toggleFlag();
                    flagCount++;
                }
            }
        }
    }
};