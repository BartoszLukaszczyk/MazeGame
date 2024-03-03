//maze.h
#ifndef MAZE_H
#define MAZE_H

#include "cell.h"
#include <vector>
#include <stack>
#include <QPainter>
#include <ctime>
#include <QObject>
#include <QTimer>
#include <QTime>


class Cell* getCell(int, int);
//maze.h
class Maze
{
public:
    Maze(int rows, int cols);

    void draw(QPainter &painter);
    void drawCell(QPainter &painter, Cell *cell);
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    Cell* getCell(int row, int col);
    void generate();
    std::pair<int, int> getPlayerPosition() const;
    void setPlayerPosition(int row, int col);
    void drawCell(QPainter &painter, Cell *cell, int row, int col);
    bool isWallBetween(Cell* a, Cell* b) const;
    void resetMaze();
    void resetPlayerPosition();  
    void startTimer();
    void stopTimer();
    int calculateShortestPathLength();


private:
    Cell* getNext(Cell* cell);
    void removeWall(Cell* a, Cell* b);

    int rows, cols;
    std::vector<std::vector<Cell>> cells;
    std::stack<Cell*> stack;
    int playerRow = 0;
    int playerCol = 0;
    QTimer *timer;
    QTime elapsedTime;
    std::vector<std::vector<int>> distances;
private slots:
    void updateTimer();

};


#endif // MAZE_H
