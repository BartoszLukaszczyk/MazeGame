//maze.cpp

#include "maze.h"
#include "mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QPainter>
#include <QMessageBox>
#include <queue>


Maze::Maze(int rows, int cols) : rows(rows), cols(cols) {
    for (int i = 0; i < rows; ++i) {
        std::vector<Cell> row;
        for (int j = 0; j < cols; ++j) {
            row.push_back(Cell(j, i));
        }
        cells.push_back(row);
    }

    Cell* current = &cells[0][0];
    playerRow = 0;
    playerCol = 0;
    cells[playerRow][playerCol].isPlayerHere = true;
    current->visited = true;
    stack.push(current);

    generate();
}


void Maze::draw(QPainter &painter) {
    qDebug() << "Drawing maze...";
    painter.fillRect(0, 0, 400, 400, Qt::green);
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {

            drawCell(painter, &cells[row][col], row, col);
        }
    }
}

void Maze::drawCell(QPainter &painter, Cell *cell, int row, int col) {
    int cellSize = 20; 

    // Obliczamy rzeczywistą pozycję komórki na ekranie
    int x = cell->x * cellSize;
    int y = cell->y * cellSize;

    // Rysujemy ściany komórki
    if (cell->walls[0]) // góra
        painter.drawLine(x, y, x + cellSize, y);
    if (cell->walls[1]) // prawo
        painter.drawLine(x + cellSize, y, x + cellSize, y + cellSize);
    if (cell->walls[2]) // dół
        painter.drawLine(x, y + cellSize, x + cellSize, y + cellSize);
    if (cell->walls[3]) // lewo
        painter.drawLine(x, y, x, y + cellSize);
    if (cell->isPlayerHere) {
        QColor playerColor = Qt::blue;
        painter.fillRect(x + 1, y + 1, cellSize - 1, cellSize - 1, playerColor);
        return; 
    }

    // Ustawiamy kolor komórki na zielony jeśli została odwiedzona, na czarny w przeciwnym razie
    QColor color = cell->visited ? Qt::green : Qt::black;

    // Jeśli jesteśmy na pozycji mety (19,19), zmień kolor komórki na czerwony
    if (row == 19 && col == 19) {
        color = Qt::red;
    }
    if (row == 0 && col == 0) {
        color = Qt::blue;
    }

    painter.fillRect(x + 1, y + 1, cellSize - 1, cellSize - 1, color);
}



void Maze::generate() {
    while (!stack.empty()) {
        Cell* current = stack.top();
        Cell* next = getNext(current);

        if (next) {
            qDebug() << "Visiting cell at (" << next->x << "," << next->y << ")\n";
            removeWall(current, next);
            next->visited = true;
            stack.push(next);
        } else {
            stack.pop();
        }
    }
}


Cell* Maze::getNext(Cell* cell) {
    std::vector<Cell*> neighbors;

    if (cell->y > 0 && !cells[cell->y - 1][cell->x].visited)
        neighbors.push_back(&cells[cell->y - 1][cell->x]);

    if (cell->x < cols - 1 && !cells[cell->y][cell->x + 1].visited)
        neighbors.push_back(&cells[cell->y][cell->x + 1]);

    if (cell->y < rows - 1 && !cells[cell->y + 1][cell->x].visited)
        neighbors.push_back(&cells[cell->y + 1][cell->x]);

    if (cell->x > 0 && !cells[cell->y][cell->x - 1].visited)
        neighbors.push_back(&cells[cell->y][cell->x - 1]);

    if (neighbors.size() > 0) {
        int random = rand() % neighbors.size();
        return neighbors[random];
    } else {
        return nullptr;
    }
}

void Maze::removeWall(Cell* a, Cell* b) {
    int dx = b->x - a->x;
    int dy = b->y - a->y;

    if (dx == 1) {
        a->walls[1] = false;
        b->walls[3] = false;
    }
    else if (dx == -1) {
        a->walls[3] = false;
        b->walls[1] = false;
    }
    else if (dy == 1) {
        a->walls[2] = false;
        b->walls[0] = false;
    }
    else if (dy == -1) {
        a->walls[0] = false;
        b->walls[2] = false;
    }
}

void Maze::resetMaze() {

    // Wyczyść stos
    while (!stack.empty()) {
        stack.pop();
    }

    // Wyczyść i zainicjalizuj komórki
    cells.clear();
    for (int i = 0; i < rows; ++i) {
        std::vector<Cell> row;
        for (int j = 0; j < cols; ++j) {
            row.push_back(Cell(j, i));
        }
        cells.push_back(row);
    }

    // Rozpocznij algorytm od komórki w lewym górnym rogu
    Cell* current = &cells[0][0];
    current->visited = true;
    stack.push(current);
    generate();
    distances = std::vector<std::vector<int>>(rows, std::vector<int>(cols, -1));
    resetPlayerPosition();
}

bool Maze::isWallBetween(Cell* a, Cell* b) const {
    int dx = b->x - a->x;
    int dy = b->y - a->y;

    if (dx == 1) {
        return a->walls[1] || b->walls[3];
    } else if (dx == -1) {
        return a->walls[3] || b->walls[1];
    } else if (dy == 1) {
        return a->walls[2] || b->walls[0];
    } else if (dy == -1) {
        return a->walls[0] || b->walls[2];
    }
    return true;
}


void Maze::setPlayerPosition(int row, int col) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        Cell* newCell = &cells[row][col];

        if (!isWallBetween(&cells[playerRow][playerCol], newCell)) {
            cells[playerRow][playerCol].isPlayerHere = false;
            playerRow = row;
            playerCol = col;
            if (distances[row][col] == -1) {
                distances[row][col] = distances[playerRow][playerCol] + 1;
            }
            newCell->isPlayerHere = true;
        }
    }
}
void Maze::resetPlayerPosition() {
    cells[playerRow][playerCol].isPlayerHere = false;
    playerRow = 0;
    playerCol = 0;
    cells[playerRow][playerCol].isPlayerHere = true;
}






Cell* Maze::getCell(int row, int col) {
    return &cells[row][col];
}

std::pair<int, int> Maze::getPlayerPosition() const {
    return std::make_pair(playerRow, playerCol);
}
int Maze::calculateShortestPathLength() {
    std::queue<std::pair<int, int>> q;
    std::vector<std::vector<int>> dist(rows, std::vector<int>(cols, -1));
    std::pair<int, int> start(0, 0);
    dist[start.first][start.second] = 0;
    q.push(start);
    std::pair<int, int> goal(rows-1, cols-1);

    std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    while (!q.empty()) {
    auto current = q.front();
    q.pop();
    for (auto dir : directions) {
        int newRow = current.first + dir.first;
        int newCol = current.second + dir.second;
        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols && dist[newRow][newCol] == -1) {
            if (!isWallBetween(&cells[current.first][current.second], &cells[newRow][newCol])) {
                dist[newRow][newCol] = dist[current.first][current.second] + 1;
                std::pair<int, int> next(newRow, newCol);
                q.push(next);
            }
        }
    }
    }

    return dist[goal.first][goal.second];
}
